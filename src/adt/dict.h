#ifndef DICT_H_
#define DICT_H_

/* Hash Tables Implementation.
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// -----------------------------------------------------------------------------
// General map.
// -----------------------------------------------------------------------------

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

// eva
#include "adt/value.h"
#include "base/error.h"

#define DICT_HT_INITIAL_SIZE 128

struct dict_entry_t {
        void                *key;
        struct value_t       v;
        struct dict_entry_t *next;
};

struct dict_ty_t {
        uint64_t (*hashFn)(const void *key);
        void *(*keyDup)(void *privdata, const void *key);
        void *(*valDup)(void *privdata, const void *obj);
        int (*keyCmp)(void *privdata, const void *key1, const void *key2);
        void (*keyFree)(void *privdata, void *key);
        void (*valFree)(void *privdata, void *obj);
};

struct dict_table_t {
        struct dict_entry_t **table;
        unsigned long         size;
        unsigned long         sizemask;
        unsigned long         used;
};

typedef struct {
        struct dict_ty_t   *type;
        void               *privdata;
        struct dict_table_t ht;
} dict_t;

dict_t *dictNew(struct dict_ty_t *type, void *privDataPtr);
void    dictFree(dict_t *d);

error_t dictExpand(dict_t *d, unsigned long size);

struct dict_entry_t *dictFind(dict_t *d, const void *key);

error_t              dictAdd(dict_t *d, void *key, void *val);
struct dict_entry_t *dictAddOrFind(dict_t *d, void *key, int *existed);
int                  dictReplace(dict_t *d, void *key, void *val);

// low-level.
struct dict_entry_t *dictAddRaw(dict_t *d, void *key,
                                struct dict_entry_t **existing);

#define dictGetKey(he)  ((he)->key)
#define dictGetData(he) ((he)->v.data)
#define dictGetI64(he)  ((he)->v.i64)
#define dictGetU64(he)  ((he)->v.u64)
#define dictGetF64(he)  ((he)->v.f64)

#define dictSetI64(entry, _val_)        \
        do {                            \
                (entry)->v.i64 = _val_; \
        } while (0)

#define dictSetU64(entry, _val_)        \
        do {                            \
                (entry)->v.u64 = _val_; \
        } while (0)

#define dictSetF64(entry, _val_)        \
        do {                            \
                (entry)->v.f64 = _val_; \
        } while (0)

#define dictSetKey(d, entry, _key_)                                  \
        do {                                                         \
                if ((d)->type->keyDup)                               \
                        (entry)->key =                               \
                            (d)->type->keyDup((d)->privdata, _key_); \
                else                                                 \
                        (entry)->key = (_key_);                      \
        } while (0)

#define dictSetData(d, entry, _val_)                                 \
        do {                                                         \
                if ((d)->type->valDup)                               \
                        (entry)->v.data =                            \
                            (d)->type->valDup((d)->privdata, _val_); \
                else                                                 \
                        (entry)->v.data = (_val_);                   \
        } while (0)

#define dictFreeKey(d, entry) \
        if ((d)->type->keyFree) (d)->type->keyFree((d)->privdata, (entry)->key)

#define dictFreeVal(d, entry)   \
        if ((d)->type->valFree) \
        (d)->type->valFree((d)->privdata, (entry)->v.data)

#define dictCompareKeys(d, key1, key2)                                      \
        (((d)->type->keyCmp) ? (d)->type->keyCmp((d)->privdata, key1, key2) \
                             : (key1) == (key2))

#define dictHashKey(d, key) (d)->type->hashFn(key)

#endif
