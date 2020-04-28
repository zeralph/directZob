#include "DirectZob.h"
#include "Events.h"
#include "tinyxml.h"
#include "ZobObject.h"
#include "SceneLoader.h"
#define LOG_BUFFER_SIZE 1024

static char buffer[MAX_PATH];
static char logBuffer[LOG_BUFFER_SIZE];
static bool g_isInEditorMode;
DirectZob *DirectZob::singleton = nullptr;

DirectZob::DirectZob()
{
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
	SceneLoader::NewScene();
	if (m_text == NULL)
	{
		m_text = new Text2D(m_engine, m_events);
	}
}

bool DirectZob::CanFastSave()
{
	return SceneLoader::CanFastSave();
}

void DirectZob::Init(bool bEditorMode)
{
	g_isInEditorMode = bEditorMode;
	m_events = new Events();
	DirectZob::LogInfo("Init engine");
	m_engine = new Engine(WIDTH, HEIGHT, m_events);
	m_cameraManager = new CameraManager();
	m_lightManager = new LightManager();
	m_materialManager = new MaterialManager();
	m_meshManager = new MeshManager();
	m_zobObjectManager = new ZobObjectManager();
	int dx = 1;
	int dy = 1;
	float r = 0.0f;
	long frame = 0;
	float rot = 0.0;
	char frameCharBuffer[sizeof(ulong)];
	int state;
	
//	m_engine->Start();
}

static float rot = 1.0f;

int DirectZob::RunAFrame()
{
	int state=0;
	if(m_engine->Started() && !m_isRendering)
	{
		m_isRendering = true;
		Color c = Color(DirectZob::GetInstance()->GetLightManager()->GetClearColor());
		m_engine->ClearBuffer(&c);
		m_engine->StartDrawingScene();
		Camera* cam = m_cameraManager->GetCurrentCamera();
		cam->Update();
		m_zobObjectManager->UpdateObjects();
		m_engine->WaitForRasterizersEnd();
		m_engine->ClearRenderQueues();
		clock_t tick = clock();
		m_zobObjectManager->DrawObjects(cam, m_engine);
		if (m_engine->ShowGrid())
		{
			m_engine->DrawGrid(cam);
		}
		if (m_text)
		{
			_snprintf_s(buffer, MAX_PATH, "Triangles : %i / %i", m_engine->GetNbDrawnTriangles(), m_engine->GetNbTriangles());
			std::string sBuf = std::string(buffer);
			m_text->Print(0, 0, 1, &sBuf, 0xFFFFFFFF);
			_snprintf_s(buffer, MAX_PATH, "render : %06.2fms, geom : %06.2f, tot : %06.2f, FPS : %06.2f", m_engine->GetRenderTime(), m_engine->GetGeometryTime(), m_engine->GetFrameTime(), m_engine->GetFps());
			float t = m_engine->GetFps();
			t = (1.0f / t) * 1000.0f;
			sBuf = std::string(buffer);
			if (t < TARGET_MS_PER_FRAME)
			{
				m_text->Print(0, 16, 1, &sBuf, 0xFF00FF00);
				SLEEP((TARGET_MS_PER_FRAME - t));
			}
			else
			{
				m_text->Print(0, 16, 1, &sBuf, 0xFFFF0000);
			}
		}
		m_engine->SetGeometryTime((float)(clock() - tick) / CLOCKS_PER_SEC * 1000);
		m_engine->EndDrawingScene();
		m_isRendering = false;
	}
	return state;
}

void DirectZob::LogInfo(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string(logBuffer);
	va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogInfo, s);
}
	else
	{
		printf("%s\n", logBuffer);
	}
}

void DirectZob::LogError(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string(logBuffer);
    va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogError, s);
}
	else
	{
		printf("%s\n", logBuffer);
	}
}
void DirectZob::LogWarning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_vsnprintf_s(logBuffer, LOG_BUFFER_SIZE, format, args);
	std::string s = std::string(logBuffer);
	va_end(args);
	if (g_isInEditorMode)
	{
		DirectZob::singleton->GetEventManager()->AddEvent(Events::LogWarning, s);
	}
	else
	{
		printf("%s\n", logBuffer);
	}
}
