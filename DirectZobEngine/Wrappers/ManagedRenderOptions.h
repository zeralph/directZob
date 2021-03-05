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

		bool GetTransparency() { return m_renderOptions->bTransparency; }
		int GetLightMode() { return (int)m_renderOptions->lightMode; };
		bool GetZBuffer() { return m_renderOptions->zBuffered; };
		void SetTransparency(bool b) { m_renderOptions->bTransparency = b; }
		void SetLightMode(int i) {m_renderOptions->lightMode = (RenderOptions::eLightMode)i;};
		void SetZBuffer(bool b) { m_renderOptions->zBuffered = b; };
		RenderOptions* m_renderOptions = NULL;
	};
}
#endif //_WINDLL