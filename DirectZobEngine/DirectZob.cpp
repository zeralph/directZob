#include "DirectZob.h"
#include "Events.h"
#include "tinyxml.h"

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

	return std::string("D:\\_PERSO\\directZob\\directZob\\resources\\");
	return std::string("C:\\_GIT\\directZob\\x64\\DLL Debug");

	char b[MAX_PATH];
	GetModuleFileName(NULL, b, MAX_PATH);
	std::string::size_type pos = std::string(b).find_last_of("\\/");
	return std::string(b).substr(0, pos);
}

void DirectZob::LoadScene(std::string& path, std::string& file)
{
	float x, y, z, fov, znear, zfar;
	std::string name, texture, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = path + file;
	doc.LoadFile(fullPath.c_str());

	if (doc.Error())
	{
		std::string err = "Error loading ";
		err.append(fullPath.c_str());
		DirectZob::Log(err);
	}
	else
	{
		TiXmlElement* root = doc.FirstChildElement("Scene");
		TiXmlElement* fe = root->FirstChildElement("Texture");
		for (TiXmlElement* e = root->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			name = e->Attribute("name");
			fullPath = path + e->Attribute("file");
			m_textureManager->LoadTexture(name, fullPath);
		}
		for (TiXmlElement* e = root->FirstChildElement("Camera"); e != NULL; e = e->NextSiblingElement("Camera"))
		{
			name = e->Attribute("name");
			TiXmlElement* f;
			f = e->FirstChildElement("Position");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 p = Vector3(x, y, z);
			f = e->FirstChildElement("Target");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 t = Vector3(x, y, z);
			f = e->FirstChildElement("Up");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 u = Vector3(x, y, z);
			f = e->FirstChildElement("Fov");
			float fov = atof(f->Value());
			m_cameraManager->CreateCamera(name, p, t, u, fov);
		}
		for (TiXmlElement* e = root->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			name = e->Attribute("name");
			texture = e->Attribute("texture");
			fullPath = path + e->Attribute("file");
			TiXmlElement* f;
			f = e->FirstChildElement("Position");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 p = Vector3(x, y, z);
			f = e->FirstChildElement("Rotation");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 r = Vector3(x, y, z);
			f = e->FirstChildElement("Position");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 u = Vector3(x, y, z);
			f = e->FirstChildElement("Scale");
			x = atof(f->Attribute("x"));
			y = atof(f->Attribute("y"));
			z = atof(f->Attribute("z"));
			Vector3 s = Vector3(x, y, z);
			const Texture* t = m_textureManager->GetTexture(texture);
			m_engine->LoadMesh(name, fullPath, t, p, r, s);
		}		
	}
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
		static float tx = 0;
		static float ty = 0;
		static float tz = 0;
		static float scale = 0.25f;// 1.0f / 15.0f;// 0.5f;

		std::vector<Mesh*>* meshes = m_engine->GetMeshVector();
		for (int i = 0; i < meshes->size(); i++)
		{
			meshes->at(i)->Init();
			meshes->at(i)->SetRotation(0, rot, 0);
			
		}
		rot += 1.1f;
		state = m_engine->Update(cam);

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