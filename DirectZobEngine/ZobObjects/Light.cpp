#include "Light.h"
#include "tinyxml.h"
#include "../DirectZob.h"
#include "../Misc/ZobXmlHelper.h"
#include "../Managers/LightManager.h"
#include "../Managers/CameraManager.h"
#include "../Components/GraphicComponents/ZobComponentSprite.h" 
#include "../Misc/ZobFilePath.h"
#include "../SceneLoader.h"

Light::Light(std::string& name, eLightType type, ZobColor color, float intensity, float distance, ZobObject* parent, bool bEditor) :
	ZobObject((bEditor?ZobEntity::type_editor:ZobEntity::type_scene), ZobEntity::subtype_zobLight, name, parent)
{
	SET_CLASS_AND_NAME
	m_lightColor = color;
	m_intensity = intensity;
	m_distance = distance;
	m_active = true;
	m_lightType = type;
	m_spotAngleRad = DEG_TO_RAD(30.0f);
	m_inFrtustrum = false;
	NewLightConfiguration();
	InitVariablesExposer();
	/*
	if (DirectZob::IsEditorMode() && !bEditor)
	{
		ZobComponentSprite* b = (ZobComponentSprite*)ZobComponentFactory::CreateComponent(this, "Sprite", true);
		std::string path = SceneLoader::GetResourcePath();
		path = "D:\\Git\\directZob\\resources\\";
		ZobFilePath zfp = ZobFilePath("light", path, "light.png", true);
		b->Set(zfp);
		b->SetForEditor();
	}
	*/
}

Light::Light(zobId id, TiXmlElement* node, ZobObject* parent)
	:ZobObject(id, node, parent)
{
	SET_CLASS_AND_NAME
	m_distance = 0;
	m_intensity = 0;
	m_active = false;
	m_spotAngleRad = 0;
	InitVariablesExposer();
	m_varExposer->ReadNode(node);
	if (DirectZob::IsEditorMode())
	{
		ZobComponentSprite* b = (ZobComponentSprite*)ZobComponentFactory::CreateComponent(this, "Sprite", true);
		ZobFilePath zfp = ZobFilePath("light", SceneLoader::GetResourcePath(), "light.png", true);
		b->Set(zfp);
		b->SetForEditor();
	}
}

Light::~Light()
{
	DirectZob::LogInfo("Delete Light %s", m_name.c_str());
	DirectZob::AddIndent();
	DirectZob::GetInstance()->GetLightManager()->RemoveLight(this);
	DirectZob::RemoveIndent();
}

void Light::InitVariablesExposer()
{
	m_varExposer->StartGroup("Light");
	m_varExposer->WrapVariable<ZobColor>("Color", &m_lightColor, NULL, false, true);
	m_varExposer->WrapVariable<float>("Intensity", &m_intensity, NULL, false, true);
	m_varExposer->WrapVariable<float>("Distance", &m_distance, NULL, false, true);
	m_varExposer->WrapVariable<float>("SpotAngle", &m_spotAngleRad, NULL, false, true);
	m_varExposer->WrapVariable<bool>("Active", &m_active, NULL, false, true);
	m_varExposer->WrapVariable<bool>("InFrustrum", &m_inFrtustrum, NULL, true, false);
	eLightType types[3] = { eLightType::eLightType_point, eLightType::eLightType_directional, eLightType::eLightType_spot };
	const char* typeStr[3] = { "Point", "Directional", "Spot"};
	m_varExposer->WrapEnum<eLightType>("Type", &m_lightType, 3, types, typeStr, NULL, false, false, true);
	m_varExposer->EndGroup();
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

void Light::Update(float dt, bool isPlaying)
{
	ZobObject::Update(dt, isPlaying);
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

void Light::drawPointGizmos(const Camera* camera, Engine* engine) const
{
	ZobVector3 t = GetWorldPosition();
	engine->QueueEllipse(camera, &t, &m_up, 1.0f, 1.0f, &m_lightColor, true, false);
	engine->QueueEllipse(camera, &t, &m_left, 1.0f, 1.0f, &m_lightColor, true, false);
	engine->QueueEllipse(camera, &t, &m_forward, 1.0f, 1.0f, &m_lightColor, true, false);
}

void Light::drawSpotGizmos(const Camera* camera, Engine* engine) const
{
	ZobVector3 t = GetWorldPosition();
	ZobVector3 v1, v2, v;
	v1 = m_forward;
	v2 = m_left;
	v1.Mul(m_distance);
	v1 = v1 + GetWorldPosition();
	float r = RAD_TO_DEG(m_spotAngleRad) / 2.0f;
	r = DEG_TO_RAD(r);
	r = tan(r) * m_distance;
	engine->QueueEllipse(camera, &v1, &m_forward, r, r, &m_lightColor, true, false);
	v2 = m_left;
	v = v1 + (v2 * r);
	engine->QueueLine(camera, &t, &v, &m_lightColor, true, false);
	v2 = m_left;
	v = v1 - (v2 * r);
	engine->QueueLine(camera, &t, &v, &m_lightColor, true, false);
	v2 = m_up;
	v = v1 + (v2 * r);
	engine->QueueLine(camera, &t, &v, &m_lightColor, true, false);
	v2 = m_up;
	v = v1 - (v2 * r);
	engine->QueueLine(camera, &t, &v, &m_lightColor, true, false);
}

void Light::drawDirectionalGizmos(const Camera* camera, Engine* engine) const
{
	if (m_type != ZobEntity::type_editor)
	{
		ZobVector3 t = GetWorldPosition();
		ZobVector3 v0 = t + m_forward;
		ZobVector3 v1 = t - m_forward;
		v0 = v0 + m_left;
		v1 = v1 + m_left;
		engine->QueueLine(camera, &v0, &v1, &m_lightColor, true, false);
		v0 = v0 + m_up;
		v1 = v1 + m_up;
		engine->QueueLine(camera, &v0, &v1, &m_lightColor, true, false);
		v0 = t + m_forward;
		v1 = t - m_forward;
		v0 = v0 - m_left;
		v1 = v1 - m_left;
		engine->QueueLine(camera, &v0, &v1, &m_lightColor, true, false);
		v0 = v0 - m_up;
		v1 = v1 - m_up;
		engine->QueueLine(camera, &v0, &v1, &m_lightColor, true, false);
		v1 = t - m_forward;
		engine->QueueEllipse(camera, &v1, &m_forward, 1.0f, 1.0f, &m_lightColor, true, false);
	}
}

void Light::DrawGizmos(const Camera* camera, Engine* engine)
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