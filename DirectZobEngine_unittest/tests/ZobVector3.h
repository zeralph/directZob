#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../DirectZobEngine/DirectZob.h"



TEST(ZobVector3, Constructor)
{
    ZobVector3 z;
    EXPECT_EQ(0, z.x);
    EXPECT_EQ(0, z.y);
    EXPECT_EQ(0, z.z);
    EXPECT_EQ(1, z.w);
}