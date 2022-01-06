#include "ZobEntity.h"
#include <algorithm>
#include <DirectZob.h>

std::map<const zobId, ZobEntity*> ZobEntity::m_entityMap;

ZobEntity::ZobEntity(ZobType t, ZobSubType s)
{
	SET_CLASS_AND_NAME
	m_type = t;
	m_subType = s;
	ulong id = GenerateId();
	m_id = (u16)t * 1000 * 1000 * 1000 + (u16)s * 1000 * 1000 + id;
	assert( m_entityMap.find(m_id) == m_entityMap.end());
	ZobEntity::m_entityMap[m_id] = this;
}

ZobEntity::ZobEntity(const zobId zid)
{
	m_id = zid;
	SET_CLASS_AND_NAME
	m_type = (ZobType)(m_id / (1000 * 1000 * 1000));
	unsigned long long  l = (zobId)m_type * 1000 * 1000 * 1000;
	m_subType = (ZobSubType)((m_id - l) / (1000 * 1000));
	assert(m_entityMap.find(m_id) == m_entityMap.end());
	ZobEntity::m_entityMap[m_id] = this;
}

ZobEntity::~ZobEntity()
{
	EraseEntry(m_id);
}

void ZobEntity::EraseEntry(const zobId zid)
{
	std::map<const zobId, ZobEntity*>::iterator it;
	it = m_entityMap.find(zid);
	if (it != m_entityMap.end())
	{
		ZobEntity::m_entityMap.erase(it);
	}
}

bool ZobEntity::ChangeId(const zobId zid)
{
	ZobType t = (ZobType)(zid / (1000 * 1000 * 1000));
	unsigned long long  l = (zobId)t * 1000 * 1000 * 1000;
	ZobSubType st = (ZobSubType)((zid - l) / (1000 * 1000));
	assert(t == m_type);
	assert(st == m_subType);
	EraseEntry(m_id);
	m_id = zid;
	m_type = t;
	m_subType = st;
	ZobEntity::m_entityMap[m_id] = this;
	return true;
}

/*
template<class T>
T* ZobEntity::GetEntity(const zobId zid)
{
	std::map<const zobId, ZobEntity*>::iterator it;
	it = m_entityMap.find(zid);
	if (it != m_entityMap.end())
	{
		return static_cast<T*>(it->second);
	}
	return NULL;
}
*/
ZobEntity::ZobType ZobEntity::GetType(const zobId zid)
{
	return (ZobType)(zid / (1000 * 1000 * 1000));
}

ZobEntity::ZobSubType ZobEntity::GetSubType(const zobId zid)
{
	ZobType t = (ZobType)(zid / (1000 * 1000 * 1000));
	unsigned long long  l = (zobId)t * 1000 * 1000 * 1000;
	return (ZobSubType)((zid - l) / (1000 * 1000));
}

DirectZobType::zobId* ZobEntity::GetIdAddress()
{
	return &m_id;
}

DirectZobType::zobId ZobEntity::GetIdValue() const
{
	return m_id;
}

bool ZobEntity::IsEditorObject()
{
	return m_type == ZobType::type_editor;
}

zobId ZobEntity::Regenerate(const zobId zid)
{
	ZobType t = ZobEntity::GetType(zid);
	ZobSubType st = ZobEntity::GetSubType(zid);
	ulong id = ZobEntity::GenerateId();
	zobId z = (u16)t * 1000 * 1000 * 1000 + (u16)st * 1000 * 1000 + id;
	return z;
}

ulong ZobEntity::GenerateId()
{	
	ulong l = ZobUtils::random(10000, 1000000);
	return l;

}

std::vector<ZobEntity*> ZobEntity::GetAllEntities()
{
	std::vector<ZobEntity*> ret;
	ret.clear();
	std::map<const zobId, ZobEntity*>::iterator it;
	it = m_entityMap.begin();
	while (it != m_entityMap.end())
	{
		ret.push_back(it->second);
		it++;
	}
	return ret;
}