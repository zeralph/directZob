#pragma once
#include "../Types.h"
#include "../Mesh.h"
#include "../Sprite.h"
#include <string>
#undef PropertyNotify
#include <fbxsdk.h>

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	Mesh* LoadMesh(ZobFilePath zfp);
	Mesh* GetMesh(const std::string& name) const;
	Mesh* GetOrLoadMesh(ZobFilePath zp);
	const int GetNbMeshes() const { return (int)m_meshes.size(); }
	std::string GetMeshList();
	const Mesh* GetMesh(const int i) const;
	void UnloadAll(); 
	FbxManager* GetFbxManager() { return m_lSdkManager; }
	ZobSprite* CreateSprite();
private:
	std::vector<Mesh*> m_meshes;
	FbxManager* m_lSdkManager = NULL;
};