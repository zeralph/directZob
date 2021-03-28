#include "ZobBehavior.h"
#include "ZobBehaviorFactory.h"
#include "../DirectZob.h"

ZobBehavior::ZobBehavior(ZobObject* zobObject)
{
	m_guid = DirectZob::GenerateZobId();
	m_zobObject = zobObject;
	m_type = eBehavior_none;
	WrapVariable(eWrapperType_zobId, "ZobId", &m_guid, true, false);
};

TiXmlNode* ZobBehavior::SaveUnderNode(TiXmlNode* node)
{
	std::string s;
	DirectZob::ZobIdToString(m_guid, s);
	TiXmlElement n = TiXmlElement("Behavior");
	n.SetAttribute("Type", GetBehaviorTypeStr());
	n.SetAttribute("Guid", s.c_str());
	for (std::vector<wrapperData>::const_iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		const wrapperData* w = &(*iter);
		if (w->bSave)
		{
			TiXmlElement o = TiXmlElement("Var");
			o.SetAttribute("name", w->name.c_str());
			switch (w->type)
			{
				case eWrapperType_int:
				{
					int* i = (int*)(w->ptr);
					std::string s = std::to_string(*i);
					o.SetAttribute("value", s.c_str());
					break;
				}
				case eWrapperType_float:
				{
					float* i = (float*)(w->ptr);
					std::string s = std::to_string(*i);
					o.SetAttribute("value", s.c_str());
					break;
				}
				case eWrapperType_string:
				{
					break;
				}
				case eWrapperType_ZobVector2:
				{
					break;
				}
				case eWrapperType_ZobVector3:
				{
					break;
				}
				case eWrapperType_bool:
				{
					bool* b = (bool*)(w->ptr);
					std::string s = b ? std::string("True") : std::string("False");
					o.SetAttribute("value", s.c_str());
					break;
				}
				case eWrapperType_enum:
				{
					int* i = (int*)(w->ptr);
					for (int k = 0; k < w->enumValues.size(); k++)
					{
						if (w->enumValues[k] == *i)
						{
							std::string s = w->enumNames[k];
							o.SetAttribute("value", s.c_str());
							break;
						}
					}
					break;
				}
				case eWrapperType_zobId:
				{
					break;
				}
			}
			n.InsertEndChild(o);
		}
	}
	node->InsertEndChild(n);
	return node;
}

const char* ZobBehavior::GetBehaviorTypeStr()
{
	return ZobBehaviorFactory::eBehaviorTypeStr[m_type];
}

void ZobBehavior::WrapVariable(eWrapperType type, const char* name, void* ptr, bool bReadOnly, bool bSave)
{
	wrapperData w;
	std::string s;
	w.name = std::string(name);
	DirectZob::ZobIdToString(m_guid, s);
	s = s.append("_").append(w.name);
	w.internalName = s;
	w.type = type;
	w.ptr = ptr;
	w.bReadOnly = bReadOnly;
	w.bSave = bSave;
	m_wrappedVariables.push_back(w);
}

void ZobBehavior::WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names, bool bReadOnly, bool bSave)
{
	wrapperData w;
	std::string s;
	w.name = std::string(name);
	DirectZob::ZobIdToString(m_guid, s);
	s = s.append("_").append(w.name);
	w.internalName = s;
	w.type = eWrapperType_enum;
	w.ptr = ptr;
	for (int i = 0; i < nbParams; i++)
	{
		w.enumValues.push_back(values[i]);
		w.enumNames.push_back(names[i]);
	}
	m_wrappedVariables.push_back(w);
}

void ZobBehavior::LoadVariables(TiXmlElement* node)
{
	const char* guid = node->Attribute("Guid");
	std::string s = std::string(guid);
	DirectZob::ZobIdFromString(s, m_guid);
	for (TiXmlElement* e = node->FirstChildElement("Var"); e != NULL; e = e->NextSiblingElement("Var"))
	{
		const char* name = e->Attribute("name");
		const char* val = e->Attribute("value");
		if (name && val)
		{
			for (std::vector<wrapperData>::const_iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
			{
				const wrapperData* w = &(*iter);
				const char* varName = w->name.c_str();
				if (strcmp(name, varName) == 0)
				{
					if (w->type == eWrapperType_string)
					{
						std::string* s = (std::string*)w->ptr;
						s->resize(0);
						s->append(val);
						break;
					}
					else if (w->type == eWrapperType_float)
					{
						float* f = (float*)w->ptr;
						*f = atof(val);
						break;
					}
					else if (w->type == eWrapperType_int)
					{
						int* i = (int*)w->ptr;
						*i = atoi(val);
						break;
					}
					else if (w->type == eWrapperType_bool)
					{
						bool* b = (bool*)w->ptr;
						*b = (strcmp(val, "True") == 0) ? true : false;
						break;
					}
					else if (w->type == eWrapperType_enum)
					{
						for (int k = 0; k < w->enumNames.size(); k++)
						{
							if (strcmp(w->enumNames[k].c_str(), val) == 0)
							{
								int ival = w->enumValues[k];
								int* i = (int*)w->ptr;
								*i = ival;
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
}