#pragma once


#ifdef LINUX
#define MAX_PATH 256
#include <unistd.h>
	#define SLEEP(a) sleep(a)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
#else
	#include <windows.h>
	#define SLEEP(a) Sleep(a)
#endif //LINUX
#include <string>
#include <iostream>
#include <algorithm>
#include "Engine.h"
#include "Cube.h"
#include "Material.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"
#include "text2D.h"
#include "Events.h"
#include "CameraManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "LightManager.h"
#include "ZobObjectManager.h"

#define kUnused(var) (void) var;
#define WIDTH  800//320//640//1920 / 2
#define HEIGHT 600//240//480//1080 / 2
#define TARGET_MS_PER_FRAME 33.33f
#define CLAMP(n, low, max) n <= low ? low : n >= max ? max : n;

class DirectZob
{
	static DirectZob* singleton;
public :
	DirectZob();
	~DirectZob();

	void Init();
	void LoadScene(std::string& path, std::string& file);
	void SaveScene(std::string& path, std::string& file);
	void SaveScene();
	void NewScene();
	bool CanFastSave();

	int RunAFrame();
	const uint* GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	std::string ExePath();

	static DirectZob* GetInstance() { return DirectZob::singleton; }
	Engine* GetEngine() const { return m_engine; }
	CameraManager* GetCameraManager() const { return m_cameraManager; }
	LightManager* GetLightManager() const { return m_lightManager; }
	ZobObjectManager* GetZobObjectManager() const { return m_zobObjectManager; }
	MeshManager* GetMeshManager() const { return m_meshManager; }
	MaterialManager* GetmaterialManager() const { return m_materialManager;  }
	Events* GetEventManager() { return m_events; }
	static void LogInfo(const char* str);
	static void LogError(const char* str);
	static void LogWarning(const char* str);
private:

	Events* m_events = NULL;
	MaterialManager* m_materialManager = NULL;
	MeshManager* m_meshManager = NULL;
	CameraManager* m_cameraManager = NULL;
	LightManager* m_lightManager = NULL;
	ZobObjectManager* m_zobObjectManager = NULL;
	Engine* m_engine = NULL;
	Text2D* m_text = NULL;

	static bool g_bShowGrid;
	static bool g_bShowMeshes;
};
