#include "MeshManager.h"
#include "DirectZob.h"
MeshManager::MeshManager()
{
	m_meshes.clear();
}

MeshManager::~MeshManager()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		delete (m_meshes[i]);
	}
	m_meshes.clear();
}

Mesh* MeshManager::LoadMesh(std::string& name, std::string& path, std::string& file)
{
	Mesh* outMesh = NULL;
	if (GetMesh(name) == NULL)
	{
		outMesh = new Mesh(name, path, file);
		m_meshes.push_back(outMesh);
	}
	return outMesh;
}

const Mesh* MeshManager::GetMesh(const int i) const
{
	if (i >= 0 && i < m_meshes.size())
	{
		return m_meshes[i];
	}
	return NULL;
}

Mesh* MeshManager::GetMesh(std::string& name) const
{
	for (std::vector<Mesh*>::const_iterator iter = m_meshes.begin(); iter != m_meshes.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}

std::string MeshManager::GetMeshList()
{
	std::string s = "[";
	for (int i = 0; i < m_meshes.size(); i++)
	{
		s.append("{\"name\":\"");
		s.append(m_meshes.at(i)->GetName());
		s.append("\"}");
		if (i < m_meshes.size() - 1)
		{
			s.append(",");
		}
	}
	s.append("]");
	return s;
}

void MeshManager::UnloadAll()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		delete (m_meshes[i]);
	}
	m_meshes.clear();
}