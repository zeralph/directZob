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
	void					PreUpdate(float dt, bool isPlaying) override;
	void					PostUpdate(bool isPlaying) override;
	void					QueueForDrawing(const Camera* camera, Engine* engine) override;
	void					EditorUpdate(bool isPlaying) override;

private:
	ZobComponentRotator(ZobObject* zobObject);

	float m_rotateXdt;
	float m_rotateYdt;
	float m_rotateZdt;

};