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
    assert(state.fail_count == 2);
    assert(
        state.first_failed_test_name &&
        0 == strcmp(state.first_failed_test_name, "t_int_eq_fail")
    );
}

static void ktest__main(struct ktest_state *ktest_state)
{
    KTEST(t_int_eq_pass)
    {
        ASSERT_INT_EQ(100, 100);
    }
    KTEST(t_int_eq_fail)
    {
        ASSERT_INT_EQ(0, 100);
    }

    KTEST(t_int_neq_pass)
    {
        ASSERT_INT_NEQ(200, -200);
    }
    KTEST(t_int_neq_fail)
    {
        ASSERT_INT_NEQ(1, 1);
    }
}
