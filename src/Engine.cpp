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
	clock_t t = clock();
	m_fps = (float)CLOCKS_PER_SEC / (float)(t - m_tick);
	m_tick = t;
	return r;
}

void Engine::DrawCircle(const float xc, const float yc, const float r, const uint c, BufferData* bufferData)
{
	/*
	float x = 0, y = r;
	float d = 3 - 2 * r;
	float scaleX = 1.0;
	float scaleY = 1.0;
	DrawPixel(xc + x, yc + y, c);
	DrawPixel(xc - x, yc + y, c);
	DrawPixel(xc + x, yc - y, c);
	DrawPixel(xc - x, yc - y, c);
	DrawPixel(xc + y, yc + x, c);
	DrawPixel(xc - y, yc + x, c);
	DrawPixel(xc + y, yc - x, c);
	DrawPixel(xc - y, yc - x, c);
	while (y >= x)
	{
		// for each pixel we will 
		// draw all eight pixels 

		x++;

		// check for decision parameter 
		// and correspondingly  
		// update d, x, y 
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		DrawPixel(xc + x, yc + y, c);
		DrawPixel(xc - x, yc + y, c);
		DrawPixel(xc + x, yc - y, c);
		DrawPixel(xc - x, yc - y, c);
		DrawPixel(xc + y, yc + x, c);
		DrawPixel(xc - y, yc + x, c);
		DrawPixel(xc + y, yc - x, c);
		DrawPixel(xc - y, yc - x, c);
	}
	*/
}

void Engine::DrawGrid(const Camera* camera)
{
	Vector3 a;
	Vector3 b;
	for (float i = -10; i <= 10; i += 1.0f)
	{
		a.x = b.x = i;
		a.y = b.y = 0.0f;
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

}

void Engine::Draw3DLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c, BufferData* bufferData)
{
	Vector3 a = Vector3(v1);
	Vector3 b = Vector3(v2);

	camera->GetViewMatrix()->Mul(&a);
	camera->GetProjectionMatrix()->Mul(&a);
	
	if (a.w != 1)
	{
		//a.x /= a.w;
		//a.y /= a.w;
		//a/.z /= a.w;
		//a.w /= a.w;
	}
	
	camera->GetViewMatrix()->Mul(&b);
	camera->GetProjectionMatrix()->Mul(&b);
	
	if (b.w != 1)
	{
		//b.x /= b.w;
		//b.y /= b.w;
		//b.z /= b.w;
		//b.w /= b.w;
	}
	
	a.x = (a.x / a.z + 1) * m_width / 2.0f;
	a.y = (a.y / a.z + 1) * m_height / 2.0f;

	b.x = (b.x / b.z + 1) * m_width / 2.0f;
	b.y = (b.y / b.z + 1) * m_height / 2.0f;
	if(a.z <0 || b.z < 0)
	{
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

void Engine::DrawTexture(const Matrix2x2* matrix, const Texture* texture, BufferData* bufferData)
{
	Vector2 v = { 0, 0 };
	Matrix2x2 m;
	const uint w = texture->GetWidth();
	const uint h = texture->GetHeight();
	for (uint j = 0; j < h; j++)
	{
		for (uint i = 0; i < w; i++)
		{
			v = { (float)i, (float)j };
			m.CopyFrom(matrix);
			m.SetTranslation((float)i, (float)j);
			int c = texture->GetData()[j * w + i];
			DrawPixel(&m, c, (float)(w / 2), (float)(h / 2), bufferData);
		}
	}
}

void Engine::DrawBuffer(const Matrix2x2* matrix, const uint color, const uint* buffer, const int bufferWidth, const int bufferHeight, BufferData* bufferData)
{
	Vector2 v = Vector2::Vector2Zero;
	Matrix2x2 m;
	
	for (int j = 0; j < bufferHeight; j++)
	{
		for (int i = 0; i < bufferWidth; i++)
		{
			if (buffer[j * bufferHeight + i] != 0)
			{
				v.x = (float)i;
				v.y = (float)j;
				m.CopyFrom(matrix);
				m.SetTranslation((float)i, (float)j);
				DrawPixel(&m, color, (float)(bufferWidth / 2), (float)(bufferHeight / 2), bufferData);
			}
		}
	}
}

void Engine::DrawPixel(const Matrix2x2* matrix, const uint color, const float dx, const float dy, BufferData* bufferData)
{
	Vector2 a = Vector2::Vector2Zero;
	Vector2 b = Vector2::Vector2Y;
	Vector2 d = Vector2::Vector2X;
	Vector2 c = Vector2::Vector2One;

	matrix->Mul(&a);
	matrix->Mul(&b);
	matrix->Mul(&c);
	matrix->Mul(&d);

	DrawTriangle(&a, &b, &c, color, bufferData);
	DrawTriangle(&a, &c, &d, color, bufferData);
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


void Engine::DrawTriangle2(const Triangle* t, const Texture* tex, BufferData* bufferData)
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
					p.z = 0;
					FillBufferPIxel(&p, t, tex, bufferData);
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
					p.z = 0;
					FillBufferPIxel(&p, t, tex, bufferData);
				}
			}
		}		
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void Engine::FillBufferPIxel(const Vector3* p, const Triangle* t, const Texture* texData, BufferData* bufferData)
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
		if(z>=m_zNear && z<m_zFar && z < zf)
		{
			bufferData->zBuffer[k] = z;
			su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
			tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;
			tu = 1.0f - tu;
			su = (int)(su * texData->GetWidth());
			tu = (int)(tu * texData->GetHeight());
			su = (int)su % texData->GetWidth();
			tu = (int)tu % texData->GetHeight();
			cl = ((w0 * t->la + w1 * t->lb + w2 * t->lc));
			c = (tu * texData->GetWidth() + su) * 4;
			r = texData->GetData()[c] * cl;
			g = texData->GetData()[c + 1] * cl;
			b = texData->GetData()[c + 2] * cl;
			a = texData->GetData()[c + 3] * cl;
			c = ((int)(r * 255) << 16) + ((int)(g * 255) << 8) + (int)(b * 255);
			bufferData->buffer[k] = c;
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

void Engine::DrawTriangle(const Vector2* va, const Vector2* vb, const Vector2* vc, const uint color, BufferData* bufferData)
{
	Vector2 v1 = Vector2( (int)va->x, (int)va->y );
	Vector2 v2 = Vector2( (int)vb->x, (int)vb->y );
	Vector2 v3 = Vector2( (int)vc->x, (int)vc->y );

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		FillBottomFlatTriangle(&v1, &v2, &v3, color, bufferData);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle(&v1, &v2, &v3, color, bufferData);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2( (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y );
		FillBottomFlatTriangle(&v1, &v2, &v4, color, bufferData);
		FillTopFlatTriangle(&v2, &v4, &v3, color, bufferData);
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