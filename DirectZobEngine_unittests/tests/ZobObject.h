#include "../../dependencies/gtest/include/gtest/gtest.h"
//#include "../../dependencies/gainput/include/gainput/gainput.h"
//#include "../../dependencies/reactphysics3d/include/reactphysics3d/reactphysics3d.h"
//#include "../../dependencies/fbxsdk/windows/include/fbxsdk.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"

TEST(ZobObject, Constructor)
{
	ZobObject* z = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "a");
	//EXPECT_NE(z, NULL);
	EXPECT_EQ(z->GetName(), "a");
}
