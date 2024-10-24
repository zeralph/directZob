#pragma once
#include "../../Types.h"
#include "../ZobComponent.h"
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>

class ZobObject;
class Bullet : public ZobComponent
{
    friend class ZobComponentFactory;
    public:
        ~Bullet() override;
		void		Init(DirectZobType::sceneLoadingCallback cb) override;
		void		PreUpdate(float dt, bool isPlaying) override;
		void		Start() override;
		void		PostUpdate(bool isPlaying) override;
		void		EditorUpdate(bool isPlaying) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		Bullet(ZobObject* zobObject);

        float       m_lifeTimeSec;
        float       m_speed;
		float       m_remainingTime;
};