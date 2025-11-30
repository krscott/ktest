#include "cstart.h"
#include "strbuf.h"
#include <assert.h>
#include <stddef.h>

char *cstart_create_greeting(char const *const name)
{
    bool ok = true;
    struct strbuf s = {0};

    ok = strbuf_append_terminated(&s, "Hello, ");
    if (!ok)
    {
        goto error;
    }

    ok = strbuf_append_terminated(&s, name ? name : "World");
    if (!ok)
    {
        goto error;
    }

    ok = strbuf_append_terminated(&s, "!");

error:
    if (ok)
    {
        return s.ptr;
    }
    else
    {
        strbuf_deinit(&s);
        return NULL;
    }
}
