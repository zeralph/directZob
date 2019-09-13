#include "DirectZobWrapper.h"

namespace CLI
{

	/*static void int_array_conversion(array<unsigned int>^ data)
	{
		pin_ptr<unsigned int> arrayPin = &data[0];
		unsigned int size = data->Length;
	}*/

	DirectZobWrapper::DirectZobWrapper():ManagedObject(new DirectZob, true)
	{

	}

	void DirectZobWrapper::Init()
	{
		m_Instance->Init();
	}

	void DirectZobWrapper::LoadScene(System::String^ path, System::String^ file)
	{
		std::string stdPath;
		MarshalString(path, stdPath);
		std::string stdFile;
		MarshalString(file, stdFile);
		m_Instance->LoadScene(stdPath, stdFile);
	}

	void DirectZobWrapper::NewScene()
	{
		m_Instance->NewScene();
	}

	bool DirectZobWrapper::CanFastSave()
	{
		return m_Instance->CanFastSave();
	}

	void DirectZobWrapper::SaveScene()
	{
		m_Instance->SaveScene();
	}

	void DirectZobWrapper::SaveScene(System::String^ path, System::String^ file)
	{
		std::string stdPath;
		MarshalString(path, stdPath);
		std::string stdFile;
		MarshalString(file, stdFile);
		m_Instance->SaveScene(stdPath, stdFile);
	}

	int DirectZobWrapper::RunAFrame()
	{
		return m_Instance->RunAFrame();
	}

	array<System::String^>^ DirectZobWrapper::GetEventsAndClear()
	{
		const std::vector<std::string>* data = m_Instance->GetEventManager()->GetEvents();
		int l = data->size();
		array<System::String ^>^ arr = gcnew array<System::String ^>(l);
		for (int i = 0; i < l; i++)
		{
			arr[i] = gcnew System::String(data->at(i).c_str());
		}
		m_Instance->GetEventManager()->ClearEvents();
		return arr;
	}
}