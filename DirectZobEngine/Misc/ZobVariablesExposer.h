#pragma once
#include "../Types.h"
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"
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
		__eWrapperType_MAX__
	};

	class wrapperData
	{
	public:
		wrapperData() {}
		~wrapperData() {}
		eWrapperType type;
		bool bReadOnly;
		bool bSave;
		void* ptr;
		void* ptr_1;
		void* ptr_2;
		void* ptr_3;
		std::string name;
		std::string internalName;
		std::string strValue;
		//for enums
		std::vector<int> enumValues;
		std::vector<std::string> enumNames;
		void Load();
		void Init();
	};

	ZobVariablesExposer(std::string& zobGUID);
	~ZobVariablesExposer();

	const std::vector<wrapperData>* GetWrappedVariables() const { return &m_wrappedVariables; }
	TiXmlNode* SaveUnderNode(TiXmlNode* node);
	void Load();
	void ReadNode(TiXmlNode* node);

	template<typename T>
	void WrapVariable(const char* name, T* ptr, bool bReadOnly, bool bSave)
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
		else if (std::is_same<T, DirectZobType::ZobFilePath>::value)
		{
			type = eWrapperType_path;
		}
		else
		{
			assert(false);
		}

		assert(type != eWrapperType::__eWrapperType_MAX__);
		wrapperData w;
		w.Init();
		std::string s;
		w.name = std::string(name);
		s = m_zobGUID;
		s = s.append("_").append(w.name);
		w.internalName = s;
		w.type = type;
		w.ptr = (void*)ptr;
		w.bReadOnly = bReadOnly;
		w.bSave = bSave;
		m_wrappedVariables.push_back(w);
	}

	template<typename E>
	void WrapEnum(const char* name, E* ptr, int nbParams, E* enumValues, const char** enumNames, bool bReadOnly, bool bSave)
	{
		wrapperData w;
		std::string s;
		w.name = std::string(name);
		s = m_zobGUID;
		s = s.append("_").append(w.name);
		w.internalName = s;
		w.type = eWrapperType_enum;
		w.ptr = ptr;
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
	std::string m_zobGUID;
	std::vector<wrapperData> m_wrappedVariables;
};