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

TEST(ZobObject, chain_parent)
{
	ZobObject* z1 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_1");
	ZobObject* z2 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	ZobObject* z3 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	z2->SetParent(z1);
	z3->SetParent(z2);
	EXPECT_EQ(z2->GetParent(), z1);
	EXPECT_EQ(z1->GetNbChildren(), 1);
	EXPECT_EQ(z2->GetNbChildren(), 1);
	EXPECT_EQ(z3->GetNbChildren(), 0);
	EXPECT_EQ(z3->GetParent(), z2);
	z3->SetParent(z1);
	EXPECT_EQ(z3->GetParent(), z1);
	EXPECT_EQ(z2->GetNbChildren(), 0);
	EXPECT_EQ(z1->GetNbChildren(), 2);
	delete(z3);
	delete(z2);
	delete(z1);
}

TEST(ZobObject, chain_move)
{
	ZobObject* z1 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_1");
	ZobObject* z2 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	ZobObject* z3 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	z2->SetParent(z1);
	z3->SetParent(z2);
	EXPECT_EQ(z2->GetParent(), z1);
	z1->SetWorldPosition(2, 2, 2);
	EXPECT_EQ(z1->GetWorldPosition(), z2->GetWorldPosition());
	z2->SetLocalPosition(4, 0, 0);
	EXPECT_EQ(z3->GetWorldPosition(), ZobVector3(6,2,2));
	z1->SetLocalPosition(0, 0, 0);
	EXPECT_EQ(z3->GetWorldPosition(), ZobVector3(4, 0, 0));
	z1->SetWorldPosition(10, 9, 8);
	EXPECT_EQ(z3->GetWorldPosition(), ZobVector3(14, 9, 8));
	z2->SetWorldPosition(10, 10, 10);
	z3->SetLocalPosition(-1, -1, -1);
	EXPECT_EQ(z3->GetWorldPosition(), ZobVector3(9, 9, 9));
	z1->SetWorldPosition(0, 0, 0);
	z2->SetLocalPosition(1, 1, 1);
	z3->SetLocalPosition(2, 1, 1);
	ZobVector3 v = z3->GetWorldPosition();
	EXPECT_EQ(v, ZobVector3(3, 2, 2));
	z3->SetWorldPosition(2, 1, 1);
	v = z3->GetWorldPosition();
	EXPECT_EQ(v, ZobVector3(2, 1, 1));
	delete(z3);
	delete(z2);
	delete(z1);
}

TEST(ZobObject, chain_scale)
{
	ZobObject* z1 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_1");
	ZobObject* z2 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	ZobObject* z3 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	z2->SetParent(z1);
	z3->SetParent(z2);
	z1->SetWorldPosition(0, 0, 0);
	z2->SetWorldPosition(1, 2, 3);
	z1->SetLocalScale(1, 2, 3);
	EXPECT_EQ(z2->GetWorldPosition(), ZobVector3(1, 4, 9));
	EXPECT_EQ(z3->GetWorldPosition(), ZobVector3(1, 4, 9));
	z2->SetLocalScale(2, 2, 2);
	ZobVector3 v = z3->GetWorldPosition();
	EXPECT_EQ(v, ZobVector3(1, 4, 9));
	//z2->SetLocalPosition(1, 0, 0);
	//v = z3->GetWorldPosition();
	//EXPECT_EQ(v, ZobVector3(2, 4, 9));
	delete(z3);
	delete(z2);
	delete(z1);
}

TEST(ZobObject, chain_rotate)
{
	ZobObject* z1 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_1");
	ZobObject* z2 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	ZobObject* z3 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	z2->SetParent(z1);
	z3->SetParent(z2);
	z1->SetWorldPosition(0, 0, 0);
	z2->SetLocalPosition(1, 1, 1);
	z3->SetLocalPosition(1, 1, 1);

	z1->SetLocalRotation(0, 30, 0);
	z2->SetLocalRotation(0, 30, 0);
	z3->SetLocalRotation(0, -60, 0);
	EXPECT_EQ(z3->GetWorldRotation(), ZobVector3(0, 0, 0));

	delete(z3);
	delete(z2);
	delete(z1);
}

TEST(ZobObject, chain_all)
{
	ZobObject* z1 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_1");
	ZobObject* z2 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	ZobObject* z3 = new ZobObject(ZOBGUID::ZobType::type_scene, ZOBGUID::ZobSubType::subtype_mesh, "zob_object_2");
	z2->SetParent(z1);
	z3->SetParent(z2);
	z1->SetLocalPosition(-3.f, 2.f, -2.f);
	z1->SetLocalRotation(34.f, 66.f, 21.f);
	z1->SetLocalScale(1.8f, 2.f, 3.f);
	z2->SetLocalPosition(2.f, 4.f, -2.f);
	z2->SetLocalRotation(34.f, -8.f, -21.f);
	z2->SetLocalScale(0.2f, 0.2f, 0.4f);
	z3->SetLocalPosition(-6.f, -6.f, -5.f);
	z3->SetLocalRotation(8.f, 5.f, 7.f);
	z3->SetLocalScale(2.f, 2.f, 5.f);
	ZobVector3 v = z3->GetWorldPosition();
	EXPECT_EQ(v, ZobVector3(-11.5383844f, 14.8799629f, -6.68275261f)) << "World position error : " << v.ToString();
	v = z3->GetWorldRotation();
	EXPECT_EQ(v, ZobVector3(43.886f, 62.586f, -11.887f)) << "World rotation error : " << v.ToString();
	v = z3->GetWorldScale();
	EXPECT_EQ(v, ZobVector3(0.720, 0.8, 6)) << "World scale error : " << v.ToString();

	delete(z3);
	delete(z2);
	delete(z1);
}