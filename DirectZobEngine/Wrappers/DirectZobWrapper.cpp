#ifdef _WINDLL
#include "../DirectZob.h"
#include "DirectZobWrapper.h"
#include "../../dependencies/minifb/include/MiniFB_enums.h"

namespace CLI
{
	DirectZobWrapper::DirectZobWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel):ManagedObject(new DirectZob, true)
	{
		m_run = false;
		m_sceneLoadedCb = nullptr;
		m_ZobObjectManagerWrapper = nullptr;
		m_objectTreeviewPanel = objectTreeviewPanel;
		m_objectPropertiesPanel = objectPropertiesPanel;
	}

	void DirectZobWrapper::StartPhysic()
	{
		if (GetInstance())
		{
			GetInstance()->StartPhysic();
		}
	}

	void DirectZobWrapper::StopPhysic(bool reset)
	{
		if (GetInstance())
		{
			GetInstance()->StopPhysic(reset);
		}
	}

	bool DirectZobWrapper::IsPhysicPlaying()
	{
		if (GetInstance())
		{
			return GetInstance()->IsPhysicPlaying();
		}
		return false;
	}

	void DirectZobWrapper::Init(int width, int height)
	{
		if (GetInstance())
		{
			GetInstance()->Init(0, width, height, true);
			m_ZobObjectManagerWrapper = gcnew ZobObjectManagerWrapper(m_objectTreeviewPanel, m_objectPropertiesPanel);
		}
	}

	void DirectZobWrapper::LoadScene(System::String^ path, System::String^ file, engineCallback^ loaded)
	{
		if (GetInstance())
		{
			m_sceneLoadedCb = loaded;
			std::string stdPath;
			MarshalString(path, stdPath);
			std::string stdFile;
			MarshalString(file, stdFile);
			GetInstance()->LoadScene(stdPath, stdFile, (DirectZob::engineCallback)CallSceneLoadedCallback);
		}
	}

	void DirectZobWrapper::LoadZobObject(System::String^ path, System::String^ file)
	{
		if (GetInstance())
		{
			std::string stdPath;
			MarshalString(path, stdPath);
			std::string stdFile;
			MarshalString(file, stdFile);
			GetInstance()->LoadZobObject(stdPath, stdFile);
		}
	}

	void DirectZobWrapper::NewScene()
	{
		if (GetInstance())
		{
			GetInstance()->NewScene();
		}
	}

	void DirectZobWrapper::Unload()
	{
		if (GetInstance() != NULL)
		{
			GetInstance()->Unload();
		}
		m_Instance = NULL;
	}


	bool DirectZobWrapper::CanFastSave()
	{
		if (GetInstance())
		{
			return GetInstance()->CanFastSave();
		}
		return false;
	}

	void DirectZobWrapper::SaveScene()
	{
		if (GetInstance())
		{
			GetInstance()->SaveScene();
		}
	}

	void DirectZobWrapper::SaveScene(System::String^ path, System::String^ file)
	{
		if (GetInstance())
		{
			std::string stdPath;
			MarshalString(path, stdPath);
			std::string stdFile;
			MarshalString(file, stdFile);
			GetInstance()->SaveScene(stdPath, stdFile);
		}
	}

	void DirectZobWrapper::test()
	{

	}

	void DirectZobWrapper::CallSceneUpdatedCallback()
	{
		if (m_sceneUpdatedCb != nullptr)
		{
			m_sceneUpdatedCb();
		};
	}

	void DirectZobWrapper::CallSceneLoadedCallback()
	{
		if (m_sceneLoadedCb != nullptr)
		{
			m_sceneLoadedCb();
		};
	}

	void DirectZobWrapper::CallQueuingCallback()
	{
		if (m_queuingCb != nullptr)
		{
			try
			{
				m_queuingCb();
			}
			catch (exception e)
			{
				DirectZob::GetInstance()->LogError("error : %s", e.what());
			}
		};
	}

	int DirectZobWrapper::Run(engineCallback^ cbStart, engineCallback^ cbEnd, engineCallback^ sceneUpdated, engineCallback^ queuing)
	{

		m_sceneUpdatedCb = sceneUpdated;
		m_queuingCb = queuing;
		m_run = true;
		while(m_run)
		{
			if (m_run)
			{
				cbStart();
			}
			if (GetInstance())
			{
				GetInstance()->RunAFrame((DirectZob::engineCallback)DirectZobWrapper::CallSceneUpdatedCallback, (DirectZob::engineCallback)DirectZobWrapper::CallQueuingCallback);
			}
			if (m_run)
			{
				cbEnd();
			}
		}
		return 0;
	}

	void DirectZobWrapper::EditorUpdate()
	{
		if (GetInstance())
		{
			GetInstance()->EditorUpdate();
			OnEditorUpdateEvent();
		}
	}

	int DirectZobWrapper::Stop()
	{
		m_run = false;
		return 0;
	}

	void DirectZobWrapper::Lock()
	{
		if (GetInstance())
		{
			GetInstance()->Lock();
		}
	}

	void DirectZobWrapper::Unlock()
	{
		if (GetInstance())
		{
			GetInstance()->Unlock();
		}
	}

	System::IntPtr DirectZobWrapper::GetBufferDataPointer()
	{
		System::IntPtr ptr;
		if (GetInstance())
		{
			uint* p = GetInstance()->GetEditorBufferDataNoConst();
			ptr = (System::IntPtr)p;
		}
		return ptr;
	}

	int DirectZobWrapper::GetBufferDataLength()
	{
		return GetInstance()->GetBufferDataLenght();
	}

	void DirectZobWrapper::RegenerateZobIds()
	{
		if (GetInstance())
		{
			GetInstance()->RegenerateZobIds();
		}
	}

	int DirectZobWrapper::RunAFrame()
	{
		if (GetInstance())
		{
			return GetInstance()->RunAFrame(0);
		}
		return 0;
	}

	cli::array<System::String^>^ DirectZobWrapper::GetEventsAndClear()
	{
		if (GetInstance())
		{
			const std::vector<std::string>* data = GetInstance()->GetEventManager()->GetEvents();
			int l = (int)data->size();
			cli::array<System::String^>^ arr = gcnew cli::array<System::String^>(l);
			for (int i = 0; i < l; i++)
			{
				arr[i] = gcnew System::String(data->at(i).c_str());
			}
			GetInstance()->GetEventManager()->ClearEvents();
			return arr;
		}
		return gcnew cli::array<System::String^>(0);
	}

	void DirectZobWrapper::Resize(int w, int h)
	{
		if (GetInstance())
		{
			if (w > 0 && h > 0 && w <= 1920 && h <= 1080)
			{
				GetInstance()->Resize(w, h);
			}
		}
	}
	System::String^ DirectZobWrapper::GetResourcePath()
	{
		if (GetInstance())
		{
			std::string s = GetInstance()->GetResourcePath();
			return gcnew System::String(s.c_str());
		}
		return nullptr;
	}
}
#endif //_WINDLL