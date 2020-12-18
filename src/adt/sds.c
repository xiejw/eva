#include "adt/sds.h"

#include <stdio.h>  // vsnprintf
#include <stdlib.h>
#include <string.h>  // memcpy/memset

const char* SDS_NOINIT = "SDS_NOINIT";

/* Create a new sds string with the content specified by the 'init' pointer
 * and 'initlen'.
 *
 * Special values of `init`:
 * - If NULL is used for 'init' the string is initialized with zero bytes.
 * - If SDS_NOINIT is used, the buffer is left uninitialized;
 *
 * The string is always null-termined (all the sds strings are, always) so
 * even if you create an sds string with:
 *
 *     mystring = sdsNewLen("abc",3);
 *
 * You can print the string with printf() as there is an implicit \0 at the
 * end of the string. However the string is binary safe and can contain
 * \0 characters in the middle, as the length is stored in the sds header.
 *
 * The sdsLen(s) will be `initlen` even `strlen(init)` is different (larger or
 * smaller).
 *
 * To allocate a large, uninitialized, do the following:
 *
 *     sds_t s = sdsNew(SDS_NOINIT, allocates_size);
 *     sdsClear(s);
 *
 */
sds_t sdsNewLen(const void* init, size_t initlen) {
  int   hdrlen = sizeof(sdshdr);
  void* buf    = malloc(hdrlen + initlen + 1);
  if (buf == NULL) return NULL;

  if (init == SDS_NOINIT) {
    init = NULL;
  } else if (init == NULL) {
    memset(buf, 0, hdrlen + initlen + 1);
  }

  sdshdr* phdr = (sdshdr*)buf;
  phdr->len    = initlen;
  phdr->alloc  = initlen;

  sds_t s = (sds_t)buf + hdrlen;
  if (initlen && init) memcpy(s, init, initlen);
  s[initlen] = '\0';
  return s;
}

sds_t sdsNew(const char* init) {
  size_t initlen = (init == NULL) ? 0 : strlen(init);
  return sdsNewLen(init, initlen);
}

void sdsFree(sds_t s) {
  if (s == NULL) return;
  free((void*)SDS_HDR(s));
}
sds_t sdsEmpty() { return sdsNewLen("", 0); }
sds_t sdsDup(const sds_t s) { return sdsNewLen(s, sdsLen(s)); }

void sdsReserve(sds_t* s, size_t addlen) {
  size_t avail = sdsAvail(*s);
  if (avail >= addlen) return;

  size_t len    = sdsLen(*s);
  size_t newlen = len + addlen;

  newlen *= 2;

  int   hdrlen = sizeof(sdshdr);
  void* buf    = SDS_HDR(*s);
  buf          = realloc(buf, hdrlen + newlen + 1);
  if (buf == NULL) {
    *s = NULL;
    return;
  }
  *s = (sds_t)buf + hdrlen;
  sdsSetCap(*s, newlen);
}

void sdsCatLen(sds_t* s, const void* t, size_t len) {
  size_t curlen = sdsLen(*s);
  sdsReserve(s, len);
  if (*s == NULL) return;
  size_t newlen = curlen + len;
  memcpy((*s) + curlen, t, len);
  sdsSetLen(*s, newlen);
  (*s)[newlen] = '\0';
}

void sdsCat(sds_t* s, const char* t) { sdsCatLen(s, t, strlen(t)); }
void sdsCatSds(sds_t* s, const sds_t t) { sdsCatLen(s, t, sdsLen(t)); }
void sdsCatPrintf(sds_t* s, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  sdsCatVprintf(s, fmt, ap);
  va_end(ap);
}

void sdsCatVprintf(sds_t* s, const char* fmt, va_list ap) {
  va_list cpy;
  char    staticbuf[1024], *buf = staticbuf;
  size_t  buflen = strlen(fmt) * 2;

  /* We try to start using a static buffer for speed.
   * If not possible we revert to heap allocation. */
  if (buflen > sizeof(staticbuf)) {
    buf = malloc(buflen);
    if (buf == NULL) {
      *s = NULL;
      return;
    }
  } else {
    buflen = sizeof(staticbuf);
  }

  /* Try with buffers two times bigger every time we fail to
   * fit the string in the current buffer size. */
  while (1) {
    buf[buflen - 2] = '\0';
    va_copy(cpy, ap);
    vsnprintf(buf, buflen, fmt, cpy);
    va_end(cpy);
    if (buf[buflen - 2] != '\0') {
      if (buf != staticbuf) free(buf);
      buflen *= 2;
      buf = malloc(buflen);
      if (buf == NULL) {
        *s = NULL;
        return;
      }
      continue;
    }
    break;
  }

  /* Finally concat the obtained string to the SDS string and return it. */
  sdsCat(s, buf);
  if (buf != staticbuf) free(buf);
}
