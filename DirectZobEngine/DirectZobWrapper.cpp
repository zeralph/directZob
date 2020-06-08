#ifdef _WINDLL
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

	void DirectZobWrapper::StartPhysic()
	{
		m_Instance->StartPhysic();
	}

	void DirectZobWrapper::StopPhysic(bool reset)
	{
		m_Instance->StopPhysic(reset);
	}

	bool DirectZobWrapper::IsPhysicPlaying()
	{
		return m_Instance->IsPhysicPlaying();
	}

	void DirectZobWrapper::Init(int width, int height)
	{
		m_Instance->Init(width, height, true);
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

	void DirectZobWrapper::Unload()
	{
		m_Instance->Unload();
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

	cli::array<System::String^>^ DirectZobWrapper::GetEventsAndClear()
	{
		const std::vector<std::string>* data = m_Instance->GetEventManager()->GetEvents();
		int l = (int)data->size();
		cli::array<System::String ^>^ arr = gcnew cli::array<System::String ^>(l);
		for (int i = 0; i < l; i++)
		{
			arr[i] = gcnew System::String(data->at(i).c_str());
		}
		m_Instance->GetEventManager()->ClearEvents();
		return arr;
	}

	void DirectZobWrapper::Resize(int w, int h)
	{
		if (w > 0 && h > 0 && w <= 1920 && h <= 1080)
		{
			m_Instance->Resize(w, h);
		}
	}
}
#endif //_WINDLL