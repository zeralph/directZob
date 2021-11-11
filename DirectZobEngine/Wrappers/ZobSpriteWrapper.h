#ifdef _WINDLL
#pragma once
#include <string>
#include "ZobObjectWrapper.h"
#include "../ZobObjects/ZobObject.h"
#include "../ZobObjects/ZobSprite.h"


namespace CLI {
	public ref class ZobSpriteWrapper : ZobObjectWrapper
	{

	public:
		ZobSpriteWrapper(ZobSprite* zobObject, Panel^ panel);
		void				SetTexture(System::String^ path);

	};
}
#endif //_WINDLL 