#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../DirectZob.h"
#include <string> 
//using namespace System;


namespace CLI
{
	// delegate void onFrameEndCallback();
	public delegate void engineCallback();
	public ref class DirectZobWrapper: public ManagedObject<DirectZob>
	{
	public:

		static void					CallSceneUpdatedCallback();
		static void					CallQueuingCallback();
		DirectZobWrapper();
		void					StartPhysic();
		void					StopPhysic(bool reset);
		bool					IsPhysicPlaying();
		void					Init(int width, int height);
		void					LoadScene(System::String^ path, System::String^ file);
		void					LoadZobObject(System::String^ path, System::String^ file);
		void					NewScene();
		void					Unload();
		bool					CanFastSave();
		void					SaveScene();
		void					SaveScene(System::String^ path, System::String^ file);
		static DirectZob*		GetDirectZobInstance() { return m_directZobInstance; }
		int						RunAFrame();
		int						Run(engineCallback^ cbStart, engineCallback^ cbEnd, engineCallback^ sceneUpdated, engineCallback^ queuing);
		int						Stop();
		void					Lock();
		void					Unlock();
		System::IntPtr			GetBufferData();
		cli::array<System::String^>^ GetEventsAndClear();
		void					Resize(int width, int height);
		static void test();
	private:
		//array<int>^ m_buffer;
		//void					CallSceneUpdatedCallback();
		delegate void			Del();
		static DirectZob*		m_directZobInstance=NULL;
		volatile bool			m_run;
		static engineCallback^  m_sceneUpdatedCb;
		static engineCallback^	m_queuingCb;
	};
}
#endif //_WINDLL