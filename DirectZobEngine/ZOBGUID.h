#pragma once
#include "Types.h"
#include <vector>

class ZOBGUID
{
public :

	enum Type
	{
		type_unknown=0,
		type_internal,
		type_editor,
		type_scene,
		type_MAX = 15,
	};

	enum SubType
	{
		subtype_unknown=0,
		subtype_zobOject,
		subtype_zobCamera,
		subtype_zobLight,
		subtype_mesh,
		subtype_sprite,
		subtype_MAX = 15,
	};

	ZOBGUID(Type t, SubType s);
	ZOBGUID(DirectZobType::guid id);
	~ZOBGUID();
	const Type GetType()const;
	const SubType GetSubType()const;
	void ChangeType(Type t);
	DirectZobType::guid GetId();
	static void Reset() { sCurrentId = 0; m_guidList.clear(); };
private:
	bool IsUsed(DirectZobType::guid id);
	DirectZobType::guid m_id;
	ZOBGUID::Type m_type;
	ZOBGUID::SubType m_subType;
	static std::vector<DirectZobType::guid> m_guidList;
	static u16 sCurrentId;
};
