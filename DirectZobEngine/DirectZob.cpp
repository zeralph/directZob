#include "DirectZob.h"
#include "Events.h"
#include "tinyxml.h"
#include "ZobObject.h"
#include "SceneLoader.h"

static char buffer[MAX_PATH];

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
	delete m_text;
	delete m_events;
}

std::string DirectZob::ExePath() {

	//return std::string("D:\\_PERSO\\directZob\\directZob\\resources\\");
	return std::string("C:\\_GIT\\directZob\\resources");

	char b[MAX_PATH];
	GetModuleFileName(NULL, b, MAX_PATH);
	std::string::size_type pos = std::string(b).find_last_of("\\/");
	return std::string(b).substr(0, pos);
}

void DirectZob::LoadScene(std::string& path, std::string& file)
{
	SceneLoader::LoadScene(path, file, m_zobObjectManager, m_meshManager, m_materialManager);
	std::string font("Font");
	m_text = new Text2D(m_engine, m_materialManager->GetTexture(font), 32, 8, m_events);
}

void DirectZob::SaveScene(std::string& path, std::string& file)
{
	SceneLoader::SaveScene(path, file, m_zobObjectManager, m_meshManager, m_materialManager);
}

void DirectZob::SaveScene()
{
	SceneLoader::SaveScene(m_zobObjectManager, m_meshManager, m_materialManager);
}

void DirectZob::NewScene()
{
	SceneLoader::NewScene(m_engine, m_zobObjectManager, m_meshManager, m_materialManager);
	std::string font("Font");
	m_text = new Text2D(m_engine, m_materialManager->GetTexture(font), 32, 8, m_events);
}

bool DirectZob::CanFastSave()
{
	return SceneLoader::CanFastSave();
}

void DirectZob::Init()
{
	m_events = new Events();
	DirectZob::LogInfo("Init engine");
	m_engine = new Engine(WIDTH, HEIGHT, m_events);
	m_cameraManager = new CameraManager();
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
	
	m_engine->Start();
}

static float rot = 1.0f;

int DirectZob::RunAFrame()
{
	int state=0;
	if(m_engine->Started())
	{
		//m_engine->ClearBuffer(&Color(255,63,149,255));
		m_engine->ClearBuffer(&Color::White);

		Camera* cam = m_cameraManager->GetCurrentCamera();
		cam->Update();

		if(m_engine->ShowGrid())
		{
			m_engine->DrawGrid(cam);
		}
		clock_t tick = clock();
		m_zobObjectManager->UpdateObjects();
		m_zobObjectManager->DrawObjects(cam, m_engine);
		m_engine->SetGeometryTime( (float)(clock() - tick) / CLOCKS_PER_SEC * 1000);

		m_engine->StartDrawingScene();
		m_engine->EndDrawingScene();
		if (m_text)
		{
			snprintf(buffer, MAX_PATH, "Triangles : %lu / %lu", m_engine->GetNbDrawnTriangles(), m_engine->GetNbTriangles());
			m_text->Print(0, 0, 1, &std::string(buffer), 0xFFFFFFFF);

			const Vector3* cp = cam->GetPosition();
			const Vector3* ct = cam->GetTarget();
			const Vector3* cf = cam->GetForward();
			snprintf(buffer, MAX_PATH, "Cam %s pos :  %.2f, %.2f, %.2f, tar : %.2f, %.2f, %.2f, fw : %.2f, %.2f, %.2f",
				cam->GetName().c_str(),
				cp->x, cp->y, cp->z,
				ct->x, ct->y, ct->z,
				cf->x, cf->y, cf->z
			);
			m_text->Print(0, 8, 1, &std::string(buffer), 0xFFFFFFFF);

			snprintf(buffer, MAX_PATH, "render : %06.2fms, geom : %06.2f, tot : %06.2f, FPS : %06.2f", m_engine->GetRenderTime(), m_engine->GetGeometryTime(), m_engine->GetFrameTime(), m_engine->GetFps());
			float t = m_engine->GetFps();
			t = (1.0f / t) * 1000.0f;
			if (t < TARGET_MS_PER_FRAME)
			{
				m_text->Print(0, 16, 1, &std::string(buffer), 0xFF00FF00);
				Sleep(TARGET_MS_PER_FRAME - t);
			}
			else
			{
				m_text->Print(0, 16, 1, &std::string(buffer), 0xFFFF0000);
			}
		}
	}
	return state;
}

void DirectZob::LogInfo(const char* str)
{
	DirectZob::singleton->GetEventManager()->AddEvent(Events::LogInfo, str);
}
void DirectZob::LogError(const char* str)
{
	DirectZob::singleton->GetEventManager()->AddEvent(Events::LogError, str);
}
void DirectZob::LogWarning(const char* str)
{
	DirectZob::singleton->GetEventManager()->AddEvent(Events::LogWarning, str);
}