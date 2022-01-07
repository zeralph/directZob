#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobComponent.h"
#include <string>
#include <vector>

class ZobComponentLight : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentLight() override;
	void					Init(DirectZobType::sceneLoadingCallback cb) override;
	void					PreUpdate(float dt) override;
	void					PostUpdate() override;
	void					UpdateBeforeObject(float dt) override;
	void					UpdateAfterObject(float dt) override;
	void					QueueForDrawing(const Camera* camera, Engine* engine) override;
	void					EditorUpdate() override;

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