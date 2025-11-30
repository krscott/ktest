#ifndef STRBUF_H_
#define STRBUF_H_

#include <stddef.h>

// Mutable string
struct str
{
    char *ptr;
    size_t len;
};
#define str__type char
#define str__mut 1
#define str__terminated true, '\0'

#define ktl_slice str
#include "ktl_slice.h"
#undef ktl_slice

// Immutable string view
struct sv
{
    char const *ptr;
    size_t len;
};
#define sv__type char
#define sv__terminated true, '\0'

#define ktl_slice sv
#include "ktl_slice.h"
#undef ktl_slice

// Dynamic string buffer
struct strbuf
{
    char *ptr;
    size_t len;
    size_t cap;
};
#define strbuf__type char
#define strbuf__terminated true, '\0'
// #define strbuf__infallible_alloc true
#define ktl_vec strbuf
#include "ktl_vec.h"
#define ktl_slice str
#include "ktl_vec_slice.h"
#undef ktl_slice
#define ktl_slice sv
#include "ktl_vec_slice.h"
#undef ktl_slice
#undef ktl_vec

#endif
