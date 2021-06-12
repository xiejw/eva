#ifndef SHA256_H_
#define SHA256_H_

#include <stdint.h>

// mlvm
#include "adt/sds.h"

#define SHA224_256_BLOCK_SIZE (512 / 8)  // 64 bytes

struct sha256_t {
        // All internal fields.
        unsigned char block[SHA224_256_BLOCK_SIZE];  // Unprocessed bytes.
        uint64_t      len;        // Unprocessed bytes stored in block.
        uint64_t      total_len;  // Total of bytes processed in history.
        uint32_t      h[8];       // Hash values.
        int           finalized;
};

extern void    sha256Reset(struct sha256_t *);
extern error_t sha256Update(struct sha256_t *, const unsigned char *, uint64_t);
extern sds_t   sha256Digest(struct sha256_t *);

extern sds_t sha256DigestStr(const char *);

#endif
