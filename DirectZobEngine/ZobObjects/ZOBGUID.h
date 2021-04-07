#pragma once
#include "../Types.h"
#include <vector>

class ZOBGUID
{
public :

	enum ZobType
	{
		type_unknown=1,
		type_internal=2,
		type_editor=3,
		type_scene=4,
		type_MAX = 5,
	};

	enum ZobSubType
	{
		subtype_unknown=1,
		subtype_zobOject=2,
		subtype_zobCamera=3,
		subtype_zobLight=4,
		subtype_mesh=5,
		subtype_sprite=6,
		subtype_behavior=7,
		subtype_cameraController = 8,
		subtype_MAX = 9,
	};

	ZOBGUID(ZobType t, ZobSubType s);
	ZOBGUID(std::string id);
	~ZOBGUID();
	const ZobType			GetType()const;
	const ZobSubType		GetSubType()const;
	zobId*	GetIdAddress();
	zobId	GetIdValue();
	std::string				ZobGuidToString()
	{
		return std::to_string((ulong)m_id);
	}
	void ZobGuidFromString(std::string& id)
	{
		char* pEnd;
		m_id = strtoull(id.c_str(), &pEnd, 10);
		m_type = (ZobType)(m_id / (1000 * 1000 * 1000));
		unsigned long long  l = (zobId)m_type * 1000 * 1000 * 1000;
		m_subType = (ZobSubType)((m_id - l)/(1000 * 1000));

	}
	void ZobGuidRegenerate()
	{
		ulong id = GenerateId();
		if (m_type <= 0 || m_type >= ZobType::type_MAX)
		{
			m_type = ZobType::type_unknown;
		}
		if (m_subType <= 0 || m_subType >= ZobSubType::subtype_MAX)
		{
			m_subType = ZobSubType::subtype_unknown;
		}
		m_id = (u16)m_type * 1000 * 1000 * 1000 + (u16)m_subType * 1000 * 1000 + id;
	}
private:
	zobId		GenerateId();
	zobId		m_id;
	ZOBGUID::ZobType			m_type;
	ZOBGUID::ZobSubType			m_subType;

};
