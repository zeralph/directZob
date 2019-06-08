#include "Engine.h"
#include "TextureTest.h"
#include "TextureTestAlpha.h"
int main()
{
	Engine* m_engine = new Engine(800, 600);
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
	int state;
	for (;;)
	{
		frame++;
		m_engine->ClearBuffer(&Color::Black);

		rot += 0.8f;
		//rot = 47.00;
		m.Identity();
		m.SetTranslation(-300, -300);
		m.SetScale(64, 64);
		//m.SetRotation(rot);
		m_engine->DrawTexture(&m, testTest, m_engine->GetBufferData());

		for (int i = 100; i < 250; i += 8)
		{
			m_engine->DrawLine(0, (float)i, 800, (float)i, Color::Red.GetRawValue(), m_engine->GetBufferData());
			m_engine->DrawLine((float)i, 0, (float)i, 600, Color::Red.GetRawValue(), m_engine->GetBufferData());
		}

		m.Identity(); 
		m.SetTranslation(500, 500);
		m_engine->DrawChar(&m, 'A', Color::White.GetRawValue(), m_engine->GetBufferData());

		m.Identity();
		m.SetTranslation(100, 500);
		m.SetScale(1, 1);
		m.SetRotation(rot);
		m_engine->DrawString(&m, "123\n456", Color::Magenta.GetRawValue(), m_engine->GetBufferData());
		m_engine->DrawCircle((float)x, (float)y, 50, Color::White.GetRawValue(), m_engine->GetBufferData());
		
		m.Identity();
		m.SetTranslation(5, 5);
		frameCharBuffer[0] = '\0';
		_ultoa_s((long)m_engine->GetFps(), frameCharBuffer, 10);
		m_engine->DrawString(&m, frameCharBuffer, Color::White.GetRawValue(), m_engine->GetBufferData());


		for (int i = 0; i < 50; i++)
		{
			m.Identity();
			m.SetTranslation((float)(i*64), 300);
			m.SetScale(1, 1);
			m.SetShear(0.5, 0);
			m.SetRotation(rot);
			m_engine->DrawTexture(&m, testTest, m_engine->GetBufferData());
		}




		state = m_engine->Update();

		if (state < 0)
			break;
	}

	mfb_close();

	return 0;
}