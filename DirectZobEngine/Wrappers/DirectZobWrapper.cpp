#ifdef _WINDLL
#include "../Types.h"
#include "../DirectZob.h"
#include "DirectZobWrapper.h"
#include "../../dependencies/minifb/include/MiniFB_enums.h"
#include "../SceneLoader.h"
#include "../Managers/CameraManager.h"
#include "../Managers/LightManager.h"
#include "../Managers/ZobInputManager.h"

namespace CLI
{
	DirectZobWrapper::DirectZobWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel, Panel^ globalPropertiesPanel, PictureBox^ renderWindow, Resources::ResourceManager^ rsMgr):ManagedObject(new DirectZob, true)
	{
		m_events = gcnew DirectZobWrapperEvents();
		m_run = false;
		m_sceneLoadedCb = nullptr;
		m_ZobObjectManagerWrapper = nullptr;
		m_ZobEngineWrapper = nullptr;
		m_objectTreeviewPanel = objectTreeviewPanel;
		m_objectPropertiesPanel = objectPropertiesPanel;
		m_globalPropertiesPanel = globalPropertiesPanel;
		m_renderWindow = renderWindow;
		m_lastFrameTime = 0;
		m_singleton = this;
		m_resourcesManager = rsMgr;
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

			m_ZobObjectManagerWrapper = gcnew ZobObjectManagerWrapper(m_objectTreeviewPanel, m_objectPropertiesPanel, m_resourcesManager);
			m_ZobGlobalsWrapper = gcnew ZobGlobalsWrapper(m_globalPropertiesPanel);
			m_ZobEngineWrapper = gcnew EngineWrapper(m_renderWindow);
		}
	}

	void DirectZobWrapper::CreateLight(int type)
	{
		GetInstance()->GetLightManager()->CreateLight((Light::eLightType)type);
	}

	void DirectZobWrapper::CreateEditorCamera()
	{
		Camera* c = GetInstance()->GetCameraManager()->CreateEditorCamera();
		GetInstance()->GetCameraManager()->SetNextCamera(c->GetName());
	}

	void DirectZobWrapper::CreateCamera(System::String^ sType)
	{
		Camera::eCameraType type = Camera::eCamera_base;
		std::string sTypeC;
		MarshalString(sType, sTypeC);
		if (sTypeC == "Fixed")
		{
			type = Camera::eCamera_base;
		}
		else if (sTypeC == "Follow")
		{
			type = Camera::eCamera_follow;
		}
		else if (sTypeC == "FPS")
		{
			type = Camera::eCamera_fps;
		}
		else if (sTypeC == "Orbital")
		{
			type = Camera::eCamera_orbital;
		}
		GetInstance()->GetCameraManager()->CreateCamera(type, NULL);
	}

	void DirectZobWrapper::LoadScene(System::String^ path, System::String^ file, engineCallback^ loaded, sceneLoadingCallback^ loadingCb)
	{
		if (GetInstance())
		{
			m_sceneLoadedCb = loaded;
			m_sceneLoadingCb = loadingCb;
			std::string stdPath;
			MarshalString(path, stdPath);
			std::string stdFile;
			MarshalString(file, stdFile);
			GetInstance()->LoadScene(stdPath, stdFile, (DirectZob::engineCallback)CallSceneLoadedCallback, (DirectZobType::sceneLoadingCallback)CallSceneLoadingCallback);
		}
	}

	void DirectZobWrapper::RegisterObjectCallbacks(engineCallback^ onSelected)
	{
		m_objectSelectedCb = onSelected;
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

	void DirectZobWrapper::NewScene(System::String^ path)
	{
		if (GetInstance())
		{
			std::string stdPath;
			MarshalString(path, stdPath);
			m_ZobObjectManagerWrapper->SelectObject(NULL);
			Unload();
			m_events->FireOnNewSceneEvent();
			GetInstance()->NewScene(stdPath);
			DirectZob::GetInstance()->GetCameraManager()->CreateEditorCamera();
			m_ZobObjectManagerWrapper->Refresh();
			m_ZobObjectManagerWrapper->AddEditorGizmos();
			DirectZob::GetInstance()->GetEngine()->ShowGrid(true);
			m_ZobEngineWrapper->Start();
		}
	}

	void DirectZobWrapper::Unload()
	{
		if (GetInstance() != NULL)
		{
			m_ZobEngineWrapper->Stop();
			GetInstance()->Unload();
		}
		//m_Instance = NULL;
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

	void DirectZobWrapper::CallSceneLoadingCallback(int nbObj, int curObj, std::string s)
	{
		if (m_sceneLoadingCb != nullptr)
		{
			String^ cs = gcnew String(s.c_str());
			m_sceneLoadingCb(nbObj, curObj, cs);
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

	int DirectZobWrapper::RunAFrame()
	{
		if (GetInstance())
		{	
			GetInstance()->RunAFrame((DirectZob::engineCallback)DirectZobWrapper::CallSceneUpdatedCallback, (DirectZob::engineCallback)DirectZobWrapper::CallQueuingCallback);
			m_lastFrameTime = GetInstance()->GetFrameTime() / 1000.0f;
			EditorUpdate();
			m_ZobEngineWrapper->Update(m_lastFrameTime);
			m_ZobEngineWrapper->QueueObjectsToRender();
			
		}
		return 0;
	}

	void DirectZobWrapper::EditorUpdate()
	{
		if (GetInstance())
		{
			GetInstance()->EditorUpdate();
			m_ZobObjectManagerWrapper->EditorUpdate();
			m_events->FireOnEditorUpdateEvent();
		}
	}

	void DirectZobWrapper::EditorInputsUpdate(float dt, IntPtr window)
	{
		
		GetInstance()->GetInputManager()->Update(dt, (HWND)window.ToPointer());
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

	cli::array<System::String^>^ DirectZobWrapper::GetCameraList()
	{
		const std::vector<std::string> data = DirectZob::GetInstance()->GetCameraManager()->GetCameraList();
		int l = (int)data.size();
		cli::array<System::String^>^ arr = gcnew cli::array<System::String^>(l);
		for (int i = 0; i < l; i++)
		{
			arr[i] = gcnew System::String(data.at(i).c_str());
		}
		return arr;
	}

	void DirectZobWrapper::SetCurrentCamera(System::String^ name)
	{
		std::string stdName;
		MarshalString(name, stdName);
		DirectZob::GetInstance()->GetCameraManager()->SetNextCamera(stdName);
	}

	System::String^ DirectZobWrapper::GetCurrentCameraName()
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			return gcnew System::String(c->GetName().c_str());
		}
		return nullptr;
	}

	void DirectZobWrapper::SaveEditorSettings(EditorSettings^ s)
	{
		s->wireframe = DirectZob::GetInstance()->GetEngine()->WireFrame();
		s->showText = DirectZob::GetInstance()->GetEngine()->ShowText();
		s->showGrid = DirectZob::GetInstance()->GetEngine()->ShowGrid();
		s->showBBoxes = DirectZob::GetInstance()->GetEngine()->ShowBBoxes();
		s->drawPhysicsGizmos = DirectZob::GetInstance()->GetEngine()->DrawPhysyicsGizmos();
		s->showNormals = DirectZob::GetInstance()->GetEngine()->ShowNormals();
		s->drawGizmos = DirectZob::GetInstance()->GetEngine()->DrawGizmos();
		s->drawCameraGizmos = DirectZob::GetInstance()->GetEngine()->DrawCameraGizmos();
		s->drawZobObjectGizmos = DirectZob::GetInstance()->GetEngine()->DrawZobObjectGizmos();
		s->currentWorkingDir = gcnew System::String(SceneLoader::GetResourcePath().c_str());
	}

	void DirectZobWrapper::LoadEditorSettings(EditorSettings^ s)
	{
		DirectZob::GetInstance()->GetEngine()->WireFrame(s->wireframe);
		DirectZob::GetInstance()->GetEngine()->ShowText(s->showText);
		DirectZob::GetInstance()->GetEngine()->ShowGrid(s->showGrid);
		DirectZob::GetInstance()->GetEngine()->ShowBBoxes(s->showBBoxes);
		DirectZob::GetInstance()->GetEngine()->DrawPhysyicsGizmos(s->drawPhysicsGizmos);
		DirectZob::GetInstance()->GetEngine()->ShowNormals(s->showNormals);
		DirectZob::GetInstance()->GetEngine()->DrawGizmos(s->drawGizmos);
		DirectZob::GetInstance()->GetEngine()->DrawCameraGizmos(s->drawCameraGizmos);
		DirectZob::GetInstance()->GetEngine()->DrawZobObjectGizmos(s->drawZobObjectGizmos);
		if (s->currentWorkingDir != nullptr)
		{
			std::string str;
			MarshalString(s->currentWorkingDir, str);
			SceneLoader::SetWorkSpace(str);
		}
	}
}
#endif //_WINDLL