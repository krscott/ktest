#include "ktest.h"

static void ktest__main(struct ktest_state *ktest_state);

int main(int const argc, char const *const *const argv)
{
    struct ktest_state state = {.opts = ktest_opts_parse(argc, argv)};
    ktest__main(&state);
    return ktest_end(&state);
}

static void ktest__main(struct ktest_state *ktest_state)
{
    KTEST(t_smoke)
    {
        ASSERT_TRUE(true);
    }
}
