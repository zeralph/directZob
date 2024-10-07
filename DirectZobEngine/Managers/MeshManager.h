#pragma once
#include "../Types.h"
#include "../Mesh.h"
#include "../Sprite.h"
#include <string>
#undef PropertyNotify
#ifdef ENABLE_FBX_SUPPORT
#include <fbxsdk.h>
#endif
class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	Mesh* LoadMesh(ZobFilePath* zfp, ZobComponent* zm);
	Mesh* GetMesh(const std::string& name) const;
	Mesh* GetOrLoadMesh(ZobFilePath* zp, ZobComponent* zm);
	const int GetNbMeshes() const { return (int)m_meshes.size(); }
	std::string GetMeshList();
	const Mesh* GetMesh(const int i) const;
	void UnloadAll(); 
#ifdef ENABLE_FBX_SUPPORT
	FbxManager* GetFbxManager() { return m_lSdkManager; }
#endif
	//ZobSprite* CreateSprite();
private:
	std::vector<Mesh*> m_meshes;
#ifdef ENABLE_FBX_SUPPORT
	FbxManager* m_lSdkManager = NULL;
#endif
};