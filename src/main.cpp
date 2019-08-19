#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "Engine.h"
#include "Cube.h"
#include "Texture.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"
#include "text2D.h"

#define kUnused(var) (void) var;
#define WIDTH  800
#define HEIGHT 600
#define TARGET_MS_PER_FRAME 0//33.33f / 2.0f
#define CLAMP(n, low, max) n <= low ? low : n >= max ? max : n;

static char buffer[MAX_PATH];

Engine* m_engine = NULL;
Camera* m_lookAtCam = NULL;
Camera* m_FPSCam = NULL;
Camera* m_curCam = NULL;

static Vector3 camRot = Vector3(0, 0, 0);
static Vector3 camPos = Vector3(0, -1.0f, -10);
static Vector3 camTarget = Vector3(0, 0.1f, 1.0f);
static Vector3 up = Vector3(0, 1.0f, 0);

static int m_mouseLastX;
static int m_mouseLastY;
static bool g_bShowGrid = true;
static bool g_bShowMeshes = true;

std::string ExePath() {
	char b[MAX_PATH];
	GetModuleFileName(NULL, b, MAX_PATH);
	std::string::size_type pos = std::string(b).find_last_of("\\/");
	return std::string(b).substr(0, pos);
}

void active(struct Window *window, bool isActive) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > active: %d\n", window_title, isActive);
}

void resize(struct Window *window, int width, int height) {
	uint32_t x = 0;
	uint32_t y = 0;
	//m_engine->Resize(width, height);
	//mfb_set_viewport(window, x, y, width, height);
}

void keyboard(struct Window *window, Key key, KeyMod mod, bool isPressed) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	//fprintf(stdout, "%s > keyboard: key: %s (pressed: %d) [KeyMod: %x]\n", window_title, mfb_get_key_name(key), isPressed, mod);
	if (isPressed == false)
	{
		if (key == KB_KEY_ESCAPE) {
			mfb_close(window);
		}
		else if (key == KB_KEY_F1)
		{
			m_curCam = m_FPSCam;
		}
		else if (key == KB_KEY_F2)
		{
			m_curCam = m_lookAtCam;
		}
		else if (key == KB_KEY_C)
		{
			m_engine->ToggleZbufferOutput();
		}
		else if (key == KB_KEY_S)
		{
			g_bShowMeshes = !g_bShowMeshes;
		}
		else if (key == KB_KEY_G)
		{
			g_bShowGrid = !g_bShowGrid;
		}
	}
	if (key == KB_KEY_DOWN)
	{
		camPos.z = camPos.z - 0.1f;
	}
	else if (key == KB_KEY_UP)
	{
		camPos.z = camPos.z + 0.1f;
	}
	else if (key == KB_KEY_LEFT)
	{
		camPos.x = camPos.x - 0.1f;
	}
	else if (key == KB_KEY_RIGHT)
	{
		camPos.x = camPos.x + 0.1f;
	}
	else if (key == KB_KEY_PAGE_DOWN)
	{
		camPos.y = camPos.y - 0.1f;
	}
	else if (key == KB_KEY_PAGE_UP)
	{
		camPos.y = camPos.y + 0.1f;
	}
}

void char_input(struct Window *window, unsigned int charCode) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > charCode: %d\n", window_title, charCode);
}

void mouse_btn(struct Window *window, MouseButton button, KeyMod mod, bool isPressed) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > mouse_btn: button: %d (pressed: %d) [KeyMod: %x]\n", window_title, button, isPressed, mod);
}

void mouse_move(struct Window *window, int x, int y) 
{
	kUnused(window);
	if (m_mouseLastX < 0)
	{
		m_mouseLastX = x;
	}
	if (m_mouseLastY < 0)
	{
		m_mouseLastY = y;
	}
	int dx = m_mouseLastX - x;
	int dy = m_mouseLastY - y;
	//fprintf(stdout, "mouse %i %i", dx, dy);
	camRot.x += -dy * 0.1f;
	camRot.y += dx * 0.1f;
	camRot.y = CLAMP(camRot.y, -90, 90);
	m_mouseLastX = x;
	m_mouseLastY = y;
}

void mouse_scroll(struct Window *window, KeyMod mod, float deltaX, float deltaY) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > mouse_scroll: x: %f, y: %f [KeyMod: %x]\n", window_title, deltaX, deltaY, mod);
}

int main()
{
	std::cout << "Init Window";
	struct Window *window = mfb_open_ex("Noise Test", WIDTH, HEIGHT, WF_RESIZABLE);
	mfb_active_callback(window, active);
	mfb_resize_callback(window, resize);
	mfb_keyboard_callback(window, keyboard);
	mfb_char_input_callback(window, char_input);
	mfb_mouse_button_callback(window, mouse_btn);
	mfb_mouse_move_callback(window, mouse_move);
	mfb_mouse_scroll_callback(window, mouse_scroll);
	std::cout << " OK\n";
	std::cout << "Init engine";
	m_engine = new Engine(WIDTH, HEIGHT);
	std::cout << " OK\n";
	int x = m_engine->Width() / 2;
	int y = m_engine->Height() / 2;
	int dx = 1;
	int dy = 1;
	float r = 0.0f;
	long frame = 0;
	float rot = 0.0;
	char frameCharBuffer[sizeof(ulong)];
	m_mouseLastX = -1;
	m_mouseLastY = -1;
	Matrix2x2 m;
	int state;
	Mesh* mesh = NULL;
	Mesh* mesh2 = NULL;
	Mesh* mesh3 = NULL;
	Mesh* mesh4 = NULL;
	Text2D* text = NULL;
	std::string path = ExePath();
	std::string file = path + "\\..\\..\\resources\\cottage_diffuse.png";
	Texture* tex = new Texture(file.c_str());
	file = path + "\\..\\..\\resources\\font2.png";
	Texture* fontTex = new Texture(file.c_str());
	file = path + "\\..\\..\\resources\\cottage_obj.obj";
	mesh = new Mesh(file.c_str(), tex);
	file = path + "\\..\\..\\resources\\camaro.obj";
	mesh2 = new Mesh(file.c_str(), tex);
	file = path + "\\..\\..\\resources\\man.obj";
	mesh3 = new Mesh(file.c_str(), tex);
	file = path + "\\..\\..\\resources\\sphere.obj";
	mesh4 = new Mesh(file.c_str(), tex);
	m_engine->Add(mesh);
	m_engine->Add(mesh2);
	m_engine->Add(mesh3);
	m_engine->Add(mesh4);
	text = new Text2D(m_engine, fontTex, 32, 8);

	//m_lookAtCam = new Camera();
	m_FPSCam = new Camera();

	m_curCam = m_FPSCam;

	for (;;)
	{
		frame++;
		m_engine->ClearBuffer(&Color::Grey);

		static float fov = 45.0f;
		//m_lookAtCam->setProjectionMatrix(fov, m_engine->Width(), m_engine->Height(), 0.01f, 1000.0f);
		//m_lookAtCam->InitView();
		//m_lookAtCam->SetLookAt(&camPos, &camTarget, &up);

		m_FPSCam->setProjectionMatrix(fov, m_engine->Width(), m_engine->Height(), 0.01f, 1000.0f);
		m_FPSCam->InitView();
		camTarget = camPos;
		camTarget.z = camTarget.z + 1.0f;
		m_FPSCam->SetLookAt(&camPos, &camTarget, &up);
		//m_FPSCam->SetPosition(&camPos);
		//FPSCam->SetRotation(&camRot);

		if (g_bShowGrid)
		{
			m_engine->DrawGrid(m_curCam);
		}
		static float tx = 0;
		static float ty = 0;
		static float tz = 0;
		static float scale = 0.25f;// 1.0f / 15.0f;// 0.5f;
		rot += 0.5f;
		if (g_bShowMeshes)
		{
			if (mesh)
			{
				mesh->Init();
				mesh->SetTranslation(0,0,-10);
				mesh->SetSCale(scale, scale, scale);
				mesh->SetRotation(0, rot, 0);
				//mesh->Draw(m_curCam, m_engine);
			}
			if (mesh2)
			{
				mesh2->Init();
				mesh2->SetTranslation(0, 0, 0);
				mesh2->SetSCale(scale, scale, scale);
				mesh2->SetRotation(0, rot+10, 0);
				//mesh2->Draw(m_curCam, m_engine);
			}
			if (mesh3)
			{
				mesh3->Init();
				mesh3->SetTranslation(-5, -1, -1);
				mesh3->SetSCale(scale, scale, scale);
				mesh3->SetRotation(rot-10, rot + 20, rot);
				//mesh3->Draw(m_curCam, m_engine);
			}
			if (mesh4)
			{
				mesh4->Init();
				mesh4->SetTranslation(5, 5, -2);
				mesh4->SetSCale(scale, scale, scale);
				mesh4->SetRotation(rot, rot + 30, rot);
				//mesh4->Draw(m_curCam, m_engine);
			}
		}
		snprintf(buffer, MAX_PATH, "Triangles : %lu / %lu", m_engine->GetNbDrawnTriangles(), m_engine->GetNbTriangles());
		text->Print(0, 0, 1, &std::string(buffer), 0xFFFFFFFF);

		const Vector3* cp = m_curCam->GetPosition();
		snprintf(buffer, MAX_PATH, "Cam pos : %.2f, %.2f, %.2f", cp->x, cp->y, cp->z);
		text->Print(0, 8, 1, &std::string(buffer), 0xFFFFFFFF);

		snprintf(buffer, MAX_PATH, "Cam rot : %.2f, %.2f, %.2f", camRot.x, camRot.y, camRot.z);
		text->Print(0, 24, 1, &std::string(buffer), 0xFFFFFFFF);

		snprintf(buffer, MAX_PATH, "Nb pixels : %i", m_engine->GetNbPixels());
		text->Print(0, 32, 1, &std::string(buffer), 0xFFFFFFFF);
		

		snprintf(buffer, MAX_PATH, "FPS : %.2f, render : %.2fms, geom : %.2f", m_engine->GetFps(), m_engine->GetRenderTime(), m_engine->GetGeometryTime());
		float t = m_engine->GetFps();
		t = (1.0f / t) * 1000.0f;
		if (t < TARGET_MS_PER_FRAME)
		{
			text->Print(0, 16, 1, &std::string(buffer), 0xFF00FF00);
			Sleep(TARGET_MS_PER_FRAME - t);
		}
		else
		{
			text->Print(0, 16, 1, &std::string(buffer), 0xFFFF0000);
		}
		state = m_engine->Update(window, m_curCam);
		if (state < 0)
			break;
	}

	mfb_close(window);

	return 0;
}

