#ifndef SHA256_H_
#define SHA256_H_

#include "adt/sds.h"

#define SHA224_256_BLOCK_SIZE (512 / 8)  // 32

struct sha256_t {
        uint64_t total_len;  // Total of bytes processed.
        unsigned char
            block[2 * SHA224_256_BLOCK_SIZE];  // Stores the unprocessed content
                                               // with `m_len_` bytes.
        uint64_t len;
        uint32_t h[8];
        int      finalized;
};

extern void    sha256Reset(struct sha256_t*);
extern error_t sha256Update(struct sha256_t*, const unsigned char*, uint64_t);
extern sds_t   sha256Digest(struct sha256_t*);

extern sds_t sha256DigestStr(const char*);

#endif
