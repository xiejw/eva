#include "sha256.h"

#include <assert.h>
#include <string.h>

#include <fcntl.h>  // open
#include <stdio.h>
#include <unistd.h>

#include "base/error.h"

#define DIGEST_SIZE (256 / 8)

#define SHA2_SHFR(x, n)   (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x)      (SHA2_ROTR(x, 2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x)      (SHA2_ROTR(x, 6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x)      (SHA2_ROTR(x, 7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x, 3))
#define SHA256_F4(x)      (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                        \
        {                                            \
                *((str) + 3) = (uint8_t)((x));       \
                *((str) + 2) = (uint8_t)((x) >> 8);  \
                *((str) + 1) = (uint8_t)((x) >> 16); \
                *((str) + 0) = (uint8_t)((x) >> 24); \
        }
#define SHA2_PACK32(str, x)                               \
        {                                                 \
                *(x) = ((uint32_t) * ((str) + 3)) |       \
                       ((uint32_t) * ((str) + 2) << 8) |  \
                       ((uint32_t) * ((str) + 1) << 16) | \
                       ((uint32_t) * ((str) + 0) << 24);  \
        }

// Initialize array of round constants: first 32 bits of the fractional parts
// of the cube roots of the first 64 primes 2..311
static const uint32_t sha256_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static void
sha256Transform(struct sha256_t* s, const unsigned char* message,
                unsigned int num_chunks)
{
        uint32_t             w[64];
        uint32_t             wv[8];
        uint32_t             t1, t2;
        const unsigned char* sub_block;
        int                  i;
        int                  j;
        for (i = 0; i < (int)num_chunks; i++) {
                sub_block = message + (i << 6);
                for (j = 0; j < 16; j++) {
                        SHA2_PACK32(&sub_block[j << 2], &w[j]);
                }
                for (j = 16; j < 64; j++) {
                        w[j] = SHA256_F4(w[j - 2]) + w[j - 7] +
                               SHA256_F3(w[j - 15]) + w[j - 16];
                }
                for (j = 0; j < 8; j++) {
                        wv[j] = s->h[j];
                }
                for (j = 0; j < 64; j++) {
                        t1 = wv[7] + SHA256_F2(wv[4]) +
                             SHA2_CH(wv[4], wv[5], wv[6]) + sha256_k[j] + w[j];
                        t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
                        wv[7] = wv[6];
                        wv[6] = wv[5];
                        wv[5] = wv[4];
                        wv[4] = wv[3] + t1;
                        wv[3] = wv[2];
                        wv[2] = wv[1];
                        wv[1] = wv[0];
                        wv[0] = t1 + t2;
                }
                for (j = 0; j < 8; j++) {
                        s->h[j] += wv[j];
                }
        }
}

void
sha256Reset(struct sha256_t* s)
{
        // First 32 bits of the fractional parts of the square roots of the
        // first 8 primes 2..19.
        s->h[0]      = 0x6a09e667;
        s->h[1]      = 0xbb67ae85;
        s->h[2]      = 0x3c6ef372;
        s->h[3]      = 0xa54ff53a;
        s->h[4]      = 0x510e527f;
        s->h[5]      = 0x9b05688c;
        s->h[6]      = 0x1f83d9ab;
        s->h[7]      = 0x5be0cd19;
        s->len       = 0;
        s->total_len = 0;
        s->finalized = 0;
}

error_t
sha256Update(struct sha256_t* s, const unsigned char* message, uint64_t len)
{
        if (s->finalized)
                return errNew(
                    "SHA256 has been finalized. Call `sha256Reset()`.");

        uint64_t current_len, chunk_remaining_len;

        chunk_remaining_len = SHA224_256_BLOCK_SIZE - s->len;
        current_len = len < chunk_remaining_len ? len : chunk_remaining_len;
        memcpy(&s->block[s->len], message, current_len);

        if (s->len + len < SHA224_256_BLOCK_SIZE) {
                // Current chunk has not been filled.
                s->len += len;
                return OK;
        }

        // Handles the s->block, which has been just filled.
        sha256Transform(s, s->block, 1);

        // Handles all (rest) full chunks in message (after current_len, until
        // the final full 512-bit chunk.
        uint64_t remaining_len_in_msg = len - current_len;
        uint64_t num_chunks = remaining_len_in_msg / SHA224_256_BLOCK_SIZE;
        const unsigned char* shifted_message = message + current_len;
        sha256Transform(s, shifted_message, num_chunks);

        // Now hanele the final, unfulfilled, chunk. Copy the content to
        // s->block, and bookmark the s->len and s->total_len.
        current_len = remaining_len_in_msg % SHA224_256_BLOCK_SIZE;
        memcpy(s->block, &shifted_message[num_chunks << 6], current_len);

        s->len = current_len;
        s->total_len += (num_chunks + 1) << 6;
        return OK;
}

static void
sha256Finalize(struct sha256_t* s, unsigned char* digest)
{
        s->finalized = 1;

        // Algorithrm:
        //
        // Pre-processing (Padding)
        // - begin with the original message of length L bits
        // - append a single '1' bit
        // - append K '0' bits, where K is the minimum number >= 0 such that L +
        // 1 +
        //   K + 64 is a multiple of 512
        // - append L as a 64-bit big-endian integer, making the total
        // post-processed
        //   length a multiple of 512 bits

        // Calculates the number of padding bytes.
        //
        // - We need to reserve 64 + 8 = 72 bits for the padding, so 9 bytes.
        // - So, if we do not have sufficient space, we need to allocate one
        // more
        //   chunk.

        assert(
            s->len <=
            SHA224_256_BLOCK_SIZE);  // Seems the following mod is not needed.
        uint64_t num_chunks = (1 + ((SHA224_256_BLOCK_SIZE - 9) <
                                    (s->len % SHA224_256_BLOCK_SIZE)));
        uint64_t pm_len     = num_chunks << 6;

        // Zeros the paddings. The first bit should be 1.
        memset(s->block + s->len, 0, pm_len - s->len);
        s->block[s->len] = 0x80;

        // Converts from total number of bytes to total number of bits.
        uint64_t total_bits = (s->total_len + s->len) << 3;
        // Append total bits as a 64-bit big-endian integer.  As total_bits is
        // 64 bit, we will unpack different parts of it separatedly.
        {
                // First part.
                uint32_t first_half = (total_bits >> 32) & 0xFFFFFFFFul;
                SHA2_UNPACK32(first_half, s->block + pm_len - 8);
        }
        {
                // Second part.
                uint32_t second_half = (total_bits & 0xFFFFFFFFul);
                SHA2_UNPACK32(second_half, s->block + pm_len - 4);
        }

        // Process the final `num_chunks` chunks.
        sha256Transform(s, s->block, num_chunks);

        // Unpack the value into `digest`.
        for (int i = 0; i < 8; i++) {
                SHA2_UNPACK32(s->h[i], &digest[i << 2]);
        }
}

sds_t
sha256Digest(struct sha256_t* s)
{
        sds_t buf = sdsEmpty();
        sdsReserve(&buf, 2 * DIGEST_SIZE);
        buf[2 * DIGEST_SIZE] = 0;

        // Prepares the buffer for digest and zeros it.
        unsigned char digest[DIGEST_SIZE];
        memset(digest, 0, DIGEST_SIZE);

        // Finalize the checksum calculation.
        sha256Finalize(s, digest);

        // Format the hash value.
        for (int i = 0; i < DIGEST_SIZE; i++)
                sprintf(buf + i * 2, "%02x", digest[i]);
        return buf;
}

sds_t
sha256DigestStr(const char* msg)
{
        // Generates the checksum.
        struct sha256_t s;
        sha256Reset(&s);
        sha256Update(&s, (unsigned char*)msg, strlen(msg));
        return sha256Digest(&s);
}

//
// Error SHA256::DigestForFile(const char *path, std::string &digest) {
//   constexpr unsigned int kBufferSize = 4096 * 16;
//
//   auto fd = open(path, O_RDONLY);
//   if (fd == -1) {
//     fprintf(stderr, "Faied to open file for %s digest: %s", path,
//             strerror(errno));
//     return kFAILURE;
//   }
//
//   unsigned char buffer[kBufferSize];
//   ssize_t size;
//
//   // Generates the checksum.
//   SHA256 hash{};
//   while ((size = read(fd, buffer, kBufferSize)) > 0)
//   hash.Update(buffer, size);
//
//   if (size == -1) {
//     fprintf(stderr, "Faied to read file for %s digest: %s", path,
//             strerror(errno));
//     return kFAILURE;
//   }
//
//   if (close(fd) == -1) {
//     fprintf(stderr, "Faied to close file for %s digest: %s", path,
//             strerror(errno));
//     return kFAILURE;
//   }
//
//   digest = hash.Digest();
//   return kOK;
// }
//
// }  // namespace eva::crypto
