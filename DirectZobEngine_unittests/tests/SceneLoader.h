#pragma once
#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/DirectZob.h"
#include "../../DirectZobEngine/SceneLoader.h"
#include "Utils.h"

TEST(SceneLoader, LoadScene)
{
	std::string name = "carTest.dzs";
	std::string path = MyUtils::GetPath() + "resources\\";
	SceneLoader::LoadScene(path, name);
	SceneLoader::Update();
	EXPECT_EQ(SceneLoader::GetNbZobObjectsInScene(), 58);
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbTextures(), 42);
	SceneLoader::UnloadScene();
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbTextures(), 0);
	SceneLoader::LoadScene(path, name);
	SceneLoader::Update();
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbTextures(), 42);
	EXPECT_EQ(SceneLoader::GetNbZobObjectsInScene(), 58);
}
