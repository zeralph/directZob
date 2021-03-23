#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include <map>
#include <vector>
#include <string>
#include<map>

#define START_EXPOSE_PROPERTIES()	public:																			\
									std::vector<std::string> properties;											\
									const std::vector<std::string>* GetProperties() const {return &properties;};	
#define ADD_PROPERTY(type, name)	type name;																		\
				
#define GET_VARIABLE_NAME(Variable) (#Variable)

class ZobObject;
class Camera;
class ZobBehavior
{
public:

	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_menu,
	};
	virtual ~ZobBehavior() {};
	virtual TiXmlNode*	SaveUnderNode(TiXmlNode* node)=0;
	virtual void		PreUpdate()=0;
	virtual void		Init()=0;
	virtual void		Update(float dt) = 0;
	eBehaviorType		GetBehaviorType() const { return m_type; }
	virtual const char* GetBehaviorTypeStr()=0;
	virtual	void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}
protected:
	ZobBehavior(ZobObject* zobObject, TiXmlElement* node) 
	{ 
		m_myFloat = 0;
		m_zobObject = zobObject;  
		m_type = eBehavior_none; 	
		SetValueFloat("myFloat", 32.0f);
	};
	eBehaviorType m_type;
	ZobObject* m_zobObject;
	std::map<const char*, float*> m_wrapperMapFloat;
	std::map<const char*, int*> m_wrapperMapInt;
	std::map<const char*, ZobVector3*> m_wrapperMapZobVector3;
	std::map<const char*, std::string*> m_wrapperMapString;
private:

	void CreateMap()
	{
		m_wrapperMapFloat["myFloat"] = &m_myFloat;
		m_wrapperMapInt["myInt"] = &m_myInt;
		m_wrapperMapZobVector3["myVector"] = &m_myVector;
		m_wrapperMapString["myString"] = &m_myString;
	}

	//START_EXPOSE_PROPERTIES()
	//	ADD_PROPERTY(float, m_prout)
	void SetValueFloat(const char* variable, float f)
	{
		float* i = m_wrapperMapFloat[variable];
		*i = f;
	}
	float GetValmueFloat(const char* variable)
	{
		return *m_wrapperMapFloat[variable];
	}

	float m_myFloat;
	int m_myInt;
	ZobVector3 m_myVector;
	std::string m_myString;
	
	//std::map<std::string, T*> m{ {"CPU", &m_type}, {"GPU", &a}, {"RAM", &b}, };

};