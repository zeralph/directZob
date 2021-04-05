#pragma once
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

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
	};

	ZobVariablesExposer(std::string& zobGUID);
	~ZobVariablesExposer();

	const std::vector<wrapperData>* GetWrappedVariables() const { return &m_wrappedVariables; }
	TiXmlNode* SaveUnderNode(TiXmlNode* node);
	void Load();
	void ReadNode(TiXmlNode* node);
	void WrapVariable(eWrapperType type, const char* name, void* ptr, bool bReadOnly, bool bSave);
	void WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names, bool bReadOnly, bool bSave);
	void WrapPath(const char* name, void* ptrName, void* ptrPath, void* ptrFile, bool bReadOnly, bool bSave);

private:
	std::string m_zobGUID;
	std::vector<wrapperData> m_wrappedVariables;
};