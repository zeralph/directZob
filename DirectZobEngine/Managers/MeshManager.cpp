#include "MeshManager.h"
#include "../DirectZob.h"

MeshManager::MeshManager()
{
	m_meshes.clear();
#ifdef ENABLE_FBX_SUPPORT
	m_lSdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(m_lSdkManager, IOSROOT);
	m_lSdkManager->SetIOSettings(ios);
#endif
}

MeshManager::~MeshManager()
{
#ifdef ENABLE_FBX_SUPPORT
	m_lSdkManager->Destroy();
#endif
	for (int i = 0; i < m_meshes.size(); i++)
	{
		Mesh* m = m_meshes[i];
		m_meshes[i] = NULL;
		delete m;
	}
	m_meshes.clear();
}

ZobSprite* MeshManager::CreateSprite()
{
	std::string n = std::string("Sprite");
	ZobSprite* s = new ZobSprite(n);
	m_meshes.push_back(s);
	return s;
}

Mesh* MeshManager::LoadMesh(ZobFilePath* zfp)
{
    DirectZob::LogInfo("Loading mesh '%s' at '%s'", zfp->GetName().c_str(), zfp->GetFullPath().c_str());
	Mesh* outMesh = NULL;
	//if (GetMesh(name) == NULL)
	{
		outMesh = new Mesh(zfp);
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

Mesh* MeshManager::GetMesh(const std::string& name) const
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

Mesh* MeshManager::GetOrLoadMesh(ZobFilePath* zfp)
{
	Mesh* m = GetMesh(zfp->GetName());
	if (!m)
	{
		m = LoadMesh(zfp);
	}
	return m;
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
