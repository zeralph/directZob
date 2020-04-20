#include "Light.h"

Light::Light(std::string &name, Vector3 color, float intensity, float distance, ZobObject* parent) :
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobLight, name, NULL, parent)
{
	m_color = color;
	m_intensity = intensity;
	m_distance = distance;
}

Light::~Light()
{

}