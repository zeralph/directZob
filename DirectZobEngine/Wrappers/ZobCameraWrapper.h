#ifdef _WINDLL
#pragma once
#include "ZobObjectWrapper.h"
#include "../ZobObjects/Camera.h"

namespace CLI {
	public ref class ZobCameraWrapper : ZobObjectWrapper
	{
	public:
		ZobCameraWrapper(Camera* zobObject, Panel^ panel);
		float							GetFOV();
		void							SetFOV(float fov);
	private:

	};
}
#endif //_WINDLL 