#include "InterfaceWrapper.h"

namespace CLI
{

	/*static void int_array_conversion(array<unsigned int>^ data)
	{
		pin_ptr<unsigned int> arrayPin = &data[0];
		unsigned int size = data->Length;
	}*/

	InterfaceWrapper::InterfaceWrapper():ManagedObject(new Core::Interface)
	{

	}

	int InterfaceWrapper::RunAFrame()
	{
		return m_Instance->RunAFrame();
	}

	int InterfaceWrapper::GetBufferWidth()
	{
		return m_Instance->GetBufferWidth();
	}
	int InterfaceWrapper::GetBufferHeight()
	{
		return m_Instance->GetBufferHeight();
	}
	System::IntPtr InterfaceWrapper::GetBufferData()
	{
		return System::IntPtr(m_Instance->GetBufferData());
		//pin_ptr<int> arrayPin = &m_Instance->GetBufferData()[0];
		//return arrayPin;
	}

	int InterfaceWrapper::Test()
	{
		return m_Instance->Test();
	}

	array<System::String^>^ InterfaceWrapper::GetEventsAndClear()
	{
		const std::vector<std::string>* data = m_Instance->GetEvents();
		int l = data->size();
		array<System::String ^>^ arr = gcnew array<System::String ^>(l);
		for (int i = 0; i < l; i++)
		{
			arr[i] = gcnew System::String(data->at(i).c_str());
		}
		m_Instance->ClearEvents();
		return arr;
	}
}