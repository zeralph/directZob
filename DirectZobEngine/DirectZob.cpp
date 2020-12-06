#include <mutex>
#include <thread>
#include "DirectZob.h"
#include "Events.h"
#include "tinyxml.h"
#include "ZobObjects/ZobObject.h"
#include "SceneLoader.h"
#include "ZobPhysic/ZobPhysicsEngine.h"

#define LOG_BUFFER_SIZE 1024

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
}

DirectZob::~DirectZob()
{
	//delete m_engine;
	delete m_meshManager;
	delete m_materialManager;
	delete m_cameraManager;
	delete m_lightManager;
	delete m_text;
	delete m_events;
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
	m_lightManager = new LightManager();
	m_materialManager = new MaterialManager();
	m_meshManager = new MeshManager();
	m_cameraManager = new CameraManager();
	int dx = 1;
	int dy = 1;
	float r = 0.0f;
	long frame = 0;
	float rot = 0.0;
	char frameCharBuffer[sizeof(ulong)];
	int state;
	m_initialized = true;
	m_frameTick = clock();
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
		RunAFrame();
		func();
	}
}

int DirectZob::RunAFrame(DirectZob::engineCallback OnSceneUpdated /*=NULL*/, DirectZob::engineCallback OnQueuing /*=NULL*/)
{
	g_render_mutex.lock();
	int state=0;
	m_frameTime =  (float)(clock() - m_frameTick) / CLOCKS_PER_SEC * 1000;
	m_fps = 1000.0f / m_frameTime;
	m_frameTick = clock();
	if(m_initialized && m_engine->Started())
	{
		m_cameraManager->UpdateAfter();
		Color c = Color(DirectZob::GetInstance()->GetLightManager()->GetClearColor());
		m_engine->ClearBuffer(&c);
		Camera* cam = m_cameraManager->GetCurrentCamera();
		if (cam)
		{
			bool bPhysicUpdated = false;
//			cam->UpdateViewProjectionMatrix();
			m_zobObjectManager->PreUpdate();
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
				m_physicsEngine->StartUpdatePhysic(m_frameTime/1000.0f);
				m_zobObjectManager->UpdateBehavior();
			}
			m_zobObjectManager->StartUpdateScene(cam, m_engine);
			m_geometryTime = m_zobObjectManager->WaitForUpdateObjectend();
			m_renderTime = m_engine->WaitForRasterizersEnd();
			m_physicTime = 0;
			if (bPhysicUpdated)
			{
				m_physicTime = m_physicsEngine->WaitForUpdatePhysicEnd();
			}
			m_engine->ClearRenderQueues();
			m_copyTick = clock();
			//if (OnQueuing)
			//{
			//	OnQueuing();
			//}
			m_zobObjectManager->QueueForDrawing(cam, m_engine);
			m_copyTime = (float)(clock() - m_copyTick) / CLOCKS_PER_SEC * 1000;
			if (m_engine->ShowGrid())
			{
				m_engine->DrawGrid(cam);
			}
		}
		else
		{
			if (m_text)
			{
				_snprintf_s(buffer, MAX_PATH, "WARNING : NO CAMERA", NULL);
				std::string sBuf = std::string(buffer);
				m_text->Print(m_engine->GetBufferData()->width / 2, m_engine->GetBufferData()->height / 2, 1, &sBuf, 0xFFFF0000);
			}
		}
		if (m_text)
		{
			_snprintf_s(buffer, MAX_PATH, "Triangles : %i", m_engine->GetNbDrawnTriangles());
			std::string sBuf = std::string(buffer);
			m_text->Print(0, 0, 1, &sBuf, 0xFFFFFFFF);
			_snprintf_s(buffer, MAX_PATH, "render : %03i, geom : %03i, phys : %03i, cpy : %03i, tot : %03i, FPS : %03i", (int)m_renderTime, (int)m_geometryTime, (int)m_physicTime, (int)m_copyTime, (int)m_frameTime, (int)m_fps);
			sBuf = std::string(buffer);
			if (m_frameTime < TARGET_MS_PER_FRAME)
			{
				m_text->Print(0, 16, 1, &sBuf, 0xFF00FF00);
				//SLEEP((TARGET_MS_PER_FRAME - m_frameTime));
			}
			else
			{
				m_text->Print(0, 16, 1, &sBuf, 0xFFFF0000);
			}
			switch(m_engine->GetLightingPrecision())
			{
				default:
				case eLightingPrecision_noLighting:
					sBuf = std::string("LighingPrecision : no lighting");
					break;
				case eLightingPrecision_pixel:
					sBuf = std::string("LighingPrecision : pixel");
					break;
				case eLightingPrecision_vertex:
					sBuf = std::string("LighingPrecision : vertex");
					break;

			}
			m_text->Print(0, 32, 1, &sBuf, 0xFFFFFFFF);
		}
		m_engine->SetDisplayedBuffer();
	}
	g_render_mutex.unlock();
	return state;
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
			s.append(std::string("\u2502"));
		}
		s.append(std::string("\u251C"));
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