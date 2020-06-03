#ifdef _WINDLL
#include "ZobCameraWrapper.h"
#include "Camera.h"
using namespace directZobCLI;

ZobCameraWrapper::ZobCameraWrapper(directZob::ZobObject* zobObject):ZobObjectWrapper(zobObject)
{
	m_isValid = zobObject != NULL && IsCamera();
}

#endif //_WINDLL