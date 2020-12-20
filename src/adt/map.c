#include "adt/map.h"

// this is not the full structure
// the space will look like
//
//     nodt_t | key | value
//      \-> pointer header
typedef struct map_node_t {
  unsigned           hash;    // hash value. see `map_hash`.
  void *             pvalue;  // point to the address of content of value.
  struct map_node_t *next;    // next map node.
                              // other fields allocated by malloc after here.
} map_node_t;

static unsigned    map_hash(const char *str);
static map_node_t *map_newnode(const char *key, void *value, int vsize);

static int map_bucketidx(map_base_t *m, unsigned hash) {
  /* If the implementation is changed to allow a non-power-of-2 bucket count,
   * the line below should be changed to use mod instead of AND */
  return hash & (m->nbuckets - 1);
}

static void map_addnode(map_base_t *m, map_node_t *node) {
  int n         = map_bucketidx(m, node->hash);
  node->next    = m->buckets[n];
  m->buckets[n] = node;
}

static map_node_t **map_getref(map_base_t *m, const char *key) {
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

int map_set_(map_base_t *m, const char *key, void *value, int vsize) {
  int          n, err = OK;
  map_node_t **next, *node;
  /* Find & replace existing node */
  next = map_getref(m, key);
  if (next) {
    memcpy((*next)->pvalue, value, vsize);
    return 0;
  }
  /* Add new node */
  node = map_newnode(key, value, vsize);
  if (node == NULL) goto fail;
  if (m->nnodes >= m->nbuckets) {
    n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
    // TODO err = map_resize(m, n);
    if (err) goto fail;
  }
  map_addnode(m, node);
  m->nnodes++;
  return 0;
fail:
  if (node) free(node);
  return -1;
}

// impl
unsigned map_hash(const char *str) {
  unsigned hash = 5381;
  while (*str) hash = ((hash << 5) + hash) ^ *str++;
  return hash;
}

map_node_t *map_newnode(const char *key, void *value, int vsize) {
  // calculate the offset of the address for `value`.
  // - `voffset` considers the padding of the `key`.
  // - `sizeof` is `unsigned long` so, the % is always positive result.
  int ksize = strlen(key) + 1;
  int voffset = ksize + ((sizeof(void *) - ksize) % sizeof(void *));

  map_node_t *node = malloc(sizeof(*node) + voffset + vsize);
  if (!node) return NULL;

  memcpy(node + 1, key, ksize);
  node->hash   = map_hash(key);
  node->pvalue = ((char *)(node + 1)) + voffset;
  memcpy(node->pvalue, value, vsize);
  return node;
}
