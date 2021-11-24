#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/DirectZob.h"
#include "../../DirectZobEngine/Mesh.h"
#include "Utils.h"

TEST(Mesh, Constructor_OBJ_MAN)
{
	std::string name = "mesh1";
	std::string path = MyUtils::GetPath() + "resources\\";
	std::string file = "man.obj";
	Mesh* m = new Mesh(name, path, file, true);
	EXPECT_EQ(m->GetNbTriangles(), 48918);
	EXPECT_EQ(m->GetNbVertices(), 24461);
	EXPECT_EQ(m->GetNbNormals(), 24460);
	EXPECT_EQ(m->GetNbUvs(), 0);
	EXPECT_EQ(m->GetNbSubMeshes(), 0);
	EXPECT_EQ(m->GetName(), name);
	delete(m);
}

TEST(Mesh, Constructor_OBJ_CAMARO)
{
	std::string name = "mesh2";
	std::string path = MyUtils::GetPath() + "resources\\";
	std::string file = "camaro.obj";
	Mesh* m = new Mesh(name, path, file, true);
	EXPECT_EQ(m->GetNbTriangles(), 20044);
	EXPECT_EQ(m->GetNbVertices(), 13542);
	EXPECT_EQ(m->GetNbNormals(), 15838);
	EXPECT_EQ(m->GetNbUvs(), 4740);
	EXPECT_EQ(m->GetNbSubMeshes(), 0);
	EXPECT_EQ(m->GetName(), name);
	delete(m);
}

TEST(Mesh, Constructor_FBX_11)
{
	std::string name = "mesh1";
	std::string path = MyUtils::GetPath() + "resources\\";
	std::string file = "11.fbx";
	Mesh* m = new Mesh(name, path, file, true);
	EXPECT_EQ(m->GetNbTriangles(), 0);
	EXPECT_EQ(m->GetNbVertices(), 0);
	EXPECT_EQ(m->GetNbNormals(), 0);
	EXPECT_EQ(m->GetNbUvs(), 0);
	EXPECT_EQ(m->GetName(), name);
	EXPECT_EQ(m->GetNbSubMeshes(), 16);
	const Mesh* m2 = m->GetSubMesh(10);
	EXPECT_EQ(m2->GetNbTriangles(), 1460);
	EXPECT_EQ(m2->GetNbVertices(), 3418);
	EXPECT_EQ(m2->GetNbNormals(), 3418);
	EXPECT_EQ(m2->GetNbUvs(), 3418);
	EXPECT_EQ(m2->GetName(), "mesh1.hw_zhidao210_daolu_02");
	delete(m);
}
