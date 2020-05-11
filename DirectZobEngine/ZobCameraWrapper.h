#ifdef _WINDLL
#pragma once
#include "ZobObjectWrapper.h"
#include "Camera.h"

namespace CLI {
	public ref class ZobCameraWrapper : ZobObjectWrapper
	{
	public:
		ZobCameraWrapper(ZobObject* zobObject);
	private:

	};
}
#endif //_WINDLL 