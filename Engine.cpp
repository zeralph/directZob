#include "Engine.h"
#include <algorithm>
#include "TextureTest.h"

Engine::Engine(int width, int height)
{
	m_currentFrame = 0;
	m_width = width;
	m_height = height;
	m_buffer = (uint*)malloc(sizeof(uint) * m_width * m_height);
	m_curBuffer = 0;
	int result = mfb_open("Noise Test", m_width, m_height);
	m_fps = 0.0;

	m_bufferData.height = m_height;
	m_bufferData.width = m_width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.size = m_width * m_height;
	m_tick = clock();

	m_camera = new Camera();
	m_camera->setProjectionMatrix(90.0f, 0.01f, 100.0f);

}

Engine::~Engine()
{

}

int Engine::Update()
{	
	int r;
	r = mfb_update(m_buffer);
	m_currentFrame++;
	clock_t t = clock();
	m_fps = (float)CLOCKS_PER_SEC / (float)(t - m_tick);
	m_tick = t;
	return r;
}

void Engine::DrawString(const Matrix2x2* matrix, std::string s, const uint color, BufferData* bufferData)
{
	Matrix2x2 m(matrix);
	int w = Text2D::GetCharWidth();
	int h = Text2D::GetCharHeight();
	Vector2 initialTransaltion = m.GetTranslation();
	uint* buffer = bufferData->buffer;
	for (int i = 0; i < s.length(); i++)
	{
		m.SetTranslation((float)w, 0);
		if (s[i] == '\n')
		{
			m.CopyFrom(matrix);
			m.SetTranslation(0, (float)h);
			continue;
		}
		DrawChar(&m, s[i], color, bufferData);
	}
}

void Engine::DrawChar(const Matrix2x2* matrix, char c, const uint color, BufferData* bufferData)
{
	const uint* b = Text2D::GetCharBuffer(c);
	int w = Text2D::GetCharWidth();
	int h = Text2D::GetCharHeight();
	if (matrix)
	{
		//Matrix2x2 m = *matrix;
		//m.SetTransaltion(-w / 2, -h / 2);
		DrawBuffer(matrix, color, b, w, h, bufferData);
	}
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

void Engine::ClearBuffer(const Color* color)
{
	int v = color->GetRawValue();
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_buffer[i] = v;
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
	float xMin = fmin(v1->x, fmin(v2->x, v3->x));
	float xMax = fmax(v1->x, fmax(v2->x, v3->x));
	float yMin = fmin(v1->y, fmin(v2->y, v3->y));
	float yMax = fmax(v1->y, fmax(v2->y, v3->y));

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


void Engine::DrawTriangle2(const Vector2* va, const Vector2* vb, const Vector2* vc, const Texture* tex, BufferData* bufferData)
{
	Vector2 v1 = Vector2((int)va->x, (int)va->y);
	Vector2 v2 = Vector2((int)vb->x, (int)vb->y);
	Vector2 v3 = Vector2((int)vc->x, (int)vc->y);

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		FillBottomFlatTriangle2(&v1, &v2, &v3, tex, bufferData);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle2(&v1, &v2, &v3, tex, bufferData);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, tex, bufferData);
		FillTopFlatTriangle2(&v2, &v4, &v3, tex, bufferData);
	}
}

void Engine::FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Texture* tex,  BufferData* bufferData)
{
	uint* buffer = bufferData->buffer;
	float xMin = fmin(v1->x, fmin(v2->x, v3->x));
	float xMax = fmax(v1->x, fmax(v2->x, v3->x));
	float yMin = fmin(v1->y, fmin(v2->y, v3->y));
	float yMax = fmax(v1->y, fmax(v2->y, v3->y));

	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);

	float curx1 = v1->x;
	float curx2 = v1->x;

	Vector2 dv1 = Vector2(v2->x - v1->x, v2->y - v1->y);
	Vector2 dv2 = Vector2(v3->x - v1->x, v3->y - v1->y);
	Vector2 dvp1 = Vector2(0, 0);
	Vector2 dvp2 = Vector2(0, 0);
	float lv1 = dv1.lenght();
	float lv2 = dv2.lenght();
	

	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		int k;
		if (scanlineY >= 0 && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				float a = fmin(curx1, curx2);
				float b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				for (int i = a; i < b; i++)
				{
					dvp1.x = dvp2.x = i - v1->x;
					dvp1.y = dvp2.y = scanlineY - v1->y;

					float u = dv1.Dot(&dvp1) / (lv1);
					float v = dv2.Dot(&dvp2) / (lv2);

					dvp1.x = u * dv1.x;
					dvp1.y = u * dv1.y;

					dvp2.x = v * dv2.x;
					dvp2.y = v * dv2.y;

					if ( dv1.x != 0.0f)
					{
						u = (dvp1.x / dv1.x)* tex->GetWidth();
					}
					else if (dv1.y != 0.0f)
					{
						u = (dvp1.y / dv1.y)* tex->GetWidth();
					}
					else
					{
						int g = 0;
						g++;
					}

					if (dv2.x != 0.0f)
					{
						v = (dvp2.y / dv2.y)* tex->GetHeight();
					}
					else if (dv2.y != 0.0f)
					{
						v = (dvp2.y / dv2.y)* tex->GetHeight();
					}
					else
					{
						int g = 0;
						g++;
					}

					

					uint z = (int)v * tex->GetHeight() + (int)u;
					uint c = 0;// tex->GetData()[z];
					if(c == 0) c = 0xFFFF;
					k = scanlineY * m_width + i;
					buffer[k] = c;
				}
			}
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}

	Matrix2x2 m;
	m.SetTranslation(v1->x, v1->y);
	DrawChar(&m, '1', 0xFFFFFF, bufferData);
	m.Identity();
	m.SetTranslation(v2->x, v2->y);
	DrawChar(&m, '2', 0xFFFFFF, bufferData);
	m.Identity();
	m.SetTranslation(v3->x, v3->y);
	DrawChar(&m, '3', 0xFFFFFF, bufferData);
}

void Engine::FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Texture* tex, BufferData* bufferData)
{
	uint* buffer = bufferData->buffer;
	float xMin = fmin(v1->x, fmin(v2->x, v3->x));
	float xMax = fmax(v1->x, fmax(v2->x, v3->x));
	float yMin = fmin(v1->y, fmin(v2->y, v3->y));
	float yMax = fmax(v1->y, fmax(v2->y, v3->y));

	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);

	float curx1 = v3->x;
	float curx2 = v3->x;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		int k;
		if (scanlineY >= 0 && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				float a = fmin(curx1, curx2);
				float b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				for (int i = a; i < b; i++)
				{
					int u = (int)((float)(b - i) / (b - a) * tex->GetWidth());
					int v = (int)((float)(v1->y - scanlineY) / (v1->y - v3->y) * tex->GetHeight());
					int z = v * tex->GetHeight() + u;
					uint c = tex->GetData()[z];
					k = scanlineY * m_width + i;
					buffer[k] = c;
				}
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
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
