#include "MeshManager.h"

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

void MeshManager::LoadMesh(std::string& name, std::string& file, const Texture* tex)
{
	if (GetMesh(name) == NULL)
	{
		Mesh* t = new Mesh(name, file, tex);
		m_meshes.push_back(t);
	}
	else
	{
		//output error
	}
}

Mesh* MeshManager::GetMesh(std::string& name)
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