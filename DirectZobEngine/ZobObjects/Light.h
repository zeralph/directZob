#pragma once
#include "../Rendering/ZobVector3.h"
#include "ZobObject.h"

class Light : public ZobObject
{
public:

	enum eLightType
	{
		eLightType_point=0,
		eLightType_spot,
		eLightType_directional,
		_eLightType_MAX_

	};

	Light(std::string& name, eLightType type, ZobColor color, float intensity, float distance, ZobObject* parent);
	Light(std::string id, TiXmlElement* node, ZobObject* parent);
	~Light() override;

	void				DrawGizmos(const Camera* camera, Engine* engine) override;
	void				Update(float dt) override;
	const ZobColor*		GetColor() const { return &m_lightColor; }
	const float			GetFallOffDistance() const { return m_distance; }
	const float			GetIntensity() const { return m_intensity; }
	const float			GetSpotAngleRad() const { return m_spotAngleRad; }
	const float			GetSpotAngleDeg() const { return RAD_TO_DEG(m_spotAngleRad); }
	void				SetSpotAngle(float f) { m_spotAngleRad = fmax(0, fmin(90, fabs(f))); m_spotAngleRad = DEG_TO_RAD(m_spotAngleRad); }
	const eLightType	GetType() const { return m_lightType; }
	inline bool			IsActive() const { return m_active && m_inFrtustrum; }

	void				SetColor(const ZobColor* v) { m_lightColor = v; }
	void				SetFallOffDistance(float f) { m_distance = f; }
	void				SetIntensity(float f ) { m_intensity=f; }
	void				SetType(eLightType t) { m_lightType=t; }
	void				SetActive(bool b) { m_active = b; }
	
protected:
	void				InitVariablesExposer() override;
	
private:

	void				NewLightConfiguration();
	void				drawSpotGizmos(const Camera* camera, Engine* engine) const;
	void				drawPointGizmos(const Camera* camera, Engine* engine) const;
	void				drawDirectionalGizmos(const Camera* camera, Engine* engine) const;
	void				ComputeBoundingBoxes(const ZobMatrix4x4& modelMatrix, const Camera* camera, Engine* engine);

	ZobColor m_lightColor;
	float m_intensity;
	float m_distance;
	float m_spotAngleRad;
	bool m_active;
	bool m_inFrtustrum;
	eLightType m_lightType;
	Box m_OBB;
	Box m_AABB;
};


