#include "ZobObjectManager.h"

ZobObjectManager::ZobObjectManager()
{
	m_zobObjects.clear();
}

ZobObjectManager::~ZobObjectManager()
{

}

void ZobObjectManager::AddZobObject(ZobObject* z)
{
	if (!GetZobObject(z->GetName()))
	{
		m_zobObjects.push_back(z);
	}
}

ZobObject* ZobObjectManager::GetZobObject(const std::string& name) const
{
	for (std::vector<ZobObject*>::const_iterator iter = m_zobObjects.begin(); iter != m_zobObjects.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}

void ZobObjectManager::UpdateObjects()
{
	Matrix4x4 m;
	m.Identity();
	for (int i = 0; i < m_zobObjects.size(); i++)
	{
		m_zobObjects.at(i)->Update(&m);
	}
}

void ZobObjectManager::DrawObjects(const Camera* camera, Core::Engine* engine)
{
	for (int i = 0; i < m_zobObjects.size(); i++)
	{
		m_zobObjects.at(i)->Draw(camera, engine);
	}
}