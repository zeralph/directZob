#include "ZOBGUID.h"
#include <algorithm>
#include <DirectZob.h>

ZOBGUID::ZOBGUID()
{
	m_type = ZobType::type_unknown;
	m_subType = ZobSubType::subtype_unknown;
	m_id = 0;
}

ZOBGUID::ZOBGUID(ZobType t, ZobSubType s)
{
	m_type = t;
	m_subType = s;
	ulong id = GenerateId();
	m_id = (u16)t * 1000 * 1000 * 1000 + (u16)s * 1000 * 1000 + id;
}

ZOBGUID::ZOBGUID(std::string id)
{
	assert(id.length());
	ZobGuidFromString(id);
}

ZOBGUID::~ZOBGUID()
{

}

const ZOBGUID::ZobType ZOBGUID::GetType() const
{
	return m_type;
}

const ZOBGUID::ZobSubType ZOBGUID::GetSubType() const
{
	return m_subType;
}

DirectZobType::zobId* ZOBGUID::GetIdAddress()
{
	return &m_id;
}

DirectZobType::zobId ZOBGUID::GetIdValue()
{
	return m_id;
}

DirectZobType::zobId ZOBGUID::GenerateId()
{
	DirectZobType::zobId u = 0;
	for (int i = 0; i < 6; i++)
	{
		ulong l = (rand() % 10);
		if (l == 0 && i == 0)
		{
			l = 1;
		}
		u += l * pow(10, i);
	}
	return u;
}