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

unsigned long long* ZOBGUID::GetIdAddress()
{
	return &m_id;
}

unsigned long long ZOBGUID::GetIdValue()
{
	return m_id;
}

unsigned long long ZOBGUID::GenerateId()
{
	unsigned long long u = 0;
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