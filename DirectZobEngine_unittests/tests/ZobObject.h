#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"

TEST(ZobObject, Constructor)
{
	ZobObject* z = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "a");
	//EXPECT_NE(z, NULL);
	EXPECT_EQ(z->GetName(), "a");
	delete(z);
}
