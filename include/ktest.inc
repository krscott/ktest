#include "ktest.h" // IWYU pragma: export

#include "kcli.inc" // IWYU pragma: export
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct ktest_opts
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

void ktest_complete_test(struct ktest_state *const state)
{
    if (state->prev_name)
    {
        if (state->prev_fail)
        {
            if (!state->first_failed_test_name)
            {
                state->first_failed_test_name = state->prev_name;
            }
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
    }

    state->prev_name = NULL;
    state->prev_fail = false;
}

bool ktest_define_test(struct ktest_state *const state, char const *const name)
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
                default:
                    assert(false);
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

int ktest_end(struct ktest_state const *const state)
{
    switch (state->opts.cmd)
    {
        case KTEST_CMD_RUN_ALL:
        case KTEST_CMD_RUN_ONE:
            ktest_infof("%s", "");
            if (state->first_failed_test_name)
            {
                ktest_infof(
                    "First failed test: %s",
                    state->first_failed_test_name
                );
            }
            ktest_infof("Total failures: %d", state->fail_count);
            break;
        case KTEST_CMD_LIST:
            break;
    }

    return state->fail_count > 0;
}
