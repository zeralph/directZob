#include "ZobBehavior.h"
#include "ZobBehaviorFactory.h"

ZobBehavior::ZobBehavior(ZobObject* zobObject, TiXmlElement* node)
{
	m_zobObject = zobObject;
	m_type = eBehavior_none;
};

TiXmlNode* ZobBehavior::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement n = TiXmlElement("Behavior");
	n.SetAttribute("Type", GetBehaviorTypeStr());
	for (std::vector<wrapperData>::const_iterator iter = m_wrappedVariables.begin(); iter != m_wrappedVariables.end(); iter++)
	{
		const wrapperData* w = &(*iter);
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
		}
		n.InsertEndChild(o);
	}
	node->InsertEndChild(n);
	return node;
}

const char* ZobBehavior::GetBehaviorTypeStr()
{
	return ZobBehaviorFactory::eBehaviorTypeStr[m_type];
}

void ZobBehavior::WrapVariable(eWrapperType type, const char* name, void* ptr)
{
	wrapperData w;
	w.type = type;
	w.name = std::string(name);
	w.ptr = ptr;
	m_wrappedVariables.push_back(w);
}

void ZobBehavior::WrapEnum(const char* name, void* ptr, int nbParams, int* values, const char** names)
{
	wrapperData w;
	w.type = eWrapperType_enum;
	w.name = std::string(name);
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
	for (TiXmlElement* e = node->FirstChildElement("Var"); e != NULL; e = e->NextSiblingElement("Var"))
	{
		const char* name = e->Attribute("name");
		const char* val = e->Attribute("value");
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
				}
				else if (w->type == eWrapperType_float)
				{
					float* f = (float*)w->ptr;
					float fVal = atof(val);
					*f = fVal;
				}
				else if (w->type == eWrapperType_int)
				{
					int* i = (int*)w->ptr;
					*i = atoi(val);
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
				}
			}
		}
	}
}