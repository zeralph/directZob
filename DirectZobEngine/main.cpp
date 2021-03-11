//#define LINUX 

#define OPTIK_PROFILING 
#undef None
#include "../dependencies/optick/include/optick.h"

#include "DirectZob.h"
#include <fstream>

DirectZob m_directZob;
static int m_mouseLastX;
static int m_mouseLastY;
bool bStartPhysics = false;
bool bPause = false;
std::string m_path;
std::string m_file;
struct mfb_window* m_window;

void active(struct mfb_window* window, bool isActive) {
	const char* window_title = "";
	if (window) {
		window_title = (const char*)mfb_get_user_data(window);
	}
	fprintf(stdout, "%s > active: %d\n", window_title, isActive);
}

void resize(struct mfb_window* window, int width, int height) {
	uint32_t x = 0;
	uint32_t y = 0;
	//m_engine->Stop();
	//m_engine->Resize(width, height);
	mfb_set_viewport(window, x, y, width, height);
	//m_engine->Start();
}

int UpdateImageToScreen()
{
	OPTICK_EVENT()
	return mfb_update(m_window, (void*)m_directZob.GetBufferData());
}

int main(int argc, char* argv[])
{
	printf("Go\n");
	bool bBench = false;
	bool btest = false;
	std::string scenePath = "";
	int width = 640;
	int height = 480;
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " --scene <scene to load>" << std::endl;
		std::cerr << "Additional options : " << std::endl;
		std::cerr << "\t--bench  : run a benchmark" << std::endl;
		std::cerr << "\t--width  : width" << std::endl;
		std::cerr << "\t--height : height" << std::endl;
		printf("enter --scene argument");
		return 0;
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
				printf("--scene option requires one argument.");
				return 0;
			}
		}
		else if (std::string(argv[i]) == "--test")
		{
			btest = true;
		}
		else if(std::string(argv[i]) == "--bench")
		{
			bBench = true;
		}
		else if (std::string(argv[i]) == "--width")
		{
			if (i + 1 < argc)
			{
				int w = strtol(argv[i + 1], NULL, 10);
				if (w >= 0 && w <= 4096)
				{
					width = w;
				}
				else
				{
					std::cout << "width must be between 1 and 4096 px." << std::endl;
					printf("width must be between 1 and 4096 px.");
				}
				i++;
			}
			else
			{
				std::cout << "--width option requires one argument." << std::endl;
				printf("--width option requires one argument.");
				return 0;
			}
		}
		else if (std::string(argv[i]) == "--height")
		{
			if (i + 1 < argc)
			{
				int h = strtol(argv[i + 1], NULL, 10);
				if (h >= 0 && h <= 4096)
				{
					height = h;
				}
				else
				{
					std::cout << "height must be between 1 and 4096 px." << std::endl;
					printf("height must be between 1 and 4096 px.");
				}
				i++;
			}
			else
			{
				std::cout << "--height option requires one argument." << std::endl;
				printf("--height option requires one argument.");
				return 0;
			}
		}
		i++;
	}

	std::ifstream f(scenePath.c_str());
	if (!f.good())
	{
		std::cout << "Cannot open " << scenePath << " : File not found or not accessible."<< std::endl;
		printf("Cannot open %s : File not found or not accessible.\n", scenePath.c_str());
		return 0;
	}
#ifdef WINDOWS
	std::size_t found = scenePath.rfind('\\');
#elif LINUX
	std::size_t found = scenePath.rfind('/');
#elif MACOS
	std::size_t found = scenePath.rfind('/');
#endif	
	if (found == std::string::npos)
	{
		std::cerr << "cannot parse path to file " << scenePath << std::endl;
		printf("cannot parse path to file %s\n", scenePath.c_str());
		return 0;
	}
	m_path = scenePath.substr(0,found+1);
	m_file = scenePath.substr(found + 1, scenePath.length() - found - 1);
	
	m_mouseLastX = -1;
	m_mouseLastY = -1;
	printf("Init Window %ix%i\n", width, height);
	mfb_set_target_fps(60);
	m_window = mfb_open_ex("DirectZob", width, height, WF_RESIZABLE);
	/*
	mfb_set_active_callback(m_window, active);
	mfb_set_resize_callback(m_window, resize);
	mfb_set_keyboard_callback(m_window, keyboard);
	mfb_set_char_input_callback(m_window, char_input);
	mfb_set_mouse_button_callback(m_window, mouse_btn);
	mfb_set_mouse_move_callback(m_window, mouse_move);
	mfb_set_mouse_scroll_callback(m_window, mouse_scroll);
	*/
	m_directZob.Init(width, height, false);
	m_directZob.LoadScene(m_path, m_file);

	float rot = 0.0f;
	float to = 0.5f;
	float zoom = 1.0f;
    printf("Start rendering\n");
	float benchFps = 0.0f;
	float benchRender = 0.0f;
	float benchGeom = 0.0f;
	float benchTot = 0.0f;
	float benchCpy = 0.0f;
	ulong frames = 0;

	m_directZob.GetEngine()->ShowBBoxes(false);
	m_directZob.GetEngine()->ShowNormals(false);
	m_directZob.GetEngine()->ShowGrid(false);
	m_directZob.GetEngine()->SetRenderMode(eRenderMode::eRenderMode_fullframe);
	m_directZob.GetEngine()->DrawGizmos(true);

	if (true)
	{
		m_directZob.GetEngine()->ShowBBoxes(false);
		m_directZob.GetEngine()->ShowGrid(false);
		m_directZob.GetEngine()->SetRenderMode(eRenderMode::eRenderMode_fullframe);
		m_directZob.GetEngine()->DrawGizmos(true);
	}

	//m_directZob.GetEngine()->GetBufferData()->zFar = 70.0f;
	//m_directZob.GetZobObjectManager()->GetZobObject("fbx_example")->SetRotation(0, 90, 0);
	//Camera* c = m_directZob.GetCameraManager()->GetCurrentCamera();
	//ZobVector3 camPos = c?c->GetWorldPosition(): ZobVector3();
	//ZobVector3 initialCamPos = camPos;
	//m_directZob.StartPhysic();
	int bTestFrame = 0;
	bStartPhysics = true;
	OPTICK_THREAD("main thread");
	for (;;)
	{
#ifdef OPTIK_PROFILING
		OPTICK_FRAME("MainThread");
#endif
		if (bStartPhysics && !m_directZob.IsPhysicPlaying())
		{
			DirectZob::GetInstance()->GetZobObjectManager()->SaveTransforms();
			m_directZob.StartPhysic();
		}
		else if(!bStartPhysics && m_directZob.IsPhysicPlaying())
		{
			m_directZob.StopPhysic(true);
			DirectZob::GetInstance()->GetZobObjectManager()->RestoreTransforms();
		}
		if (bBench)
		{
			//if (m_directZob.GetCameraManager()->GetCurrentCamera())
			//{
			//	m_directZob.GetCameraManager()->GetCurrentCamera()->SetWorldPosition(camPos.x, camPos.y, camPos.z);
			//}
			Light* red = m_directZob.GetLightManager()->GetLight("red");
			Light* blue = m_directZob.GetLightManager()->GetLight("blue");
			Light* green = m_directZob.GetLightManager()->GetLight("green");

			if (!bPause)
			{
			}
		}
		if (!bPause)
		{
			rot += 1.0f;
			if (btest && bTestFrame >= 200)
			{
				break;
			}
			bTestFrame++;
//			c->RotateAroundPointAxis(&ZobVector3::Vector3Zero, &ZobVector3::Vector3Y, &ZobVector3::Vector3Y, rot, false);
		}
		m_directZob.RunAFrame(m_window);
		benchFps += m_directZob.GetFps();
		benchRender += m_directZob.GetRenderTime();
		benchGeom += m_directZob.GetGeometryTime();
		benchTot += m_directZob.GetFrameTime();
		benchCpy += m_directZob.GetCopyTime();
		frames++;
		int state = UpdateImageToScreen();
		if (state < 0)
		{
			break;
		}
		
	}
    printf("Closing\n");
    m_directZob.NewScene();
    printf("Exiting\n");
	mfb_close(m_window);
	if (bBench)
	{
		int fps = (int)(benchFps / (float)frames);
		int r = (int)(benchRender / (float)frames);
		int g = (int)(benchGeom / (float)frames);
		int t = (int)(benchTot / (float)frames);
		int c = (int)(benchCpy / (float)frames);
		std::cout << "\n\t\tBenchmark:\nRender\tGeom\tCpy\tFrame\tFps\n" << r <<"\t"<<g<<"\t"<<c<<"\t"<<t<<"\t"<<fps<< std::endl;
	}
#ifdef OPTIK_PROFILING
	OPTICK_SHUTDOWN();
#endif
	m_directZob.Shutdown();
	return 0;
}

