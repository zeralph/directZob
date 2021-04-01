#include "Light.h"
#include "tinyxml.h"
#include "DirectZob.h"
#include "Misc/ZobXmlHelper.h"

Light::Light(std::string &name, eLightType type, ZobVector3 color, float intensity, float distance, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, name, parent)
{
	m_color = color;
	m_intensity = intensity;
	m_distance = distance;
	m_active = true;
	m_lightType = type;
	m_spotAngleRad = DEG_TO_RAD(30.0f);
	m_inFrtustrum = false;
	NewLightConfiguration();
}

Light::Light(std::string id, TiXmlElement* node, ZobObject* parent)
	:ZobObject(id, node, parent)
{
		float x, y, z;
		TiXmlElement* f = node->FirstChildElement(XML_ELEMENT_LIGHT_COLOR);
		if (f)
		{
			x = atof(f->Attribute("r"));
			y = atof(f->Attribute("g"));
			z = atof(f->Attribute("b"));
			m_color = ZobVector3(x/255.0f, y/255.0f, z/255.0f);
		}
		f = node->FirstChildElement(XML_ELEMENT_LIGHT_INTENSITY);
		float intensity = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement(XML_ELEMENT_LIGHT_FALLOFF);
		float falloff = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement(XML_ATTR_TYPE);
		std::string type  = std::string(f->GetText()?f->GetText() :"");
		m_lightType = eLightType_point;
		if (f)
		{
			if (type == XML_ELEMENT_LIGHT_TYPE_POINT)
			{
				m_lightType = eLightType_point;
			}
			else if(type == XML_ELEMENT_LIGHT_TYPE_SPOT)
			{
				m_lightType = eLightType_spot;
			}
			else if (type == XML_ELEMENT_LIGHT_TYPE_DIRECTIONAL)
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

void Light::Update(float dt)
{
	ZobObject::Update(dt);
	if (m_lightType == eLightType_directional)
	{
		m_inFrtustrum = true;
	}
	else
	{
		ZobVector3 min, max;
		if (m_lightType == eLightType_point)
		{
			min.x = -m_distance / 2;
			min.y = -m_distance / 2;
			min.z = -m_distance / 2;
			max.x = m_distance / 2;
			max.y = m_distance / 2;
			max.z = m_distance / 2;
		}
		else
		{
			min.x = -m_distance / 2;
			min.y = -m_distance / 2;
			min.z = -m_distance / 2;
			max.x = m_distance / 2;
			max.y = m_distance / 2;
			max.z = m_distance / 2;
		}
		const Engine* e = DirectZob::GetInstance()->GetEngine();
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		e->ComputeBoundingBoxes(&m_modelMatrix, &min, &max, &m_OBB, &m_AABB);
		m_inFrtustrum = e->IsInFrustrum(c, &m_AABB);
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
	//engine->QueueWorldBox(camera, &m_AABB, 0xFFFFFF, false, false);
	//engine->QueueWorldBox(camera, &m_OBB, 0xDDDDDD, false, false);
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
	ne->SetAttribute(XML_ATTR_TYPE, XML_ATTR_TYPE_LIGHT);
	ZobVector3 c = GetColor();
	int r = (int)(c.x * 255.0f);
	int g = (int)(c.y * 255.0f);
	int b = (int)(c.z * 255.0f);
	TiXmlElement color = TiXmlElement(XML_ELEMENT_LIGHT_COLOR);
	color.SetAttribute("r", r);
	color.SetAttribute("g", g);
	color.SetAttribute("b", b);
	ne->InsertEndChild(color);
	TiXmlText t("");
	TiXmlElement intensity = TiXmlElement(XML_ELEMENT_LIGHT_INTENSITY);
	_snprintf_s(tmpBuffer, 256, "%.2f", GetIntensity());
	t.SetValue(tmpBuffer);
	intensity.InsertEndChild(t);
	ne->InsertEndChild(intensity);
	TiXmlElement fallOff = TiXmlElement(XML_ELEMENT_LIGHT_FALLOFF);
	_snprintf_s(tmpBuffer, 256, "%.2f", GetFallOffDistance());
	t.SetValue(tmpBuffer);
	fallOff.InsertEndChild(t);
	ne->InsertEndChild(fallOff);
	switch (m_lightType)
	{
	case eLightType_directional:
		t.SetValue(XML_ELEMENT_LIGHT_TYPE_DIRECTIONAL);
		break;
	case eLightType_spot:
		t.SetValue(XML_ELEMENT_LIGHT_TYPE_SPOT);
		break;
	case eLightType_point:
	default:
		t.SetValue(XML_ELEMENT_LIGHT_TYPE_POINT);
		break;
	}
	TiXmlElement lightType = TiXmlElement(XML_ATTR_TYPE);
	lightType.InsertEndChild(t);
	ne->InsertEndChild(lightType);
	return n;
}