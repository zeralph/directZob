#ifdef _WINDLL

#ifndef DZOB_ZOB_SPRITE_WRAPPER_H
#define DZOB_ZOB_SPRITE_WRAPPER_H

#include <string>
#include "ZobObjectWrapper.h"
#include "ZobObject.h"
#include "ZobSprite.h"

namespace directZobCLI {
	public ref class ZobSpriteWrapper : ZobObjectWrapper
	{

	public:
		ZobSpriteWrapper(directZob::ZobSprite* zobObject);
		void				SetTexture(System::String^ path);

	};
}
#endif //DZOB
#endif //_WINDLL 