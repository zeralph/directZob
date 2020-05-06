#include "ZOBGUID.h"

ulong ZOBGUID::sCurrentId = 0;

ZOBGUID::ZOBGUID(Type t, SubType s)
{

	m_id = sCurrentId;
	m_type = t;
	m_subType = s;
}

const ZOBGUID::Type ZOBGUID::GetType() const
{
	return m_type;
}

const ZOBGUID::SubType ZOBGUID::GetSubType() const
{
	return m_subType;
}

uint ZOBGUID::GetId()
{
	return m_id;
}