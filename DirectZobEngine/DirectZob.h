#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include "Rendering/Engine.h"
#include "ZobMaterial.h"
#include "Mesh.h"
#include "Rendering/text2D.h"
#include "Events.h"
#include "Managers/CameraManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Managers/LightManager.h"
#include "Managers/ZobObjectManager.h"
#include "Managers/ZobInputManager.h"
#include "Managers/ZobHUDManager.h"
#include "../minifb/src/WindowData.h"

#define kUnused(var) (void) var;
#define CLAMP(n, low, max) n <= low ? low : n >= max ? max : n;
using namespace std;
class ZobPhysicsEngine;
class DirectZob
{
	
	static DirectZob* singleton;
public :

#ifdef WINDOWS
	struct timespec { long tv_sec; long tv_nsec; };    //header part
	int SaveTime(struct timespec* spec)      //C-file part
	{
		__int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970
		spec->tv_sec = wintime / 10000000i64;           //seconds
		spec->tv_nsec = wintime % 10000000i64 * 100;      //nano-seconds
		return 0;
	}
#elif LINUX || MACOS
	int SaveTime(struct timespec* spec)
	{
		return clock_gettime(CLOCK_REALTIME, spec);
	}
#endif
	
	enum eDirectZobLogLevel
	{
		eDirectZobLogLevel_info=0,
		eDirectZobLogLevel_warning,
		eDirectZobLogLevel_error,
		eDirectZobLogLevel_none
	};

	typedef void (*engineCallback)(void);

	DirectZob();
	~DirectZob();


	static DirectZobType::zobId	GenerateZobId()
							{
								unsigned long long rand1 = abs(rand());
								unsigned long long rand2 = abs(rand());
								rand1 = rand1 << (sizeof(int) * 8);
								unsigned long long randULL = (rand1 | rand2);
								return (DirectZobType::zobId)randULL;
							}
	void					StartPhysic() { m_physicStarted = true; };
	void					StopPhysic(bool reset);
	inline bool				IsPhysicPlaying() const { return m_physicStarted; }
	void					Init(mfb_window* window, int width, int height, bool bEditorMode);
	void					LoadScene(std::string& path, std::string& file, DirectZob::engineCallback OnSceneLoaded);
	void					LoadZobObject(std::string& path, std::string& file);
	void					Unload();
	void					SaveScene(std::string& path, std::string& file);
	void					SaveScene();
	void					NewScene();
	bool					CanFastSave();
	void					Exit();
	void					RegenerateZobIds();
	void					Resize(int width, int height);
	void					OnSceneLoaded();
	const float				GetFps() const { return m_fps; };
	const float				GetRenderTime() const { return m_renderTime; };
	const float				GetGeometryTime() const { return m_geometryTime; };
	const float				GetFrameTime() const { return m_frameTime; };
	const float				GetCopyTime() const { return m_copyTime; };
	int						RunAFrame(engineCallback = NULL, DirectZob::engineCallback OnQueuing = NULL);
	void					EditorUpdate();
	int						Run( void func(void) );
	int						GetBufferDataLenght() const { return m_engine->GetBufferData()->size; }
	const uint*				GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	int						GetBufferWidth() const { return m_engine->GetBufferData()->width; }
	int						GetBufferHeight() const { return m_engine->GetBufferData()->height; }
	uint*					GetEditorBufferDataNoConst() { return m_engine->GetBufferData()->buffer; }
	std::string				ExePath();
	double					GetDeltaTime_MS(timespec& start, timespec& end) const;
	inline static DirectZob*GetInstance() { return DirectZob::singleton; }
	inline Engine*			GetEngine() const { return m_engine; }
	const inline Engine*	GetEngineConst() const { return m_engine; }
	inline CameraManager*	GetCameraManager() const { return m_cameraManager; }
	inline LightManager*	GetLightManager() const { return m_lightManager; }
	inline ZobHUDManager*	GetHudManager() const { return m_hudManager; }
	inline ZobObjectManager*GetZobObjectManager() const { return m_zobObjectManager; }
	inline MeshManager*		GetMeshManager() const { return m_meshManager; }
	inline MaterialManager*	GetMaterialManager() const { return m_materialManager;  }
	inline ZobPhysicsEngine*GetPhysicsEngine() const { return m_physicsEngine; }
	inline Events*			GetEventManager() { return m_events; }
	inline ZobInputManager*	GetInputManager() { return m_inputManager; }
	inline Text2D*			GetTextManager() { return m_text; }
	static const std::string& GetResourcePath();
	static void				LogInfo(const char* format, ...);
	static void				LogError(const char* format, ...);
	static void				LogWarning(const char* format, ...);
	static void				AddIndent() { s_logIndent++; }
	static void				RemoveIndent() { s_logIndent--; if (s_logIndent < 0) { s_logIndent = 0; } }
	void					Lock();
	void					Unlock();
	float					GetLastDt() const { return m_frameTime / 1000.0f; }
	void					SleepMS(float ms);
	void					Shutdown();
private:
	int						RunInternal(void func(void));
	void					PrintObjectList();
	void					WaitToTargetFrameTime(float dt);
	
private:
	mfb_window* m_window;
	Events* m_events = NULL;
	MaterialManager* m_materialManager = NULL;
	MeshManager* m_meshManager = NULL;
	CameraManager* m_cameraManager = NULL;
	LightManager* m_lightManager = NULL;
	ZobObjectManager* m_zobObjectManager = NULL;
	ZobPhysicsEngine* m_physicsEngine = NULL;
	ZobHUDManager* m_hudManager = NULL;
	Engine* m_engine = NULL;
	Text2D* m_text = NULL;
	ZobInputManager* m_inputManager = NULL;
	bool m_isRendering = false;
	static bool g_bShowGrid;
	static bool g_bShowMeshes;
	bool m_initialized = false;
	float m_renderTime;
	float m_geometryTime;
	volatile float m_frameTime;
	float m_fps;
	float m_copyTime;
	float m_physicTime;
	float m_frameTick;
	static int s_logIndent;
	bool m_physicStarted;
	DirectZob::engineCallback m_onSceneLoaded;
};
