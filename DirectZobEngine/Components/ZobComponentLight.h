#pragma once

#undef min
#undef max
#undef None

#include "../Rendering/Color.h"
#include "ZobComponent.h"
#include <string>
#include <vector>

class ZobComponentLight : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentLight() override;
	void					Init(DirectZobType::sceneLoadingCallback cb) override;
	void					PreUpdate(float dt, bool isPlaying) override;
	void					PostUpdate(bool isPlaying) override;
	void					QueueForDrawing(const Camera* camera, Engine* engine) override;
	void					EditorUpdate(bool isPlaying) override;

private:
	ZobComponentLight(ZobObject* zobObject, bool bEditorZobComponent);

	ZobColor m_lightColor;
	float m_intensity;
	float m_distance;
	float m_spotAngleRad;
	bool m_active;
	bool m_inFrtustrum;
	//eLightType m_lightType;
	Box m_OBB;
	Box m_AABB;

};