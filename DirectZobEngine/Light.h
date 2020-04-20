#pragma once
#include "Vector3.h"
#include "ZobObject.h"

class Light : public ZobObject
{
public:
	Light(std::string& name, Vector3 color, float intensity, float distance);
	~Light();

	const Vector3* GetColor() const { return &m_color; }
	const float GetFallOffDistance() const { return m_distance; }
	const float GetIntensity() const { return m_intensity; }
private:
	Vector3 m_color;
	float m_intensity;
	float m_distance;
};


