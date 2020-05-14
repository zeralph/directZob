#include "ZOBGUID.h"

ulong ZOBGUID::sCurrentId = 0;

ZOBGUID::ZOBGUID(Type t, SubType s)
{

	m_id = sCurrentId;
	m_type = t;
	m_subType = s;
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

uint ZOBGUID::GetId()
{
	return m_id;
}