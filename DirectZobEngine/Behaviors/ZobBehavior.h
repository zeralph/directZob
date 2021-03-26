#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include <map>
#include <vector>
#include <string>
#include<map>


class ZobObject;
class Camera;
class ZobBehavior
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
		__eWrapperType_MAX__
	};

	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_menu,
		__eBehavior_MAX__,
	};

	struct wrapperData
	{
		eWrapperType type;
		void* ptr;
		std::string name;
		//for enums
		std::vector<int> enumValues;
		std::vector<std::string> enumNames;
	};

	virtual ~ZobBehavior() {};
	virtual void						PreUpdate()=0;
	virtual void						Init()=0;
	virtual void						Update(float dt) = 0;
	virtual	void						DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}

	const char*							GetBehaviorTypeStr();
	TiXmlNode*							SaveUnderNode(TiXmlNode* node);
	eBehaviorType						GetBehaviorType() const { return m_type; }
	const std::vector<wrapperData>*		GetWrappedVariables() const { return &m_wrappedVariables; }
	void								LoadVariables(TiXmlElement* node);
protected:
	ZobBehavior(ZobObject* zobObject, TiXmlElement* node);
	eBehaviorType m_type;
	ZobObject* m_zobObject;
	std::vector<wrapperData> m_wrappedVariables;

protected:

	void WrapVariable(eWrapperType type, const char* name, void* ptr);
	void WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names);
};