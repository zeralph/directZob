#pragma once
#include "../Types.h"
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"
#include "../Rendering/Color.h"
#include "ZobFilePath.h"

//#include "../DirectZobEngine/ZobObjects/ZOBGUID.h"
//#include "../DirectZobEngine/ZobObjects/ZobObject.h"
//#include "../DirectZobEngine/Rendering/ZobVector3.h"
//#include "../DirectZobEngine/Rendering/ZobVector2.h"

class ZOBGUID;
class ZobVector2;
class ZobVector3;
class ZobObject;
class ZobVariablesExposer
{
public:

	typedef void (*wrapperCallback)(zobId id);
	enum eWrapperType
	{
		eWrapperType_unset = 0,
		eWrapperType_int,
		eWrapperType_float,
		eWrapperType_string,
		eWrapperType_ZobVector2,
		eWrapperType_ZobVector3,
		eWrapperType_enum,
		eWrapperType_bool,
		eWrapperType_zobId,
		eWrapperType_path,
		eWrapperType_zobObject,
		eWrapperType_zobColor,
		eWrapperType_uint,
		__eWrapperType_MAX__
	};

	class wrapperData
	{
	public:
		wrapperData(zobId id) { id = id; ptr = NULL; }
		~wrapperData() { ptr = NULL; }
		zobId id;
		eWrapperType type;
		bool bReadOnly;
		bool bSave;
		void* ptr;
		wrapperCallback callback;
		std::string name;
		std::string internalName;
		std::string strValue;
		//for enums
		std::vector<int> enumValues;
		std::vector<std::string> enumNames;
		void Load();
		void Init();
	};

	ZobVariablesExposer(zobId id);
	~ZobVariablesExposer();

	const std::vector<wrapperData>* GetWrappedVariables() const { return &m_wrappedVariables; }
	TiXmlNode* SaveUnderNode(TiXmlNode* node);
	void Load();
	void ReadNode(TiXmlNode* node);

	template<typename T>
	void WrapVariable(const char* name, T* ptr, wrapperCallback cb, bool bReadOnly, bool bSave)
	{
		eWrapperType type = eWrapperType::__eWrapperType_MAX__;
		if (std::is_same<T, int>::value)
		{
			type = eWrapperType_int;
		}
		else if (std::is_same<T, float>::value)
		{
			type = eWrapperType_float;
		}
		else if (std::is_same<T, std::string>::value)
		{
			type = eWrapperType_string;
		}
		else if (std::is_same<T, ZobVector2>::value)
		{
			type = eWrapperType_ZobVector2;
		}
		else if (std::is_same<T, ZobVector3>::value)
		{
			type = eWrapperType_ZobVector3;
		}
		else if (std::is_same<T, bool>::value)
		{
			type = eWrapperType_bool;
		}
		else if (std::is_same<T, DirectZobType::zobId>::value)
		{
			type = eWrapperType_zobId;
		}
		else if (std::is_same<T, ZobObject*>::value)
		{
			type = eWrapperType_zobObject;
		}
		else if (std::is_same<T, ZobFilePath>::value)
		{
			type = eWrapperType_path;
		}
		else if (std::is_same<T, ZobColor>::value)
		{
			type = eWrapperType_zobColor;
		}
		else if (std::is_same<T, uint>::value)
		{
			type = eWrapperType_uint;
		}
		else
		{
			assert(false);
		}

		assert(type != eWrapperType::__eWrapperType_MAX__);
		wrapperData w = wrapperData(m_zobId);
		w.Init();
		std::string s;
		w.name = std::string(name);
		s = "";
		s = s.append("_").append(w.name);
		w.id = m_zobId;
		w.internalName = s;
		w.type = type;
		w.ptr = (void*)ptr;
		w.callback = cb;
		w.bReadOnly = bReadOnly;
		w.bSave = bSave;
		m_wrappedVariables.push_back(w);
	}

	template<typename E>
	void WrapEnum(const char* name, E* ptr, int nbParams, E* enumValues, const char** enumNames, wrapperCallback cb, bool bReadOnly, bool bSave)
	{
		wrapperData w = wrapperData(m_zobId);
		w.Init();
		std::string s;
		w.name = std::string(name);
		s = "";
		s = s.append("_").append(w.name);
		w.internalName = s;
		w.id = m_zobId;
		w.type = eWrapperType_enum;
		w.ptr = ptr;
		w.callback = cb;
		w.bSave = bSave;
		for (int i = 0; i < nbParams; i++)
		{
			int v = (int)enumValues[i];
			//E val = E[i];
			//int v = (int)ptr[i];
			//E e0 = static_cast<std::underlying_type<E>::type>(0);
			//E e1 = static_cast<std::underlying_type<E>::type>(1);
			w.enumValues.push_back(v);
			w.enumNames.push_back(enumNames[i]);
		}
		m_wrappedVariables.push_back(w);
	}

private:
	zobId m_zobId;
	std::vector<wrapperData> m_wrappedVariables;
};