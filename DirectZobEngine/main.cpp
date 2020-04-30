//#define LINUX 
#include "DirectZob.h"
#include <fstream>

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

int main(int argc, char* argv[])
{
	bool bBench = false;
	std::string scenePath = "";
	int width = 640;
	int height = 480;
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " --scene <scene to load>" << std::endl;
		std::cerr << "Additional options : " << std::endl;
		std::cerr << "\t--bench : run a becnhmark" << std::endl;
		return 1;
	}
	int i = 0;
	while( i < argc) 
	{
		if (std::string(argv[i]) == "--scene") 
		{
			if (i + 1 < argc && argv[i+1][0] != '-')
			{
				i++;
				scenePath = argv[i];
			}
			else 
			{ 
				std::cerr << "--scene option requires one argument." << std::endl;
				return 1;
			}
		}
		else if(std::string(argv[i]) == "--bench")
		{
			bBench = true;
		}
		i++;
	}

	std::ifstream f(scenePath.c_str());
	if (!f.good())
	{
		std::cerr << "Cannot open " << scenePath << " : File not found or not accessible."<< std::endl;
		return 1;
	}

	std::size_t found = scenePath.rfind('\\');
	if (found == std::string::npos)
	{
		std::cerr << "cannot parse path to file " << scenePath << std::endl;
		return 1;
	}
	std::string path = scenePath.substr(0,found+1);
	std::string file = scenePath.substr(found + 1, scenePath.length() - found - 1);

	m_mouseLastX = -1;
	m_mouseLastY = -1;
	printf("Init Window\n");
	struct Window* window = mfb_open_ex("DirectZob", width, height, WF_RESIZABLE);
	mfb_active_callback(window, active);
	mfb_resize_callback(window, resize);
	mfb_keyboard_callback(window, keyboard);
	mfb_char_input_callback(window, char_input);
	mfb_mouse_button_callback(window, mouse_btn);
	mfb_mouse_move_callback(window, mouse_move);
	mfb_mouse_scroll_callback(window, mouse_scroll);

	m_directZob.Init(width, height, false);

	m_directZob.LoadScene(path, file);
	float rot = 0.0f;
	float to = 0.5f;
    printf("Start rendering\n");
	Vector3 camPos = Vector3(0, 1, 0);
	Vector3 camTo = Vector3(0, 1, 1);
	Vector3 camUp = Vector3(0, 1, 0);
	float benchFps = 0.0f;
	float benchRender = 0.0f;
	float benchGeom = 0.0f;
	ulong frames = 0;
	for (;;)
	{
		if (bBench)
		{
			m_directZob.GetCameraManager()->GetCurrentCamera()->SetLookAt(&camPos, &camTo, &camUp);
		}
		else
		{
			m_directZob.GetZobObjectManager()->GetRootObject()->SetRotation(0, rot, 0);
		}
		camPos.z += to;
		camTo.z += to;
		if (camPos.z >= 70.0f)
		{
			camPos.z = 0.0f;
			camTo.z = 1.0f;
			if (bBench)
			{
//				break;
			}
		}
		m_directZob.RunAFrame();
		benchFps += m_directZob.GetEngine()->GetFps();
		benchRender += m_directZob.GetEngine()->GetRenderTime();
		benchGeom += m_directZob.GetEngine()->GetGeometryTime();
		frames++;
		int state = mfb_update(window, (void*)m_directZob.GetBufferData() );
		if (state < 0)
		{
			break;
		}
		rot += 1.0f;
	}
    printf("Closing\n");
    m_directZob.NewScene();
    printf("Exiting\n");
	mfb_close(window);
	if (bBench)
	{
		float f = benchFps / (float)frames;
		float r = benchRender / (float)frames;
		float g = benchGeom / (float)frames;
		std::cout << "\n\t\tBenchmark:\nRender\tGeom\tFps\n" << r <<"\t"<<g<<"\t"<<f<< std::endl;
	}
	return 0;
}

