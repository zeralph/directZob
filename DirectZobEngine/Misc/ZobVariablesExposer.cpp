#include "ZobVariablesExposer.h"
#include "ZobXmlHelper.h"
#include "ZobUtils.h"
#include "../DirectZobEngine/DirectZob.h"

ZobVariablesExposer::ZobVariablesExposer(zobId id)
{
	m_wrappedVariables.clear();
	m_zobId = id;

}

ZobVariablesExposer::~ZobVariablesExposer()
{
}

void ZobVariablesExposer::wrapperData::Init()
{
	type = eWrapperType_unset;
	bReadOnly = true;
	bSave = false;
	ptr = NULL;
	callback = NULL;
	name = "";
	internalName = "";
	strValue = "";
	enumValues.clear();
	enumNames.clear();
}

void ZobVariablesExposer::wrapperData::Load()
{
	if (strValue.size() == 0)
	{
		return;
	}
	if (type == eWrapperType_string)
	{
		std::string* s = (std::string*)ptr;
		s->resize(0);
		s->append(strValue);
	}
	else if (type == eWrapperType_float)
	{
		float* f = (float*)ptr;
		*f = atof(strValue.c_str());
	}
	else if (type == eWrapperType_int)
	{
		int* i = (int*)ptr;
		*i = atoi(strValue.c_str());
	}
	else if (type == eWrapperType_bool)
	{
		bool* b = (bool*)ptr;
		*b = (strcmp(strValue.c_str(), "True") == 0) ? true : false;
	}
	else if (type == eWrapperType_enum)
	{
		for (int k = 0; k < enumNames.size(); k++)
		{
			if (strcmp(enumNames[k].c_str(), strValue.c_str()) == 0)
			{
				int ival = enumValues[k];
				int* i = (int*)ptr;
				*i = ival;
				break;
			}
		}
	}
	else if (type == eWrapperType_string)
	{
		std::string* s = (std::string*)(ptr);
		s->resize(0);
		s->assign(strValue);
	}
	else if (type == eWrapperType_ZobVector2)
	{
		ZobVector2* v = (ZobVector2*)(ptr);
		ZobVector2 p;
		std::string s = std::string(strValue);
		if (p.FromString(s))
		{
			v->x = p.x;
			v->y = p.y;
		}
	}
	else if (type == eWrapperType_ZobVector3)
	{
		ZobVector3* v = (ZobVector3*)(ptr);
		ZobVector3 p;
		std::string s = std::string(strValue);
		if (p.FromString(s))
		{
			v->x = p.x;
			v->y = p.y;
			v->z = p.z;
		}
	}
	else if (type == eWrapperType_path)
	{
		std::string s = std::string(strValue);
		ZobFilePath* zb = (ZobFilePath*)(ptr);
		zb->Unserialize(s);
	}
	else if (type == eWrapperType_zobObject)
	{
		std::string s = std::string(strValue);
		ZobObject* z = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(s);
		if (z)
		{
			uintptr_t addr = reinterpret_cast<uintptr_t>(&*z);
			ZobObject** zPtr = (ZobObject**)ptr;
			*zPtr = (ZobObject*)addr;
		}
	}
	else if (type == eWrapperType_zobId)
	{

	}
	else if (type == eWrapperType_zobColor)
	{
		ZobColor* v = (ZobColor*)(ptr);
		ZobColor p;
		std::string s = std::string(strValue);
		
		if (p.FromString(s))
		{
			v->Set(p.GetAlpha(), p.GetRed(), p.GetGreen(), p.GetBlue());
		}
	}
	else
	{
		assert(false);
	}
}

TiXmlNode* ZobVariablesExposer::SaveUnderNode(TiXmlNode* node)
{
	for (std::vector<wrapperData>::const_iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		const wrapperData* w = &(*iter);
		if (w->bSave)
		{
			TiXmlElement o = TiXmlElement(XML_ATTR_VAR);
			o.SetAttribute(XML_ATTR_NAME, w->name.c_str());
			switch (w->type)
			{
			case eWrapperType_int:
			{
				int* i = (int*)(w->ptr);
				std::string s = std::to_string(*i);
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_float:
			{
				float* i = (float*)(w->ptr);
				std::string s = ZobUtils::floatToString(*i, 3);

				//std::string s = std::to_string(*i);
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_string:
			{
				std::string* s = (std::string*)(w->ptr);
				o.SetAttribute(XML_ATTR_VALUE, s->c_str());
				break;
			}
			case eWrapperType_ZobVector2:
			{
				ZobVector2* v = (ZobVector2*)(w->ptr);
				std::string s = v->ToString();
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_ZobVector3:
			{
				ZobVector3* v = (ZobVector3*)(w->ptr);
				std::string s = v->ToString();
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_bool:
			{
				bool* b = (bool*)(w->ptr);
				std::string s = *b ? std::string(XML_VALUE_TRUE) : std::string(XML_VALUE_FALSE);
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
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
						o.SetAttribute(XML_ATTR_VALUE, s.c_str());
						break;
					}
				}
				break;
			}
			case eWrapperType_path:
			{
				ZobFilePath zp;
				zp = *(ZobFilePath*)(w->ptr);
				std::string s = zp.Serialize();
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_zobId:
			{
				break;
			}
			case eWrapperType_zobObject:
			{
				ZobObject** z = reinterpret_cast<ZobObject**>(w->ptr);
				if (z && *z)
				{
					std::string s = (*z)->ZobGuidToString();
					o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				}
				break;
			}
			case eWrapperType_zobColor:
			{
				ZobColor* v = (ZobColor*)(w->ptr);
				std::string s = v->ToString();
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			default:
			{
				assert(false);
				break;
			}
			}
			node->InsertEndChild(o);
		}
	}
	return node;
}

void ZobVariablesExposer::Load()
{
	for (std::vector<wrapperData>::iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		wrapperData* w = &(*iter);
		w->Load();
	}
}

void ZobVariablesExposer::ReadNode(TiXmlNode* node)
{
	for (TiXmlElement* e = node->FirstChildElement(XML_ATTR_VAR); e != NULL; e = e->NextSiblingElement("Var"))
	{
		const char* name = e->Attribute(XML_ATTR_NAME);
		const char* val = e->Attribute(XML_ATTR_VALUE);
		if (name && val)
		{
			for (std::vector<wrapperData>::iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
			{
				wrapperData* w = &(*iter);
				const char* varName = w->name.c_str();
				if (strcmp(name, varName) == 0)
				{
					w->strValue.resize(0);
					w->strValue.append(val);
					w->Load();
				}
			}
		}
		else
		{
			DirectZob::LogWarning("Attributes not found for mesh loading");
		}
	}
}