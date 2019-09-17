#pragma once
#include "Types.h"
#include "Mesh.h"
#include <string>

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	void LoadMesh(std::string& name, std::string& path, std::string& file);
	Mesh* GetMesh(std::string& name) const;
	const int GetNbMeshes() const { return m_meshes.size(); }
	std::string GetMeshList();
	const Mesh* GetMesh(const int i) const;
	void UnloadAll(); 
private:
	std::vector<Mesh*> m_meshes;
};