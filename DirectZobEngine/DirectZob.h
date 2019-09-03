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
	int RunAFrame();
	Engine* GetEngine() { return m_engine; }
	const uint* GetBufferData() const { return m_engine->GetBufferData()->buffer; }
	std::string ExePath();

	std::vector<std::string>* GetEvents()
	{
		return m_events->GetEvents();
	}
	void ClearEvents()
	{
		return m_events->ClearEvents();
	}

	static DirectZob* GetInstance() { return DirectZob::singleton; }
	Engine* GetEngine() const { return m_engine; }

private:

	Events* m_events = NULL;
	Engine* m_engine = NULL;
	Camera* m_freeCam = NULL;
	Camera* m_FPSCam = NULL;
	Camera* m_curCam = NULL;
	Text2D* m_text = NULL;

	static bool g_bShowGrid;
	static bool g_bShowMeshes;
};
