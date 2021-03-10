#include <mutex>
#include <thread>
#include "DirectZob.h"
#include "Events.h"
#include "tinyxml.h"
#include "ZobObjects/ZobObject.h"
#include "SceneLoader.h"
#include "ZobPhysic/ZobPhysicsEngine.h"
#include "Rendering/Rasterizer.h"
#ifdef WINDOWS
#include "../minifb/src/windows/WindowData_Win.h"
#elif LINUX
#include "../minifb/src/x11/WindowData_X11.h"
#endif
#include "../../dependencies/optick/include/optick.h"
#define LOG_BUFFER_SIZE 1024

static float TargetMSPerFrame = 16.66666667f;

static char buffer[MAX_PATH];
static char logBuffer[LOG_BUFFER_SIZE];
static bool g_isInEditorMode;
static int s_logIndent;
static std::mutex g_render_mutex;
static std::thread g_editorModeThread;
int DirectZob::s_logIndent = 0;
DirectZob *DirectZob::singleton = nullptr;

DirectZob::DirectZob()
{
	
	m_initialized = false;
	DirectZob::singleton= this; 
	m_frameTime = 1.0f;
}

DirectZob::~DirectZob()
{
	//delete m_engine;
	delete m_hudManager;
	delete m_meshManager;
	delete m_materialManager;
	delete m_cameraManager;
	delete m_lightManager;
	delete m_text;
	delete m_events;
	delete m_inputManager;
}

std::string DirectZob::ExePath() {

	//return std::string("D:\\_PERSO\\directZob\\directZob\\resources\\");
	return std::string("C:\\_GIT\\directZob\\resources");
}

void DirectZob::LoadScene(std::string& path, std::string& file)
{
	SceneLoader::LoadScene(path, file);
	if (m_text == NULL)
	{
		m_text = new Text2D(m_engine, m_events);
	}
}
void DirectZob::LoadZobObject(std::string& path, std::string& file)
{
	SceneLoader::LoadZobObject(path, file);
}

void DirectZob::Lock()
{
	g_render_mutex.lock();
}

void DirectZob::Unlock()
{
	g_render_mutex.unlock();
}

void DirectZob::SaveScene(std::string& path, std::string& file)
{
	SceneLoader::SaveScene(path, file);
}

void DirectZob::SaveScene()
{
	SceneLoader::SaveScene();
}

void DirectZob::NewScene()
{
	g_render_mutex.lock();
	m_physicStarted = false;
	SceneLoader::NewScene();
	if (m_text == NULL)
	{
		m_text = new Text2D(m_engine, m_events);
	}
	g_render_mutex.unlock();
}

void DirectZob::Resize(int width, int height)
{
	g_render_mutex.lock();
	m_engine->Resize(width, height);
	g_render_mutex.unlock();
}

void DirectZob::Unload()
{
	g_render_mutex.lock();
	SceneLoader::UnloadScene();
	DirectZob::GetInstance()->GetEngine()->Stop();
	delete DirectZob::GetInstance();
	g_render_mutex.unlock();
}

bool DirectZob::CanFastSave()
{
	return SceneLoader::CanFastSave();
}

void DirectZob::Init(int width, int height, bool bEditorMode)
{
	g_isInEditorMode = bEditorMode;
	m_events = new Events();
	DirectZob::LogInfo("Init engine");
	m_engine = new Engine(width, height, m_events);
	m_physicsEngine = new ZobPhysicsEngine();
	m_zobObjectManager = new ZobObjectManager();
	m_hudManager = new ZobHUDManager();
	m_lightManager = new LightManager();
	m_materialManager = new MaterialManager();
	m_meshManager = new MeshManager();
	m_cameraManager = new CameraManager();
	m_inputManager = new ZobInputManager(width, height);
	int dx = 1;
	int dy = 1;
	float r = 0.0f;
	long frame = 0;
	float rot = 0.0;
	char frameCharBuffer[sizeof(ulong)];
	int state;
	m_initialized = true;
	m_frameTick=0;
	m_frameTime = 1.0f;
	m_physicStarted = false;
}

static float rot = 1.0f;

void DirectZob::StopPhysic(bool reset)
{ 
	m_physicStarted = false; 
	if (reset)
	{
		DirectZob::GetInstance()->GetZobObjectManager()->ResetPhysic();
		m_physicsEngine->ResetAccumulator();
	}
}

int	DirectZob::Run(void func(void))
{
	g_editorModeThread = std::thread(&DirectZob::RunInternal, this, func);
	return 12;
}

int DirectZob::RunInternal(void func(void))
{
	for (;;)
	{
		RunAFrame(0);
		func();
	}
}

int DirectZob::RunAFrame(mfb_window* window, DirectZob::engineCallback OnSceneUpdated /*=NULL*/, DirectZob::engineCallback OnQueuing /*=NULL*/)
{
	OPTICK_EVENT();
	ZobVector3 color = ZobVector3(1, 1, 1);
	timespec tstart;
	timespec tend;
	SaveTime(&tstart);
	g_render_mutex.lock();
	int state=0;
	m_fps = 1000.0f / m_frameTime;
	if(m_initialized && m_engine->Started())
	{
#ifdef WINDOWS
		HWND hWnd = 0;
		SWindowData* window_data = (SWindowData*)window;
		if (window_data)
		{
			SWindowData_Win* window_data_win = (SWindowData_Win*)window_data->specific;
			hWnd = window_data_win->window;
		}
		m_inputManager->Update(m_frameTime, hWnd);
#elif LINUX
		Display* display = NULL;
		SWindowData* window_data = (SWindowData*)window;
		if (window_data)
		{
			SWindowData_X11* window_data_x11 = (SWindowData_X11*)window_data->specific;
			display = window_data_x11->display;
		}
		m_inputManager->Update(m_frameTime, display);
#else
		m_inputManager->Update(m_frameTime);
#endif
		m_cameraManager->UpdateAfter();
		Color c = Color(DirectZob::GetInstance()->GetLightManager()->GetClearColor());
		m_engine->ClearBuffer(&c);
		Camera* cam = m_cameraManager->GetCurrentCamera();
		if (cam)
		{
			bool bPhysicUpdated = false;
//			cam->UpdateViewProjectionMatrix();

			m_zobObjectManager->PreUpdate();
			m_hudManager->PreUpdate();
			m_lightManager->PreUpdate();
			m_engine->StartDrawingScene();
			if (OnSceneUpdated)
			{
				OnSceneUpdated();
			}
//			cam->UpdateViewProjectionMatrix();
			if (m_physicStarted)
			{
				bPhysicUpdated = true;
				m_zobObjectManager->UpdateBehavior(m_frameTime / 1000.0f);
				m_hudManager->UpdateBehavior(m_frameTime / 1000.0f);
				m_physicsEngine->StartUpdatePhysic(m_frameTime/1000.0f);
				
			}
			cam->UpdateAfter();
			m_zobObjectManager->StartUpdateScene(cam, m_engine, m_frameTime / 1000.0f);			
			m_hudManager->UpdateObjects(cam, m_engine, m_frameTime / 1000.0f);
			m_geometryTime = m_zobObjectManager->WaitForUpdateObjectend();
			m_renderTime = m_engine->WaitForRasterizersEnd();
			m_physicTime = 0;
			if (bPhysicUpdated)
			{
				m_physicTime = m_physicsEngine->WaitForUpdatePhysicEnd();
			}
			m_engine->ClearRenderQueues();
			SaveTime( &tend);
			m_copyTime = GetDeltaTime_MS(tstart, tend);
			if (OnQueuing)
			{
				//OnQueuing();
			}
  			m_zobObjectManager->QueueForDrawing(cam, m_engine);
			m_hudManager->QueueForDrawing(cam, m_engine);
			if (m_engine->DrawPhysyicsGizmos())
			{
				m_physicsEngine->DrawGizmos();
			}
			if (m_engine->ShowGrid())
			{
				m_engine->DrawGrid(cam);
			}
		}
		else
		{
			if (m_text)
			{
				m_hudManager->Print(0.5f, 0.5f, 0.01f, 0.01f, &color, "WARNING : %s", "NO CAMERA");
			}
		}
		m_hudManager->Print(0.01f, 0.01f, 0.0125f, 0.0125f, &color, "Triangles : %i", m_engine->GetNbDrawnTriangles());
		color = ZobVector3(0, 1, 0);
		if (m_frameTime <= TargetMSPerFrame)
		{
			ZobVector3 color = ZobVector3(1, 0, 0);
		}
		m_hudManager->Print(0.01f, 0.04f, 0.0125f, 0.0125f, &color, "render : %03i, geom : %03i, phys : %03i, cpy : %03i, tot : %03i, FPS : %03i", (int)m_renderTime, (int)m_geometryTime, (int)m_physicTime, (int)m_copyTime, (int)m_frameTime, (int)m_fps);
		color = ZobVector3(1, 1, 1);
		m_hudManager->Print(0.01f, 0.07f, 0.01f, 0.01f, &color, "Controller LX : %.2f, LY  : %.2f, RX : %.2f, RY : %.2f, LT : %.2f, RT : %.2f", 
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftStickX), 
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftStickY),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightStickX),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightStickY),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftShoulder),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightShoulder));
		/*
		if (m_text)
		{			
			//PrintObjectList();
			if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::WireFrame))
			{
				m_engine->WireFrame(!m_engine->WireFrame());
			}
			if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::Gizmos))
			{
				m_engine->DrawGizmos(!m_engine->DrawGizmos());
			}
			if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::Quit))
			{
				mfb_close(window);
			}
		}
		*/
		m_engine->SetDisplayedBuffer();
	}
	g_render_mutex.unlock();
	SaveTime(&tend);
	float dt = (float)GetDeltaTime_MS(tstart, tend);
	WaitToTargetFrameTime(dt);
	return state;
}

void DirectZob::WaitToTargetFrameTime(float dt)
{
	OPTICK_EVENT();
	if (dt < 0)
	{
		throw "rput";
	}
	float targetDt = TargetMSPerFrame - dt;
	if (targetDt > 0.0f)
	{
		SleepMS(targetDt);
		m_frameTime = TargetMSPerFrame;
	}
	else
	{
		m_frameTime = dt;
	}
}

void DirectZob::SleepMS(float ms)
{
#ifdef LINUX
	usleep((uint)(ms * 1000.0f))
#elif WINDOWS
	//DWORD d = *reinterpret_cast<DWORD const*>(&ms);
	//Sleep(d);
	std::this_thread::sleep_for(std::chrono::milliseconds((uint)ms/2));
#elif MACOS
	usleep((uint)(ms*1000.0f))
#endif //LINUX
}

double DirectZob::GetDeltaTime_MS(timespec& start, timespec& end) const
{
	double f;
	double billion = 1000000000.0;
	if (end.tv_sec < start.tv_sec)
	{
		throw "Time error";
	}
	else if (end.tv_sec == start.tv_sec)
	{
		f = (double)(end.tv_nsec - start.tv_nsec) / billion;
	}
	else 
	{
		f =  (double)(billion - start.tv_nsec + end.tv_nsec) / billion + (double)(end.tv_sec - start.tv_sec - 1 );
	}
	return f * 1000.0;
}

void DirectZob::PrintObjectList()
{
	int txtW = m_engine->GetBufferData()->width - 190;
	std::vector<const ZobObject*> v;
	m_zobObjectManager->GetZobObjectList(v);
	for (int i = 0; i < v.size(); i++)
	{
		const ZobObject* z = v.at(i);
		int c = 0xFF0000FF;
		if (z->GetMesh())
		{
			c = 0xFFFF0000;
			if (z->GetMesh()->IsDrawn())
			{
				c = 0xFF00FF00;
			}
		}
		m_text->Print(txtW, (i*10), c, z->GetName().c_str());
	}
}

void DirectZob::LogInfo(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string("");
	if (s_logIndent > 0)
	{
		for (int i = 0; i < s_logIndent-1; i++)
		{
			s.append(std::string("|"));
		}
		s.append(std::string(">"));
	}
	s.append(std::string(logBuffer));
	va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogInfo, s);
}
	else
	{
		printf("%s\n", s.c_str());
	}
}

void DirectZob::LogError(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string(logBuffer);
	for (int i = 0; i < s_logIndent; i++)
	{
		s = std::string("\t").append(s);
	}
    va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogError, s);
}
	else
	{
		printf("%s\n", s.c_str());
	}
}
void DirectZob::LogWarning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string(logBuffer);
	for (int i = 0; i < s_logIndent; i++)
	{
		s = std::string("\t").append(s);
	}
	va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogWarning, s);
	}
	else
	{
		printf("%s\n", s.c_str());
	}
}