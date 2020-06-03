#ifndef DZOB_MANAGED_OBJECT_H
#define DZOB_MANAGED_OBJECT_H
#ifdef _WINDLL
#include <string> 
#include <iostream>
namespace directZobCLI 
{
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
			if (m_manageDelete && m_Instance != nullptr)
			{
				delete m_Instance;
			}
		}
		!ManagedObject()
		{
			if (m_manageDelete && m_Instance != nullptr)
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
#endif //_WINDLL
#endif //DZOB
