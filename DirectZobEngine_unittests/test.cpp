#include "pch.h"
#include "tests/ZobVector3.h"
#include "tests/ZobMatrix4x4.h"
#include "tests/ZobObject.h"
#include "tests/Mesh.h"
#include "tests/SceneLoader.h"

int main(int argc, char** argv)
{
    DirectZob* d = new DirectZob();
    d->Init(NULL, 0, 0, false);
    DirectZob::SetLogLevel(DirectZob::eDirectZobLogLevel_error);
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    delete d;
    return 0;
}