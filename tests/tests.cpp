#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "../uint1024_t.c"

using namespace ::testing;
using ::testing::Return;

TEST(COMP, Whitebox){
    uint1024_t a = from_uint(5);
    uint1024_t b = from_uint(5);
    bool f = comp(a, b);
    EXPECT_TRUE(f);

    for(int i = 0; i < 32; i++){
        a.a[i] = 4294967295;
        b.a[i] = 0;
    }
    f = comp(a, b);
    EXPECT_FALSE(f);
    for(int i = 0; i < 32; i++){
        a.a[i] = i;
        b.a[i] = i;
    }
    f = comp(a, b);
    EXPECT_TRUE(f);
}

TEST(ADD_OPP, WhiteBox){
    uint1024_t a = from_uint(1);
    uint1024_t b = from_uint(2);
    struct uint1024_t c = from_uint(3);
    bool f = comp(add_op(a, b), c);
    EXPECT_TRUE(f);

    a.a[0] = 3735928559; //deadbeef
    b.a[0] = 305419896; //12345678


    c.a[0] = 4041348455; //foe21567

    f = comp(add_op(a, b), c);
    EXPECT_TRUE(f);

    a.a[0] = 305419896; //12345678
    b.a[0] = 3203391149; //beefdead
    a.a[1] = 3735928559; //deadbeef
    b.a[1] = 2271560481; //87654321



    c.a[0] = 3508811045; //d1243525
    c.a[1] = 1712521744; //66130210
    c.a[2] = 1;

    f = comp(add_op(a, b), c);
    EXPECT_TRUE(f);

    for (int i = 0; i < 32; i++){
        a.a[i] = 4294967295;
        b.a[i] = 0;
        c.a[i] = 0;
    }
    c.a[0] = 1;
    f = comp(add_op(a, c), b);
    EXPECT_TRUE(f);
}

TEST(SUB_OPP, WhiteBox){
    uint1024_t a = from_uint(1);
    uint1024_t b = from_uint(0);
    bool f = comp(a, subtr_op(a, b));
    EXPECT_TRUE(f);

    a = from_uint(98765);
    b = from_uint(12345);
    uint1024_t res = from_uint(86420);
    f = comp(res, subtr_op(a, b));
    EXPECT_TRUE(f);

    a.a[1] = 1;
    a.a[0] = 0;
    b.a[0] = 1;
    res.a[0] = 4294967295;
    f = comp(res, subtr_op(a, b));
    EXPECT_TRUE(f);

    for (int i = 0; i < 32; i++){
        a.a[i] = 0;
        b.a[i] = 0;
        res.a[i] = 4294967295;
    }
    b.a[0] = 1;
    f = comp(res, subtr_op(a, b));
    EXPECT_TRUE(f);
}

TEST(MULT_OP, WhiteBox){
    uint1024_t a = from_uint(2);
    uint1024_t b = from_uint(2);
    uint1024_t res = from_uint(4);
    bool f = comp(res, mult_op(a, b));
    EXPECT_TRUE(f);

    a.a[0] = 0;
    b.a[0] = 12345;
    res.a[0] = 0;
    f = comp(res, mult_op(a, b));
    EXPECT_TRUE(f);

    a.a[0] = 305419896; //12345678
    b.a[0] = 3735928559; //deadbeef
    res.a[0] = 1445054984; //5621ca08
    res.a[1] = 265666030; //fd5bdee
    f = comp(res, mult_op(a, b));
    EXPECT_TRUE(f);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}