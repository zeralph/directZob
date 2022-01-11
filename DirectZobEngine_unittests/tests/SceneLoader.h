#pragma once
#include "../../dependencies/gtest/include/gtest/gtest.h"
#include "../../DirectZobEngine/DirectZob.h"
#include "../../DirectZobEngine/SceneLoader.h"
#include "Utils.h"

#define NB_OBJECTS 60
#define NB_ELEMENTS 38

TEST(SceneLoader, LoadScene)
{
	std::string name = "carTest.dzs";
	std::string path = MyUtils::GetPath() + "resources\\";
	SceneLoader::LoadScene(path, name, NULL);
	SceneLoader::Update(); 
	EXPECT_EQ(SceneLoader::GetNbZobObjectsInScene(), NB_OBJECTS);
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbMaterials(), NB_ELEMENTS);
	SceneLoader::UnloadScene();
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbMaterials(), 0);
	SceneLoader::LoadScene(path, name, NULL);
	SceneLoader::Update();
	EXPECT_EQ(DirectZob::GetInstance()->GetMaterialManager()->GetNbMaterials(), NB_ELEMENTS);
	EXPECT_EQ(SceneLoader::GetNbZobObjectsInScene(), NB_OBJECTS);
}
