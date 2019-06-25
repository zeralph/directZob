#include <windows.h>
#include <string>
#include <iostream>
#include "Engine.h"
#include "Cube.h"
#include "Texture.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"

#define kUnused(var) (void) var;
#define WIDTH  800
#define HEIGHT 600

static char buffer[MAX_PATH];

Engine* m_engine = NULL;
static Vector3 camPos = Vector3(2, 1, -4);
static Vector3 camTarget = Vector3(00, 00, 00);
static Vector3 up = Vector3(0, 01, 0);

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
	m_engine->Resize(width, height);
	mfb_set_viewport(window, x, y, width, height);
}

void keyboard(struct Window *window, Key key, KeyMod mod, bool isPressed) {
	const char *window_title = "";
	if (window) {
		window_title = (const char *)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > keyboard: key: %s (pressed: %d) [KeyMod: %x]\n", window_title, mfb_get_key_name(key), isPressed, mod);
	if (key == KB_KEY_ESCAPE) {
		mfb_close(window);
	}
	else if (key == KB_KEY_DOWN)
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
	else if (key == KB_KEY_KP_SUBTRACT)
	{
		camPos.x = camPos.x - 0.1f;
	}
	else if (key == KB_KEY_KP_ADD)
	{
		camPos.x = camPos.x + 0.1f;
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

void mouse_move(struct Window *window, int x, int y) {
	kUnused(window);
	kUnused(x);
	kUnused(y);
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

	Matrix2x2 m;
	int state;
	Mesh* mesh = NULL;
	std::string path = ExePath();
	std::string file = path + "\\..\\..\\resources\\landscape.png";
	Texture* tex = new Texture(file.c_str());
	file = path + "\\..\\..\\resources\\man.obj";
	mesh = new Mesh(file.c_str(), tex);

	for (;;)
	{
		frame++;
		m_engine->ClearBuffer(&Color::Grey);

		static float fov = 45.0f;
		m_engine->GetCamera()->setProjectionMatrix(fov, m_engine->Width(), m_engine->Height(), 0.01f, 1000.0f);
		m_engine->GetCamera()->SetLookAt(&camPos, &camTarget, &up);
		m_engine->DrawGrid();

		

		if (mesh)
		{
			rot += 1.5f;
			mesh->Init();
			static float tx = 0;
			static float ty = 0;
			static float tz = 0;
			mesh->SetTranslation(tx, ty, tz);
			static float scale = 1.0f / 15.0f;// 0.5f;
			mesh->SetSCale(scale, scale, scale);
			mesh->SetRotation(0, rot, 0);
			mesh->Draw(m_engine);
		}
		state = m_engine->Update(window);

		if (state < 0)
			break;
	}

	mfb_close(window);

	return 0;
}

