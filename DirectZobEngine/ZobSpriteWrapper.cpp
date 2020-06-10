#ifdef _WINDLL
#include "ZobSpriteWrapper.h"
namespace CLI
{
	ZobSpriteWrapper::ZobSpriteWrapper(ZobSprite* zobObject):ZobObjectWrapper(zobObject)
	{
		m_isValid = zobObject != NULL && IsSprite();
	}

	void ZobSpriteWrapper::SetTexture(System::String^ path)
	{
		std::string n;
		MarshalString(path, n);
		ZobSprite* s = (ZobSprite*)m_Instance;
		s->SetTexture(n);
	}
}
#endif //_WINDLL