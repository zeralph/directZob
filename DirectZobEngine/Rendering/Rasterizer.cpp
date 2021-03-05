#include "Rasterizer.h"
#include "DirectZob.h"
#include "ZobObjects/Light.h"
#include "Texture.h"

static ZobVector3 sFog = ZobVector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

Rasterizer::Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_endHeight = endHeight;
	m_width = width;
	m_lines.clear();
	m_triangles.clear();
	m_time = 0.0f;
}

void Rasterizer::Init()
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
}

void Rasterizer::Start(const bool wireFrame, const eRenderMode renderMode, const bool bEvenFrame, const eLightingPrecision lp, ZobVector3 camForward)
{
	m_wireFrame = wireFrame;
	m_thread = std::thread(&Rasterizer::Render, this);
	m_bEvenFrame = bEvenFrame ? 1 : 0;
	m_renderMode = renderMode;
	m_lightingPrecision = lp;
	m_camDir = ZobVector3(-camForward.x, -camForward.y, -camForward.z);
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
	m_lines.clear();
	m_triangles.clear();
}

void Rasterizer::QueueTriangle(const Triangle* t)
{
	m_triangles.push_back(t);
}

void Rasterizer::QueueLine(const Line3D* l)
{
	m_lines.push_back(l);
}

void Rasterizer::Render() 
{
	m_tick = clock();
	//warning : invert lightdir ! https://fr.wikipedia.org/wiki/Ombrage_de_Phong
	m_lights.clear();
	LightManager* lm = DirectZob::GetInstance()->GetLightManager();
	if (lm)
	{
		m_lights = lm->GetActiveLights();
		m_ambientColor = lm->GetAmbientColor();
		m_ambientIntensity = lm->GetAmbientColorIntensity();
		m_fogColor = lm->GetFogColor();
		m_fogType = lm->GetFogType();
		m_fogDensity = lm->GetFogDensity();
		m_fogDistance = lm->GetFogDistance();
	}
	for (int i = 0; i < m_triangles.size(); i++)
	{
		const Triangle* t = m_triangles[i];
		//if (t->draw)
		{
			DrawTriangle(t);
		}
	}
	for (int i = 0; i < m_lines.size(); i++)
	{
		const Line3D* l = m_lines.at(i);
		DrawLine(l);
	}
	m_time = (float)(clock() - m_tick) / CLOCKS_PER_SEC * 1000;
}

void Rasterizer::plotLine(int x0, int y0, int x1, int y1) const
{

}

void Rasterizer::DrawLine(const Line3D* l) const
{	
	int x = (int)l->xa;
	int x2 = (int)l->xb;
	int y = (int)l->ya;
	int y2 = (int)l->yb;
	bool yLonger = false;
	int incrementVal;
	int shortLen = y2 - y;
	int longLen = x2 - x;
	float sz = l->za;
	float dz = l->zb - l->za;
	if (abs(shortLen) > abs(longLen)) 
	{
		int swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	if (longLen < 0)
	{
		incrementVal = -1;
	}
	else
	{
		incrementVal = 1;
	}
	double multDiff;
	int k;
	int px;
	int py;
	float zRatio;
	int s = m_bufferData->width * m_bufferData->height;
	if (longLen == 0.0)
	{
		multDiff = (double)shortLen;
	}
	else
	{
		multDiff = (double)shortLen / (double)longLen;
	}
	int watchDog = 0;
	dz = dz / fabsf((float)longLen);
	if (l->noZ)
	{
		sz = m_bufferData->zNear;
		dz = 0.0f;
	}
	if (yLonger) 
	{
		
		float z = sz;
		for (int i = 0; i != longLen; i += incrementVal) 
		{
			px = x + (int)((double)i * multDiff);
			py = y + i;
			if (px < m_bufferData->width && py < m_bufferData->height)
			{
				zRatio = (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				if (l->noZ)
				{
					zRatio = 0;
				}
				k = py * m_bufferData->width + px;
				if (m_bufferData->zBuffer[k] > zRatio)
				{
					m_bufferData->buffer[k] = l->c;
					m_bufferData->zBuffer[k] = zRatio;
					if (l->bold)
					{
						m_bufferData->buffer[k + 1] = l->c;
						m_bufferData->zBuffer[k + 1] = zRatio;
					}
				}
			}
			sz += dz;
			watchDog++;
			if (watchDog > 20000)
			{
				break;
			}
		}
	}
	else 
	{
		for (int i = 0; i != longLen; i += incrementVal) 
		{
			px = x + i;
			py = y + (int)((double)i * multDiff);
			k = py * m_bufferData->width + px;
			if (px < m_bufferData->width && py < m_bufferData->height)
			{
				zRatio = (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				k = py * m_bufferData->width + px;
				if (m_bufferData->zBuffer[k] > zRatio)
				{
					m_bufferData->buffer[k] = l->c;
					m_bufferData->zBuffer[k] = zRatio;
				}
			}
			sz += dz;
			watchDog++;
			if (watchDog > 20000)
			{
				break;
			}
		}
	}
}

inline ZobVector3 Rasterizer::ComputeLightingAtPoint(const ZobVector3* position, const ZobVector3* normal, RenderOptions::eLightMode lighting) const
{
	ZobVector3 outColor = ZobVector3(0, 0, 0);
	if (normal)
	{
		ZobVector3 lightDir = ZobVector3(0, 0, 0);
		float lightPower, cl, sl = 0.0f;
		for (std::vector<const Light*>::const_iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
		{
			const Light* l = (*iter);
			if (l->IsActive())
			{
				if (l->GetType() == Light::eLightType_directional)
				{
					lightDir = l->GetForward();
					//lightDir.Mul(-1.0f);
					//lightDir.Normalize();
					lightPower = l->GetIntensity();
				}
				else if(l->GetType() == Light::eLightType_point)
				{
					lightDir = position - l->GetWorldPosition();
					lightPower = 1.0f - (lightDir.sqrtLength() / l->GetFallOffDistance());
					lightPower = clamp2(lightPower, 0.0f, 1.0f) * l->GetIntensity();
					lightDir.Normalize();
				}
				else if (l->GetType() == Light::eLightType_spot)
				{
					lightDir = position - l->GetWorldPosition();
					lightPower = 1.0f - (lightDir.sqrtLength() / l->GetFallOffDistance());
					lightDir.Normalize();
					ZobVector3 fw = l->GetForward();
					float f = 1.0f - clamp2(ZobVector3::Dot(&fw, &lightDir), 0.0f, 1.0f);
					if (f > (l->GetSpotAngleRad() / 2.0f))
					{
						f = 0.0f;
					}
					else
					{
						f = 1.0f;
					}
					lightPower = clamp2(f * lightPower, 0.0f, 1.0f) * l->GetIntensity();
				}
				if (lightPower > 0.0f)
				{
					//Todo : specular intensity setup
					static int specularIntensity = 50;
					cl = computeLighting(normal, &lightDir);
					sl = 0.0f;
					if (lighting == RenderOptions::eLightMode_none)
					{
						cl = 0.0f;
						sl = 0.0f;
					}
					else if (lighting == RenderOptions::eLightMode_gouraud)
					{
						sl = computeSpecular(normal, &lightDir, &m_camDir, cl, specularIntensity);
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
	ZobVector2 v1 = ZobVector2((int)t->pa->x, (int)t->pa->y);
	ZobVector2 v2 = ZobVector2((int)t->pb->x, (int)t->pb->y);
	ZobVector2 v3 = ZobVector2((int)t->pc->x, (int)t->pc->y);
	ZobVector3 la, lb, lc = ZobVector3(0, 0, 0);
	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);
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

	//w1 = edgeFunction(t->pa, t->pb, p);
	//w2 = edgeFunction(t->pb, t->pc, p);
	//w0 = edgeFunction(t->pc, t->pa, p);

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

	const ZobMaterial* material = t->material;
	z = (t->pa->z * w0 + t->pb->z * w1 + t->pc->z * w2);
	if (!t->options->zBuffered)
	{
		zRatio = 0.0f;
	}
	else
	{
		zRatio = (z - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
	}
	
	float zf = m_bufferData->zBuffer[k];
	if ( zRatio >= 0.0f &&  (zf < 0.0f  || zRatio < zf ))
	{
		cl = 1.0f;
		RenderOptions::eLightMode lighting = t->options->lightMode;
		if (lighting == RenderOptions::eLightMode_flat || lighting == RenderOptions::eLightMode_flatPhong || lighting == RenderOptions::eLightMode_none)
		{
			normal = ZobVector3((t->na->x + t->nb->x +t->nc->x),
				(t->na->y + t->nb->y + t->nc->y),
				(t->na->z + t->nb->z + t->nc->z));
			normal.Div(3.0f);
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
				su = w0 * t->ua->x + w1 * t->ub->x + w2 * t->uc->x;
				tu = w0 * t->ua->y + w1 * t->ub->y + w2 * t->uc->y;
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
		//else
		{
			/*
			float ra = (float)((t->ca & 0xFF0000) >> 16) / 255.0f;
			float rb = (float)((t->cb & 0xFF0000) >> 16) / 255.0f;
			float rc = (float)((t->cc & 0xFF0000) >> 16) / 255.0f;
			float ga = (float)((t->ca & 0x00FF00) >> 8) / 255.0f;
			float gb = (float)((t->cb & 0x00FF00) >> 8) / 255.0f;
			float gc = (float)((t->cc & 0x00FF00) >> 8) / 255.0f;
			float ba = (float)((t->ca & 0x0000FF)) / 255.0f;
			float bb = (float)((t->cb & 0x0000FF)) / 255.0f;
			float bc = (float)((t->cc & 0x0000FF)) / 255.0f;
			r *= (w0 * ra + w1 * rb + w2 * rc);
			g *= (w0 * ga + w1 * gb + w2 * gc);
			b *= (w0 * ba + w1 * bb + w2 * bc);
			a = 1.0f;
			*/
			r *= (w0 * t->ca->x + w1 * t->cb->x + w2 * t->cc->x);
			g *= (w0 * t->ca->y + w1 * t->cb->y + w2 * t->cc->y);
			b *= (w0 * t->ca->z + w1 * t->cb->z + w2 * t->cc->z);
		}
		m_bufferData->zBuffer[k] = zRatio;
		if (lighting != RenderOptions::eLightMode_none)
		{
			//lighting ambient
			fr = r * (m_ambientColor->x * m_ambientIntensity);
			fg = g * (m_ambientColor->y * m_ambientIntensity);
			fb = b * (m_ambientColor->z * m_ambientIntensity);

			if (m_lightingPrecision == eLightingPrecision_vertex)
			{
				//Vertex lighting
				float w0t = 1.0f / 3.0f;
				float w1t = 1.0f / 3.0f;
				float w2t = 1.0f / 3.0f;
				if (lighting == RenderOptions::eLightMode_flat || lighting == RenderOptions::eLightMode_flatPhong)
				{
c					w0t = w0;
					w1t = w1;
					w2t = w2;
				}
				fr += (w0t * la->x + w1t * lb->x + w2t * lc->x) * r;
				fg += (w0t * la->y + w1t * lb->y + w2t * lc->y) * g;
				fb += (w0t * la->z + w1t * lb->z + w2t * lc->z) * b;
			}
			else if (m_lightingPrecision == eLightingPrecision_pixel)
			{
				ZobVector3 tpos = ZobVector3((t->va->x * w0 + t->vb->x * w1 + t->vc->x * w2),
					(t->va->y * w0 + t->vb->y * w1 + t->vc->y * w2),
					(t->va->z * w0 + t->vb->z * w1 + t->vc->z * w2));
				ZobVector3 l = ComputeLightingAtPoint(&tpos, &normal, lighting);
				fr += l.x * r;
				fg += l.y * g;
				fb += l.z * b;
			}
			fr = clamp2(fr, 0.0f, 1.0f);
			fg = clamp2(fg, 0.0f, 1.0f);
			fb = clamp2(fb, 0.0f, 1.0f);
			z = ComputeFog(z);
			//fr = fr * z + (1.0f - z) * m_fogColor->x;
			//fg = fg * z + (1.0f - z) * m_fogColor->y;
			//fb = fb * z + (1.0f - z) * m_fogColor->z;
			fr = fr * (1.0f - z) + z * m_fogColor->x;
			fg = fg * (1.0f - z) + z * m_fogColor->y;
			fb = fb * (1.0f - z) + z * m_fogColor->z;
		}
		else
		{
			fr = r;
			fg = g;
			fb = b;
		}
		c = ((int)(fr * 255) << 16) + ((int)(fg * 255) << 8) + (int)(fb * 255);
		m_bufferData->buffer[k] = c;
	}
}

const float Rasterizer::ComputeFog(float z) const
{
	float dz = /*(m_bufferData->zFar - m_bufferData->zNear) **/ m_fogDistance;
	z = fmaxf(0, z - dz);
	z = (m_bufferData->zFar - z) / (m_bufferData->zFar - m_bufferData->zNear);
	z = (1.0f - z);
	if (m_fogType == eFogType::eFogType_NoFog)
	{
		z = 0.0f;
	}
	else if (m_fogType == eFogType::eFogType_Linear)
	{
		z = clamp2(z * m_fogDensity, 0.0f, 1.0f);
	}
	else if (m_fogType == eFogType::eFogType_Exp)
	{
		z = 1.0f - clamp2(exp(-m_fogDensity * z), 0.0f, 1.0f);
		//z = exp(-m_fogDensity * z);
	}
	else
	{
		z = 1.0f - clamp2(exp(-pow(m_fogDensity * z, 2)), 0.0f, 1.0f);
		//z = exp(-pow(m_fogDensity * z, 2));
	}
	return z;// clamp2(z, 0.0f, 1.0f);
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
