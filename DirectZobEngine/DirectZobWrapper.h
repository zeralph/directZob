#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "DirectZob.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public delegate void onFrameEndCallback();
	public delegate void onFrameStartCallback();
	public ref class DirectZobWrapper: public ManagedObject<DirectZob>
	{
	public:

		DirectZobWrapper();
		void					StartPhysic();
		void					StopPhysic(bool reset);
		bool					IsPhysicPlaying();
		void					Init(int width, int height);
		void					LoadScene(System::String^ path, System::String^ file);
		void					NewScene();
		void					Unload();
		bool					CanFastSave();
		void					SaveScene();
		void					SaveScene(System::String^ path, System::String^ file);
		static DirectZob*		GetDirectZobInstance() { return m_directZobInstance; }
		int						RunAFrame();
		int						Run(onFrameStartCallback^cbStart, onFrameEndCallback ^cbEnd);
		int						Stop();
		void					OnFrameEnd();
		cli::array<System::String^>^ GetEventsAndClear();
		void					Resize(int width, int height);
		static void test();
	private:
		//array<int>^ m_buffer;
		static DirectZob*		m_directZobInstance=NULL;
		onFrameEndCallback^		m_onFrameEndcallback;
		volatile bool			m_run;
	};
}
#endif //_WINDLL