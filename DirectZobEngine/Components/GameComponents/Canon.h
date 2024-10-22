#pragma once
#include "../../Types.h"
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
		void		PreUpdate(float dt) override;
		void		PostUpdate() override;
		void		EditorUpdate() override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		Canon(ZobObject* zobObject);
};