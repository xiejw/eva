#include "dict.h"

static void
_dictReset(struct dict_table_t *ht)
{
        ht->table    = NULL;
        ht->size     = 0;
        ht->sizemask = 0;
}

error_t
_dictInit(struct dict_t *d, struct dict_ty_t *type, void *privDataPtr)
{
        _dictReset(&d->ht);
        d->type     = type;
        d->privdata = privDataPtr;
        return OK;
}

/* Create a new hash table */
struct dict_t *
dictNew(struct dict_ty_t *type, void *privDataPtr)
{
        struct dict_t *d = malloc(sizeof(*d));
        _dictInit(d, type, privDataPtr);
        return d;
}

/* Our hash table capability is a power of two */
static unsigned long
_dictNextPower(unsigned long size)
{
        unsigned long i = DICT_HT_INITIAL_SIZE;

        if (size >= LONG_MAX) return LONG_MAX + 1LU;
        while (1) {
                if (i >= size) return i;
                i *= 2;
        }
}

error_t
_dictExpand(struct dict_t *d, unsigned long size)
{
        struct dict_table_t n; /* the new hash table */
        unsigned long       realsize = _dictNextPower(size);

        if (realsize == d->ht.size) return OK;

        /* Allocate the new hash table and initialize all pointers to NULL */
        n.size     = realsize;
        n.sizemask = realsize - 1;
        n.table    = calloc(realsize, sizeof(struct dict_entry_t *));

        /* Is this the first initialization? If so it's not really a rehashing
         * we just set the first hash table so that it can accept keys. */
        d->ht = n;
        return OK;
}

/* return DICT_ERR if expand was not performed */
error_t
dictExpand(struct dict_t *d, unsigned long size)
{
        return _dictExpand(d, size);
}

/* Expand the hash table if needed */
static error_t
_dictExpandIfNeeded(struct dict_t *d)
{
        /* If the hash table is empty expand it to the initial size. */
        if (d->ht.size == 0) return dictExpand(d, DICT_HT_INITIAL_SIZE);

        // TODO
        //
        // /* If we reached the 1:1 ratio, and we are allowed to resize the hash
        //  * table (global setting) or we should avoid it but the ratio between
        //  * elements/buckets is over the "safe" threshold, we resize doubling
        //  * the number of buckets. */
        // if (d->ht[0].used >= d->ht[0].size &&
        //     (dict_can_resize ||
        //      d->ht[0].used/d->ht[0].size > dict_force_resize_ratio) &&
        //     dictTypeExpandAllowed(d))
        // {
        //     return dictExpand(d, d->ht[0].used + 1);
        // }
        return OK;
}

/* Returns the index of a free slot that can be populated with
 * a hash entry for the given 'key'.
 * If the key already exists, -1 is returned
 * and the optional output parameter may be filled.
 */
static long
_dictKeyIndex(struct dict_t *d, const void *key, uint64_t hash,
              struct dict_entry_t **existing)
{
        unsigned long        idx;
        struct dict_entry_t *he;
        if (existing) *existing = NULL;

        /* Expand the hash table if needed */
        if (_dictExpandIfNeeded(d))
                return errEmitNote("failed to expand the table.");

        idx = hash & d->ht.sizemask;
        /* Search if this slot does not already contain the given key */
        he = d->ht.table[idx];
        while (he != NULL) {
                if (key == he->key || dictCompareKeys(d, key, he->key)) {
                        if (existing) *existing = he;
                        return -1;
                }
                he = he->next;
        }
        return idx;
}

/*
 * Low level add or find:
 *
 * This function adds the entry but instead of setting a value returns the
 * dictEntry structure to the user, that will make sure to fill the value
 * field as he wishes.
 *
 * This function is also directly exposed to the user API to be called
 * mainly in order to store non-pointers inside the hash value, example:
 *
 * entry = dictAddRaw(dict,mykey,NULL);
 * if (entry != NULL) dictSetSignedIntegerVal(entry,1000);
 *
 * Return values:
 *
 * If key already exists NULL is returned, and "*existing" is populated
 * with the existing entry if existing is not NULL.
 *
 * If key was added, the hash entry is returned to be manipulated by the caller.
 */
struct dict_entry_t *
dictAddRaw(struct dict_t *d, void *key, struct dict_entry_t **existing)
{
        long                 index;
        struct dict_entry_t *entry;
        struct dict_table_t *ht = &d->ht;

        /* Get the index of the new element, or -1 if the element already
         * exists. */
        if ((index = _dictKeyIndex(d, key, dictHashKey(d, key), existing)) ==
            -1)
                return NULL;

        /* Allocate the memory and store the new entry.
         * Insert the element in top, with the assumption that in a database
         * system it is more likely that recently added entries are accessed
         * more frequently. */
        entry            = malloc(sizeof(*entry));
        entry->next      = ht->table[index];
        ht->table[index] = entry;

        /* Set the hash entry fields. */
        dictSetKey(d, entry, key);
        return entry;
}

error_t
dictAdd(struct dict_t *d, void *key, void *val)
{
        struct dict_entry_t *entry = dictAddRaw(d, key, NULL);

        if (!entry) return errNew("entry already existed.");
        dictSetVal(d, entry, val);
        return OK;
}

/*
 * Add or Overwrite:
 *
 * Add an element, discarding the old value if the key already exists.
 * Return 1 if the key was added from scratch, 0 if there was already an
 * element with such key and dictReplace() just performed a value update
 * operation. */
int
dictReplace(struct dict_t *d, void *key, void *val)
{
        struct dict_entry_t *entry, *existing, auxentry;

        /* Try to add the element. If the key
         * does not exists dictAdd will succeed. */
        entry = dictAddRaw(d, key, &existing);
        if (entry) {
                dictSetVal(d, entry, val);
                return 1;
        }

        /* Set the new value and free the old one. Note that it is important
         * to do that in this order, as the value may just be exactly the same
         * as the previous one. In this context, think to reference counting,
         * you want to increment (set), and then decrement (free), and not the
         * reverse. */
        auxentry = *existing;
        dictSetVal(d, existing, val);
        dictFreeVal(d, &auxentry);
        return 0;
}

/* Add or Find:
 * dictAddOrFind() is simply a version of dictAddRaw() that always
 * returns the hash entry of the specified key, even if the key already
 * exists and can't be added (in that case the entry of the already
 * existing key is returned.)
 *
 * See dictAddRaw() for more information. */
struct dict_entry_t *
dictAddOrFind(struct dict_t *d, void *key)
{
        struct dict_entry_t *entry, *existing;
        entry = dictAddRaw(d, key, &existing);
        return entry ? entry : existing;
}

/* Destroy an entire dictionary */
error_t
_dictClear(struct dict_t *d, struct dict_table_t *ht)
{
        unsigned long i;

        /* Free all the elements */
        for (i = 0; i<ht->size> 0; i++) {
                struct dict_entry_t *he, *nextHe;

                if ((he = ht->table[i]) == NULL) continue;

                while (he) {
                        nextHe = he->next;
                        dictFreeKey(d, he);
                        dictFreeVal(d, he);
                        free(he);
                        he = nextHe;
                }
        }
        free(ht->table);
        _dictReset(ht);
        return OK; /* never fails */
}

/* Clear & Release the hash table */
void
dictFree(struct dict_t *d)
{
        if (d == NULL) return;
        _dictClear(d, &d->ht);
        free(d);
}

struct dict_entry_t *
dictFind(struct dict_t *d, const void *key)
{
        struct dict_entry_t *he;
        uint64_t             h, idx;

        h   = dictHashKey(d, key);
        idx = h & d->ht.sizemask;
        he  = d->ht.table[idx];
        while (he) {
                if (key == he->key || dictCompareKeys(d, key, he->key))
                        return he;
                he = he->next;
        }
        return NULL;
}
