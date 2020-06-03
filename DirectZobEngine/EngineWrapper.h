#ifdef _WINDLL
#ifndef DZOB_ENGINE_WRAPPER_H
#define DZOB_ENGINE_WRAPPER_H

#include "ManagedObject.h"
#include "ManagedVector3.h"
#include "Engine.h"
#include "DirectZob.h"
namespace directZobCLI
{
	public ref class EngineWrapper: public ManagedObject<directZob::Engine>
	{
	public:

		EngineWrapper();
		int				GetBufferWidth();
		int				GetBufferHeight();
		System::IntPtr	GetBufferData();
		void			ShowGrid(bool b) { m_Instance->ShowGrid(b); }
		void			WireFrame(bool b) { m_Instance->WireFrame(b); }
		void			ShowNormals(bool b) { m_Instance->ShowNormals(b); }
		void			DrawGizmos(bool b) { m_Instance->DrawGizmos(b); }
		void			SetRenderOutput(int r);
		void			SetRenderMode(int r);
		void			SetLightingPrecision(int r);
		bool			GetProjectedCoords(ManagedVector3^ worldSpacePos);
		float			GetDistanceToCamera(ManagedVector3^ worldPos);
	private:
		//array<int>^ m_buffer;
	};
}
#endif //DZOB
#endif //_WINDLL