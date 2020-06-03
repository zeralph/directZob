#ifdef _WINDLL
#include "ZobSpriteWrapper.h"
using namespace directZobCLI;

ZobSpriteWrapper::ZobSpriteWrapper(directZob::ZobSprite* zobObject):ZobObjectWrapper(zobObject)
{
	m_isValid = zobObject != NULL && IsSprite();
}

void ZobSpriteWrapper::SetTexture(System::String^ path)
{
	std::string n;
	MarshalString(path, n);
	directZob::ZobSprite* s = (directZob::ZobSprite*)m_Instance;
	s->SetTexture(n);
}

#endif //_WINDLL