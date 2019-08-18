#include "Engine.h"
#include <algorithm>
#include "TextureTest.h"


Engine::Engine(int width, int height)
{
	m_currentFrame = 0;
	m_width = width;
	m_height = height;
	m_zNear = 0.01f;
	m_zFar = 1000.0f;
	m_buffer = (uint*)malloc(sizeof(uint) * m_width * m_height);
	m_zBuffer = (float*)malloc(sizeof(float) * m_width * m_height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_fps = 0.0;
	m_bufferData.height = m_height;
	m_bufferData.width = m_width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.size = m_width * m_height;
	m_tick = clock();
	m_nbPixels = 0;

	m_rasterQueue1.clear();
	m_rasterQueue2.clear();

	m_rasterizer1 = new Rasterizer(m_width, 0, m_height/2, m_zNear, m_zFar);
	m_rasterizer2 = new Rasterizer(m_width, m_height / 2, m_height, m_zNear, m_zFar);
	//m_camera = new Camera();
	//m_camera->setProjectionMatrix(90.0f, width, height, m_zNear, m_zFar);
}

Engine::~Engine()
{

}

void Engine::Resize(int width, int height)
{
	m_width = width;
	m_height = height;

	delete m_buffer;
	delete m_zBuffer;
	m_buffer = (uint*)malloc(sizeof(uint) * m_width * m_height);
	m_zBuffer = (float*)malloc(sizeof(float) * m_width * m_height);

	m_bufferData.height = m_height;
	m_bufferData.width = m_width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.size = m_width * m_height;
	m_tick = clock();
	//	m_camera->setProjectionMatrix(90.0f, width, height, m_zNear, m_zFar);
}

void Engine::ClearBuffer(const Color* color)
{
	uint v = color->GetRawValue();
	memset(m_buffer, v, sizeof(uint) * m_width * m_height);
	memset(m_zBuffer, 0, sizeof(float) * m_width * m_height);
	m_rasterQueue1.clear();
	m_rasterQueue2.clear();
}

int Engine::Update(struct Window *window)
{

	m_rasterizer1->Render(&m_rasterQueue1, &m_bufferData);
	m_rasterizer2->Render(&m_rasterQueue2, &m_bufferData);

	if (m_showZBuffer)
	{
		uint c;
		for (int i = 0; i < m_width*m_height; i++)
		{
			c = (uint)(m_zBuffer[i] * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[i] = c;
		}
	}
	int r = 0;
	r = mfb_update(window, m_buffer);
	m_currentFrame++;
	m_nbPixels = 0;
	clock_t t = clock();
	m_fps = (float)CLOCKS_PER_SEC / (float)(t - m_tick);
	m_tick = t;
	return r;
}

void Engine::DrawGrid(const Camera* camera)
{
	Vector3 a;
	Vector3 b;
	for (float i = -10; i < 10; i += 1.0f)
	{
		a.x = i;
		b.x = i;
		a.y = 0.0f;
		b.y = 0.0f;
		a.z = -10;
		b.z = 10;
		Draw3DLine(camera, &a, &b, 0xFFFFFF, GetBufferData());
	}
	for (float i = -10; i <= 10; i += 1.0f)
	{
		a.z = b.z = i;
		a.y = b.y = 0.0f;
		a.x = -10;
		b.x = 10;
		Draw3DLine(camera, &a, &b, 0xFFFFFF, GetBufferData());
	}

	Draw3DLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3X, 0xFF0000, GetBufferData());
	Draw3DLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Y, 0x00FF00, GetBufferData());
	Draw3DLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0x0000FF, GetBufferData());
	a.x = b.x = 1;
	a.y = b.y = 0.0f;
	a.z = -10;
	b.z = 10;
	//Draw3DLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0xFFFFFF, GetBufferData());
}

void Engine::Draw3DLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c, BufferData* bufferData)
{
	Vector3 a = Vector3(v1);
	Vector3 b = Vector3(v2);

	camera->GetViewMatrix()->Mul(&a);
	camera->GetProjectionMatrix()->Mul(&a);
	camera->GetViewMatrix()->Mul(&b);
	camera->GetProjectionMatrix()->Mul(&b);

	if (ClipSegment(&a, &b))
	{
		if (a.w != 1)
		{
			a.x /= a.w;
			a.y /= a.w;
			a.z /= a.w;
			a.w /= a.w;
		}
		if (b.w != 1)
		{
			b.x /= b.w;
			b.y /= b.w;
			b.z /= b.w;
			b.w /= b.w;
		}
		a.x = (a.x + 1) * m_width / 2.0f;
		a.y = (a.y + 1) * m_height / 2.0f;
		b.x = (b.x + 1) * m_width / 2.0f;
		b.y = (b.y + 1) * m_height / 2.0f;
		DrawLine(a.x, a.y, b.x, b.y, c, bufferData);
	}
}

void Engine::DrawLine(const float xa, const float ya, const float xb, const float yb, const uint c, BufferData* bufferData)
{
	float x1 = xa;
	float x2 = xb;
	float y1 = ya;
	float y2 = yb;
	uint* buffer = bufferData->buffer;
	const bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = abs(y2 - y1);
	float scaleX = 1.0;
	float scaleY = 1.0;
	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;
	for (int x = (int)x1; x < maxX; x++)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			if (steep)
			{
				uint k = x * bufferData->width + y;
				if (k < bufferData->size)
				{
					buffer[k] = c;
				}
			}
			else
			{
				uint k = y * bufferData->width + x;
				if (k < bufferData->size)
				{
					buffer[k] = c;
				}
			}
		}
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Engine::CopyBuffer(uint* source, uint * dest)
{
	int v, r, g, b;
	for (int i = 0; i < m_width*m_height; i++)
	{
		v = source[i];
		r = (int)(float)((v & 0xFF0000) >> 16);
		g = (int)(float)((v & 0x00FF00) >> 8);
		b = (int)(float)((v & 0x0000FF));
		dest[i] = MFB_RGB(r, g, b);
	}
}

void Engine::GetPixelColor(Color* color, int x, int y)
{
	int i = y * m_width + x;
	color->Set(m_buffer[i]);
}

bool Engine::ClipSegment(Vector3* a, Vector3* b)
{
	if (a->w < m_zNear && b->w < m_zNear)
	{
		return false;
	}
	else if (a->w < m_zNear || b->w < m_zNear)
	{
		if (a->w < m_zNear)
		{
			float n = (a->w - m_zNear) / (a->w - b->w);
			a->x = (n * a->x) + ((1 - n) * b->x);
			a->y = (n * a->y) + ((1 - n) * b->y);
			a->z = (n * a->z) + ((1 - n) * b->z);
			a->w = m_zNear;
		}
		else
		{
			float n = (b->w - m_zNear) / (b->w - a->w);
			b->x = (n * b->x) + ((1 - n) * a->x);
			b->y = (n * b->y) + ((1 - n) * a->y);
			b->z = (n * b->z) + ((1 - n) * a->z);
			b->w = m_zNear;
		}
	}
	return true;
}

void Engine::QueueTriangle(const Triangle* t)
{
	float min = std::min<float>(t->va->y, std::min<float>(t->vb->y, t->vc->y));
	float max = std::max<float>(t->va->y, std::max<float>(t->vb->y, t->vc->y));
	if (min < m_height / 2 && max < m_height / 2)
	{
		m_rasterQueue1.push_back(t);
	}
	else if (min >= m_height / 2 && max >= m_height / 2)
	{
		m_rasterQueue2.push_back(t);
	}
	else
	{
		m_rasterQueue1.push_back(t);
		m_rasterQueue2.push_back(t);
	}
}

void Engine::DrawHorizontalLine(const float x1, const float x2, const float y, const uint color, BufferData* bufferData)
{
	int k;
	float alpha = 1;// Color::GetAlpha(color);
	uint* buffer = bufferData->buffer;
	if (alpha != 0 && y >= 0 && y < m_height)
	{
		if (x1 != x2)
		{
			float a = fmin(x1, x2);
			float b = fmax(x1, x2);
			b++;
			a = (int)(a < 0.0f ? 0.0f : a);
			b = (int)(b > m_width ? m_width : b);
			for (int i = a; i < b; i++)
			{
				k = y * m_width + i;
				uint g = buffer[k];
				buffer[k] = color;
			}
		}
	}
}
