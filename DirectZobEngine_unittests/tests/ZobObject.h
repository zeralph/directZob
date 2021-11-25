#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"

TEST(ZobObject, Constructor)
{
	ZobObject* z = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object");
	EXPECT_EQ(z->GetName(), "zob_object");
	EXPECT_EQ(z->GetType(), ZOBGUID::ZobType::type_scene);
	EXPECT_EQ(z->GetSubType(), ZOBGUID::ZobSubType::subtype_mesh);
	delete(z);
}
