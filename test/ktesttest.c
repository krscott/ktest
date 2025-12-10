#include "ktest.h"
#include <stdlib.h>
#include <string.h>

// Start unit under test

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
#include <assert.h>
#include <stddef.h>

char *ktest_create_greeting(char const *name);

char *ktest_create_greeting(char const *const name)
{
    struct strbuf s = {0};

    strbuf_append_terminated(&s, "Hello, ");
    strbuf_append_terminated(&s, name ? name : "World");
    strbuf_append_terminated(&s, "!");

    return s.ptr;
}
// End unit under test

KTEST_MAIN
{
    KTEST(t_greet_name)
    {
        char *greeting = ktest_create_greeting("Kris");
        ASSERT_TRUE(greeting);

        ASSERT_TRUE(0 == strcmp(greeting, "Hello, Kris!"));

        free(greeting);
    }

#if 0
    KTEST(t_fail)
    {
        ASSERT_TRUE(false);
    }
#endif

    KTEST(t_greet_null)
    {
        char *greeting = ktest_create_greeting(NULL);
        ASSERT_TRUE(greeting);

        ASSERT_TRUE(0 == strcmp(greeting, "Hello, World!"));

        free(greeting);
    }
}
