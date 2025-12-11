#ifndef KTEST_H_
#define KTEST_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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

struct ktest_state
{
    struct ktest_opts opts;
    bool done;
    bool prev_fail;
    char const *prev_name;
    char const *first_failed_test_name;
    int test_count;
    int fail_count;
};

struct ktest_opts
ktest_opts_parse(int const argc, char const *const *const argv);
void ktest_complete_test(struct ktest_state *const state);
bool ktest_define_test(struct ktest_state *const state, char const *const name);
int ktest_end(struct ktest_state const *const state);

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

#define ktest_info(msg) ktest_infof("%s", msg)

#define KTEST_MAIN                                                             \
    static void ktest__main(struct ktest_state *ktest_state);                  \
                                                                               \
    int main(int const argc, char const *const *const argv)                    \
    {                                                                          \
        struct ktest_state state = {.opts = ktest_opts_parse(argc, argv)};     \
                                                                               \
        ktest__main(&state);                                                   \
                                                                               \
        return ktest_end(&state);                                              \
    }                                                                          \
                                                                               \
    static void ktest__main(struct ktest_state *ktest_state)

// Declare dummy int to cause "Redefinition" compiler error on duplicate name
#define KTEST(test_name)                                                       \
    int test_name;                                                             \
    (void)test_name;                                                           \
    while (ktest_define_test(ktest_state, #test_name))

static inline void ktest_assert_fail(
    struct ktest_state *const state, char const *const filename, int const line
)
{
    state->prev_fail = true;
    ktest_infof("Assert %s:%d", filename, line);
}

#define ASSERT_TRUE(cond)                                                      \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Condition failed: `" #cond "`");                     \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

static inline void ktest_str_cmp(char const *left, char const *right)
{
    if (left)
    {
        ktest_infof("    Left : %s", left);
    }
    else
    {
        ktest_info("    Left is NULL");
    }
    if (right)
    {
        ktest_infof("    Right: %s", right);
    }
    else
    {
        ktest_info("    Right is NULL");
    }
}

#define ASSERT_STR_EQ(left, right)                                             \
    {                                                                          \
        char const *left_ = (left);                                            \
        char const *right_ = (right);                                          \
        if (!left_ || !right_ || 0 != strcmp(left_, right_))                   \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected equal strings: " #left " == " #right);      \
            ktest_str_cmp(left_, right_);                                      \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

#define ASSERT_STR_NEQ(left, right)                                            \
    {                                                                          \
        char const *left_ = (left);                                            \
        char const *right_ = (right);                                          \
        if (!left_ || !right_ || 0 == strcmp(left_, right_))                   \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected unequal strings: " #left " != " #right);    \
            ktest_str_cmp(left_, right_);                                      \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

static inline void ktest_int_cmp(int64_t left, int64_t right)
{
    ktest_infof("    Left : %ld", left);
    ktest_infof("    Right: %ld", right);
}

#define ASSERT_INT_EQ(left, right)                                             \
    {                                                                          \
        if (left != right)                                                     \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected equal ints: " #left " == " #right);         \
            ktest_int_cmp((int64_t)(left), (int64_t)(right));                  \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

#define ASSERT_INT_NEQ(left, right)                                            \
    {                                                                          \
        if (left == right)                                                     \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected unequal ints: " #left " != " #right);       \
            ktest_int_cmp((int64_t)(left), (int64_t)(right));                  \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

static inline void ktest_uint_cmp(uint64_t left, uint64_t right)
{
    ktest_infof("    Left : %lu", left);
    ktest_infof("    Right: %lu", right);
}

#define ASSERT_UINT_EQ(left, right)                                            \
    {                                                                          \
        if (left != right)                                                     \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected equal uints: " #left " == " #right);        \
            ktest_uint_cmp((uint64_t)(left), (uint64_t)(right));               \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

#define ASSERT_UINT_NEQ(left, right)                                           \
    {                                                                          \
        if (left == right)                                                     \
        {                                                                      \
            ktest_assert_fail(ktest_state, __FILE__, __LINE__);                \
            ktest_info("  Expected unequal uints: " #left " != " #right);      \
            ktest_uint_cmp((uint64_t)(left), (uint64_t)(right));               \
            continue;                                                          \
        }                                                                      \
    }                                                                          \
    static_assert(1, "")

#endif
