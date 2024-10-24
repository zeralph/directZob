#pragma once
#include "../../Types.h"
#include "../../Misc/ZobFilePath.h"
#include "../ZobComponent.h"
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>

class ZobObject;
class Canon : public ZobComponent
{
    friend class ZobComponentFactory;
    public:
         ~Canon() override;
		void		Init(DirectZobType::sceneLoadingCallback cb) override;
		void		PreUpdate(float dt, bool isPlaying) override;
		void		Start() override;
		void		PostUpdate(bool isPlaying) override;
		void		EditorUpdate(bool isPlaying) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		Canon(ZobObject* zobObject);
        float       m_fireSpeed;
		float		m_timer;
		ZobFilePath m_bulletAsset;
};