#include "ZobVariablesExposer.h"
#include "ZobXmlHelper.h"
#include "../DirectZobEngine/ZobObjects/ZOBGUID.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../DirectZobEngine/Rendering/ZobVector2.h"
#include "../DirectZobEngine/Rendering/ZobVector3.h"
#include "../DirectZobEngine/DirectZob.h"

ZobVariablesExposer::ZobVariablesExposer(std::string &zobGUID)
{
	m_wrappedVariables.clear();
}

ZobVariablesExposer::~ZobVariablesExposer()
{

}

void ZobVariablesExposer::WrapPath(const char* name, void* ptrName, void* ptrPath, void* ptrFile, bool bReadOnly, bool bSave)
{
	wrapperData w;
	std::string s;
	w.type = eWrapperType_path;
	w.name = std::string(name);
	s = m_zobGUID;
	s = s.append("_").append(w.name);
	w.internalName = s;
	w.ptr = ptrName;
	w.ptr_1 = ptrPath;
	w.ptr_2 = ptrFile;
	w.bReadOnly = bReadOnly;
	w.bSave = bSave;
	m_wrappedVariables.push_back(w);
}

void ZobVariablesExposer::WrapVariable(eWrapperType type, const char* name, void* ptr, bool bReadOnly, bool bSave)
{
	wrapperData w;
	std::string s;
	w.name = std::string(name);
	s = m_zobGUID;
	s = s.append("_").append(w.name);
	w.internalName = s;
	w.type = type;
	w.ptr = ptr;
	w.bReadOnly = bReadOnly;
	w.bSave = bSave;
	m_wrappedVariables.push_back(w);
}

void ZobVariablesExposer::WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names, bool bReadOnly, bool bSave)
{
	wrapperData w;
	std::string s;
	w.name = std::string(name);
	s = m_zobGUID;
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
		std::size_t del1, del2 = 0;
		del1 = s.find(';');
		if (del1 != std::string::npos)
		{
			del2 = s.find(';', del1 + 1);
			if (del1 != std::string::npos)
			{
				std::string name = s.substr(0, del1);
				std::string path = s.substr(del1 + 1, del2 - (del1 + 1));
				std::string file = s.substr(del2 + 1, s.size() - 1);
				std::string* sName = (std::string*)(ptr);
				sName->resize(0);
				sName->assign(name);
				std::string* sPath = (std::string*)(ptr_1);
				sPath->resize(0);
				sPath->assign(path);
				std::string* sFile = (std::string*)(ptr_2);
				sFile->resize(0);
				sFile->assign(file);
			}
		}
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
				std::string s = std::to_string(*i);
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
				std::string s = b ? std::string(XML_VALUE_TRUE) : std::string(XML_VALUE_FALSE);
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
				std::string* name = (std::string*)(w->ptr);
				std::string* file = (std::string*)(w->ptr_1);
				std::string* path = (std::string*)(w->ptr_2);
				std::string s = "";
				s = s.append(*name).append(";").append(*file).append(";").append(*path);
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
	}
}