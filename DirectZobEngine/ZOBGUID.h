#pragma once
#include "Types.h"

class ZOBGUID
{
public :

	enum Type
	{
		type_internal = 0,
		type_editor,
		type_scene,
		type_MAX = 15,
	};

	enum SubType
	{
		subtype_zobOject = 0,
		subtype_zobCamera,
		subtype_zobLight,
		subtype_mesh,
		subtype_MAX = 15,
	};

	ZOBGUID(Type t, SubType s);

	const Type GetType()const;
	const SubType GetSubType()const;
	uint GetId();

private:
	uint m_id;
	ZOBGUID::Type m_type;
	ZOBGUID::SubType m_subType;
	static ulong sCurrentId;
};
