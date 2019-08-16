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
}

int Engine::Update(struct Window *window)
{	
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
	int r;
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
		if (x >= 0 && x <m_width && y>=0 && y<m_height)
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

void Engine::FillBottomFlatTriangle(Vector2* v1, Vector2* v2, Vector2* v3, const uint color, BufferData* bufferData)
{
	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);

	float curx1 = v1->x;
	float curx2 = v1->x;
	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		DrawHorizontalLine((int)curx1, (int)curx2, scanlineY, color, bufferData);
		curx1 += invslope1;
		curx2 += invslope2;
	}
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

void Engine::DrawTriangle(const Triangle* t, const Texture* tex, BufferData* bufferData)
{
	Vector2 v1 = Vector2((int)t->va->x, (int)t->va->y);
	Vector2 v2 = Vector2((int)t->vb->x, (int)t->vb->y);
	Vector2 v3 = Vector2((int)t->vc->x, (int)t->vc->y);

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);


	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		FillBottomFlatTriangle2(&v1, &v2, &v3, t, tex, bufferData);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle2(&v1, &v2, &v3, t, tex, bufferData);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, t, tex, bufferData);
		FillTopFlatTriangle2(&v2, &v4, &v3, t, tex, bufferData);
	}
	m_drawnTriangles++;
}

void Engine::FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Texture* tex,  BufferData* bufferData)
{
	uint* buffer = bufferData->buffer;
	Vector3 p;
	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);
	float curx1 = v1->x;
	float curx2 = v1->x;
	float a, b;
	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		int k;
		if (scanlineY >= 0 && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				a = fmin(curx1, curx2);
				b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				for (int i = a; i < b; i++)
				{
					p.x = i;
					p.y = scanlineY;
					p.z = -1;
					FillBufferPixel(&p, t, tex, bufferData);
				}
			}
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void Engine::FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Texture* tex, BufferData* bufferData)
{
	uint* buffer = bufferData->buffer;
	Vector3 p;
	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);
	float curx1 = v3->x;
	float curx2 = v3->x;
	float a, b;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		if (scanlineY >= 0 && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				a = fmin(curx1, curx2);
				b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				for (int i = a; i < b; i++)
				{			
					p.x = i;
					p.y = scanlineY;
					p.z = -1;
					FillBufferPixel(&p, t, tex, bufferData);
				}
			}
		}		
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void Engine::FillBufferPixel(const Vector3* p, const Triangle* t, const Texture* texData, BufferData* bufferData)
{
	
	float w2 = edgeFunction(t->va, t->vb, p);
	float w0 = edgeFunction(t->vb, t->vc, p);
	float w1 = edgeFunction(t->vc, t->va, p);
	float su, tu, cl, r, g, b, a, z;
	uint c, k;
	if (w0 >= 0 || w1 >= 0 || w2 >= 0) 
	{
		w0 /= t->area;
		w1 /= t->area;
		w2 /= t->area;

		z = 1.0f / (t->va->z * w0 + t->vb->z * w1 + t->vc->z * w2);
		k = p->y * m_width + p->x;
		//if (z>=m_zNear && z<=m_zFar && z < bufferData->zBuffer[k])
		float zf = (bufferData->zBuffer[k] == 0.0f)?m_zFar: bufferData->zBuffer[k];
		zf = bufferData->zBuffer[k];
		if(z>=m_zNear && z<m_zFar && z > zf)
		{
			bufferData->zBuffer[k] = z;
			su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
			tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;
			tu = 1.0f - tu;
			su = (int)(su * texData->GetWidth());
			tu = (int)(tu * texData->GetHeight());
			su = (int)su % texData->GetWidth();
			tu = (int)tu % texData->GetHeight();
			cl = ((w0 * t->la + w1 * t->lb + w2 * t->lc)) + 0.1f;
			c = (tu * texData->GetWidth() + su) * 4;
			const float* d = texData->GetData();
			r = d[c] * cl;
			g = d[c + 1] * cl;
			b = d[c + 2] * cl;
			a = d[c + 3] * cl;
			c = ((int)(r * 255) << 16) + ((int)(g * 255) << 8) + (int)(b * 255);
			bufferData->buffer[k] = c;
			m_nbPixels++;
		}
	}
}

void Engine::FillTopFlatTriangle(Vector2* v1, Vector2* v2, Vector2* v3, const uint color, BufferData* bufferData)
{
	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);

	float curx1 = v3->x;
	float curx2 = v3->x;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		DrawHorizontalLine((int)curx1, (int)curx2, scanlineY, color, bufferData);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void Engine::DrawHorizontalLine(const float x1, const float x2, const float y, const uint color, BufferData* bufferData)
{
	int k;
	float alpha = 1;// Color::GetAlpha(color);
	uint* buffer = bufferData->buffer;
	if (alpha != 0 &&y>=0 && y<m_height)
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

void Engine::sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3)
{
	if (v3->y < v2->y)
	{
		std::swap(*v2, *v3);
	}
	if (v2->y < v1->y) std::swap(*v1, *v2);
	if (v3->y < v2->y) std::swap(*v2, *v3);
}

void Engine::sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3)
{
	if (v3->y < v2->y)
	{
		std::swap(*v2, *v3);
		std::swap(*uv2, *uv3);
	}
	if (v2->y < v1->y)
	{
		std::swap(*v1, *v2);
		std::swap(*uv1, *uv2);
	}
	if (v3->y < v2->y)
	{
		std::swap(*v2, *v3);
		std::swap(*uv2, *uv3);
	}
}