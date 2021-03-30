#include "ZOBGUID.h"
#include <algorithm>
#include <DirectZob.h>


ZOBGUID::ZOBGUID(ZobType t, ZobSubType s)
{
	m_type = t;
	m_subType = s;
	ulong id = GenerateId();
	m_id = (u16)t * 1000 * 1000 * 1000 + (u16)s * 1000 * 1000 + id;
}

ZOBGUID::ZOBGUID(DirectZobType::guid id)
{
	m_type = (ZOBGUID::ZobType)(id  / 1000 / 1000 / 1000);
	m_subType = (ZOBGUID::ZobSubType)((m_type - id) / 1000 / 1000);
	m_id = id;
}

ZOBGUID::~ZOBGUID()
{

}

bool ZOBGUID::IsUsed(DirectZobType::guid id)
{
	return false;
}

const ZOBGUID::ZobType ZOBGUID::GetType() const
{
	return m_type;
}

const ZOBGUID::ZobSubType ZOBGUID::GetSubType() const
{
	return m_subType;
}

ulong ZOBGUID::GetId()
{
	return m_id;
}

ulong ZOBGUID::GenerateId()
{
	ulong u = 0;
	for (int i = 0; i < 6; i++)
	{
		u += (rand()%10) * pow(10, i);
	}
	return u;
}