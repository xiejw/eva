/* This is a simplified version for sds, heavily based on Salvatore's sds.
 *
 * SDSLib 2.0 -- A C dynamic strings library
 *
 * Copyright (c) 2020, Jianwei Xie
 * Copyright (c) 2006-2015, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2015, Oran Agra
 * Copyright (c) 2015, Redis Labs, Inc
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

#ifndef SDS_H_
#define SDS_H_

#include <assert.h>
#include <stdarg.h>  // va_list
#include <stdlib.h>

#include "base/defs.h"

typedef char *sds_t;

typedef struct {
  size_t len;
  size_t alloc;
} sdshdr;

sds_t sdsNew(const char *init);
sds_t sdsEmpty(void);
sds_t sdsEmptyWithCap(size_t cap);
sds_t sdsDup(const sds_t s);
void  sdsFree(sds_t s);

void  sdsReserve(_mut_ sds_t *s, size_t addlen);

#define sdsLen(s)       _sdsLen(s)
#define sdsCap(s)       _sdsCap(s)
#define sdsAvail(s)     _sdsAvail(s)
#define sdsSetLen(s, l) _sdsSetLen((s), (l))
#define sdsIncLen(s, l) _sdsIncLen((s), (l))
#define sdsSetCap(s, l) _sdsSetCap((s), (l))
#define sdsClear(s)     _sdsClear(s)

void sdsCat(_mut_ sds_t *s, const char *t);
void sdsCatSds(_mut_ sds_t *s, const sds_t t);
void sdsCatLen(_mut_ sds_t *s, const void *t, size_t len);
void sdsCatVprintf(_mut_ sds_t *s, const char *fmt, va_list ap);
void sdsCatPrintf(_mut_ sds_t *s, const char *fmt, ...);

void sdsCpyLen(_mut_ sds_t *s, const char *t, size_t len);
void sdsCpy(_mut_ sds_t *s, const char *t);

int sdsCmp(const sds_t s1, const sds_t s2);

// -----------------------------------------------------------------------------
// impl of static inline fns.
// -----------------------------------------------------------------------------

#define SDS_HDR(s) ((sdshdr *)((s) - (sizeof(sdshdr))))
static inline size_t _sdsLen(const sds_t s) { return s == NULL? 0:SDS_HDR(s)->len; }
static inline size_t _sdsCap(const sds_t s) { return s == NULL? 0:SDS_HDR(s)->alloc; }
static inline size_t _sdsAvail(const sds_t s) {
  if (s==NULL) return 0;
  sdshdr *p = SDS_HDR(s);
  return p->alloc - p->len;
}
static inline void _sdsSetLen(const sds_t s, size_t newlen) {
  assert(s!=NULL);
  SDS_HDR(s)->len = newlen;
}
static inline void _sdsIncLen(const sds_t s, size_t inc) {
  assert(s!=NULL);
  SDS_HDR(s)->len += inc;
}
static inline void _sdsSetCap(const sds_t s, size_t newcap) {
  assert(s!=NULL);
  SDS_HDR(s)->alloc = newcap;
}
static inline void _sdsClear(sds_t s) {
  assert(s!=NULL);
  sdsSetLen(s, 0);
  s[0] = '\0';
}

#endif
