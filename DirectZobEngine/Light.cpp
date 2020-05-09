#include "Light.h"
#include "tinyxml.h"

Light::Light(std::string &name, Vector3 color, float intensity, float distance, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, name, nullptr, parent)
{
	m_color = color;
	m_intensity = intensity;
	m_distance = distance;
	m_active = true;
}

Light::Light(TiXmlElement* node, ZobObject* parent)
	:ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, node, NULL, parent)
{
		float x, y, z;
		TiXmlElement* f = node->FirstChildElement("Color");
		if (f)
		{
			x = atof(f->Attribute("r"));
			y = atof(f->Attribute("g"));
			z = atof(f->Attribute("b"));
			m_color = Vector3(x/255.0f, y/255.0f, z/255.0f);
		}
		f = node->FirstChildElement("Intensity");
		float intensity = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement("FallOffDistance");
		float falloff = f ? atof(f->GetText()) : 1.0f;
		m_distance = falloff;
		m_intensity = intensity;
		m_active = true;
}

Light::~Light()
{
}

void Light::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	ZobObject::DrawGizmos(camera, engine);
	uint c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);	
	engine->QueueEllipse(camera, &m_translation, &m_up, 1.0f, 1.0f, c, true);
	engine->QueueEllipse(camera, &m_translation, &m_left, 1.0f, 1.0f, c, true);
	engine->QueueEllipse(camera, &m_translation, &m_forward, 1.0f, 1.0f, c, true);
}

TiXmlNode* Light::SaveUnderNode(TiXmlNode* node)
{
	char tmpBuffer[256];
	TiXmlNode* n = ZobObject::SaveUnderNode(node);
	TiXmlElement* ne = (TiXmlElement*)n;
	ne->SetAttribute("type", "pointlight");
	Vector3 c = GetColor();
	int r = (int)(c.x * 255.0f);
	int g = (int)(c.y * 255.0f);
	int b = (int)(c.z * 255.0f);
	TiXmlElement color = TiXmlElement("Color");
	color.SetAttribute("r", r);
	color.SetAttribute("g", g);
	color.SetAttribute("b", b);
	ne->InsertEndChild(color);
	TiXmlText t("");
	TiXmlElement intensity = TiXmlElement("Intensity");
	_snprintf_s(tmpBuffer, 256, "%.2f", GetIntensity());
	t.SetValue(tmpBuffer);
	intensity.InsertEndChild(t);
	ne->InsertEndChild(intensity);
	TiXmlElement fallOff = TiXmlElement("FallOffDistance");
	_snprintf_s(tmpBuffer, 256, "%.2f", GetFallOffDistance());
	t.SetValue(tmpBuffer);
	fallOff.InsertEndChild(t);
	ne->InsertEndChild(fallOff);
	return n;
}