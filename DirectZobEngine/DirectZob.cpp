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
	delete m_textureManager;
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
	SceneLoader::LoadScene(path, file, m_zobObjectManager, m_textureManager);


	std::string font("Font");
	m_text = new Text2D(m_engine, m_textureManager->GetTexture(font), 32, 8, m_events);
}


void DirectZob::Init()
{
	m_events = new Events();
	m_events->AddEvent(0, "Init engine");
	m_engine = new Engine(WIDTH, HEIGHT, m_events);
	m_cameraManager = new CameraManager();
	m_textureManager = new TextureManager();
	m_zobObjectManager = new ZobObjectManager();
	m_events->AddEvent(0, " OK\n");
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
	//for (;;)
	{
		//m_engine->ClearBuffer(&Color(255,63,149,255));
		m_engine->ClearBuffer(&Color::White);

		Camera* cam = m_cameraManager->GetCurrentCamera();
		cam->Update();

		//if (g_bShowGrid)
		{
			m_engine->DrawGrid(cam);
		}

		m_zobObjectManager->UpdateObjects();
		m_zobObjectManager->DrawObjects(cam, m_engine);
		m_engine->DrawScene();

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

		snprintf(buffer, MAX_PATH, "FPS : %.2f, render : %.2fms, geom : %.2f", m_engine->GetFps(), m_engine->GetRenderTime(), m_engine->GetGeometryTime());
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
	return state;
}

void DirectZob::Log(std::string& str)
{
	DirectZob::singleton->GetEventManager()->AddEvent(0, str);
}
void DirectZob::Log(const char* str)
{
	DirectZob::singleton->GetEventManager()->AddEvent(0, str);
}