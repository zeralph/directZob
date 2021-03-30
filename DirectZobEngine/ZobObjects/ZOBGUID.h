#pragma once
#include "../Types.h"
#include <vector>

class ZOBGUID
{
public :

	enum ZobType
	{
		type_unknown=0,
		type_internal,
		type_editor,
		type_scene,
		type_MAX = 15,
	};

	enum ZobSubType
	{
		subtype_unknown=0,
		subtype_zobOject,
		subtype_zobCamera,
		subtype_zobLight,
		subtype_mesh,
		subtype_sprite,
		subtype_MAX = 15,
	};

	ZOBGUID(ZobType t, ZobSubType s);
	ZOBGUID(DirectZobType::guid id);
	~ZOBGUID();
	const ZobType GetType()const;
	const ZobSubType GetSubType()const;
	ulong GetId();
	static void Reset() {};
private:
	bool IsUsed(DirectZobType::guid id);
	ulong GenerateId();
	ulong m_id;
	ZOBGUID::ZobType m_type;
	ZOBGUID::ZobSubType m_subType;

};
