#pragma once
#include "Types.h"
#include "Light.h"
#include "Vector3.h"
#include <string>
#include <vector>

class LightManager
{
public:
	LightManager();
	~LightManager();

	Light* CreatePointLight(std::string& name, Vector3 position, Vector3 color, float intensity, float distance);
	const std::vector<Light*>*  GetActiveLights() const;
	const Vector3* GetAmbientColor() const { return &m_ambientColor; };
	const float GetAmbientColorIntensity() const { return m_ambientColorIntensity; };
	void UnloadAll();
private:
	std::vector<Light*> m_lights;
	Vector3 m_ambientColor;
	float m_ambientColorIntensity;
};