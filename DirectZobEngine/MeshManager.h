#pragma once
#include "Types.h"
#include "Mesh.h"
#include <string>

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	void LoadMesh(std::string& name, std::string& file, const Texture* tex);
	Mesh* GetMesh(std::string& name);

private:
	std::vector<Mesh*> m_meshes;
};