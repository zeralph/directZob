#include "ZobBehavior.h"
#include "ZobBehaviorFactory.h"
#include "../DirectZob.h"
#include "../Misc/ZobXmlHelper.h"

ZobBehavior::ZobBehavior(ZobObject* zobObject, bool bEditorZobBehavior):ZobEntity((bEditorZobBehavior? ZobEntity::type_editor:ZobEntity::type_internal), ZobEntity::subtype_behavior)
{
	m_zobObject = zobObject;
	m_type = eBehavior_none;
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	m_varExposer->WrapVariable<DirectZobType::zobId>("ZobId", GetIdAddress(), NULL, true, false);
};

TiXmlNode* ZobBehavior::SaveUnderNode(TiXmlNode* node)
{
	if (ZobEntity::GetType(m_id) == ZobEntity::type_editor)
	{
		return node;
	}
	std::string s;
	TiXmlElement n = TiXmlElement(XML_ELEMENT_BEHAVIOR);
	n.SetAttribute(XML_ATTR_TYPE, GetBehaviorTypeStr());
	n.SetAttribute(XML_ATTR_GUID, ZobIdToString().c_str());
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
	assert(ZobEntity::GetType(this->GetIdValue()) != ZobEntity::type_editor);
	const char* guid = node->Attribute(XML_ATTR_GUID);
	if (guid)
	{
		std::string s = guid;
		zobId zid = ZobEntity::ZobIdFromString(s);
		ChangeId(zid);
	}
	m_varExposer->ReadNode(node);
}

void ZobBehavior::ReLoadVariables()
{
	m_varExposer->Load();
}