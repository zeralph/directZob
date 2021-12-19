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
#undef None
#include "../../dependencies/optick/include/optick.h"
#define LOG_BUFFER_SIZE 1024

static const int fpsTargetsN = 6;
static const float fpsTargets[fpsTargetsN] = { 0, 16.6666667, 33.3333333f, 41.666667f,  50.0f, 1000.0f };
static int sTargetMSPerFrameIdx = 1;
static char buffer[MAX_PATH];
static char logBuffer[LOG_BUFFER_SIZE];
bool DirectZob::g_isInEditorMode = false;
static int s_logIndent;
//static std::mutex g_render_mutex;
static std::thread g_editorModeThread;
int DirectZob::s_logIndent = 0;
DirectZob *DirectZob::singleton = nullptr;
DirectZob::eDirectZobLogLevel DirectZob::sLogLevel = DirectZob::eDirectZobLogLevel_info;

DirectZob::DirectZob()
{
	m_initialized = false;
	DirectZob::singleton= this; 
	m_frameTime = 1.0f;
	m_window = NULL;
	m_onSceneLoaded = NULL;
}

DirectZob::~DirectZob()
{
	delete m_hudManager;
	delete m_meshManager;
	delete m_materialManager;
	delete m_cameraManager;
	delete m_lightManager;
	delete m_text;
	delete m_events;
	delete m_inputManager;
	delete m_engine;
}

void DirectZob::Shutdown()
{

}

std::string DirectZob::ExePath() {

	//return std::string("D:\\_PERSO\\directZob\\directZob\\resources\\");
	return std::string("C:\\_GIT\\directZob\\resources");
}

void DirectZob::LoadScene(std::string& path, std::string& file, DirectZob::engineCallback OnSceneLoaded)
{
	m_onSceneLoaded = OnSceneLoaded;
	SceneLoader::LoadScene(path, file);
	if (m_text == NULL)
	{
		m_text = new Text2D(m_engine, m_events);
	}
}

void DirectZob::RegenerateZobIds()
{
	m_zobObjectManager->GetRootObject()->RegenerateZobIds();
}


void DirectZob::OnSceneLoaded()
{
	if (m_onSceneLoaded)
	{
		m_onSceneLoaded();
	}
}

void DirectZob::LoadZobObject(std::string& path, std::string& file)
{
	SceneLoader::LoadZobObject(path, file);
}

void DirectZob::Lock()
{
	//g_render_mutex.lock();
}

void DirectZob::Unlock()
{
	//g_render_mutex.unlock();
}

void DirectZob::SaveScene(std::string& path, std::string& file)
{
	SceneLoader::SaveScene(path, file);
}

void DirectZob::SaveScene()
{
	SceneLoader::SaveScene();
}

void DirectZob::NewScene(std::string workspace)
{
	//g_render_mutex.lock();
	m_physicStarted = false;
	SceneLoader::NewScene(workspace);
	if (m_text == NULL)
	{
		m_text = new Text2D(m_engine, m_events);
	}
	DirectZob::GetInstance()->GetEngine()->Start();
	//g_render_mutex.unlock();
}

void DirectZob::Resize(int width, int height)
{
	//g_render_mutex.lock();
	m_engine->Resize(width, height);
	//g_render_mutex.unlock();
}

void DirectZob::Unload()
{
	//g_render_mutex.lock();
	DirectZob::GetInstance()->GetEngine()->Stop();
	SceneLoader::UnloadScene();
	
	//delete DirectZob::GetInstance();
	//g_render_mutex.unlock();
}

bool DirectZob::CanFastSave()
{
	return SceneLoader::CanFastSave();
}

void DirectZob::Init(mfb_window* window, int width, int height, bool bEditorMode)
{
	m_window = window;
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

void DirectZob::EditorUpdate()
{
	m_zobObjectManager->EditorUpdate();
}

int DirectZob::RunAFrame(DirectZob::engineCallback OnSceneUpdated /*=NULL*/, DirectZob::engineCallback OnQueuing /*=NULL*/)
{
	OPTICK_EVENT();
	ZobVector3 color = ZobVector3(1, 1, 1);
	timespec tstart;
	timespec tend;
	SaveTime(&tstart);
	//g_render_mutex.lock();
	int state=0;
	m_fps = 1000.0f / m_frameTime;
	if(m_initialized && m_engine->Started())
	{
#ifdef WINDOWS
		HWND hWnd = 0;
		SWindowData* window_data = (SWindowData*)m_window;
		if (window_data)
		{
			SWindowData_Win* window_data_win = (SWindowData_Win*)window_data->specific;
			hWnd = window_data_win->window;
		}
		m_inputManager->Update(m_frameTime, hWnd);
#elif LINUX
		Display* display = NULL;
		SWindowData* window_data = (SWindowData*)m_window;
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
		Camera* cam = m_cameraManager->GetCurrentCamera();
		if (cam)
		{
			float dt = m_frameTime / 1000.0f;
			m_engine->SwapBuffers();
			bool bPhysicUpdated = false;
			m_zobObjectManager->PreUpdate(dt);
			m_hudManager->PreUpdate(dt);
			m_lightManager->PreUpdate(dt);
			m_engine->StartDrawingScene();
			ZobColor c = DirectZob::GetInstance()->GetLightManager()->GetClearColor();
			if (OnSceneUpdated)
			{
				OnSceneUpdated();
			}
			m_zobObjectManager->UpdateObjects(cam, m_engine, dt);
			if (m_physicStarted)
			{
				bPhysicUpdated = true;
				//m_zobObjectManager->UpdatePhysic(dt);
				m_physicsEngine->StartUpdatePhysic(dt);

			}
					
			m_hudManager->UpdateObjects(cam, m_engine, dt);
			m_zobObjectManager->PostUpdate();
			m_engine->ClearBuffer(&c);
			m_renderTime = m_engine->WaitForRasterizersEnd();
			//ZobLightManager update is made after resterizers' work because it changes the active lights vectors
			m_lightManager->Update();
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
			//m_zobObjectManager->PostUpdate();
			m_zobObjectManager->QueueForDrawing(cam, m_engine);
			m_hudManager->QueueForDrawing(cam, m_engine);
			if (m_engine->DrawPhysyicsGizmos())
			{
				m_physicsEngine->DrawGizmos();
			}
			if (m_engine->DrawGizmos())
			{
				m_zobObjectManager->DrawGizmos(cam, m_engine);
			}
		}
		else
		{	
			m_hudManager->Print(ZobHUDManager::eHudUnit_ratio, 0.5f, 0.5f, 2, "Arial", &color, "WARNING : %s", "NO CAMERA");
		}
		m_hudManager->Print(ZobHUDManager::eHudUnit_ratio, 0.01f, 0.01f, 1, "VCR OSD Mono", &color, "Triangles : %i color depth : %i", m_engine->GetNbDrawnTriangles(), m_engine->GetNbBitsPerColorDepth());
		color = ZobVector3(0, 1, 0);
		if (m_frameTime <= fpsTargets[sTargetMSPerFrameIdx])
		{
			ZobVector3 color = ZobVector3(1, 0, 0);
		}
		m_hudManager->Print(ZobHUDManager::eHudUnit_ratio, 0.01f, 0.04f, 1, "VCR OSD Mono", &color, "render : %03i, geom : %03i, phys : %03i, cpy : %03i, tot : %03i, FPS : %03i", (int)m_renderTime, (int)m_geometryTime, (int)m_physicTime, (int)m_copyTime, (int)m_frameTime, (int)m_fps);
		color = ZobVector3(1, 1, 1);
		m_hudManager->Print(ZobHUDManager::eHudUnit_ratio, 0.01f, 0.07f, 1, "VCR OSD Mono", &color, "Controller LX : %.2f, LY  : %.2f, RX : %.2f, RY : %.2f, LT : %.2f, RT : %.2f",
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftStickX), 
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftStickY),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightStickX),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightStickY),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::LeftShoulder),
			m_inputManager->GetMap()->GetFloat(ZobInputManager::RightShoulder));
		if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::WireFrame))
		{
			m_engine->WireFrame(!m_engine->WireFrame());
		}
		if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::Gizmos))
		{
			bool b = !m_engine->DrawPhysyicsGizmos();
			m_engine->DrawGizmos(b);
			m_engine->DrawPhysyicsGizmos(b);
		}
		if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::Quit))
		{
			Exit();
		}
		if (m_inputManager->GetMap()->GetBoolIsNew(ZobInputManager::SwitchFPS))
		{
			sTargetMSPerFrameIdx++;
			if (sTargetMSPerFrameIdx == fpsTargetsN)
			{
				sTargetMSPerFrameIdx = 0;
			}
			LogWarning("FPS set to %i", fpsTargets[sTargetMSPerFrameIdx]?(int)(1000.0f / fpsTargets[sTargetMSPerFrameIdx]):0);
		}
		m_engine->PrintRasterizersInfos();
		m_engine->SetDisplayedBuffer();
	}
	SaveTime(&tend);
	float dt = (float)GetDeltaTime_MS(tstart, tend);
	WaitToTargetFrameTime(dt);
	SceneLoader::Update();
	//g_render_mutex.unlock();
	return state;
}

void DirectZob::Exit()
{
	mfb_close(m_window);
}

void DirectZob::WaitToTargetFrameTime(float dt)
{
	OPTICK_CATEGORY("WaitToTargetFrameTime", Optick::Category::Wait);
	if (dt < 0)
	{
		throw "rput";
	}
	float targetDt = fpsTargets[sTargetMSPerFrameIdx] - dt;
	if (targetDt > 0.0f)
	{
		SleepMS(targetDt);
		m_frameTime = fpsTargets[sTargetMSPerFrameIdx];
	}
	else
	{
		m_frameTime = dt;
	}
}

void DirectZob::SleepMS(float ms)
{
#ifdef LINUX
	usleep((uint)(ms * 1000.0f));
#elif WINDOWS
	std::this_thread::sleep_for(std::chrono::milliseconds((uint)ms/2));
#elif MACOS
	usleep((uint)(ms*1000.0f));
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
		/*if (z->GetMesh())
		{
			c = 0xFFFF0000;
			if (z->GetMesh()->IsDrawn())
			{
				c = 0xFF00FF00;
			}
		}
		*/
		m_text->Print(txtW, (i*10), c, z->GetName().c_str());
	}
}

const std::string& DirectZob::GetResourcePath()
{ 
	return SceneLoader::GetResourcePath(); 
}

void DirectZob::LogInfo(const char* format, ...)
{
	if(sLogLevel > eDirectZobLogLevel_info)
	{
		return;
	}
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
	if(sLogLevel > eDirectZobLogLevel_error)
	{
		return;
	}
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
	if(sLogLevel > eDirectZobLogLevel_warning)
	{
		return;
	}
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