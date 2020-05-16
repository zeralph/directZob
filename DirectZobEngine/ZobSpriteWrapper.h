#ifdef _WINDLL
#pragma once
#include <string>
#include "ZobObjectWrapper.h"
#include "ZobObject.h"
#include "ZobSprite.h"


namespace CLI {
	public ref class ZobSpriteWrapper : ZobObjectWrapper
	{

	public:
		ZobSpriteWrapper(ZobSprite* zobObject);
		void				SetTexture(System::String^ path);

	};
}
#endif //_WINDLL 