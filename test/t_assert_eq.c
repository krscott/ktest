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
    assert(state.fail_count == 6);
    assert(
        state.first_failed_test_name &&
        0 == strcmp(state.first_failed_test_name, "t_int_eq_fail")
    );
}

static void ktest__main(struct ktest_state *ktest_state)
{
    KTEST(t_int_eq_pass)
    {
        ASSERT_INT_EQ(-100, -100);
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
        ASSERT_INT_NEQ(-1, -1);
    }

    KTEST(t_uint_eq_pass)
    {
        ASSERT_UINT_EQ(100, 100);
    }
    KTEST(t_uint_eq_fail)
    {
        ASSERT_UINT_EQ(0, 100);
    }

    KTEST(t_uint_neq_pass)
    {
        ASSERT_UINT_NEQ(123, 124);
    }
    KTEST(t_uint_neq_fail)
    {
        ASSERT_UINT_NEQ(1, 1);
    }

    KTEST(t_double_eq_pass)
    {
        ASSERT_FLOAT_APPROX(1.001, 0.999, 0.1);
        ASSERT_FLOAT_APPROX(-0.5, -0.5, 0.001);
    }
    KTEST(t_double_eq_fail)
    {
        ASSERT_FLOAT_APPROX(1.001, 0.999, 0.0001);
    }

    KTEST(t_double_neq_pass)
    {
        ASSERT_FLOAT_NOT_APPROX(1.0, 2.0, 0.1);
    }
    KTEST(t_double_neq_fail)
    {
        ASSERT_FLOAT_NOT_APPROX(10.0, 10.01, 0.1);
    }
}
