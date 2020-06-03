#ifndef DZOB_GUID_H
#define DZOB_GUID_H

#include "Types.h"
namespace directZob {
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
		subtype_sprite,
		subtype_MAX = 15,
	};

	ZOBGUID(Type t, SubType s);
	~ZOBGUID();
	const Type GetType()const;
	const SubType GetSubType()const;
	void ChangeType(Type t);
	uint GetId();

private:
	uint m_id;
	ZOBGUID::Type m_type;
	ZOBGUID::SubType m_subType;
	static ulong sCurrentId;
};
}
#endif