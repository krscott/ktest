#include "ktest.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

static void t_greet_name(void)
{
    char *greeting = ktest_create_greeting("Kris");
    assert(greeting);

    assert(0 == strcmp(greeting, "Hello, Kris!"));

    free(greeting);
}

static void t_greet_null(void)
{
    char *greeting = ktest_create_greeting(NULL);
    assert(greeting);

    assert(0 == strcmp(greeting, "Hello, World!"));

    free(greeting);
}

#define RUN(test)                                                              \
    do                                                                         \
    {                                                                          \
        printf("Test: " #test "\n");                                           \
        fflush(stdout);                                                        \
        test();                                                                \
    } while (0)

int main(void)
{
    RUN(t_greet_name);
    RUN(t_greet_null);

    return 0;
}
