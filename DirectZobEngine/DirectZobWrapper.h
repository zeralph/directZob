#ifdef _WINDLL
#ifndef DZOB_DIRECTZOB_WRAPPER_H
#define DZOB_DIRECTZOB_WRAPPER_H

#include "ManagedObject.h"
#include "DirectZob.h"
#include <string> 
namespace directZobCLI
{
	public ref class DirectZobWrapper: public ManagedObject<directZob::DirectZob>
	{
	public:

		DirectZobWrapper();
		void					Init(int width, int height);
		void					LoadScene(System::String^ path, System::String^ file);
		void					NewScene();
		void					Unload();
		bool					CanFastSave();
		void					SaveScene();
		void					SaveScene(System::String^ path, System::String^ file);
		static directZob::DirectZob*		GetDirectZobInstance() { return m_directZobInstance; }
		int						RunAFrame();
		array<System::String^>^ GetEventsAndClear();
		void					Resize(int width, int height);
	private:
		//array<int>^ m_buffer;
		static directZob::DirectZob* m_directZobInstance=NULL;
	};
}
#endif //DZOB
#endif //_WINDLL