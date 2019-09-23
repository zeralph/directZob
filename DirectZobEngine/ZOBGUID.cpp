#include "ZOBGUID.h"

ulong ZOBGUID::sCurrentId = 0;

ZOBGUID::ZOBGUID(Type t, SubType s)
{
	zobId g = (zobId)1 << 63;
	g += (zobId)t << 56;
	g += (zobId)s << 48;
	g += (zobId)sCurrentId;
	m_guid = g;
	sCurrentId++;
	ZOBGUID::Type t2 = GetType();
	ZOBGUID::SubType s2 = GetSubType();
	ulong i2 = GetId();
}

const ZOBGUID::Type ZOBGUID::GetType() const
{
	zobId g = m_guid;
	g = g & 0x0F00000000000000;
	g = g >> 56;
	return (ZOBGUID::Type)g;
}

const ZOBGUID::SubType ZOBGUID::GetSubType() const
{
	zobId g = m_guid;
	g = g & 0x00F0000000000000;
	g = g >> 48;
	return (ZOBGUID::SubType)g;
}

uint ZOBGUID::GetId()
{
	zobId g = m_guid;
	g = g & 0x000000000000FFFF;
	return (uint)g;
}