#pragma once

#ifdef LINUX
#include <unistd.h>
#else
#include <windows.h>
#endif //LINUX
#include <string>
#include <iostream>
#include <algorithm>
#include "Engine.h"
#include "Cube.h"
#include "Texture.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"
#include "text2D.h"
#include "Events.h"
#include "CameraManager.h"
#include "TextureManager.h"


#define kUnused(var) (void) var;
#define WIDTH  1920 / 2
#define HEIGHT 1080 / 2
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
	int RunAFrame();
	const uint* GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	std::string ExePath();

	static DirectZob* GetInstance() { return DirectZob::singleton; }
	Engine* GetEngine() const { return m_engine; }
	CameraManager* GetCameraManager() const { return m_cameraManager; }
	Events* GetEventManager() { return m_events; }
	static void Log(std::string& str);
	static void Log(const char* str);
	
private:

	Events* m_events = NULL;
	TextureManager* m_textureManager = NULL;
	CameraManager* m_cameraManager = NULL;
	Engine* m_engine = NULL;
	Text2D* m_text = NULL;

	static bool g_bShowGrid;
	static bool g_bShowMeshes;
};
