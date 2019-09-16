#pragma once
#include "Types.h"

class ZOBGUID
{
public :
	//    60   56   52   48   44   40   36   32   28   24   20   16   12    8    4    0
	//     |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
	//  0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000  
	//|res |type|styp|


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

	Type GetType();
	SubType GetSubType();
	ulong GetId();

private:
	zobId m_guid;
	static ulong sCurrentId;
};
