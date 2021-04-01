#pragma once
#include "../Types.h"
#include <vector>

class ZOBGUID
{
public :

	enum ZobType
	{
		type_unknown=1,
		type_internal,
		type_editor,
		type_scene,
		type_MAX = 15,
	};

	enum ZobSubType
	{
		subtype_unknown=1,
		subtype_zobOject,
		subtype_zobCamera,
		subtype_zobLight,
		subtype_mesh,
		subtype_sprite,
		subtype_behavior,
		subtype_MAX = 15,
	};

	ZOBGUID(ZobType t, ZobSubType s);
	ZOBGUID(std::string id);
	~ZOBGUID();
	const ZobType			GetType()const;
	const ZobSubType		GetSubType()const;
	unsigned long long*		GetIdAddress();
	unsigned long long		GetIdValue();
	std::string				ZobGuidToString()
	{
		return std::to_string((ulong)m_id);
	}
	void ZobGuidFromString(std::string& id)
	{
		char* pEnd;
		m_id = strtoull(id.c_str(), &pEnd, 10);
	}
	void ZobGuidRegenerate()
	{
		ulong id = GenerateId();
		if (m_type <= 0 || m_type >= ZobType::type_MAX)
		{
			m_type = ZobType::type_unknown;
		}
		if (m_subType <= 0 || m_type >= ZobSubType::subtype_MAX)
		{
			m_subType = ZobSubType::subtype_unknown;
		}
		m_id = (u16)m_type * 1000 * 1000 * 1000 + (u16)m_subType * 1000 * 1000 + id;
	}
private:
	unsigned long long		GenerateId();
	unsigned long long m_id;
	ZOBGUID::ZobType m_type;
	ZOBGUID::ZobSubType m_subType;

};
