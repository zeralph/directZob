#include "DirectZobWrapper.h"

namespace CLI
{

	/*static void int_array_conversion(array<unsigned int>^ data)
	{
		pin_ptr<unsigned int> arrayPin = &data[0];
		unsigned int size = data->Length;
	}*/

	DirectZobWrapper::DirectZobWrapper():ManagedObject(new DirectZob)
	{

	}

	void DirectZobWrapper::Init()
	{
		m_Instance->Init();
	}

	void DirectZobWrapper::LoadScene(System::String^ file)
	{
		std::string stdFile;
		MarshalString(file, stdFile);
		m_Instance->LoadScene(stdFile);
	}

	int DirectZobWrapper::RunAFrame()
	{
		return m_Instance->RunAFrame();
	}

	array<System::String^>^ DirectZobWrapper::GetEventsAndClear()
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