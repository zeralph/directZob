#include "ZobBehavior.h"
#include "ZobBehaviorFactory.h"
#include "../DirectZob.h"
#include "../Misc/ZobXmlHelper.h"

ZobBehavior::ZobBehavior(ZobObject* zobObject, bool bEditorZobBehavior):ZOBGUID((bEditorZobBehavior? ZOBGUID::type_editor:ZOBGUID::type_internal), ZOBGUID::subtype_behavior)
{
	m_zobObject = zobObject;
	m_type = eBehavior_none;
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	m_varExposer->WrapVariable<DirectZobType::zobId>("ZobId", GetIdAddress(), NULL, true, false);
};

TiXmlNode* ZobBehavior::SaveUnderNode(TiXmlNode* node)
{
	if (ZOBGUID::GetType(m_id) == ZOBGUID::type_editor)
	{
		return node;
	}
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
	assert(ZOBGUID::GetType(this->GetIdValue()) != ZOBGUID::type_editor);
	const char* guid = node->Attribute(XML_ATTR_GUID);
	if (guid)
	{
		std::string s = guid;
		zobId zid = ZOBGUID::ZobIdFromString(s);
		ChangeId(zid);
	}
	m_varExposer->ReadNode(node);
}

void ZobBehavior::ReLoadVariables()
{
	m_varExposer->Load();
}