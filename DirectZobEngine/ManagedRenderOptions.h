#ifdef _WINDLL

#ifndef DZOB_MANAGED_RENDER_OPTIONS_H
#define DZOB_MANAGED_RENDER_OPTIONS_H


#include "Types.h"
namespace directZobCLI {
	public ref class ManagedRenderOptions
	{
	public:
		ManagedRenderOptions(directZob::RenderOptions *r)
		{
			m_renderOptions = r;
		}

		void SetTransparency(bool b) { m_renderOptions->bTransparency = b; }
		void SetColorization(int r, int g, int b) { m_renderOptions->colorization = directZob::Vector3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f); };
		void EnableColorization(bool b) { m_renderOptions->bColorize = b; };
		void SetLightMode(int i) {m_renderOptions->lightMode = (directZob::RenderOptions::eLightMode)i;};
		void ZetZBuffer(bool b) { m_renderOptions->zBuffered = b; };
		directZob::RenderOptions* m_renderOptions = NULL;
	};
}
#endif //DZOB
#endif //_WINDLL