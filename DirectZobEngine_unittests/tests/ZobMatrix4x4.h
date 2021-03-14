#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/DirectZob.h"
#include "../../DirectZobEngine/Rendering/ZobMatrix4x4.h"


TEST(ZobMatrix4x4, Constructor)
{
    ZobMatrix4x4 m;
    EXPECT_EQ((m == ZobMatrix4x4::IdentityMatrix), false);
}

TEST(ZobMatrix4x4, Assignment)
{
    ZobMatrix4x4 m;
    float k = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m(i, j) = k;
            k++;
        }
    }
    k = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(m(i, j), k);
            k++;
        }
    } 
}

TEST(ZobMatrix4x4, Mul)
{
    ZobMatrix4x4 m;
    m.Mul(&ZobMatrix4x4::IdentityMatrix);
    EXPECT_EQ((m == ZobMatrix4x4::IdentityMatrix), true);
}