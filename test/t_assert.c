#include "ktest.inc"
#include <assert.h>
#include <string.h>

static void ktest__main(struct ktest_state *ktest_state);

int main(void)
{
    char const *argv[] = {"test", "-v"};
    int argc = KCLI_COUNTOF(argv);
    struct ktest_state state = {.opts = ktest_opts_parse(argc, argv)};

    ktest__main(&state);

    int result = ktest_end(&state);

    assert(result == 1);
    assert(
        state.first_failed_test_name &&
        0 == strcmp(state.first_failed_test_name, "t_fail")
    );
}

static void ktest__main(struct ktest_state *ktest_state)
{
    KTEST(t_pass_0)
    {
        ASSERT_TRUE(true);
    }

    KTEST(t_fail)
    {
        ASSERT_TRUE(false);
    }

    KTEST(t_pass_1)
    {
        ASSERT_TRUE(true);
    }
}
