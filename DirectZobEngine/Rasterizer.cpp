#include "Rasterizer.h"
#include "DirectZob.h"
#include "Light.h"
#include "Texture.h"

static ZobVector3 sFog = ZobVector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

Rasterizer::Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_endHeight = endHeight;
	m_width = width;
}

void Rasterizer::Init()
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
}

void Rasterizer::Start(const Triangle* triangles, const uint nbTriangles, const std::vector<Line3D>* lines, const bool wireFrame, const eRenderMode renderMode, const bool bEvenFrame, const eLightingPrecision lp)
{
	m_lines = lines;
	m_triangles = triangles;
	m_nbTriangles = nbTriangles;
	m_wireFrame = wireFrame;
	m_thread = std::thread(&Rasterizer::Render, this);
	m_bEvenFrame = bEvenFrame ? 1 : 0;
	m_renderMode = renderMode;
	m_lightingPrecision = lp;
}

float Rasterizer::WaitForEnd() 
{
	if (m_thread.joinable())
		m_thread.join();
	return m_time;
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
	m_tick = clock();
	if (DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera())
	{
		const ZobVector3 camDir = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->GetForward();
		m_camDir = ZobVector3(-camDir.x, -camDir.y, -camDir.z);
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
	m_time = (float)(clock() - m_tick) / CLOCKS_PER_SEC * 1000;
}

void Rasterizer::plotLine(int x0, int y0, int x1, int y1) const
{

}

void Rasterizer::DrawLine(const Line3D* l) const
{
	float zRatio, zf = 0.0f;
	int x0 = (int)l->xa;
	int x1 = (int)l->xb;
	int y0 = (int)l->ya;
	int y1 = (int)l->yb;
	float z0 = l->za;
	float z1 = l->zb;
	float dz = l->zb - l->za;
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	dz /= (float)(abs(x1 - x0) + abs(y1 - y0));
	int err = dx + dy, e2; /* error value e_xy */
	bool boldX = false;
	bool boldY = false;
	int k, kx, ky;
	float z = z0;
	for (;;)
	{  /* loop */

		if (x0 >= 0 && x0 < m_bufferData->width && y0 >= m_startHeight && y0 < m_endHeight)
		{
			k = y0 * m_bufferData->width + x0;
			kx = y0 * m_bufferData->width + (x0+1);
			ky = (y0+1) * m_bufferData->width + x0;
			if (k < m_bufferData->size)
			{
				float zf = m_bufferData->zBuffer[k];
				float zRatio = (z  - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				if (zRatio >= 0.0f && (zf < 0.0f || zRatio < zf))
				{
					m_bufferData->buffer[k] = l->c;
					m_bufferData->zBuffer[k] = zRatio;
					if (boldX && kx < m_bufferData->size)
					{
						m_bufferData->buffer[kx] = l->c;
						m_bufferData->zBuffer[kx] = zRatio;
					}
					if (boldY && ky < m_bufferData->size)
					{
						m_bufferData->buffer[ky] = l->c;
						m_bufferData->zBuffer[ky] = zRatio;
					}
				}
			}
		}
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		boldX = false;
		boldY = false;
		if (e2 >= dy) { err += dy; x0 += sx; boldY = l->bold; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; boldX = l->bold; } /* e_xy+e_y < 0 */
		z += dz;
	}
}

inline ZobVector3 Rasterizer::ComputeLightingAtPoint(const ZobVector3* position, const ZobVector3* normal, RenderOptions::eLightMode lighting) const
{
	ZobVector3 outColor = ZobVector3(0, 0, 0);
	if (m_lights)
	{
		ZobVector3 lightDir = ZobVector3(0, 0, 0);
		float lightPower, cl, sl = 0.0f;
		for (std::vector<Light*>::const_iterator iter = m_lights->begin(); iter != m_lights->end(); iter++)
		{
			const Light* l = (*iter);
			if (l->IsActive())
			{
				if (l->GetType() == Light::eLightType_directional)
				{
					lightDir = l->GetForward();
					lightPower = 1.0f;
				}
				else if(l->GetType() == Light::eLightType_point)
				{
					lightDir = position - l->GetWorldPosition();// tpos;
					lightPower = 1.0f - (lightDir.sqrtLength() / l->GetFallOffDistance());
					lightPower = clamp2(lightPower, 0.0f, 1.0f) * l->GetIntensity();
					lightDir.Normalize();
				}
				else if (l->GetType() == Light::eLightType_spot)
				{
					lightDir = position - l->GetWorldPosition();// tpos;
					lightPower = 1.0f - (lightDir.sqrtLength() / l->GetFallOffDistance());
					lightDir.Normalize();
					ZobVector3 fw = l->GetForward();
					float f = ZobVector3::Dot(&fw, &lightDir);
					/*
					float r = l->GetSpotAngle() / 2.0f;
					r = DEG_TO_RAD(r);
					r = cos(r);
					if (fabs(f) < fabs(r))
					{
						lightPower = 0;
					}
					*/
					f = f - (90.0f - l->GetSpotAngle()/2.0f) / 90.0f;
					lightPower *= f;
					lightPower = clamp2(lightPower, 0.0f, 1.0f) * l->GetIntensity();
				}
				if (lightPower > 0.0f)
				{
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
	/*ZobVector2 v1 = ZobVector2((int)t->va->x, (int)t->va->y);
	ZobVector2 v2 = ZobVector2((int)t->vb->x, (int)t->vb->y);
	ZobVector2 v3 = ZobVector2((int)t->vc->x, (int)t->vc->y);*/

	ZobVector2 v1 = ZobVector2((int)t->pa->x, (int)t->pa->y);
	ZobVector2 v2 = ZobVector2((int)t->pb->x, (int)t->pb->y);
	ZobVector2 v3 = ZobVector2((int)t->pc->x, (int)t->pc->y);

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);

	//vertex lighting computing
	if(true/*vertex lighting*/)
	{
		ZobVector3 la, lb, lc = ZobVector3(0, 0, 0);
		
	}

	ZobVector3 la, lb, lc = ZobVector3(0, 0, 0);
	if (m_lightingPrecision == eLightingPrecision_vertex)
	{
		la = ComputeLightingAtPoint(t->va, t->na, t->options->lightMode);
		lb = ComputeLightingAtPoint(t->vb, t->nb, t->options->lightMode);
		lc = ComputeLightingAtPoint(t->vc, t->nc, t->options->lightMode);
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
		ZobVector2 v4 = ZobVector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, t, &la, &lb, &lc);
		FillTopFlatTriangle2(&v2, &v4, &v3, t, &la, &lb, &lc);
	}
//	m_drawnTriangles++;
}

void Rasterizer::FillBottomFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const
{
	uint* buffer = m_bufferData->buffer;
	ZobVector3 p;
	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);
	float curx1 = v1->x;
	float curx2 = v1->x;
	float a, b;
	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		int k;
		if (RenderLine(scanlineY) && scanlineY >= m_startHeight && scanlineY < m_endHeight)
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

void Rasterizer::FillTopFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const
{
	uint* buffer = m_bufferData->buffer;
	ZobVector3 p;
	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);
	float curx1 = v3->x;
	float curx2 = v3->x;
	float a, b;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		if (RenderLine(scanlineY) && scanlineY >= m_startHeight && scanlineY < m_endHeight)
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

inline const void Rasterizer::FillBufferPixel(const ZobVector3* p, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const
{
	float w0, w1, w2, su, tu, cl, sl, r, g, b, a, z, zRatio, fr, fg, fb, lightPower;
	float texPixelData[4];
	int c, k;
	ZobVector3 normal, lightDir;

	k = p->y * m_width + p->x;

	w2 = edgeFunction(t->pa, t->pb, p);
	w0 = edgeFunction(t->pb, t->pc, p);
	w1 = edgeFunction(t->pc, t->pa, p);

	w0 /= t->area;
	w1 /= t->area;
	w2 /= t->area;

	if (t->options->bTransparency)
	{
		if (((int)p->x  + ((int)p->y % 2))%2 == 0)
		{
			return;
		}
	}

	ZobVector3 tpos = ZobVector3(	(t->va->x * w0 + t->vb->x * w1 + t->vc->x * w2),
							(t->va->y * w0 + t->vb->y * w1 + t->vc->y * w2),
							(t->va->z * w0 + t->vb->z * w1 + t->vc->z * w2));

	const ZobMaterial* material = t->material;
	z = (t->pa->z * w0 + t->pb->z * w1 + t->pc->z * w2);
	zRatio = (z - m_bufferData->zNear ) / (m_bufferData->zFar - m_bufferData->zNear);
	
	float zf = m_bufferData->zBuffer[k];
	if ( zRatio >= 0.0f &&  (!t->options->zBuffered || zf < 0.0f  || zRatio < zf ))
	{
		if (t->options->bColorize)
		{
			if (((int)p->x + ((int)p->y % 2)) % 2 == 0)
			{
				c = ((int)(t->options->colorization.x * 255) << 16) + ((int)(t->options->colorization.y * 255) << 8) + (int)(t->options->colorization.z * 255);
				m_bufferData->buffer[k] = c;
				return;
			}
		}


		su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
		tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;

		cl = 1.0f;
		RenderOptions::eLightMode lighting = t->options->lightMode;
		if (lighting == RenderOptions::eLightMode_flat || lighting == RenderOptions::eLightMode_flatPhong)
		{
			normal = t->n;
		}
		else
		{
			normal = ZobVector3((w0 * t->na->x + w1 * t->nb->x + w2 * t->nc->x),
				(w0 * t->na->y + w1 * t->nb->y + w2 * t->nc->y),
				(w0 * t->na->z + w1 * t->nb->z + w2 * t->nc->z)); 
			//normal.Mul(-1.0f);
		}
		if (material)
		{
			const Texture* texture = material->GetDiffuseTexture();
			if (texture && texture->GetData())
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
				a = texPixelData[3];
				if (a == 0.0f)
				{
					return;
				}
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
			g = 0.0f;
			b = 1.0f;
			a = 1.0f;
		}
		m_bufferData->zBuffer[k] = zRatio;
		//lighting
		fr = 0.0f;
		fg = 0.0f;
		fb = 0.0f;

		if (m_lightingPrecision == eLightingPrecision_vertex)
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
		else if(m_lightingPrecision == eLightingPrecision_pixel)
		{
			ZobVector3 l = ComputeLightingAtPoint(&tpos, &normal, lighting);
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
		if (m_fogType == eFogType::eFogType_NoFog)
		{
			z = 1.0f;
		}
		else if (m_fogType == eFogType::eFogType_Linear)
		{
			z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
		}
		else if (m_fogType == eFogType::eFogType_Exp)
		{
			//z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
			z = exp(-m_fogDensity * zRatio);
		}
		else
		{
			//z = (m_bufferData->zFar - z) / (m_fogDistance - m_bufferData->zNear);
			z = exp(-pow(m_fogDensity*zRatio,2));
		}
		z = clamp2(abs(z), 0.0f, 1.0f);
		fr = fr * z + (1.0f - z) * m_fogColor->x;
		fg = fg * z + (1.0f - z) * m_fogColor->y;
		fb = fb * z + (1.0f - z) * m_fogColor->z;
		c = ((int)(fr * 255) << 16) + ((int)(fg * 255) << 8) + (int)(fb * 255);
		m_bufferData->buffer[k] = c;
	}
}

void Rasterizer::sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3) const
{
	if (v3->y < v2->y)
	{
		std::swap(*v2, *v3);
	}
	if (v2->y < v1->y) std::swap(*v1, *v2);
	if (v3->y < v2->y) std::swap(*v2, *v3);
}

void Rasterizer::sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, ZobVector2* uv1, ZobVector2* uv2, ZobVector2* uv3) const
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
