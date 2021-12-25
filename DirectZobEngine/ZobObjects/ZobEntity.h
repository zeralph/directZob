#pragma once
#include "../Types.h"
#include <vector>
#include <map>

class ZobEntity
{
public :

	static void Init()
	{
		if (ZobEntity::m_entityMap.size() > 0)
		{
			ZobEntity::m_entityMap.clear();
		}
	}

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
		subtype_behavior=7,
		subtype_cameraController = 8,
		subtype_MAX = 9,
	};

	template<class T>
	static T* GetEntity(const zobId zid)
	{
		std::map<const zobId, ZobEntity*>::iterator it;
		it = m_entityMap.find(zid);
		if (it != m_entityMap.end())
		{
			return static_cast<T*>(it->second);
		}
		return NULL;
	}


	static ZobType			GetType(const zobId zid);
	static ZobSubType		GetSubType(const zobId zid);
	zobId*	GetIdAddress();
	zobId	GetIdValue() const;
	bool	IsEditorObject();
	bool	ChangeId(const zobId zid);
	std::string				ZobIdToString()
	{
		return std::to_string((ulong)m_id);
	}
	static zobId ZobIdFromString(std::string& id)
	{
		char* pEnd;
		return strtoull(id.c_str(), &pEnd, 10);
	}
	static zobId Regenerate(const zobId zid);

protected:
	ZobEntity(ZobType t, ZobSubType s);
	ZobEntity(const zobId zid);
	~ZobEntity();
	zobId											m_id;
	ZobEntity::ZobType								m_type;
	ZobEntity::ZobSubType								m_subType;
private:
	static ulong									GenerateId();
	void											EraseEntry(const zobId zid);
	static std::map<const zobId, ZobEntity*>			m_entityMap;

};