#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "DirectZob.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public ref class DirectZobWrapper: public ManagedObject<DirectZob>
	{
	public:

		DirectZobWrapper();
		void Init();
		void LoadScene(System::String^ path, System::String^ file);
		void NewScene();
		bool CanFastSave();
		void SaveScene();
		void SaveScene(System::String^ path, System::String^ file);
		static DirectZob* GetDirectZobInstance() { return m_directZobInstance; }
		int RunAFrame();

		array<System::String^>^ GetEventsAndClear();

	private:
		//array<int>^ m_buffer;
		static DirectZob* m_directZobInstance=NULL;
	};
}
#endif //_WINDLL