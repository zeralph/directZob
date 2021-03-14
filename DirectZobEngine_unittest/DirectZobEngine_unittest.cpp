#include "../dependencies/gtest/include/gtest/gtest.h"
#include "../DirectZobEngine/DirectZob.h"
#include "tests/ZobVector3.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

