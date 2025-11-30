#include "strbuf.h"

#define ktl_slice str
#include "ktl_slice.c"
#undef ktl_slice

#define ktl_slice sv
#include "ktl_slice.c"
#undef ktl_slice

#define ktl_vec strbuf
#include "ktl_vec.c"
#define ktl_slice str
#include "ktl_vec_slice.c"
#undef ktl_slice
#define ktl_slice sv
#include "ktl_vec_slice.c"
#undef ktl_slice
#undef ktl_vec
