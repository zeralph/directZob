#pragma once
#include "ZobVector3.h"
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
	Light(TiXmlElement* node, ZobObject* parent);
	~Light() override;

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;

	const ZobVector3*		GetColor() const { return &m_color; }
	const float			GetFallOffDistance() const { return m_distance; }
	const float			GetIntensity() const { return m_intensity; }
	const float			GetSpotAngle() const { return m_spotAngle; }
	void				SetSpotAngle(float f) { m_spotAngle= fmax(0, fmin(90, fabs(f))); }
	const eLightType	GetType() const { return m_lightType; }
	inline bool			IsActive() const { return m_active; }

	void				SetColor(const ZobVector3* v) { m_color = v; }
	void				SetFallOffDistance(float f) { m_distance = f; }
	void				SetIntensity(float f ) { m_intensity=f; }
	void				SetType(eLightType t) { m_lightType=t; }
	void				SetActive(bool b) { m_active = b; }
	

	
private:

	void				NewLightConfiguration();
	void				drawSpotGizmos(const Camera* camera, Core::Engine* engine);
	void				drawPointGizmos(const Camera* camera, Core::Engine* engine);
	void				drawDirectionalGizmos(const Camera* camera, Core::Engine* engine);

	ZobVector3 m_color;
	float m_intensity;
	float m_distance;
	float m_spotAngle;
	bool m_active;
	eLightType m_lightType;
};


