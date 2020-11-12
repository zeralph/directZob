#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include "Engine.h"
#include "ZobMaterial.h"
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

using namespace std;
class ZobPhysicsEngine;
class DirectZob
{
	
	static DirectZob* singleton;
public :

	typedef void (*engineCallback)(void);

	DirectZob();
	~DirectZob();

	void				StartPhysic() { m_physicStarted = true; };
	void				StopPhysic(bool reset);
	inline bool			IsPhysicPlaying() const { return m_physicStarted; }
	void				Init(int width, int height, bool bEditorMode);
	void				LoadScene(std::string& path, std::string& file);
	void				Unload();
	void				SaveScene(std::string& path, std::string& file);
	void				SaveScene();
	void				NewScene();
	bool				CanFastSave();
	void				Resize(int width, int height);
	const float			GetFps() const { return m_fps; };
	const float			GetRenderTime() const { return m_renderTime; };
	const float			GetGeometryTime() const { return m_geometryTime; };
	const float			GetFrameTime() const { return m_frameTime; };
	const float			GetCopyTime() const { return m_copyTime; };
	int					RunAFrame(engineCallback = NULL, DirectZob::engineCallback OnQueuing = NULL);
	int					Run( void func(void) );
	const uint*			GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	std::string			ExePath();

	static DirectZob*	GetInstance() { return DirectZob::singleton; }
	Engine*				GetEngine() const { return m_engine; }
	CameraManager*		GetCameraManager() const { return m_cameraManager; }
	LightManager*		GetLightManager() const { return m_lightManager; }
	ZobObjectManager*	GetZobObjectManager() const { return m_zobObjectManager; }
	MeshManager*		GetMeshManager() const { return m_meshManager; }
	MaterialManager*	GetMaterialManager() const { return m_materialManager;  }
	ZobPhysicsEngine*	GetPhysicsEngine() const { return m_physicsEngine; }
	Events*				GetEventManager() { return m_events; }
	static void			LogInfo(const char* format, ...);
	static void			LogError(const char* format, ...);
	static void			LogWarning(const char* format, ...);
	static void			AddIndent() { s_logIndent++; }
	static void			RemoveIndent() { s_logIndent--; if (s_logIndent < 0) { s_logIndent = 0; } }
	void				Lock();
	void				Unlock();

private:
	int					RunInternal(void func(void));

private:
	
	Events* m_events = NULL;
	MaterialManager* m_materialManager = NULL;
	MeshManager* m_meshManager = NULL;
	CameraManager* m_cameraManager = NULL;
	LightManager* m_lightManager = NULL;
	ZobObjectManager* m_zobObjectManager = NULL;
	ZobPhysicsEngine* m_physicsEngine = NULL;
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
	float m_copyTime;
	float m_physicTime;
	clock_t m_copyTick;
	clock_t	m_frameTick;
	static int s_logIndent;
	bool m_physicStarted;

	//

};
