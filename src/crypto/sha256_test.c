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

static char*
test_long_str()
{
        sds_t sds = sha256DigestStr(
            "Vanilla Policy Gradient is the most basic, entry-level algorithm "
            "in the deep RL space because it completely predates the advent of "
            "deep RL altogether. The core elements of VPG go all the way back "
            "to the late 80s / early 90s. It started a trail of research which "
            "ultimately led to stronger algorithms such as TRPO and then PPO "
            "soon after.\n\nA key feature of this line of work is that all of "
            "these algorithms are on-policy: that is, they don’t use old data, "
            "which makes them weaker on sample efficiency. But this is for a "
            "good reason: these algorithms directly optimize the objective you "
            "care about—policy performance—and it works out mathematically "
            "that you need on-policy data to calculate the updates. So, this "
            "family of algorithms trades off sample efficiency in favor of "
            "stability—but you can see the progression of techniques (from VPG "
            "to TRPO to PPO) working to make up the deficit on sample "
            "efficiency.");
        ASSERT_TRUE("digest", 0 == strcmp("2c2ca89ccbfa44169a32172be34361b85050"
                                          "0b0dd8121a1ef52d4bb43b767266",
                                          sds));
        sdsFree(sds);
        return NULL;
}

static char*
test_stream()
{
        const char* msg = "grape";

        struct sha256_t s;
        sha256Reset(&s);
        for (int i = 0; i < 5; i++) {
                sha256Update(&s, (unsigned char*)msg + i, 1);
        }
        sds_t sds = sha256Digest(&s);

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
        RUN_TEST(test_long_str);
        RUN_TEST(test_stream);
        return NULL;
}
