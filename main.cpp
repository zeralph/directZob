#include "Engine.h"
#include "Cube.h"
#include "TextureTest.h"
#include "TextureTestAlpha.h"

int main()
{
	Engine* m_engine = new Engine(600, 600);
	int x = m_engine->Width() / 2;
	int y = m_engine->Height() / 2;
	int dx = 1;
	int dy = 1;
	float r = 0.0f;
	long frame = 0;
	float rot = 0.0;
	char frameCharBuffer[sizeof(ulong)];
	const TextureTestAlpha* testTest = new TextureTestAlpha();
	Matrix2x2 m;
	Cube cube = Cube(testTest);
	int state;
	for (;;)
	{
		frame++;
		m_engine->ClearBuffer(&Color::Black);
		m.Identity();
		m.SetTranslation(5, 5);
		//frameCharBuffer[0] = '\0';
		//_ultoa_s((long)m_engine->GetFps(), frameCharBuffer, 10);
		//m_engine->DrawString(&m, frameCharBuffer, Color::White.GetRawValue(), m_engine->GetBufferData());

		rot += 0.1f;
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
		cube.Init();
		cube.SetSCale(0.5f, 0.5f, 1);
		cube.SetRotation(0, 0, rot);
		cube.Draw(m_engine);


		state = m_engine->Update();

		if (state < 0)
			break;
	}

	mfb_close();

	return 0;
}