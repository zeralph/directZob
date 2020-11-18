#include "LightManager.h"
#include "DirectZob.h"

LightManager::LightManager()
{
	ReInitGlobalSettings();
}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
}

void LightManager::ReInitGlobalSettings()
{
	m_fogColor = ZobVector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_clearColor = ZobVector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_ambientColor = ZobVector3(0.4f, 0.4f, 0.4f);
	m_fogDistance = 500.0f;
	m_lights.clear();
	m_fogDensity = 2.0f;
	m_fogType = eFogType::eFogType_NoFog;
}

void LightManager::Setup(ZobVector3* fogColor, ZobVector3* ambientColor, ZobVector3* clearColor, float fogDistance, float fogDensity, eFogType fogType)
{
	m_fogColor = fogColor;
	m_ambientColor = ambientColor;
	m_clearColor = clearColor;
	m_fogDistance = fogDistance;
	m_fogDensity = fogDensity;
	m_fogType = fogType;
}

void LightManager::RemoveLight(Light* l)
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i] == l)
		{
			std::swap(m_lights.at(i), m_lights.at(m_lights.size() - 1));
			m_lights.pop_back();
		}
	}
}

void LightManager::UnloadAll()
{
	//for (int i = 0; i < m_lights.size(); i++)
	//{
	//	delete (m_lights[i]);
	//}
	m_lights.clear();
}

Light* LightManager::CreateLight(std::string& name, Light::eLightType type, ZobVector3 position, ZobVector3 color, float intensity, float distance, ZobObject* parent)
{
	Light* l = new Light(name, type, color, intensity, distance, parent);
	l->SetWorldPosition(position.x, position.y, position.z);
	AddLight(l);
	return l;
}

void LightManager::AddLight(Light* l)
{
	m_lights.push_back(l);
}

Light* LightManager::CreateLight(Light::eLightType type)
{
	int l = m_lights.size();
	std::string name = std::string("Light_").append(std::to_string((l)));
	ZobVector3 color = ZobVector3(252.0f / 255.0f, 212 / 255.0f, 64.0f / 255.0f);
	return CreateLight(name, type, ZobVector3(0, 10, 0), color, 5.0f, 100.0f, NULL);
}

Light* LightManager::GetLight(const std::string& name) const
{
	for (std::vector<Light*>::const_iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}

const std::vector<Light*>* LightManager::GetActiveLights() const
{
	if(m_lightingEnabled)
	{
		return &m_lights;
	}
	else
	{
		return NULL;
	}
}

void LightManager::LoadFromNode(TiXmlElement* node)
{
	if (node)
	{
		TiXmlElement* e;
		ZobVector3 ambient = GetAmbientColor();
		e = node->FirstChildElement("AmbientColor");
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			ambient = ZobVector3(x, y, z);
		}
		ZobVector3 fog = GetFogColor();
		e = node->FirstChildElement("FogColor");
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			fog = ZobVector3(x, y, z);
		}
		e = node->FirstChildElement("ClearColor");
		ZobVector3 clear = GetClearColor();
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			clear = ZobVector3(x, y, z);
		}
		e = node->FirstChildElement("FogDensity");
		float fogDensity = GetFogDensity();
		if (e)
		{
			fogDensity = atof(e->GetText());
		}
		e = node->FirstChildElement("FogDistance");
		float FogDistance = GetFogDistance();
		if (e)
		{
			FogDistance = atof(e->GetText());
		}
		e = node->FirstChildElement("FogType");
		eFogType fogType = GetFogType();
		if (e)
		{
			std::string type = std::string(e->GetText() ? e->GetText():"");
			if (type == "linear")
			{
				fogType = eFogType::eFogType_Linear;
			}
			else if (type == "exp")
			{
				fogType = eFogType::eFogType_Exp;
			}
			else if (type == "exp2")
			{
				fogType = eFogType::eFogType_Exp2;
			}
			else
			{
				fogType = eFogType::eFogType_NoFog;
			}
		}
		fog /= 255.0f;
		ambient /= 255.0f;
		clear /= 255.0f;
		Setup(&fog, &ambient, &clear, FogDistance, fogDensity, fogType);
		/*
		int x = 320;
		int y = 240;
		DirectZob::GetInstance()->GetEngine()->Resize(x, y);
		*/
	}
}

void LightManager::SaveUnderNode(TiXmlElement* node)
{
	char tmpBuffer[256];
	ZobVector3 ambient = GetAmbientColor();
	ambient = Vector2Color(&ambient);
	TiXmlText t("");
	TiXmlElement e = TiXmlElement("AmbientColor");
	e.SetAttribute("r", ambient.x);
	e.SetAttribute("g", ambient.y);
	e.SetAttribute("b", ambient.z);
	node->InsertEndChild(e);
	ZobVector3 fogColor = GetFogColor();
	fogColor = Vector2Color(&fogColor);
	e = TiXmlElement("FogColor");
	e.SetAttribute("r", fogColor.x);
	e.SetAttribute("g", fogColor.y);
	e.SetAttribute("b", fogColor.z);
	node->InsertEndChild(e);
	ZobVector3 clearColor = GetClearColor();
	clearColor = Vector2Color(&clearColor);
	e = TiXmlElement("ClearColor");
	e.SetAttribute("r", clearColor.x);
	e.SetAttribute("g", clearColor.y);
	e.SetAttribute("b", clearColor.z);
	node->InsertEndChild(e);
	e = TiXmlElement("FogDensity");
	_snprintf_s(tmpBuffer, 256, "%.2f", GetFogDensity());
	t.SetValue(tmpBuffer);
	e.InsertEndChild(t);
	node->InsertEndChild(e);
	e = TiXmlElement("FogDistance");
	_snprintf_s(tmpBuffer, 256, "%.2f", GetFogDistance());
	t.SetValue(tmpBuffer);
	e.InsertEndChild(t);
	node->InsertEndChild(e);
	e = TiXmlElement("FogType");
	t.SetValue("");
	switch (GetFogType())
	{
	case eFogType_Exp:
		t.SetValue("exp");
		break;
	case eFogType_Exp2:
		t.SetValue("exp2");
		break;
	case eFogType_Linear:
		t.SetValue("linear");
		break;
	case eFogType_NoFog:
	default:
		t.SetValue("none");
		break;
	}
	e.InsertEndChild(t);
	node->InsertEndChild(e);
}
