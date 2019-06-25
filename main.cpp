#include <windows.h>
#include <string>
#include <iostream>
#include "Engine.h"
#include "Cube.h"
#include "Texture.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"

static char buffer[MAX_PATH];

std::string ExePath() {
	char b[MAX_PATH];
	GetModuleFileName(NULL, b, MAX_PATH);
	std::string::size_type pos = std::string(b).find_last_of("\\/");
	return std::string(b).substr(0, pos);
}

int main()
{
	std::cout << "Init engine";
	Engine* m_engine = new Engine(800, 600);
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
		static Vector3 from = Vector3(2, 1, -4);
		static Vector3 to = Vector3(00, 00, 00);
		static Vector3 up = Vector3(0, 01, 0);
		m_engine->GetCamera()->setProjectionMatrix(fov, 800, 600, 0.01f, 1000.0f);
		m_engine->GetCamera()->SetLookAt(&from, &to, &up);
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
		state = m_engine->Update();

		if (state < 0)
			break;
	}

	mfb_close();

	return 0;
}

