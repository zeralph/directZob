#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/DirectZob.h"
#include "../../DirectZobEngine/Mesh.h"
#include "../../DirectZobEngine/Misc/ZobFilePath.h"
#include "Utils.h"

TEST(Mesh, Constructor_OBJ_MAN)
{
	std::string resPath = MyUtils::GetPath() + "resources\\";
	ZobFilePath zfp = ZobFilePath("mesh1", resPath, "man.obj", true);
	Mesh* m = new Mesh(&zfp);
	EXPECT_EQ(m->GetNbTriangles(), 48918);
	EXPECT_EQ(m->GetNbVertices(), 24461);
	EXPECT_EQ(m->GetNbNormals(), 24460);
	EXPECT_EQ(m->GetNbUvs(), 0);
	EXPECT_EQ(m->GetNbSubMeshes(), 0);
	EXPECT_EQ(m->GetName(), "mesh1");
	delete(m);
}

TEST(Mesh, Constructor_OBJ_CAMARO)
{
	ZobFilePath zfp = ZobFilePath("mesh2", MyUtils::GetPath() + "resources\\", "camaro.obj", true);
	Mesh* m = new Mesh(&zfp);
	EXPECT_EQ(m->GetNbTriangles(), 20044);
	EXPECT_EQ(m->GetNbVertices(), 13542);
	EXPECT_EQ(m->GetNbNormals(), 15838);
	EXPECT_EQ(m->GetNbUvs(), 4740);
	EXPECT_EQ(m->GetNbSubMeshes(), 0);
	EXPECT_EQ(m->GetName(), "mesh2");
	delete(m);
}

TEST(Mesh, Constructor_FBX_11)
{
	ZobFilePath zfp = ZobFilePath("mesh3", MyUtils::GetPath() + "resources\\", "11.fbx", true);
	Mesh* m = new Mesh(&zfp);
	EXPECT_EQ(m->GetNbTriangles(), 0);
	EXPECT_EQ(m->GetNbVertices(), 0);
	EXPECT_EQ(m->GetNbNormals(), 0);
	EXPECT_EQ(m->GetNbUvs(), 0);
	EXPECT_EQ(m->GetName(), "mesh3");
	EXPECT_EQ(m->GetNbSubMeshes(), 16);
	const Mesh* m2 = m->GetSubMesh(10);
	EXPECT_EQ(m2->GetNbTriangles(), 1460);
	EXPECT_EQ(m2->GetNbVertices(), 3418);
	EXPECT_EQ(m2->GetNbNormals(), 3418);
	EXPECT_EQ(m2->GetNbUvs(), 3418);
	EXPECT_EQ(m2->GetName(), "mesh3.hw_zhidao210_daolu_02");
	delete(m);
}
