#pragma once
#include "Types.h"
#include "Rendering/Engine.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../dependencies/minifb/src/WindowData.h"

#ifdef DIRECTZOB_EXPORTS
#define DIRECTZOB_API __declspec(dllexport)
#else
#define DIRECTZOB_API __declspec(dllimport)
#endif

#define LOG_BUFFER_SIZE 1024
#define kUnused(var) (void) var;
using namespace std;
class ZobPhysicsEngine;
class Text2D;
class Events;
class MaterialManager;
class CameraManager;
class MeshManager;
class ZobObjectManager;
class ZobInputManager;
class ZobHUDManager;
class LightManager;
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
#elif LINUX
	int SaveTime(struct timespec* spec)
	{
		return clock_gettime(CLOCK_REALTIME, spec);
	}
#elif MACOS
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

	DIRECTZOB_API DirectZob();
	DIRECTZOB_API ~DirectZob();


	DIRECTZOB_API static DirectZobType::zobId	GenerateZobId()
							{
								unsigned long long rand1 = abs(rand());
								unsigned long long rand2 = abs(rand());
								rand1 = rand1 << (sizeof(int) * 8);
								unsigned long long randULL = (rand1 | rand2);
								return (DirectZobType::zobId)randULL;
							}
	DIRECTZOB_API void					StartPhysic() { m_physicStarted = true; };
	DIRECTZOB_API void					StopPhysic(bool reset);
	DIRECTZOB_API inline bool				IsPhysicPlaying() const { return m_physicStarted; }
	DIRECTZOB_API void		Init(mfb_window* window, int width, int height, bool bEditorMode);
	DIRECTZOB_API void					LoadScene(std::string& path, std::string& file, DirectZob::engineCallback OnSceneLoaded, DirectZobType::sceneLoadingCallback OnSceneLoading);
	DIRECTZOB_API void					LoadPack(std::string& path, std::string& file);
	DIRECTZOB_API void					LoadZobObject(std::string& path, std::string& file);
	DIRECTZOB_API void					Unload();
	DIRECTZOB_API void					SaveScene(std::string& path, std::string& file);
	DIRECTZOB_API void					SaveScene();
	DIRECTZOB_API void					NewScene(std::string workspace);
	DIRECTZOB_API bool					CanFastSave();
	DIRECTZOB_API void					Exit();
	DIRECTZOB_API void					RegenerateZobIds();
	DIRECTZOB_API void					Resize(int width, int height);
	DIRECTZOB_API void					OnSceneLoaded();
	DIRECTZOB_API const float				GetFps() const { return m_fps; };
	DIRECTZOB_API const float				GetRenderTime() const { return m_renderTime; };
	DIRECTZOB_API const float				GetGeometryTime() const { return m_geometryTime; };
	DIRECTZOB_API const float				GetFrameTime() const { return m_frameTime; };
	DIRECTZOB_API const float				GetCopyTime() const { return m_copyTime; };
	DIRECTZOB_API int						RunAFrame(engineCallback = NULL, DirectZob::engineCallback OnQueuing = NULL);
	DIRECTZOB_API void					EditorUpdate();;
	DIRECTZOB_API int						GetBufferDataLenght() const { return m_engine->GetBufferData()->size; }
	DIRECTZOB_API const uint*				GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	DIRECTZOB_API int						GetBufferWidth() const { return m_engine->GetBufferData()->width; }
	DIRECTZOB_API int						GetBufferHeight() const { return m_engine->GetBufferData()->height; }
	DIRECTZOB_API uint*					GetEditorBufferDataNoConst() { return m_engine->GetBufferData()->buffer; }
	DIRECTZOB_API double					GetDeltaTime_MS(timespec& start, timespec& end) const;
	DIRECTZOB_API inline static DirectZob*GetInstance() { return DirectZob::singleton; }
	DIRECTZOB_API inline Engine*			GetEngine() const { return m_engine; }
	DIRECTZOB_API const inline Engine*	GetEngineConst() const { return m_engine; }
	DIRECTZOB_API inline CameraManager*	GetCameraManager() const { return m_cameraManager; }
	DIRECTZOB_API inline LightManager*	GetLightManager() const { return m_lightManager; }
	DIRECTZOB_API inline ZobHUDManager*	GetHudManager() const { return m_hudManager; }
	DIRECTZOB_API inline ZobObjectManager*GetZobObjectManager() const { return m_zobObjectManager; }
	DIRECTZOB_API inline MeshManager*		GetMeshManager() const { return m_meshManager; }
	DIRECTZOB_API inline MaterialManager*	GetMaterialManager() const { return m_materialManager;  }
	DIRECTZOB_API inline ZobPhysicsEngine*GetPhysicsEngine() const { return m_physicsEngine; }
	DIRECTZOB_API inline Events*			GetEventManager() { return m_events; }
	DIRECTZOB_API inline ZobInputManager*	GetInputManager() { return m_inputManager; }
	DIRECTZOB_API inline Text2D*			GetTextManager() { return m_text; }
	DIRECTZOB_API static const std::string& GetResourcePath();
	DIRECTZOB_API static bool				IsEditorMode() { return g_isInEditorMode;}
	DIRECTZOB_API static void				LogInfo(const char* format, ...);
	DIRECTZOB_API static void				LogError(const char* format, ...);
	DIRECTZOB_API static void				LogWarning(const char* format, ...);
	DIRECTZOB_API static void				AddIndent() { s_logIndent++; }
	DIRECTZOB_API static void				RemoveIndent() { s_logIndent--; if (s_logIndent < 0) { s_logIndent = 0; } }
	DIRECTZOB_API static void				SetLogLevel(DirectZob::eDirectZobLogLevel l) { sLogLevel = l; }
	DIRECTZOB_API static eDirectZobLogLevel				GetLogLevel() { return sLogLevel; }
	DIRECTZOB_API void					Lock();
	DIRECTZOB_API void					Unlock();
	DIRECTZOB_API float					GetLastDt() const { return m_frameTime / 1000.0f; }
	DIRECTZOB_API void					SleepMS(float ms);
	DIRECTZOB_API void					Shutdown();
	DIRECTZOB_API Text2D*					GetDebugPrint() const { return m_text; }
private:
	int						RunInternal(void func(void));
	void					PrintObjectList();
	void					PrintEntityList();
	void					PrintInfos();
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
	//static bool g_bShowGrid;
	static bool g_bShowMeshes;
	static bool g_isInEditorMode;
	bool m_initialized = false;
	float m_renderTime;
	float m_geometryTime;
	volatile float m_frameTime;
	float m_fps;
	float m_copyTime;
	float m_physicTime;
	float m_frameTick;
	static int s_logIndent;
	static DirectZob::eDirectZobLogLevel sLogLevel;
	bool m_physicStarted;
	DirectZob::engineCallback m_onSceneLoaded;
	static char logBuffer[LOG_BUFFER_SIZE];
	static int sTargetMSPerFrameIdx;
	static int sTargetWobbleFactorIdx;
};
