#include "pch.h"
#include "tests/ZobVector3.h"
#include "tests/ZobMatrix4x4.h"
#include "tests/ZobObject.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}