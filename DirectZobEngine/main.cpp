//#define LINUX 

#include "DirectZob.h"



DirectZob m_directZob;
static int m_mouseLastX;
static int m_mouseLastY;

void active(struct Window* window, bool isActive) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > active: %d\n", window_title, isActive);
}

void resize(struct Window* window, int width, int height) {
	uint32_t x = 0;
	uint32_t y = 0;
	//m_engine->Stop();
	//m_engine->Resize(width, height);
	mfb_set_viewport(window, x, y, width, height);
	//m_engine->Start();
}

void keyboard(struct Window* window, Key key, KeyMod mod, bool isPressed) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	//fprintf(stdout, "%s > keyboard: key: %s (pressed: %d) [KeyMod: %x]\n", window_title, mfb_get_key_name(key), isPressed, mod);
	if (isPressed == false)
	{
		if (key == KB_KEY_ESCAPE) {
			mfb_close(window);
		}
		else if (key == KB_KEY_F1)
		{
			//m_curCam = m_FPSCam;
		}
		else if (key == KB_KEY_F2)
		{
			//m_curCam = m_freeCam;
		}
		else if (key == KB_KEY_C)
		{
			//m_engine->ToggleZbufferOutput();
		}
		else if (key == KB_KEY_S)
		{
			//g_bShowMeshes = !g_bShowMeshes;
		}
		else if (key == KB_KEY_G)
		{
			//g_bShowGrid = !g_bShowGrid;
		}
		//m_curCam->OnKeyboardInput(key, isPressed);
	}
}

void char_input(struct Window* window, unsigned int charCode) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > charCode: %d\n", window_title, charCode);
}

void mouse_btn(struct Window* window, MouseButton button, KeyMod mod, bool isPressed) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > mouse_btn: button: %d (pressed: %d) [KeyMod: %x]\n", window_title, button, isPressed, mod);
	//m_curCam->OnMouseButton(button, isPressed);
}

void mouse_move(struct Window* window, int x, int y)
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
	//camRot.x += -dy * 0.1f;
	//camRot.y += dx * 0.1f;
	//camRot.y = CLAMP(camRot.y, -90, 90);
	m_mouseLastX = x;
	m_mouseLastY = y;
	//m_curCam->OnMouseMove(x, y);
}

void mouse_scroll(struct Window* window, KeyMod mod, float deltaX, float deltaY) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > mouse_scroll: x: %f, y: %f [KeyMod: %x]\n", window_title, deltaX, deltaY, mod);
	//m_curCam->OnMouseScroll(deltaY);
}

int main()
{
	m_mouseLastX = -1;
	m_mouseLastY = -1;
	std::cout << "Init Window";
	struct Window* window = mfb_open_ex("DirectZob", WIDTH, HEIGHT, WF_RESIZABLE);
	mfb_active_callback(window, active);
	mfb_resize_callback(window, resize);
	mfb_keyboard_callback(window, keyboard);
	mfb_char_input_callback(window, char_input);
	mfb_mouse_button_callback(window, mouse_btn);
	mfb_mouse_move_callback(window, mouse_move);
	mfb_mouse_scroll_callback(window, mouse_scroll);

	m_directZob.Init();

	for (;;)
	{
		m_directZob.RunAFrame();
		int state = mfb_update(window, (void*)m_directZob.GetBufferData() );
		if (state < 0)
		{
			break;
		}
	}
	mfb_close(window);

	return 0;
}

