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

	struct wrapperData
	{
		eWrapperType type;
		void* ptr;
		std::string name;
	};

	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_menu,
	};
	virtual ~ZobBehavior() {};
	virtual TiXmlNode*					SaveUnderNode(TiXmlNode* node)=0;
	virtual void						PreUpdate()=0;
	virtual void						Init()=0;
	virtual void						Update(float dt) = 0;
	eBehaviorType						GetBehaviorType() const { return m_type; }
	virtual const char*					GetBehaviorTypeStr()=0;
	virtual	void						DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}
	const std::vector<wrapperData>*		GetWrappedVariables() const { return &m_wrappedVariables; }
protected:
	ZobBehavior(ZobObject* zobObject, TiXmlElement* node) 
	{ 
		m_zobObject = zobObject;  
		m_type = eBehavior_none; 	
		/*
		m_myFloat = 0;
		WrapVariable(eWrapperType_float, "float", &m_myFloat);
		WrapVariable(eWrapperType_int, "int", &m_myInt);
		WrapVariable(eWrapperType_ZobVector3, "vector", &m_myVector);
		WrapVariable(eWrapperType_string, "string", &m_myString);

		const wrapperData* w;
		void* v;
		float* flt;
		std::string* str;
		w = GetVariable("float");
		v = w->ptr;// = 1.0f;
		flt = (float*)v;
		*flt = 4;

		w = GetVariable("string");
		v = w->ptr;// = 1.0f;
		str = (std::string*)v;
		*str = std::string("prout");

		w = GetVariable("string");
		v = w->ptr;// = 1.0f;
		str = (std::string*)v;
		*str = std::string("prout2");

		w = GetVariable("float");
		v = w->ptr;// = 1.0f;
		flt = (float*)v;
		*flt = 8;
		*/ 
	};
	eBehaviorType m_type;
	ZobObject* m_zobObject;
	std::vector<wrapperData> m_wrappedVariables;

protected:

	void WrapVariable(eWrapperType type, const char* name, void* ptr)
	{
		wrapperData w;
		w.type = type;
		w.name = std::string(name);
		w.ptr = ptr;
		m_wrappedVariables.push_back(w);
	}
	/*
	const wrapperData* GetVariable(const char* variable) const
	{
		for (std::vector<wrapperData>::const_iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
		{
			if (strcmp((*iter).name.c_str(), variable) == 0)
			{
				return &(*iter);
			}
		}
		return NULL;
	}
	*/
};