#include "ZOBGUID.h"

ulong ZOBGUID::sCurrentId = 1;

ZOBGUID::ZOBGUID(Type t, SubType s)
{
	m_type = t;
	m_subType = s;
	m_id = sCurrentId + (int)s * 1000 + (int)t * 1000 * 1000;
	sCurrentId++;
}

ZOBGUID::~ZOBGUID()
{

}

const ZOBGUID::Type ZOBGUID::GetType() const
{
	return m_type;
}

const ZOBGUID::SubType ZOBGUID::GetSubType() const
{
	return m_subType;
}

void ZOBGUID::ChangeType(Type t)
{
	m_type = t;
}

ulong ZOBGUID::GetId()
{
	return m_id;
}