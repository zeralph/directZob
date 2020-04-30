#include "Light.h"

Light::Light(std::string &name, Vector3 color, float intensity, float distance, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, name, nullptr, parent)
{
	m_color = color;
	m_intensity = intensity;
	m_distance = distance;
	m_active = true;
}

Light::~Light()
{
}

void Light::Draw(const Camera* camera, Core::Engine* engine)
{
	ZobObject::Draw(camera, engine);
	Vector3 v1 = Vector3(m_translation.x - 1, m_translation.y, m_translation.z);
	Vector3 v2 = Vector3(m_translation.x + 1, m_translation.y, m_translation.z);
	Vector3 v3 = Vector3(m_translation.x, m_translation.y - 1, m_translation.z);
	Vector3 v4 = Vector3(m_translation.x, m_translation.y + 1, m_translation.z);
	Vector3 v5 = Vector3(m_translation.x, m_translation.y, m_translation.z - 1);
	Vector3 v6 = Vector3(m_translation.x, m_translation.y, m_translation.z + 1);
	int c = ((int)(m_color.x * 255) << 16) + ((int)(m_color.y * 255) << 8) + (int)(m_color.z * 255);
	m_rotationMatrix.Mul(&v1);
	m_rotationMatrix.Mul(&v2);
	m_rotationMatrix.Mul(&v3);
	m_rotationMatrix.Mul(&v4);
	m_rotationMatrix.Mul(&v5);
	m_rotationMatrix.Mul(&v6);
	engine->QueueLine(camera, &v1, &v2, c);
	engine->QueueLine(camera, &v3, &v4, c);
	engine->QueueLine(camera, &v5, &v6, c);
}