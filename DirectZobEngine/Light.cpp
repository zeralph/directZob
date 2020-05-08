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
}

Light::~Light()
{
}

void Light::Draw(const Camera* camera, Core::Engine* engine)
{
	ZobObject::Draw(camera, engine);
	int c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);	
	Vector3 up = Vector3(0, 1, 0);
	engine->QueueEllipse(camera, &m_translation, &up, 1.0f, 1.0f, c, true);
	Vector3 left = Vector3(1, 0, 0);
	engine->QueueEllipse(camera, &m_translation, &left, 1.0f, 1.0f, c, true);
	Vector3 forward = Vector3(0, 0, 1);
	engine->QueueEllipse(camera, &m_translation, &forward, 1.0f, 1.0f, c, true);
}