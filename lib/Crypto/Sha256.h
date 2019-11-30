#ifndef EVA_LIB_CRYPTO_SHA256_H
#define EVA_LIB_CRYPTO_SHA256_H

#include <string>

namespace eva {
namespace crypto {

class SHA256 {
 public:
  static const unsigned int DIGEST_SIZE = (256 / 8);

  SHA256() { reset(); };

  // Resets the internal states. Not necessary for newly created instance.
  void reset();

  // Appends `message` (length `len`) into the content for hash calculation.
  // Can be invoked repeatedly before `finalize`.
  void update(const unsigned char *message, unsigned int len);

  // Finalize the hash value calculation and store the value in `digest`.
  // `digest` must be at least `DIGEST_SIZE`.
  void finalize(unsigned char *digest);

 protected:
  typedef unsigned char uint8;
  typedef unsigned int uint32;
  typedef unsigned long long uint64;

  // First 32 bits of the fractional parts of the cube roots of the first 64
  // primes 2..311.
  const static uint32 sha256_k[];
  static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);  // 32

 protected:
  // Performs the transformation on `message` for `num_chunks` chunks. See
  // https://en.wikipedia.org/wiki/SHA-2#Pseudocode
  void transform(const unsigned char *message, unsigned int num_chunks);

 protected:
  // Total of bytes processed.
  unsigned int m_total_len_;

  // Stores the unprocessed content with `m_len_` bytes.
  unsigned char m_block_[2 * SHA224_256_BLOCK_SIZE];
  unsigned int m_len_;

  uint32 m_h[8];
};

std::string sha256(std::string input);

}  // namespace crypto
}  // namespace eva

#endif
