#pragma once
#include "../Types.h"
#include <vector>
#include <map>

#define SET_CLASS_AND_NAME  m_class = typeid(*this).name(); m_class = m_class.substr(5, m_class.length()-5); std::stringstream ss; ss << m_class << "_" << m_id; m_name = ss.str();

class ZobVariablesExposer;
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
		subtype_Component=7,
		subtype_cameraController = 8,
		subtype_MAX = 9,
	};

	static std::vector<const ZobEntity*> GetEntityByType(std::string type)
	{
		std::vector<const ZobEntity*> v;
		v.resize(0);
		std::map<const zobId, ZobEntity*>::const_iterator it = m_entityMap.begin();
		while (it != m_entityMap.end())
		{
			ZobEntity* ze = it->second;
			const char* t = typeid(ze).name();
			if (!strcmp(t, type.c_str()))
			{
				v.push_back(it->second);
			}
			it++;
		}
		return v;
	}

	template<class T>
	static std::vector<T*> GetEditorEntities()
	{
		std::vector<T*> ret;
		ret.resize(0);
		std::map<const zobId, ZobEntity*>::iterator it = m_entityMap.begin();;
		while (it != m_entityMap.end())
		{
			ZobEntity* ze = it->second;
			std::string s = typeid(T).name();
			s = s.substr(5, s.length() - 5);
			if (s == ze->m_class && ZobEntity::GetType(ze->GetIdValue()) == ZobEntity::ZobType::type_editor && ze->GetName() != "root")
			{
				ret.push_back(static_cast<T*>(ze));
			}
			it++;
		}
		return ret;
	}

	template<class T>
	static std::vector<T*> GetEntities()
	{
		std::vector<T*> ret;
		ret.resize(0);
		std::map<const zobId, ZobEntity*>::iterator it = m_entityMap.begin();;
		while(it != m_entityMap.end())
		{
			ZobEntity* ze = it->second;
			std::string s = typeid(T).name();
			s = s.substr(5, s.length() - 5);
			if (s == ze->m_class && ZobEntity::GetType(ze->GetIdValue()) != ZobEntity::ZobType::type_editor && ze->GetName() != "root")
			{
				ret.push_back(static_cast<T*>(ze));
			}
			it++;
		}
		return ret;
	}

	template<class T>
	static T* GetEntity(const zobId zid)
	{
		std::map<const zobId, ZobEntity*>::iterator it;
		it = m_entityMap.find(zid);
		if (it != m_entityMap.end())
		{
			ZobEntity* ze = it->second;
			return dynamic_cast<T*>(ze);
			/*
			std::string s = typeid(T).name();
			s = s.substr(5, s.length() - 5);
			if (s == ze->m_class || s == " ZobEntity")
			{
				return static_cast<T*>(ze);
			}
			*/
			/*
			ZobSubType st = ZobEntity::GetSubType(it->first);
			const char* type = typeid(T).name();
			if (!strcmp(type, "class ZobObject") && (st == ZobSubType::subtype_zobCamera || st == ZobSubType::subtype_zobLight || st == ZobSubType::subtype_zobOject))
			{
				return static_cast<T*>(ze);
			}
			else if (!strcmp(type, "class ZobComponent") && (st == ZobSubType::subtype_Component))
			{
				return static_cast<T*>(ze);
			}
			else if (!strcmp(type, "class ZobCameraController") && (st == ZobSubType::subtype_cameraController))
			{
				return static_cast<T*>(ze);
			}
			else
			{
				return NULL;
			}
			*/
			//return dynamic_cast<T*>(ze);
		}
		return NULL;
	}

	static std::vector<ZobEntity*>					GetAllEntities();
	static ZobType									GetType(const zobId zid);
	static ZobSubType								GetSubType(const zobId zid);
	ZobSubType										GetSubType() { return m_subType; }
	zobId*											GetIdAddress();
	zobId											GetIdValue() const;
	bool											IsEditorObject() const;
	bool											ChangeId(const zobId zid);
	inline const std::string&						GetName() const { return m_name; }
	inline const std::string&						GetClass() const { return m_class; }
	inline void										SetName(const std::string& name) { m_name = name; }
	virtual ZobEntity*								Duplicate() = 0;
	std::string										ZobIdToString()
	{
		return std::to_string((ulong)m_id);
	}
	static zobId									ZobIdFromString(std::string& id)
	{
		char* pEnd;
		return strtoull(id.c_str(), &pEnd, 10);
	}
	static zobId									Regenerate(const zobId zid);
	ZobVariablesExposer*							GetVariablesExposer() { return m_varExposer; }

protected:
	ZobEntity(ZobType t, ZobSubType s);
	ZobEntity(const zobId zid);
	~ZobEntity();
	virtual void									Init(DirectZobType::sceneLoadingCallback cb) = 0;
	zobId											m_id;
	ZobEntity::ZobType								m_type;
	ZobEntity::ZobSubType							m_subType;
	std::string										m_name;
	std::string										m_class;
	ZobVariablesExposer*							m_varExposer;
private:
	static ulong									GenerateId();
	void											EraseEntry(const zobId zid);
	static std::map<const zobId, ZobEntity*>		m_entityMap;

};
