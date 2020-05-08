#pragma once


#ifdef LINUX
    #define MAX_PATH 256
    #include <unistd.h>
	#define SLEEP(a) sleep(a/1000)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
    #define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#elif WINDOWS
	#include <windows.h>
	#define SLEEP(a) Sleep(a)
#elif MACOS
    #define MAX_PATH 256
    #include <unistd.h>
	#define SLEEP(a) sleep(a/1000)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#endif //LINUX
#include <string>
#include <iostream>
#include <algorithm>
#include "Engine.h"
#include "Cube.h"
#include "Material.h"
#include "Mesh.h"
#include "text2D.h"
#include "Events.h"
#include "CameraManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "LightManager.h"
#include "ZobObjectManager.h"


#define kUnused(var) (void) var;
#define TARGET_MS_PER_FRAME 33.33f
#define CLAMP(n, low, max) n <= low ? low : n >= max ? max : n;


class DirectZob
{
	static DirectZob* singleton;
public :
	
	DirectZob();
	~DirectZob();

	void Init(int width, int height, bool bEditorMode);
	void LoadScene(std::string& path, std::string& file);
	void Unload();
	void SaveScene(std::string& path, std::string& file);
	void SaveScene();
	void NewScene();
	bool CanFastSave();

	const float GetFps() const { return m_fps; };
	const float GetRenderTime() const { return m_geometryTime; };
	const float GetGeometryTime() const { return m_renderTime; };
	const float GetFrameTime() const { return m_frameTime; };
	int RunAFrame();
	const uint* GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	std::string ExePath();

	static DirectZob* GetInstance() { return DirectZob::singleton; }
	Engine* GetEngine() const { return m_engine; }
	CameraManager* GetCameraManager() const { return m_cameraManager; }
	LightManager* GetLightManager() const { return m_lightManager; }
	ZobObjectManager* GetZobObjectManager() const { return m_zobObjectManager; }
	MeshManager* GetMeshManager() const { return m_meshManager; }
	MaterialManager* GetMaterialManager() const { return m_materialManager;  }
	Events* GetEventManager() { return m_events; }
	static void LogInfo(const char* format, ...);
	static void LogError(const char* format, ...);
	static void LogWarning(const char* format, ...);
private:
	
	Events* m_events = NULL;
	MaterialManager* m_materialManager = NULL;
	MeshManager* m_meshManager = NULL;
	CameraManager* m_cameraManager = NULL;
	LightManager* m_lightManager = NULL;
	ZobObjectManager* m_zobObjectManager = NULL;
	Engine* m_engine = NULL;
	Text2D* m_text = NULL;
	bool m_isRendering = false;
	static bool g_bShowGrid;
	static bool g_bShowMeshes;
	bool m_initialized = false;
	float m_renderTime;
	float m_geometryTime;
	float m_frameTime;
	float m_fps;
	clock_t	m_frameTick;
};
