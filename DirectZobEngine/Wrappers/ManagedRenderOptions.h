#ifdef _WINDLL
#pragma once
#include "../Types.h"

namespace CLI {
	public ref class ManagedRenderOptions
	{
	public:
		ManagedRenderOptions(RenderOptions *r)
		{
			m_renderOptions = r;
		}

		void SetTransparency(bool b) { m_renderOptions->bTransparency = b; }
		void SetColorization(int r, int g, int b) { m_renderOptions->colorization = ZobVector3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f); };
		void EnableColorization(bool b) { m_renderOptions->bColorize = b; };
		void SetLightMode(int i) {m_renderOptions->lightMode = (RenderOptions::eLightMode)i;};
		void ZetZBuffer(bool b) { m_renderOptions->zBuffered = b; };
		RenderOptions* m_renderOptions = NULL;
	};
}
#endif //_WINDLL