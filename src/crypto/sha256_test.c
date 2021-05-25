#include "testing/testing.h"

#include "sha256.h"

#include <string.h>

static char*
test_empty_str()
{
        sds_t sds = sha256DigestStr("");
        ASSERT_TRUE("digest", 0 == strcmp("e3b0c44298fc1c149afbf4c8996fb92427ae"
                                          "41e4649b934ca495991b7852b855",
                                          sds));
        sdsFree(sds);
        return NULL;
}

static char*
test_str()
{
        sds_t sds = sha256DigestStr("grape");
        ASSERT_TRUE("digest", 0 == strcmp("0f78fcc486f5315418fbf095e71c0675ee07"
                                          "d318e5ac4d150050cd8e57966496",
                                          sds));
        sdsFree(sds);
        return NULL;
}

char*
run_crypto_sha256_suite()
{
        RUN_TEST(test_empty_str);
        RUN_TEST(test_str);
        return NULL;
}
