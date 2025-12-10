#include "ktest.h"
#include "kcli.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ktest_cmd
{
    KTEST_CMD_RUN_ALL,
    KTEST_CMD_RUN_ONE,
    KTEST_CMD_LIST,
};

struct ktest_opts
{
    enum ktest_cmd cmd;
    char const *name;
    bool stop_on_fail;
    bool list;
    bool verbose;
};

static struct ktest_opts
ktest_opts_parse(int const argc, char const *const *const argv)
{
    struct ktest_opts opts = {.cmd = KTEST_CMD_RUN_ALL};

    KCLI_PARSE(
        argc,
        argv,
        {
            .pos_name = "name",
            .ptr_str = &opts.name,
            .optional = true,
            .help = "Run single test by name",
        },
        {
            .short_name = 's',
            .long_name = "stop-on-fail",
            .ptr_flag = &opts.stop_on_fail,
            .help = "Stop tests on first failure",
        },
        {
            .short_name = 'l',
            .long_name = "list",
            .ptr_flag = &opts.list,
            .help = "List tests",
        },
        {
            .short_name = 'v',
            .long_name = "verbose",
            .ptr_flag = &opts.verbose,
            .help = "Enable extra logging",
        },
    );

    if (opts.name)
    {
        opts.cmd = KTEST_CMD_RUN_ONE;
    }

    if (opts.list)
    {
        opts.cmd = KTEST_CMD_LIST;
    }

    return opts;
}

struct ktest_state
{
    struct ktest_opts opts;
    bool done;
    bool prev_fail;
    char const *prev_name;
    int test_count;
    int fail_count;
};

#define ktest_verbosef(state, fmt, ...)                                        \
    do                                                                         \
    {                                                                          \
        if ((state)->opts.verbose)                                             \
        {                                                                      \
            printf(fmt "\n", __VA_ARGS__);                                     \
            fflush(stdout);                                                    \
        }                                                                      \
    } while (0)

#define ktest_infof(fmt, ...)                                                  \
    do                                                                         \
    {                                                                          \
        printf(fmt "\n", __VA_ARGS__);                                         \
        fflush(stdout);                                                        \
    } while (0)

static void ktest_complete_test(struct ktest_state *const state)
{
    if (state->prev_name)
    {
        if (state->prev_fail)
        {
            ++(state->fail_count);

            ktest_infof(
                "Test %d: %s FAIL",
                state->test_count,
                state->prev_name
            );

            if (state->opts.stop_on_fail)
            {
                state->done = true;
            }
        }
        else
        {
            ktest_verbosef(
                state,
                "Test %d: %s PASS",
                state->test_count,
                state->prev_name
            );
        }

        state->prev_name = NULL;
    }
}

static bool
ktest_define_test(struct ktest_state *const state, char const *const name)
{
    bool run_this;

    if (state->prev_name == name)
    {
        ktest_complete_test(state);
        run_this = false;
    }
    else
    {
        ++(state->test_count);

        if (state->done)
        {
            run_this = false;
        }
        else
        {
            switch (state->opts.cmd)
            {
                case KTEST_CMD_RUN_ALL:
                    run_this = true;
                    break;
                case KTEST_CMD_RUN_ONE:
                    if (0 == strcmp(state->opts.name, name))
                    {
                        run_this = true;
                        state->done = true;
                    }
                    else
                    {
                        run_this = false;
                    }
                    break;
                case KTEST_CMD_LIST:
                    run_this = false;
                    printf("%s\n", name);
                    break;
            }
        }
    }

    if (run_this)
    {
        ktest_verbosef(state, "Test %d: %s", state->test_count, name);
        state->prev_name = name;
    }

    return run_this;
}

static void ktest_assert_fail(
    struct ktest_state *const state,
    char const *const msg,
    char const *const filename,
    int const line
)
{
    state->prev_fail = true;
    ktest_infof("Assert failed: %s:%d", filename, line);
    ktest_infof("%s", msg);
}

#define KTEST_MAIN                                                             \
    static void ktest__main(struct ktest_state *ktest_state);                  \
                                                                               \
    int main(int const argc, char const *const *const argv)                    \
    {                                                                          \
        struct ktest_state state = {.opts = ktest_opts_parse(argc, argv)};     \
                                                                               \
        ktest__main(&state);                                                   \
                                                                               \
        switch (state.opts.cmd)                                                \
        {                                                                      \
            case KTEST_CMD_RUN_ALL:                                            \
            case KTEST_CMD_RUN_ONE:                                            \
                ktest_infof("%s", "");                                         \
                ktest_infof("Failures: %d", state.fail_count);                 \
                break;                                                         \
            case KTEST_CMD_LIST:                                               \
                break;                                                         \
        }                                                                      \
                                                                               \
        return state.fail_count > 0;                                           \
    }                                                                          \
                                                                               \
    static void ktest__main(struct ktest_state *ktest_state)

// Declare dummy int to cause "Redefinition" compiler error on duplicate name
#define KTEST(test_name)                                                       \
    int test_name;                                                             \
    (void)test_name;                                                           \
    while (ktest_define_test(ktest_state, #test_name))

#define ASSERT_TRUE(cond)                                                      \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            ktest_assert_fail(                                                 \
                ktest_state,                                                   \
                "  `" #cond "`",                                               \
                __FILE__,                                                      \
                __LINE__                                                       \
            );                                                                 \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

KTEST_MAIN
{
    KTEST(t_greet_name)
    {
        char *greeting = ktest_create_greeting("Kris");
        assert(greeting);

        ASSERT_TRUE(0 == strcmp(greeting, "Hello, Kris!"));

        free(greeting);
    }

    KTEST(t_fail)
    {
        ASSERT_TRUE(false);
    }

    KTEST(t_greet_null)
    {
        char *greeting = ktest_create_greeting(NULL);
        assert(greeting);

        ASSERT_TRUE(0 == strcmp(greeting, "Hello, World!"));

        free(greeting);
    }
}
