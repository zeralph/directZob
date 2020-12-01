#ifdef _WINDLL
#pragma once
#include <string> 
#include <iostream>
#include "../DirectZob.h"
namespace CLI {

	template<class T>
	public ref class ManagedObject
	{
	protected:
		T* m_Instance = NULL;
		bool m_manageDelete = true;
	public:
		ManagedObject(T* instance, bool manageDelete)
			: m_Instance(instance),
			m_manageDelete(manageDelete)
		{
		}
		virtual ~ManagedObject()
		{
			//if (m_manageDelete && m_Instance != nullptr)
			//{
			//	delete m_Instance;
			//}
			m_Instance = nullptr;
		}
		!ManagedObject()
		{
			//if (m_manageDelete && m_Instance != nullptr)
			//{
			//	delete m_Instance;
			//}
			m_Instance = nullptr;
		}
		T* GetInstance()
		{
			if (IsValid())
			{
				return m_Instance;
				//return dynamic_cast<T*>(m_Instance);
			}
			return NULL;
		}
		void MarshalString(System::String^ s, std::string& os) {
			using namespace System::Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(System::IntPtr((void*)chars));
		}
		virtual bool IsValid() { return m_Instance != NULL; };
	};
}
#endif //_WINDLL