#include "Rasterizer.h"
#include <thread> 

static Vector3 sFog = Vector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

Rasterizer::Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_height = endHeight;
	m_width = width;
	m_run = true;
	m_started = false;
	std::thread t = std::thread(&Rasterizer::Run, this);
	t.detach();
}

void Rasterizer::Init()
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
}

void Rasterizer::Run()
{
	while (m_run)
	{
		if (m_started)
		{
			Render();
			m_started = false;
		}
		else
		{
			//Sleep(1);
		}
	}
}

void Rasterizer::Start(const Triangle* triangles, uint nbTriangles, const std::vector<Line2D>* lines, const bool wireFrame)
{
	m_lines = lines;
	m_triangles = triangles;
	m_nbTriangles = nbTriangles;
	m_wireFrame = wireFrame;
	m_started = true;
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::Clear()
{
	m_lines = NULL;
	m_triangles = NULL;
}

void Rasterizer::Render() const
{
	for (int i = 0; i < m_lines->size(); i++)
	{
		const Line2D l = m_lines->at(i);
		DrawLine(&l);
	}
	for (int i = 0; i < m_nbTriangles; i++)
	{
		const Triangle* t = &m_triangles[i];
		DrawTriangle(t);
	}
}


void Rasterizer::DrawLine(const Line2D* l) const
{
	float x1 = l->xa;
	float x2 = l->xb;
	float y1 = l->ya;
	float y2 = l->yb;
	uint* buffer = m_bufferData->buffer;
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
		if (x >= 0 && x < m_width && y >= m_startHeight && y < m_height)
		{
			if (steep)
			{
				uint k = x * m_bufferData->width + y;
				if (k < m_bufferData->size)
				{
					buffer[k] = l->c;
				}
			}
			else
			{
				uint k = y * m_bufferData->width + x;
				if (k < m_bufferData->size)
				{
					buffer[k] = l->c;
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

void Rasterizer::DrawTriangle(const Triangle* t) const
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
		FillBottomFlatTriangle2(&v1, &v2, &v3, t);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle2(&v1, &v2, &v3, t);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, t);
		FillTopFlatTriangle2(&v2, &v4, &v3, t);
	}
//	m_drawnTriangles++;
}

void Rasterizer::FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t) const
{
	uint* buffer = m_bufferData->buffer;
	Vector3 p;
	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);
	float curx1 = v1->x;
	float curx2 = v1->x;
	float a, b;
	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		int k;
		if (scanlineY >= m_startHeight && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				a = fmin(curx1, curx2);
				b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				if (m_wireFrame)
				{
					if (a < b)
					{
						p.x = a;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
					}
				}
			}
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void Rasterizer::FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t) const
{
	uint* buffer = m_bufferData->buffer;
	Vector3 p;
	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);
	float curx1 = v3->x;
	float curx2 = v3->x;
	float a, b;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		if (scanlineY >= m_startHeight && scanlineY < m_height)
		{
			if (curx1 != curx2)
			{
				a = fmin(curx1, curx2);
				b = fmax(curx1, curx2);
				b++;
				a = (int)(a < 0.0f ? 0.0f : a);
				b = (int)(b > m_width ? m_width : b);
				if (m_wireFrame)
				{
					if (a < b)
					{
						p.x = a;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t);
					}
				}
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

inline const void Rasterizer::FillBufferPixel(const Vector3* p, const Triangle* t) const
{

	float w2 = edgeFunction(t->va, t->vb, p);
	float w0 = edgeFunction(t->vb, t->vc, p);
	float w1 = edgeFunction(t->vc, t->va, p);
	float su, tu, cl, r, g, b, a, z;
	uint c, k;
	//if (w0 >= 0 || w1 >= 0 || w2 >= 0)
	{
		w0 /= t->area;
		w1 /= t->area;
		w2 /= t->area;

		const Material* texData = t->tex;

		z = 1.0f / (t->va->z * w0 + t->vb->z * w1 + t->vc->z * w2);
		k = p->y * m_width + p->x;
		float zf = m_bufferData->zBuffer[k];

		static float zmin = 1000;
		static float zmax = 0;

		if (/*z >= m_zNear && z<=m_zFar && */z > zf)
		{
			m_bufferData->oBuffer[k] = t->owner;
			m_bufferData->zBuffer[k] = z;
			if (zmin > z)
			{
				zmin = z;
			}
			if (zmax < z)
			{
				zmax = z;
			}

			su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
			tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;
			cl = ((w0 * t->la + w1 * t->lb + w2 * t->lc)) + 0.1f;
			if (texData)
			{
				if (texData->GetData())
				{
					tu = 1.0f - tu;
					su = (int)(su * texData->GetWidth());
					tu = (int)(tu * texData->GetHeight());
					su = (int)su % texData->GetWidth();
					tu = (int)tu % texData->GetHeight();

					c = (uint)(((uint)tu * (uint)texData->GetWidth() + (uint)su) * 4);

					const float* d = texData->GetData();
					r = d[c] * cl;
					g = d[c + 1] * cl;
					b = d[c + 2] * cl;
					a = d[c + 3] * cl;
				}
				else
				{

					r = texData->GetDiffuseColor()->x * cl;
					g = texData->GetDiffuseColor()->y * cl;
					b = texData->GetDiffuseColor()->z * cl;
					a = 1.0f;
				}
			}
			else
			{
				r = 1.0f * cl;
				g = 0.0f * cl;
				b = 1.0f * cl;
				a = 1.0f;
			}
			r = clamp2(r, 0.0f, 1.0f);
			g = clamp2(g, 0.0f, 1.0f);
			b = clamp2(b, 0.0f, 1.0f);

			c = ((int)(r * 255) << 16) + ((int)(g * 255) << 8) + (int)(b * 255);
			m_bufferData->buffer[k] = c;

		}
	}
}

void Rasterizer::sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3) const
{
	if (v3->y < v2->y)
	{
		std::swap(*v2, *v3);
	}
	if (v2->y < v1->y) std::swap(*v1, *v2);
	if (v3->y < v2->y) std::swap(*v2, *v3);
}

void Rasterizer::sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3) const
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
