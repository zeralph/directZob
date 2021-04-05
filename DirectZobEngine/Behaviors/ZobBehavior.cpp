#include "ZobBehavior.h"
#include "ZobBehaviorFactory.h"
#include "../DirectZob.h"
#include "../Misc/ZobXmlHelper.h"

ZobBehavior::ZobBehavior(ZobObject* zobObject):ZOBGUID(ZOBGUID::type_internal, ZOBGUID::subtype_behavior)
{
	m_zobObject = zobObject;
	m_type = eBehavior_none;
	std::string guid = ZobGuidToString();
	m_varExposer = new ZobVariablesExposer(guid);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_zobId, "ZobId", GetIdAddress(), true, false);
};

TiXmlNode* ZobBehavior::SaveUnderNode(TiXmlNode* node)
{
	std::string s;
	TiXmlElement n = TiXmlElement(XML_ELEMENT_BEHAVIOR);
	n.SetAttribute(XML_ATTR_TYPE, GetBehaviorTypeStr());
	n.SetAttribute(XML_ATTR_GUID, ZobGuidToString().c_str());
	m_varExposer->SaveUnderNode(&n);
	node->InsertEndChild(n);
	return node;
}

const char* ZobBehavior::GetBehaviorTypeStr()
{
	return ZobBehaviorFactory::eBehaviorTypeStr[m_type];
}

void ZobBehavior::ReadNode(TiXmlElement* node)
{
	const char* guid = node->Attribute(XML_ATTR_GUID);
	if (guid)
	{
		std::string s = guid;
		ZobGuidFromString(s);
	}
	m_varExposer->ReadNode(node);
}

void ZobBehavior::ReLoadVariables()
{
	m_varExposer->Load();
}