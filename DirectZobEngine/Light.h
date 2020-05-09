#pragma once
#include "Vector3.h"
#include "ZobObject.h"

class Light : public ZobObject
{
public:
	Light(std::string& name, Vector3 color, float intensity, float distance, ZobObject* parent);
	Light(TiXmlElement* node, ZobObject* parent);
	~Light();

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;

	const Vector3*		GetColor() const { return &m_color; }
	const float			GetFallOffDistance() const { return m_distance; }
	const float			GetIntensity() const { return m_intensity; }
	void				SetActive(bool b) { m_active = b; }
	inline bool			IsActive() const { return m_active; }

	
private:
	Vector3 m_color;
	float m_intensity;
	float m_distance;
	bool m_active;
};


