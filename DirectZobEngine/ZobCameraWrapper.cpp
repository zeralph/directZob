#ifdef _WINDLL
#include "ZobCameraWrapper.h"
#include "Camera.h"
namespace CLI
{
	ZobCameraWrapper::ZobCameraWrapper(ZobObject* zobObject):ZobObjectWrapper(zobObject)
	{
		//m_isValid = zobObject != NULL && IsCamera();
	}
}
#endif //_WINDLL