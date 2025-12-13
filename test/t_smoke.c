#include "ktest.inc"

KTEST_MAIN
{
    KTEST(t_smoke)
    {
        ASSERT_TRUE(true);
        ASSERT_FALSE(false);
    }
}
