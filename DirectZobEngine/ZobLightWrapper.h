#ifdef _WINDLL
#ifndef DZOB_ZOB_LIGHT_WRAPPER_H
#define DZOB_ZOB_LIGHT_WRAPPER_H

#include "ZobObjectWrapper.h"
#include "Light.h"

namespace directZobCLI {
	public ref class ZobLightWrapper : ZobObjectWrapper
	{

	public:
		ZobLightWrapper(directZob::Light* zobObject);
		void				SetColor(ManagedVector3^ p);
		ManagedVector3^		GetColor();
		float				GetIntensity();
		void				SetIntensity(float f);
		float				GetDistance();
		void				SetDistance(float f);
		int					GetType();
		void				SetType(int f);
		float				GetSpotAngle();
		void				SetSpotAngle(float f);
	};
}
#endif //DZOB
#endif //_WINDLL 