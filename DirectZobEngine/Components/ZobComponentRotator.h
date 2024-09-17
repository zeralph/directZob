#pragma once

#include "ZobComponent.h"
#include <string>
#include <vector>

class ZobComponentRotator : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentRotator() override;
	void					Init(DirectZobType::sceneLoadingCallback cb) override;
	void					PreUpdate(float dt) override;
	void					PostUpdate() override;
	void					QueueForDrawing(const Camera* camera, Engine* engine) override;
	void					EditorUpdate() override;

private:
	ZobComponentRotator(ZobObject* zobObject);

	float m_rotateXdt;
	float m_rotateYdt;
	float m_rotateZdt;

};