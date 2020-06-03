#ifdef _WINDLL
#ifndef DZOB_ZOB_CAMERA_WRAPPER_H
#define DZOB_ZOB_CAMERA_WRAPPER_H


#include "ZobObjectWrapper.h"
#include "Camera.h"
namespace directZobCLI {
	public ref class ZobCameraWrapper : ZobObjectWrapper
	{
	public:
		ZobCameraWrapper(directZob::ZobObject* zobObject);
	private:

	};
}
#endif //DZOB
#endif //_WINDLL 