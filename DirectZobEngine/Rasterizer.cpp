#include "Rasterizer.h"
#include "DirectZob.h"
#include "Light.h"
#include "Texture.h"

static Vector3 sFog = Vector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

Rasterizer::Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_height = endHeight;
	m_width = width;
}

void Rasterizer::Init()
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
}

void Rasterizer::Start(const Triangle* triangles, const uint nbTriangles, const std::vector<Line3D>* lines, const bool wireFrame, const bool scanline, const bool bEvenFrame, RenderOptions::Lighting_precision lp)
{
	m_lines = lines;
	m_triangles = triangles;
	m_nbTriangles = nbTriangles;
	m_wireFrame = wireFrame;
	m_thread = std::thread(&Rasterizer::Render, this);
	m_bEvenFrame = bEvenFrame ? 1 : 0;
	m_scanline = scanline;
	m_lightingPrecision = lp;
}

void Rasterizer::WaitForEnd() {
	if(m_thread.joinable())
		m_thread.join();
}

void Rasterizer::End() {
	if (m_thread.joinable())
		m_thread.join();
}

Rasterizer::~Rasterizer()
{
	Clear();
}

void Rasterizer::Clear()
{
	m_lines = NULL;
	m_triangles = NULL;
	m_nbTriangles = 0;
}

void Rasterizer::Render() 
{
	const Vector3* camDir = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->GetForward();
	m_camDir = Vector3(-camDir->x, -camDir->y, -camDir->z);
	//warning : invert lightdir ! https://fr.wikipedia.org/wiki/Ombrage_de_Phong
	for (int i = 0; i < m_lines->size(); i++)
	{
		const Line3D l = m_lines->at(i);
		DrawLine(&l);
	}
	m_lights = NULL;
	LightManager* lm = DirectZob::GetInstance()->GetLightManager();
	if (lm)
	{
		m_lights = lm->GetActiveLights();
		m_ambientColor = lm->GetAmbientColor();
		m_fogColor = lm->GetFogColor();
		m_fogType = lm->GetFogType();
		m_fogDensity = lm->GetFogDensity();
		m_fogDistance = lm->GetFogDistance();
	}
	for (int i = 0; i < m_nbTriangles; i++)
	{
		const Triangle* t = &m_triangles[i];
		DrawTriangle(t);
	}
}


void Rasterizer::DrawLine(const Line3D* l) const
{
	float zRatio, zf = 0.0f;
	uint k = 0;
	float x1 = l->xa;
	float x2 = l->xb;
	float y1 = l->ya;
	float y2 = l->yb;
	float z1 = l->za;
	float z2 = l->zb;
	uint* buffer = m_bufferData->buffer;
	float* zBuffer = m_bufferData->zBuffer;
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
		std::swap(z1, z2);
	}
	const float dx = x2 - x1;
	const float dy = abs(y2 - y1);
	
	float scaleX = 1.0;
	float scaleY = 1.0;
	float scaleZ = 1.0;
	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	
	int y = (int)y1;
	
	const int maxX = (int)x2;
	const float dz = z2 - z1;
	const float zstep = dz / (maxX - x1);
	float z = z1;
	for (int x = (int)x1; x < maxX; x++)
	{
		if (x >= 0 && x < m_width && y >= m_startHeight && y < m_height)
		{
			if (RenderLine(y))
			{
				if (steep)
				{
					k = x * m_bufferData->width + y;
					if (k < m_bufferData->size)
					{
						zf = m_bufferData->zBuffer[k];
						zRatio = (z - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
						if (zRatio >= 0.0f && (zf < 0.0f || zRatio < zf))
						{
							buffer[k] = l->c;
							m_bufferData->zBuffer[k] = zRatio;
						}
					}
				}
				else
				{
					k = y * m_bufferData->width + x;
					if (k < m_bufferData->size)
					{
						zf = m_bufferData->zBuffer[k];
						zRatio = (z - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
						if (zRatio >= 0.0f && (zf < 0.0f || zRatio < zf))
						{
							buffer[k] = l->c;
							m_bufferData->zBuffer[k] = zRatio;
						}
					}
				}
			}
		}
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			z += zstep;
			error += dx;
		}
	}
}

inline Vector3 Rasterizer::ComputeLightingAtPoint(const Vector3* position, const Vector3* normal, RenderOptions::eLightMode lighting) const
{
	Vector3 outColor = Vector3(0, 0, 0);
	if (m_lights)
	{
		Vector3 lightDir = Vector3(0, 0, 0);
		float lightPower, cl, sl = 0.0f;
		for (std::vector<Light*>::const_iterator iter = m_lights->begin(); iter != m_lights->end(); iter++)
		{
			const Light* l = (*iter);
			if (l->IsActive())
			{
				lightDir = position - l->GetWorldPosition();// tpos;
				lightPower = 1.0f - (lightDir.sqrtLength() / l->GetFallOffDistance());
				lightPower = clamp2(lightPower, 0.0f, 1.0f) * l->GetIntensity();
				if (lightPower > 0.0f)
				{
					lightDir.Normalize();
					static int specularIntensity = 50;
					static float ambientIntensity = 0.4f;
					cl = computeLighting(normal, &lightDir);
					sl = computeSpecular(normal, &lightDir, &m_camDir, cl, specularIntensity);
					if (lighting == RenderOptions::eLightMode_none)
					{
						cl = 0.0f;
						sl = 0.0f;
					}
					else if (lighting == RenderOptions::eLightMode_gouraud)
					{
						sl = 0.0f;
					}
					outColor.x += (cl + sl) * l->GetColor()->x * lightPower;
					outColor.y += (cl + sl) * l->GetColor()->y * lightPower;
					outColor.z += (cl + sl) * l->GetColor()->z * lightPower;
				}
			}
		}
	}
	return outColor;
}

void Rasterizer::DrawTriangle(const Triangle* t) const
{
	/*Vector2 v1 = Vector2((int)t->va->x, (int)t->va->y);
	Vector2 v2 = Vector2((int)t->vb->x, (int)t->vb->y);
	Vector2 v3 = Vector2((int)t->vc->x, (int)t->vc->y);*/

	Vector2 v1 = Vector2((int)t->pa->x, (int)t->pa->y);
	Vector2 v2 = Vector2((int)t->pb->x, (int)t->pb->y);
	Vector2 v3 = Vector2((int)t->pc->x, (int)t->pc->y);

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);

	//vertex lighting computing
	if(true/*vertex lighting*/)
	{
		Vector3 la, lb, lc = Vector3(0, 0, 0);
		
	}

	Vector3 la, lb, lc = Vector3(0, 0, 0);
	if (m_lightingPrecision == RenderOptions::Lighting_precision_vertex)
	{
		la = ComputeLightingAtPoint(t->va, t->na, t->options.lightMode);
		lb = ComputeLightingAtPoint(t->vb, t->nb, t->options.lightMode);
		lc = ComputeLightingAtPoint(t->vc, t->nc, t->options.lightMode);
	}

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		FillBottomFlatTriangle2(&v1, &v2, &v3, t, &la, &lb, &lc);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle2(&v1, &v2, &v3, t, &la, &lb, &lc);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, t, &la, &lb, &lc);
		FillTopFlatTriangle2(&v2, &v4, &v3, t, &la, &lb, &lc);
	}
//	m_drawnTriangles++;
}

void Rasterizer::FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const
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
		if (RenderLine(scanlineY) && scanlineY >= m_startHeight && scanlineY < m_height)
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
						FillBufferPixel(&p, t, la, lb, lc);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, la, lb, lc);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, la, lb, lc);
					}
				}
			}
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void Rasterizer::FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const
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
		if (RenderLine(scanlineY) && scanlineY >= m_startHeight && scanlineY < m_height)
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
						FillBufferPixel(&p, t, la, lb, lc);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, la, lb, lc);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, la, lb, lc);
					}
				}
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

inline const void Rasterizer::FillBufferPixel(const Vector3* p, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const
{
	float w0, w1, w2, su, tu, cl, sl, r, g, b, a, z, zRatio, fr, fg, fb, lightPower;
	float texPixelData[4];
	int c, k;
	Vector3 normal, lightDir;

	w2 = edgeFunction(t->pa, t->pb, p);
	w0 = edgeFunction(t->pb, t->pc, p);
	w1 = edgeFunction(t->pc, t->pa, p);

	w0 /= t->area;
	w1 /= t->area;
	w2 /= t->area;

	Vector3 tpos = Vector3(	(t->va->x * w0 + t->vb->x * w1 + t->vc->x * w2),
							(t->va->y * w0 + t->vb->y * w1 + t->vc->y * w2),
							(t->va->z * w0 + t->vb->z * w1 + t->vc->z * w2));

	const Material* material = t->material;
	z = (t->pa->z * w0 + t->pb->z * w1 + t->pc->z * w2);
	zRatio = (z - m_bufferData->zNear ) / (m_bufferData->zFar - m_bufferData->zNear);
	k = p->y * m_width + p->x;
	float zf = m_bufferData->zBuffer[k];
	if ( zRatio >= 0.0f &&  (!t->options.ZBuffered() || zf < 0.0f  || zRatio < zf ))
	{
		m_bufferData->zBuffer[k] = zRatio;
		su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
		tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;

		cl = 1.0f;
		RenderOptions::eLightMode lighting = t->options.LightMode();
		if (lighting == RenderOptions::eLightMode_flat || lighting == RenderOptions::eLightMode_flatPhong)
		{
			normal = t->n;
		}
		else
		{
			normal = Vector3((w0 * t->na->x + w1 * t->nb->x + w2 * t->nc->x),
				(w0 * t->na->y + w1 * t->nb->y + w2 * t->nc->y),
				(w0 * t->na->z + w1 * t->nb->z + w2 * t->nc->z)); 
			//normal.Mul(-1.0f);
		}
		const Texture* texture = material?material->GetDiffuseTexture():NULL;
		if (texture)
		{
			if (texture->GetData())
			{
				tu = 1.0f - tu;
				su = (int)(su * texture->GetWidth());
				tu = (int)(tu * texture->GetHeight());
				su = (int)su % texture->GetWidth();
				tu = (int)tu % texture->GetHeight();

				c = (int)(((int)tu * (int)texture->GetWidth() + (int)su) * 4);
				const float* d = texture->GetData();
				memcpy(texPixelData, &d[c], sizeof(float) * 4);
				r = texPixelData[0];
				g = texPixelData[1];
				b = texPixelData[2];
				a = 1.0f;// texPixelData[3];
			}
			else
			{

				r = material->GetDiffuseColor()->x;
				g = material->GetDiffuseColor()->y;
				b = material->GetDiffuseColor()->z;
				a = 1.0f;
			}
		}
		else
		{
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			a = 1.0f;
		}
		//lighting
		fr = 0.0f;
		fg = 0.0f;
		fb = 0.0f;

		if (m_lightingPrecision == RenderOptions::Lighting_precision_vertex)
		{
			//Vertex lighting
			fr = (w0 * la->x + w1 * lb->x + w2 * lc->x) * r;
			fg = (w0 * la->y + w1 * lb->y + w2 * lc->y) * g;
			fb = (w0 * la->z + w1 * lb->z + w2 * lc->z) * b;
			if (fr > 0.0f || fg > 0.0f || fb > 0.0f)
			{
				int gg = 0;
				gg++;
			}
		}
		else 
		{
			Vector3 l = ComputeLightingAtPoint(&tpos, &normal, lighting);
			fr = l.x * r;
			fg = l.y * g;
			fb = l.z * b;
		}
		fr += r * m_ambientColor->x;
		fg += g * m_ambientColor->y;
		fb += b * m_ambientColor->z;
		fr = clamp2(fr, 0.0f, 1.0f);
		fg = clamp2(fg, 0.0f, 1.0f);
		fb = clamp2(fb, 0.0f, 1.0f);
		z = abs(z);
		if (m_fogType == FogType::FogType_NoFog)
		{
			z = 1.0f;
		}
		else if (m_fogType == FogType::FogType_Linear)
		{
			z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
		}
		else if (m_fogType == FogType::FogType_Exp)
		{
			//z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
			z = exp(-m_fogDensity * zRatio);
		}
		else
		{
			//z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
			z = exp(-m_fogDensity * pow(zRatio,2));
		}
		z = clamp2(z, 0.0f, 1.0f);
		fr = fr * z + (1.0f - z) * m_fogColor->x;
		fg = fg * z + (1.0f - z) * m_fogColor->y;
		fb = fb * z + (1.0f - z) * m_fogColor->z;
		c = ((int)(fr * 255) << 16) + ((int)(fg * 255) << 8) + (int)(fb * 255);
		m_bufferData->buffer[k] = c;
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
