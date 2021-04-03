#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "../ZobObjects/ZOBGUID.h"
#include <map>
#include <vector>
#include <string>
#include<map>


class ZobObject;
class Camera;
class ZobBehavior : public ZOBGUID
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

	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_menu,
		eBehavior_physicBox,
		eBehavior_physicSphere,
		eBehavior_physicCapsule,
		eBehavior_physicMesh,
		__eBehavior_MAX__,
	};

	class wrapperData
	{
	public:
		wrapperData(){}
		~wrapperData(){}
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

	virtual ~ZobBehavior() {};
	virtual void						PreUpdate()=0;
	virtual void						Init()=0;
	virtual void						Update(float dt) = 0;
	virtual void						EditorUpdate() = 0;
	virtual	void						DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}

	const char*							GetBehaviorTypeStr();
	TiXmlNode*							SaveUnderNode(TiXmlNode* node);
	eBehaviorType						GetBehaviorType() const { return m_type; }
	const std::vector<wrapperData>*		GetWrappedVariables() const { return &m_wrappedVariables; }
	void								ReadNode(TiXmlElement* node);
	void								ReLoadVariables();

protected:
	ZobBehavior(ZobObject* zobObject);
	eBehaviorType m_type;
	ZobObject* m_zobObject;
	std::vector<wrapperData> m_wrappedVariables;

protected:
	void WrapVariable(eWrapperType type, const char* name, void* ptr, bool bReadOnly, bool bSave);
	void WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names, bool bReadOnly, bool bSave);
	void WrapPath(const char* name, void* ptrName, void* ptrPath, void* ptrFile, bool bReadOnly, bool bSave);
};