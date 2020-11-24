#pragma once
#include "Types.h"
#include "Light.h"
#include "ZobVector3.h"
#include <string>
#include <vector>

class LightManager
{
public:

	LightManager();
	~LightManager();

	void						Setup(ZobVector3* fogColor, ZobVector3* ambientColor, ZobVector3* clearColor, float fogDistance, float fogDensity, eFogType fogType, float ambientIntensity);
	void						ReInitGlobalSettings();
	void						AddLight(Light* l);
	Light*						CreateLight(Light::eLightType type);
	Light*						CreateLight(std::string& name, Light::eLightType type, ZobVector3 position, ZobVector3 color, float intensity, float distance, ZobObject* parent);
	const std::vector<Light*>*  GetActiveLights() const;
	Light*						GetLight(const std::string& name) const;
	const ZobVector3*				GetAmbientColor() const { return &m_ambientColor; };
	const ZobVector3*				GetFogColor() const { return &m_fogColor; };
	const ZobVector3*				GetClearColor() const { return &m_clearColor; };
	void						SetAmbientColor(ZobVector3* c) { m_ambientColor = c; };
	void						SetFogColor(ZobVector3* c) { m_fogColor = c; };
	void						SetClearColor(ZobVector3* c) { m_clearColor = c; };
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
	ZobVector3 m_ambientColor;
	ZobVector3 m_clearColor;
	float m_ambientColorIntensity;
	ZobVector3 m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	bool m_lightingEnabled = true;
};