#pragma once
#include "Rendering/ZobVector3.h"
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

	Light(std::string& name, eLightType type, ZobVector3 color, float intensity, float distance, ZobObject* parent);
	Light(ulong id, TiXmlElement* node, ZobObject* parent);
	~Light() override;

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;

	const ZobVector3*	GetColor() const { return &m_color; }
	const float			GetFallOffDistance() const { return m_distance; }
	const float			GetIntensity() const { return m_intensity; }
	const float			GetSpotAngleRad() const { return m_spotAngleRad; }
	const float			GetSpotAngleDeg() const { return RAD_TO_DEG(m_spotAngleRad); }
	void				SetSpotAngle(float f) { m_spotAngleRad = fmax(0, fmin(90, fabs(f))); m_spotAngleRad = DEG_TO_RAD(m_spotAngleRad); }
	const eLightType	GetType() const { return m_lightType; }
	inline bool			IsActive() const { return m_active; }

	void				SetColor(const ZobVector3* v) { m_color = v; }
	void				SetFallOffDistance(float f) { m_distance = f; }
	void				SetIntensity(float f ) { m_intensity=f; }
	void				SetType(eLightType t) { m_lightType=t; }
	void				SetActive(bool b) { m_active = b; }
	

	
private:

	void				NewLightConfiguration();
	void				drawSpotGizmos(const Camera* camera, Core::Engine* engine) const;
	void				drawPointGizmos(const Camera* camera, Core::Engine* engine) const;
	void				drawDirectionalGizmos(const Camera* camera, Core::Engine* engine) const;

	ZobVector3 m_color;
	float m_intensity;
	float m_distance;
	float m_spotAngleRad;
	bool m_active;
	eLightType m_lightType;
};


