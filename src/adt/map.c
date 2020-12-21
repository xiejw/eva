#include "adt/map.h"

#include "assert.h"

// -----------------------------------------------------------------------------
// internal data structure.
// -----------------------------------------------------------------------------

#define MAP_INIT_N_BUCKETS 16

// Node type.
//
// This is not the full structure. The full space will look like:
//     nodt_t | key | value
//      \-> pointer header
typedef struct map_node_t {
  unsigned           hash;    // hash value. see `map_hash`.
  void *             pvalue;  // point to the address of content of value.
  struct map_node_t *next;    // next map node.
                              // other fields allocated by malloc after here.
} map_node_t;

// -----------------------------------------------------------------------------
// helper methods prototypes.
// -----------------------------------------------------------------------------

static unsigned     map_hash(const char *str);
static map_node_t * map_newnode(const char *key, void *value, int vsize);
static int          map_bucketidx(map_base_t *m, unsigned hash);
static void         map_addnode(map_base_t *m, map_node_t *node);
static map_node_t **map_getref(map_base_t *m, const char *key);

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

void *_mapGet(map_base_t *m, const char *key) {
  map_node_t **next = map_getref(m, key);
  return next ? (*next)->pvalue : NULL;
}

error_t _mapSet(map_base_t *m, const char *key, void *value, int vsize) {
  // find and replace existing node.
  map_node_t **next = map_getref(m, key);
  if (next) {
    memcpy((*next)->pvalue, value, vsize);
    return OK;
  }

  // add new node.
  map_node_t *node = map_newnode(key, value, vsize);
  if (node == NULL) return errMalloc();
  if (m->nnodes >= m->nbuckets) {
    int n = (m->nbuckets > 0) ? (m->nbuckets << 1) : MAP_INIT_N_BUCKETS;
    if (_mapResize(m, n)) {
      free(node);
      return errNum();
    }
  }
  map_addnode(m, node);
  m->nnodes++;
  return OK;
}

void _mapFree(map_base_t *m) {
  if (m == NULL) return;

  struct map_node_t *next, *node;
  int                i;
  i = m->nbuckets;
  assert(m->buckets != NULL);
  while (i--) {
    node = m->buckets[i];
    while (node) {
      next = node->next;
      free(node);
      node = next;
    }
  }
  free(m->buckets);
  free(m);
}

// -----------------------------------------------------------------------------
// helper methods implementation.
// -----------------------------------------------------------------------------

unsigned map_hash(const char *str) {
  unsigned hash = 5381;
  while (*str) hash = ((hash << 5) + hash) ^ *str++;
  return hash;
}

map_node_t *map_newnode(const char *key, void *value, int vsize) {
  // calculate the offset of the address for `value`.
  // - `voffset` considers the padding of the `key`.
  // - `sizeof` is `unsigned long` so, the % is always positive result.
  int ksize   = strlen(key) + 1;
  int voffset = ksize + ((sizeof(void *) - ksize) % sizeof(void *));

  map_node_t *node = malloc(sizeof(*node) + voffset + vsize);
  if (!node) return NULL;

  memcpy(node + 1, key, ksize);
  node->hash   = map_hash(key);
  node->pvalue = ((char *)(node + 1)) + voffset;
  memcpy(node->pvalue, value, vsize);
  return node;
}

// map `hash` to bucket idx.
int map_bucketidx(map_base_t *m, unsigned hash) {
  // If the implementation is changed to allow a non-power-of-2 bucket count,
  // the line below should be changed to use mod instead of AND.
  return hash & (m->nbuckets - 1);
}

void map_addnode(map_base_t *m, map_node_t *node) {
  int n         = map_bucketidx(m, node->hash);
  node->next    = m->buckets[n];
  m->buckets[n] = node;
}

map_node_t **map_getref(map_base_t *m, const char *key) {
  unsigned     hash = map_hash(key);
  map_node_t **next;
  if (m->nbuckets > 0) {
    next = &m->buckets[map_bucketidx(m, hash)];
    while (*next) {
      if ((*next)->hash == hash && !strcmp((char *)(*next + 1), key)) {
        return next;
      }
      next = &(*next)->next;
    }
  }
  return NULL;
}

error_t _mapResize(map_base_t *m, int nbuckets) {
  map_node_t * nodes, *node, *next;
  map_node_t **buckets;
  int          i;

  // chain all nodes together.
  nodes = NULL;
  i     = m->nbuckets;
  while (i--) {
    node = (m->buckets)[i];
    while (node) {
      next       = node->next;
      node->next = nodes;
      nodes      = node;
      node       = next;
    }
  }
  // reset buckets.
  buckets = realloc(m->buckets, sizeof(*m->buckets) * nbuckets);
  if (buckets == NULL) return errMalloc();
  m->buckets  = buckets;
  m->nbuckets = nbuckets;

  memset(m->buckets, 0, sizeof(*m->buckets) * m->nbuckets);
  // re-add nodes to buckets.
  node = nodes;
  while (node) {
    next = node->next;
    map_addnode(m, node);
    node = next;
  }

  return OK;
}
