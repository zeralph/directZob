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

	void						Setup(Vector3* fogColor, Vector3* ambientColor, Vector3* clearColor, float fogDistance, float fogDensity, eFogType fogType);
	void						ReInitGlobalSettings();
	void						AddLight(Light* l);
	Light*						CreateLight(Light::eLightType type);
	Light*						CreateLight(std::string& name, Light::eLightType type, Vector3 position, Vector3 color, float intensity, float distance, ZobObject* parent);
	const std::vector<Light*>*  GetActiveLights() const;
	Light*						GetLight(const std::string& name) const;
	const Vector3*				GetAmbientColor() const { return &m_ambientColor; };
	const Vector3*				GetFogColor() const { return &m_fogColor; };
	const Vector3*				GetClearColor() const { return &m_clearColor; };
	void						SetAmbientColor(Vector3* c) { m_ambientColor = c; };
	void						SetFogColor(Vector3* c) { m_fogColor = c; };
	void						SetClearColor(Vector3* c) { m_clearColor = c; };
	void						SetFogDistance(float f) { m_fogDistance = f; };
	void						SetAmbientColorIntensity(float f) { m_ambientColorIntensity = f; };
	void						SetFogDensity(float f) { m_fogDensity = f; }
	void						SetFogType(eFogType t) { m_fogType = t; }
	const float					GetFogDistance() const { return m_fogDistance; };
	const float					GetAmbientColorIntensity() const { return m_ambientColorIntensity; };
	const float					GetFogDensity() const { return m_fogDensity; }
	const eFogType				GetFogType() const { return m_fogType; }
	void						RemoveLight(Light* l);
	const bool					EnableLighting() const  { return m_lightingEnabled; }
	void						EnableLighting(bool b) { m_lightingEnabled = b; }
	void						UnloadAll();
	void						LoadFromNode(TiXmlElement* node);
	void						SaveUnderNode(TiXmlElement* node);
private:
	eFogType m_fogType;
	std::vector<Light*> m_lights;
	Vector3 m_ambientColor;
	Vector3 m_clearColor;
	float m_ambientColorIntensity;
	Vector3 m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	bool m_lightingEnabled = true;
};