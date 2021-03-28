#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ZobObjectWrapper.h"
#include "../Behaviors/ZobBehavior.h"

using namespace System;
using namespace System::Collections::Generic;
namespace CLI {
	public ref class ZobBehaviorWrapper
	{

	public:
		static List<String^>^ GetBehaviors();
		static void CreateBehavior(ZobObjectWrapper^ z, String^ s);
	private:
		static void MarshalString(System::String^ s, std::string& os) {
			using namespace System::Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(System::IntPtr((void*)chars));
		}
	};
}
#endif //_WINDLL 