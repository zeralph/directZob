#include "ZobComponent.h"
#include "ZobComponentFactory.h"
#include "../DirectZob.h"
#include "../Misc/ZobXmlHelper.h"
#include "../../dependencies/tinyxml/tinyxml.h"

ZobComponent::ZobComponent(ZobObject* zobObject, bool bEditorZobComponent):ZobEntity((bEditorZobComponent? ZobEntity::type_editor:ZobEntity::type_internal), ZobEntity::subtype_Component)
{
	m_enabled = true;
	m_zobObject = zobObject;
	m_componentType = ZobComponentFactory::eComponent_none;
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	m_varExposer->WrapVariable<DirectZobType::zobId>("ZobId", GetIdAddress(), NULL, true, false);
	m_varExposer->WrapVariable<bool>("Enabled", &m_enabled, NULL, false, true, true);
};

TiXmlNode* ZobComponent::SaveUnderNode(TiXmlNode* node)
{
	if (ZobEntity::GetType(m_id) == ZobEntity::type_editor)
	{
		return node;
	}
	std::string s;
	TiXmlElement n = TiXmlElement(XML_ELEMENT_Component);
	n.SetAttribute(XML_ATTR_TYPE, GetComponentTypeStr());
	n.SetAttribute(XML_ATTR_GUID, ZobIdToString().c_str());
	m_varExposer->SaveUnderNode(&n);
	node->InsertEndChild(n);
	return node;
}

const char* ZobComponent::GetComponentTypeStr()
{
	return ZobComponentFactory::eComponentTypeStr[m_componentType];
}

void ZobComponent::ReadNode(TiXmlElement* node)
{
	assert(ZobEntity::GetType(this->GetIdValue()) != ZobEntity::type_editor);
	const char* guid = node->Attribute(XML_ATTR_GUID);
	if (guid)
	{
		std::string s = guid;
		zobId zid = ZobEntity::ZobIdFromString(s);
		ChangeId(zid);
		m_varExposer->ChangeId(zid);
	}
	m_varExposer->ReadNode(node);
}

void ZobComponent::ReLoadVariables()
{
	m_varExposer->Load();
}

void ZobComponent::Duplicate()
{
	assert(false);
}