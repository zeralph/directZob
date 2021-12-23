#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/ZobObjects/ZOBGUID.h"

/*
TEST(ZOBGUID, Constructor)
{
	ZOBGUID g = ZOBGUID();
	std::string guid = g.ZobGuidToString();
	EXPECT_EQ(guid, "0");
}

TEST(ZOBGUID, Constructor2)
{
	ZOBGUID g;
	std::string guid;
	g = ZOBGUID(ZOBGUID::type_unknown, ZOBGUID::subtype_unknown);
	guid = g.ZobGuidToString();
	EXPECT_EQ(guid.size(), 10);
	guid = guid.substr(0, 4);
	EXPECT_EQ(guid, "1001");
	g = ZOBGUID(ZOBGUID::type_internal, ZOBGUID::subtype_behavior);
	guid = g.ZobGuidToString().substr(0, 4);
	EXPECT_EQ(guid, "2007");
}

TEST(ZOBGUID, FromString)
{
	std::string guid = std::string("4002123456");
	ZOBGUID g = ZOBGUID(guid);
	EXPECT_EQ(g.GetType(), ZOBGUID::type_scene);
	EXPECT_EQ(g.GetSubType(), ZOBGUID::subtype_zobOject);
	EXPECT_EQ(g.GetIdValue(), 4002123456);
	zobId* zid = g.GetIdAddress();
	EXPECT_EQ(*zid, 4002123456);
}

TEST(ZOBGUID, Regenerate)
{
	std::string guid = std::string("4002123456");
	ZOBGUID g = ZOBGUID(guid);
	g.ZobGuidRegenerate();
	EXPECT_EQ(g.GetType(), ZOBGUID::type_scene);
	EXPECT_EQ(g.GetSubType(), ZOBGUID::subtype_zobOject);
}
*/