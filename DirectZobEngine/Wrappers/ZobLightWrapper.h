#ifdef _WINDLL
#pragma once
#include "ZobObjectWrapper.h"
#include "../ZobObjects/Light.h"


namespace CLI {
	public ref class ZobLightWrapper : ZobObjectWrapper
	{

	public:
		ZobLightWrapper(Light* zobObject, Panel^ panel);
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
#endif //_WINDLL 