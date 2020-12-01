#ifdef _WINDLL
#include "ZobCameraWrapper.h"
#include "../ZobObjects/Camera.h"
namespace CLI
{
	ZobCameraWrapper::ZobCameraWrapper(Camera* zobObject):ZobObjectWrapper(zobObject)
	{
		//m_isValid = zobObject != NULL && IsCamera();
	}

	float ZobCameraWrapper::GetFOV()
	{
		Camera* c = (Camera*)GetInstance();
		if (c)
		{
			return c->GetFov();
		}
		return 0.0f;
	}
	void ZobCameraWrapper::SetFOV(float fov)
	{
		Camera* c = (Camera*)GetInstance();
		if (c)
		{
			return c->SetFov(fov);
		}
	}
}
#endif //_WINDLL