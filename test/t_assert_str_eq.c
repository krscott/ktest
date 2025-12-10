#include "kcli.h"
#include "ktest.h"
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
    assert(state.fail_count == 4);
    assert(
        state.first_failed_test_name &&
        0 == strcmp(state.first_failed_test_name, "t_fail_eq")
    );
}

static void ktest__main(struct ktest_state *ktest_state)
{
    KTEST(t_pass_eq)
    {
        ASSERT_STR_EQ("foo", "foo");
    }

    KTEST(t_pass_neq)
    {
        ASSERT_STR_NEQ("foo", "bar");
    }

    KTEST(t_fail_eq)
    {
        ASSERT_STR_EQ("foo", "bar");
    }

    KTEST(t_fail_neq)
    {
        ASSERT_STR_NEQ("foo", "foo");
    }

    KTEST(t_fail_left_null)
    {
        ASSERT_STR_EQ(NULL, "foo");
    }

    KTEST(t_fail_right_null)
    {
        ASSERT_STR_NEQ("foo", NULL);
    }
}
