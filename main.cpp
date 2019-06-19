#include "Engine.h"
#include "Cube.h"
#include "Texture.h"
#include "TextureTestAlpha.h"
#include "Mesh.h"

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

	Texture* tex = new Texture("D:/_PERSO/directZob/resources/landscape.png");

	//Mesh* mesh = new Mesh("D:/_PERSO/directZob/resources/earth.obj", tex);
	Mesh* mesh = new Mesh("D:/_PERSO/directZob/resources/man.obj", tex);
	//Mesh* mesh = new Mesh("D:/_GIT/directZob/resources/LowPolyFiatUNO.obj", tex);

	for (;;)
	{
		frame++;
		m_engine->ClearBuffer(&Color::Grey);

		static float fov = 90.0f;
		m_engine->GetCamera()->setProjectionMatrix(fov, 800, 600, 0.01f, 1000.0f);

		m_engine->GetCamera()->SetPosition(&Vector3(0, 0, -10));

		m.Identity();
		m.SetTranslation(5, 5);
		//frameCharBuffer[0] = '\0';
		//_ultoa_s((long)m_engine->GetFps(), frameCharBuffer, 10);
		//m_engine->DrawString(&m, frameCharBuffer, Color::White.GetRawValue(), m_engine->GetBufferData());

		
		//rot = 45.0f;
		//rot = 47.00;
		m.Identity();
		m.SetTranslation(-300, -300);
		m.SetScale(64, 64);
		//m.SetRotation(rot);
		//m_engine->DrawTexture(&m, testTest, m_engine->GetBufferData());

		m_engine->DrawLine(0, 300, 800, 300, Color::White.GetRawValue(), m_engine->GetBufferData());
		m_engine->DrawLine(400, 0, 400, 600, Color::White.GetRawValue(), m_engine->GetBufferData());

		m.Identity(); 
		m.SetTranslation(500, 500);
		//m_engine->DrawChar(&m, 'A', Color::White.GetRawValue(), m_engine->GetBufferData());

		m.Identity();
		m.SetTranslation(100, 500);
		m.SetScale(1, 1);
		m.SetRotation(rot);
		//m_engine->DrawString(&m, "123\n456", Color::Magenta.GetRawValue(), m_engine->GetBufferData());
		//m_engine->DrawCircle((float)x, (float)y, 50, Color::White.GetRawValue(), m_engine->GetBufferData());
		
		for (int i = 0; i < 50; i++)
		{
			m.Identity();
			m.SetTranslation((float)(i*64), 300);
			m.SetScale(1, 1);
			m.SetShear(0.5, 0);
			m.SetRotation(rot);
			//m_engine->DrawTexture(&m, testTest, m_engine->GetBufferData());
		}
		int starty = -6;
		/*
		for (int j = starty; j <= -starty; j += 4)
		{
			int startx = -8;
			for (int i = startx; i <= -startx; i += 4)
			{
				Cube c = Cube(testTest);
				c.Init();
				c.SetSCale(0.1f, 0.1f, 0.1f);
				c.SetTranslation(i, j, 0);
				c.SetRotation(rot, rot, rot);
				//c.SetRotation(45, 45, 45);
				c.Draw(m_engine);
			}
		}
		*/
		rot += 1.5f;

		mesh->Init();
		static float tx = 0;
		static float ty = 0;
		static float tz = 0;
		mesh->SetTranslation(tx, ty, tz);
		static float scale = 1.0f;// 0.5f;
		//mesh->SetSCale(scale, scale, scale);
		//mesh->SetRotation(0, rot, 180);
		mesh->Draw(m_engine);

		state = m_engine->Update();

		if (state < 0)
			break;
	}

	mfb_close();

	return 0;
}