#pragma once
#include "../Types.h"
#include "../ZobObjects/Light.h"
#include "../Rendering/ZobVector3.h"
#include "../Rendering/Color.h"
#include <string>
#include <vector>

class LightManager
{
public:

	LightManager();
	~LightManager();

	void								Setup(ZobColor* fogColor, ZobColor* ambientColor, ZobColor* clearColor, float fogDistance, float fogDensity, eFogType fogType, float ambientIntensity);
	void								ReInitGlobalSettings();
	void								AddLight(Light* l);
	void								Update();
	Light*								CreateLight(Light::eLightType type);
	Light*								CreateLight(std::string& name, Light::eLightType type, ZobVector3 position, ZobColor color, float intensity, float distance, ZobObject* parent);
	const std::vector<const Light*>*	GetActiveLights() const;
	const std::vector<const Light*>*	GetEditorLight() const { return (std::vector<const Light*>*)&m_editorLight; }
	Light*								GetLight(const std::string& name) const;
	const ZobColor*						GetAmbientColor() const { return &m_ambientColor; };
	const ZobColor*						GetFogColor() const { return &m_fogColor; };
	const ZobColor*						GetClearColor() const { return &m_clearColor; };
	void								SetAmbientColor(ZobColor* c) { m_ambientColor = c; };
	void								SetFogColor(ZobColor* c) { m_fogColor = c; };
	void								SetClearColor(ZobColor* c) { m_clearColor = c; };
	void								SetFogDistance(float f) { m_fogDistance = f; };
	void								SetAmbientColorIntensity(float f) { m_ambientColorIntensity = f; };
	void								SetFogDensity(float f) { m_fogDensity = f; }
	void								SetFogType(eFogType t) { m_fogType = t; }
	const float							GetFogDistance() const { return m_fogDistance; };
	const float							GetAmbientColorIntensity() const { return m_ambientColorIntensity; };
	const float							GetFogDensity() const { return m_fogDensity; }
	const eFogType						GetFogType() const { return m_fogType; }
	void								RemoveLight(Light* l);
	const bool							EnableLighting() const  { return m_lightingEnabled; }
	void								EnableLighting(bool b) { m_lightingEnabled = b; }
	void								UnloadAll();
	void								PreUpdate(float dt);
	void								LoadFromNode(TiXmlElement* node);
	void								SaveUnderNode(TiXmlElement* node);
	ZobVariablesExposer*				GetVariablesExposer() { return m_varExposer; }
private:
	std::vector<Light*> m_lights;
	std::vector<Light*> m_lightsToAdd;
	std::vector<Light*> m_lightsToRemove;
	std::vector<const Light*> m_activeLights;
	std::vector<Light*> m_editorLight;
	int m_lightIndex;
	ZobColor m_fogColor;
	ZobColor m_ambientColor;
	ZobColor m_clearColor;
	float m_ambientColorIntensity;
	eFogType m_fogType;
	float m_fogDistance;
	float m_fogDensity;
	bool m_lightingEnabled = true;
	ZobVariablesExposer* m_varExposer;
};