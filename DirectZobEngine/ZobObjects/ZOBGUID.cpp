#include "ZOBGUID.h"
#include <algorithm>
u16 ZOBGUID::sCurrentId = 1;
std::vector<DirectZobType::guid> ZOBGUID::m_guidList = std::vector<DirectZobType::guid>();

ZOBGUID::ZOBGUID(Type t, SubType s)
{
	m_type = t;
	m_subType = s;
	m_id = (u16)t * 1000 * 1000 + (u16)s * 1000 + sCurrentId;
	m_guidList.push_back(m_id);
	sCurrentId++;
}

ZOBGUID::ZOBGUID(DirectZobType::guid id)
{
	m_type = (ZOBGUID::Type)(id  / 1000 / 1000);
	m_subType = (ZOBGUID::SubType)((m_type - id) / 1000);
	u16 last_id = (u16)((m_subType - id));
	m_id = id;
	while(IsUsed(m_id))
	{
		last_id++;
		m_id = ((u16)m_type << 24) + ((u16)m_subType << 16) + last_id;
	}
	sCurrentId = std::max<int>(sCurrentId, last_id);
	m_guidList.push_back(m_id);
	sCurrentId++;
}

ZOBGUID::~ZOBGUID()
{

}

bool ZOBGUID::IsUsed(DirectZobType::guid id)
{
	for (std::vector<DirectZobType::guid>::const_iterator iter = m_guidList.begin(); iter != m_guidList.end(); iter++)
	{
		if ((DirectZobType::guid)(*iter) == id)
		{
			return true;
		}
	}
	return false;
}

const ZOBGUID::Type ZOBGUID::GetType() const
{
	return m_type;
}

const ZOBGUID::SubType ZOBGUID::GetSubType() const
{
	return m_subType;
}

DirectZobType::guid ZOBGUID::GetId()
{
	return m_id;
}