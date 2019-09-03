#pragma once
#include <string> 
#include <iostream>

namespace CLI {

	template<class T>
	public ref class ManagedObject
	{
	protected:
		T* m_Instance;
	public:
		ManagedObject(T* instance)
			: m_Instance(instance)
		{
		}
		virtual ~ManagedObject()
		{
			if (m_Instance != nullptr)
			{
				delete m_Instance;
			}
		}
		!ManagedObject()
		{
			if (m_Instance != nullptr)
			{
				delete m_Instance;
			}
		}
		T* GetInstance()
		{
			return m_Instance;
		}
		void MarshalString(System::String^ s, std::string& os) {
			using namespace System::Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(System::IntPtr((void*)chars));
		}
	};
}