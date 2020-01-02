#ifndef EVA_LIB_CRYPTO_SHA256_H
#define EVA_LIB_CRYPTO_SHA256_H

#include <string>

#include "lib/Support/Error.h"

namespace eva {
namespace crypto {

class SHA256 {
 protected:
  using uint8 = unsigned char;
  using uint32 = unsigned int;
  using uint64 = unsigned long long;

  // Assets type sizes.
  static_assert(sizeof(uint8) == 1);
  static_assert(sizeof(uint32) == 4);
  static_assert(sizeof(uint64) == 8);

 public:
  static const uint64 DIGEST_SIZE = (256 / 8);

  SHA256() { Reset(); };

  // Resets the internal states. Not necessary for newly created instance.
  void Reset();

  // Appends `message` (length `len`) into the content for digest calculation.
  //
  // Can be invoked repeatedly before any finalization method, e.g., `Digest`.
  void Update(const unsigned char *message, uint64 len);

  // Finalizes the digest calculation.
  std::string Digest();

  // Wrapper method to return digest for `message`.
  static std::string Digest(std::string message);

  // Wrapper method to generate digest for file specified by the path.
  //
  // The result will be filled in `digest`.
  static Error DigestForFile(const char *path, std::string &digest);

 protected:
  // First 32 bits of the fractional parts of the cube roots of the first 64
  // primes 2..311.
  const static uint32 sha256_k[];
  static const uint64 SHA224_256_BLOCK_SIZE = (512 / 8);  // 32

 protected:
  // Finalizes the checksum calculation and store the value in `digest`.
  // `digest` must be at least `DIGEST_SIZE`.
  void Finalize(unsigned char *digest);

  // Performs the transformation on `message` for `num_chunks` chunks. See
  // https://en.wikipedia.org/wiki/SHA-2#Pseudocode
  void Transform(const unsigned char *message, unsigned int num_chunks);

 protected:
  // Total of bytes processed.
  uint64 m_total_len_;

  // Stores the unprocessed content with `m_len_` bytes.
  unsigned char m_block_[2 * SHA224_256_BLOCK_SIZE];
  uint64 m_len_;

  uint32 m_h[8];
  bool finalized_;
};

}  // namespace crypto
}  // namespace eva

#endif
