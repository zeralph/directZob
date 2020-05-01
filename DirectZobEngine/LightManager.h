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
	void Setup(Vector3* fogColor, Vector3* ambientColor, Vector3* clearColor, float fogDistance, float fogDensity, FogType fogType);
	void ReInitGlobalSettings();
	Light* CreatePointLight(std::string& name, Vector3 position, Vector3 color, float intensity, float distance, ZobObject* parent);
	const std::vector<Light*>*  GetActiveLights() const;
	Light* GetLight(const std::string& name) const;
	const Vector3* GetAmbientColor() const { return &m_ambientColor; };
	const Vector3* GetFogColor() const { return &m_fogColor; };
	const Vector3* GetClearColor() const { return &m_clearColor; };
	const float GetFogDistance() const { return m_fogDistance; };
	const float GetAmbientColorIntensity() const { return m_ambientColorIntensity; };
	const float GetFogDensity() const { return m_fogDensity; }
	const FogType GetFogType() const { return m_fogType; }
	void RemoveLight(Light* l);
	const bool EnableLighting() const  { return m_lightingEnabled; }
	void EnableLighting(bool b) { m_lightingEnabled = b; }
	void UnloadAll();
private:
	FogType m_fogType;
	std::vector<Light*> m_lights;
	Vector3 m_ambientColor;
	Vector3 m_clearColor;
	float m_ambientColorIntensity;
	Vector3 m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	bool m_lightingEnabled = true;
};