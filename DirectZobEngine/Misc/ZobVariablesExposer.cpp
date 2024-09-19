#include "ZobVariablesExposer.h"
#include "ZobXmlHelper.h"
#include "ZobUtils.h"
#include "../ZobObjects/ZobObject.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../../dependencies/tinyxml/tinyxml.h"
ZobVariablesExposer::ZobVariablesExposer(zobId id)
{
	m_wrappedVariables.clear();
	m_zobId = id;

}

ZobVariablesExposer::~ZobVariablesExposer()
{
	m_wrappedVariables.clear();
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

void ZobVariablesExposer::wrapperData::Load(bool callCb)
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
	else if (type == eWrapperType_uint)
	{
		uint* i = (uint*)ptr;
		*i = atoi(strValue.c_str());
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
	else if (type == eWrapperType_enumCombo)
	{
		for (int k = 0; k < enumNames.size(); k++)
		{
			if (strcmp(enumNames[k].c_str(), strValue.c_str()) == 0)
			{
				int* i = (int*)ptr;
				int ival = enumValues[k];
				*i = ival | *i;
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
			v->w = p.w;
		}
	}
	else if (type == eWrapperType_path)
	{
		std::string s = std::string(strValue);
		ZobFilePath* zb = (ZobFilePath*)(ptr);
		zb->Unserialize(s);
	}
	else if (type == eWrapperType_zobEntity)
	{
		std::string s = std::string(strValue);
		zobId zid = ZobEntity::ZobIdFromString(s);
		ZobObject* z = ZobEntity::GetEntity<ZobObject>(zid);
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
	if (/*callCb && */callback)
	{
		((ZobVariablesExposer::wrapperCallback)callback)(id);
	}
}

void ZobVariablesExposer::ChangeId(zobId zid)
{
	for (std::vector<wrapperData>::iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		wrapperData* w = &(*iter);
		w->id = zid;
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
			case eWrapperType_enumCombo:
			{
				int* i = (int*)(w->ptr); 
				std::stringstream ss;
				for (int k = 0; k < w->enumValues.size(); k++)
				{
					if (w->enumValues[k] == *i)
					{
						ss <<  w->enumNames[k] << "|";
					}
				}
				o.SetAttribute(XML_ATTR_VALUE, ss.str().c_str());
				break;
			}
			case eWrapperType_path:
			{
				ZobFilePath zp;
				std::string rsp = DirectZob::GetInstance()->GetResourcePath();
				zp = *(ZobFilePath*)(w->ptr);
				/*
				size_t pos = zp.path.find(rsp);
				if (pos != std::string::npos)
				{
					zp.path.erase(pos, rsp.length());
				}
				if (zp.path.length() == 0)
				{
					zp.path = std::string("/");
				}
				*/
				std::string s = zp.Serialize();
				o.SetAttribute(XML_ATTR_VALUE, s.c_str());
				break;
			}
			case eWrapperType_zobId:
			{
				break;
			}
			case eWrapperType_zobEntity:
			{
				ZobObject** z = reinterpret_cast<ZobObject**>(w->ptr);
				if (z && *z)
				{
					std::string s = (*z)->ZobIdToString();
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

void ZobVariablesExposer::Load(bool callCb)
{
	for (std::vector<wrapperData>::iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		wrapperData* w = &(*iter);
		w->Load(callCb);
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
					w->Load(false);
				}
			}
		}
		else
		{
			DirectZob::LogWarning("Attributes not found for variable loading");
		}
	}
}