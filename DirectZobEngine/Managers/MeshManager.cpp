#include "MeshManager.h"
#include "DirectZob.h"

static int spriteIdx = 1;
static int meshIdx = 1;

MeshManager::MeshManager()
{
	m_meshes.clear();
	m_lSdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(m_lSdkManager, IOSROOT);
	m_lSdkManager->SetIOSettings(ios);
}

MeshManager::~MeshManager()
{
	m_lSdkManager->Destroy();
	for (int i = 0; i < m_meshes.size(); i++)
	{
		Mesh* m = m_meshes[i];
		m_meshes[i] = NULL;
		delete m;
	}
	m_meshes.clear();
}

Sprite* MeshManager::CreateSprite()
{
	std::string n = std::string("Sprite_");
	n.append(std::to_string(spriteIdx));
	spriteIdx++;
	Sprite* s = new Sprite(n);
	m_meshes.push_back(s);
	return s;
}

Mesh* MeshManager::LoadMesh(std::string& name, std::string& path, std::string& file)
{
	std::string p = path;
	std::replace(p.begin(), p.end(), '\\', '/');
	if (p[p.length() - 1] != '/')
	{
		p.append("/");
	}
    DirectZob::LogInfo("Loading mesh %s%s", p.c_str(), file.c_str());
	Mesh* outMesh = NULL;
	//if (GetMesh(name) == NULL)
	{
		outMesh = new Mesh(name, p, file);
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

Mesh* MeshManager::GetOrLoadMesh(std::string& name, std::string& path, std::string& file)
{
	Mesh* m = GetMesh(name);
	if (!m)
	{
		m = LoadMesh(name, path, file);
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
