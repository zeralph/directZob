#include "Light.h"
#include "tinyxml.h"
#include "DirectZob.h"

Light::Light(std::string &name, eLightType type, ZobVector3 color, float intensity, float distance, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, name, parent)
{
	m_color = color;
	m_intensity = intensity;
	m_distance = distance;
	m_active = true;
	m_lightType = type;
	m_spotAngleRad = DEG_TO_RAD(30.0f);
	NewLightConfiguration();
}

Light::Light(ulong id, TiXmlElement* node, ZobObject* parent)
	:ZobObject(id, node, parent)
{
		float x, y, z;
		TiXmlElement* f = node->FirstChildElement("Color");
		if (f)
		{
			x = atof(f->Attribute("r"));
			y = atof(f->Attribute("g"));
			z = atof(f->Attribute("b"));
			m_color = ZobVector3(x/255.0f, y/255.0f, z/255.0f);
		}
		f = node->FirstChildElement("Intensity");
		float intensity = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement("FallOffDistance");
		float falloff = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement("Type");
		std::string type  = std::string(f->GetText()?f->GetText() :"");
		m_lightType = eLightType_point;
		if (f)
		{
			if (type == "point")
			{
				m_lightType = eLightType_point;
			}
			else if(type == "spot")
			{
				m_lightType = eLightType_spot;
			}
			else if (type == "directional")
			{
				m_lightType = eLightType_directional;
			}
			else
			{
				//log a warn ...
			}
		}
		m_distance = falloff;
		m_intensity = intensity;
		m_active = true;
		m_spotAngleRad = DEG_TO_RAD(30.0f);
}

Light::~Light()
{
	DirectZob::LogInfo("Delete Light %s", m_name.c_str());
	DirectZob::AddIndent();
	DirectZob::GetInstance()->GetLightManager()->RemoveLight(this);
	DirectZob::RemoveIndent();
}

void Light::NewLightConfiguration()
{
	switch (m_lightType)
	{
	case eLightType_spot:
		SetWorldRotation(-90, 0, 0);
		//m_rotation = ZobVector3(-90.0f, 0.0f, 0.0f);
		m_distance = 10.0f;
		m_spotAngleRad = DEG_TO_RAD(30.0f);
		break;
	case eLightType_directional:
		SetWorldRotation(-70.0f, 30.0f, 30.0f);
		break;
	default:
		break;
	}
}

void Light::drawPointGizmos(const Camera* camera, Core::Engine* engine) const
{
	ZobVector3 t = GetWorldPosition();
	uint c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);
	engine->QueueEllipse(camera, &t, &m_up, 1.0f, 1.0f, c, true, false);
	engine->QueueEllipse(camera, &t, &m_left, 1.0f, 1.0f, c, true, false);
	engine->QueueEllipse(camera, &t, &m_forward, 1.0f, 1.0f, c, true, false);
}

void Light::drawSpotGizmos(const Camera* camera, Core::Engine* engine) const
{
	ZobVector3 t = GetWorldPosition();
	uint c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);
	ZobVector3 v1, v2, v;
	v1 = m_forward;
	v2 = m_left;
	v1.Mul(m_distance);
	v1 = v1 + GetWorldPosition();
	float r = RAD_TO_DEG(m_spotAngleRad) / 2.0f;
	r = DEG_TO_RAD(r);
	r = tan(r) * m_distance;
	engine->QueueEllipse(camera, &v1, &m_forward, r, r, c, true, false);
	v2 = m_left;
	v = v1 + (v2 * r);
	engine->QueueLine(camera, &t, &v, c, true, false);
	v2 = m_left;
	v = v1 - (v2 * r);
	engine->QueueLine(camera, &t, &v, c, true, false);
	v2 = m_up;
	v = v1 + (v2 * r);
	engine->QueueLine(camera, &t, &v, c, true, false);
	v2 = m_up;
	v = v1 - (v2 * r);
	engine->QueueLine(camera, &t, &v, c, true, false);
}

void Light::drawDirectionalGizmos(const Camera* camera, Core::Engine* engine) const
{
	ZobVector3 t = GetWorldPosition();
	uint c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);
	ZobVector3 v0 = t + m_forward;
	ZobVector3 v1 = t - m_forward;
	v0 = v0 + m_left;
	v1 = v1 + m_left;
	engine->QueueLine(camera, &v0, &v1, c, true, false);
	v0 = v0 + m_up;
	v1 = v1 + m_up;
	engine->QueueLine(camera, &v0, &v1, c, true, false);
	v0 = t + m_forward;
	v1 = t - m_forward;
	v0 = v0 - m_left;
	v1 = v1 - m_left;
	engine->QueueLine(camera, &v0, &v1, c, true, false);
	v0 = v0 - m_up;
	v1 = v1 - m_up;
	engine->QueueLine(camera, &v0, &v1, c, true, false);
	v1 = t - m_forward;
	engine->QueueEllipse(camera, &v1, &m_forward, 1.0f, 1.0f, c, true, false);
}

void Light::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	ZobObject::DrawGizmos(camera, engine);
	switch (m_lightType)
	{
	case eLightType_spot:
		drawSpotGizmos(camera, engine);
		break;
	case eLightType_directional:
		drawDirectionalGizmos(camera, engine);
		break;
	default:
		drawPointGizmos(camera, engine);
		break;
	}
}

TiXmlNode* Light::SaveUnderNode(TiXmlNode* node)
{
	char tmpBuffer[256];
	TiXmlNode* n = ZobObject::SaveUnderNode(node);
	TiXmlElement* ne = (TiXmlElement*)n;
	ne->SetAttribute("type", "light");
	ZobVector3 c = GetColor();
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
	switch (m_lightType)
	{
	case eLightType_directional:
		t.SetValue("directional");
		break;
	case eLightType_spot:
		t.SetValue("spot");
		break;
	case eLightType_point:
	default:
		t.SetValue("point");
		break;
	}
	TiXmlElement lightType = TiXmlElement("Type");
	lightType.InsertEndChild(t);
	ne->InsertEndChild(lightType);
	return n;
}