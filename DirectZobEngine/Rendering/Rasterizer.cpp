#include "Rasterizer.h"
#include "DirectZob.h"
#include "ZobObjects/Light.h"
#include "Texture.h"
#undef None
#include "../../dependencies/optick/include/optick.h"

static ZobVector3 sFog = ZobVector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

volatile bool bStartDraw;

Rasterizer::Rasterizer(uint width, uint height, uint startHeight, uint endHeight, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_endHeight = endHeight;
	m_width = width;
	m_height = height;
	m_lines.clear();
	m_triangles.clear();
	m_time = 0.0f;
	m_runThread = true;
}

void Rasterizer::Init(std::condition_variable* cv, std::mutex *m)
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
	//m_drawMutex = drawMutex;
	m_startConditionVariable = cv;
	m_drawMutex = m;
	bStartDraw = false;
	m_thread = std::thread(&Rasterizer::Render, this);
	m_thread.detach();
}

void Rasterizer::Start()
{
	//OPTICK_EVENT();
	//const bool wireFrame, const eRenderMode renderMode, const bool bEvenFrame, const eLightingPrecision lp, ZobVector3 camForward)

	const Engine* e = DirectZob::GetInstance()->GetEngine();
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	bStartDraw = true;
	m_wireFrame = e->WireFrame();
	m_renderMode = e->GetRenderMode();
	m_lightingPrecision = e->GetLightingPrecision();
	m_bEvenFrame = (e->GetCurrentFrame() % 2 ==0)? 1 : 0;
	m_camPos = c->GetWorldPosition();
}

float Rasterizer::WaitForEnd() 
{
	//if (m_thread.joinable())
	//	m_thread.join();
	return m_time;
}

void Rasterizer::End() 
{
	m_runThread = false;
}

Rasterizer::~Rasterizer()
{
	End();
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
	OPTICK_THREAD("render thread");
	while (m_runThread)
	{
		std::unique_lock<std::mutex> g(*m_drawMutex);
		(*m_startConditionVariable).wait(g, [] { return bStartDraw; });
		RenderInternal();
		bStartDraw = false;
	}
}
void Rasterizer::RenderInternal()
{
	OPTICK_CATEGORY("Rendering", Optick::Category::Rendering);
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
				zRatio = m_bufferData->zNear + (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				if (l->noZ)
				{
					zRatio = m_bufferData->zNear;
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
				zRatio = m_bufferData->zNear + (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
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

inline void Rasterizer::ComputeLightingAtPoint(const ZobVector3* position, const ZobVector3* normal, const Triangle* t, LightingData& outlData) const
{
	outlData.Init();
	if (normal)
	{
		ZobVector3 lightDir = ZobVector3(0, 0, 0);
		ZobVector3 lightPos = ZobVector3(0, 0, 0);
		float lightPower, cl, sl = 0.0f;
		for (std::vector<const Light*>::const_iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
		{
			const Light* l = (*iter);
			if (l->IsActive())
			{
				lightPos = l->GetWorldPosition();
				if (l->GetType() == Light::eLightType_directional)
				{
					lightDir = l->GetForward();
					lightPos = l->GetForward();
					//for a directional light, simulate a far one
					lightPos.Mul(-10000.0f);
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
					cl = clamp2(-ZobVector3::Dot(normal, &lightDir), 0.0f, 1.0f);
					sl = 0.0f;
					if (t->options->lightMode == RenderOptions::eLightMode_none)
					{
						cl = 0.0f;
						sl = 0.0f;
					}
					else if (t->options->lightMode == RenderOptions::eLightMode_phong || t->options->lightMode == RenderOptions::eLightMode_flatPhong)
					{
						sl = computeSpecular(normal, &lightPos, position, t);
					}
					float a = l->GetColor()->x * lightPower;
					float b = l->GetColor()->y * lightPower;
					float c = l->GetColor()->z * lightPower;
					outlData.LightingDiffuseR += cl * a;
					outlData.LightingDiffuseG += cl * b;
					outlData.LightingDiffuseB += cl * c;
					outlData.LightingSpecularR += sl * a;
					outlData.LightingSpecularG += sl * b;
					outlData.LightingSpecularB += sl * c;
				}
			}
		}
	}
}

void Rasterizer::DrawTriangle(const Triangle* t) const
{
	ZobVector2 v1 = ZobVector2((int)t->pa->x, (int)t->pa->y);
	ZobVector2 v2 = ZobVector2((int)t->pb->x, (int)t->pb->y);
	ZobVector2 v3 = ZobVector2((int)t->pc->x, (int)t->pc->y);
	LightingData verticeALightingData, verticeBLightingData, verticeCLightingData;
	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(&v1, &v2, &v3);
	if (m_lightingPrecision == eLightingPrecision_vertex)
	{
		ComputeLightingAtPoint(t->va, t->na, t, verticeALightingData);
		ComputeLightingAtPoint(t->vb, t->nb, t, verticeBLightingData);
		ComputeLightingAtPoint(t->vc, t->nc, t, verticeCLightingData);
	}

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		FillBottomFlatTriangle2(&v1, &v2, &v3, t, &verticeALightingData, &verticeBLightingData, &verticeCLightingData);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		FillTopFlatTriangle2(&v1, &v2, &v3, t, &verticeALightingData, &verticeBLightingData, &verticeCLightingData);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		ZobVector2 v4 = ZobVector2((int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		FillBottomFlatTriangle2(&v1, &v2, &v4, t, &verticeALightingData, &verticeBLightingData, &verticeCLightingData);
		FillTopFlatTriangle2(&v2, &v4, &v3, t, &verticeALightingData, &verticeBLightingData, &verticeCLightingData);
	}
//	m_drawnTriangles++;
}

void Rasterizer::FillBottomFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const
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
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
			}
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void Rasterizer::FillTopFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const
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
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
						p.x = b - 1;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						p.z = -1;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

inline const void Rasterizer::FillBufferPixel(const ZobVector3* p, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const
{
	float wa, wb, wc, su, tu, cl, sl, dr, dg, db, sr, sg, sb, a, z, zRatio, fr, fg, fb, lightPower;
	float texPixelData[4];
	int c, k;
	ZobVector3 normal, lightDir;



	k = p->y * m_width + p->x;

	wa = edgeFunction(t->pb, t->pc, p);
	wb = edgeFunction(t->pc, t->pa, p);
	wc = edgeFunction(t->pa, t->pb, p);
	wa /= t->area;
	wb /= t->area;
	wc /= t->area;
	
	float w = (t->pa->w * wa + t->pb->w * wb + t->pc->w * wc);

	z = (t->pa->z * wa + t->pb->z * wb + t->pc->z * wc);
	//p->z = z;
	// 
	//if (p->x <0 || p->x > m_width - 1 || p->y <0 || p->y > m_height - 1)// || z <= (m_bufferData->zNear + 0.01f))
	//{
	//	return;
	//}

	if (t->options->bTransparency)
	{
		if (((int)p->x  + ((int)p->y % 2))%2 == 0)
		{
			return;
		}
	}

	const ZobMaterial* material = t->material;
	
	if (!t->options->zBuffered)
	{
		zRatio = m_bufferData->zNear;
	}
	else
	{
		zRatio = m_bufferData->zNear + (z - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
	}
	
	float zf = m_bufferData->zBuffer[k];
	if ( zRatio > 0.0f &&  (zf == 0.0f  || zRatio < zf ))
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
			normal = ZobVector3((wa * t->na->x + wb * t->nb->x + wc * t->nc->x),
				(wa * t->na->y + wb * t->nb->y + wc * t->nc->y),
				(wa * t->na->z + wb * t->nb->z + wc * t->nc->z)); 
			//normal.Mul(-1.0f);
		}
		//Start with vertex color
		dr = (wa * t->ca->x + wb * t->cb->x + wc * t->cc->x);
		dg = (wa * t->ca->y + wb * t->cb->y + wc * t->cc->y);
		db = (wa * t->ca->z + wb * t->cb->z + wc * t->cc->z);
		sr = 0.0f;
		sg = 0.0f;
		sb = 0.0f;
		a = 1.0f;
		if (material)
		{
			const Texture* texture = material->GetDiffuseTexture();
			if (texture && texture->GetData())
			{
				su = wa * t->ua->x + wb * t->ub->x + wc * t->uc->x;
				tu = wa * t->ua->y + wb * t->ub->y + wc * t->uc->y;
				tu = 1.0f - tu;
				su = (int)(su * texture->GetWidth());
				tu = (int)(tu * texture->GetHeight());
				su = (int)su % texture->GetWidth();
				tu = (int)tu % texture->GetHeight();

				c = (int)(((int)tu * (int)texture->GetWidth() + (int)su) * 4);
				const float* d = texture->GetData();
				memcpy(texPixelData, &d[c], sizeof(float) * 4);
				dr *= texPixelData[0];
				dg *= texPixelData[1];
				db *= texPixelData[2];
				a *= texPixelData[3];
				if (a == 0.0f)
				{
					return;
				}
			}
			else
			{
				dr *= material->GetDiffuseColor()->x;
				dg *= material->GetDiffuseColor()->y;
				db *= material->GetDiffuseColor()->z;
				a = 1.0f;
			}
			sr = material->GetSpecularColor()->x;
			sg = material->GetSpecularColor()->y;
			sb = material->GetSpecularColor()->z;
			sr = 1.0f;
			sg = 1.0f;
			sb = 1.0f;
		}
		if (lighting != RenderOptions::eLightMode_none)
		{
			//lighting ambient
			fr = dr * (m_ambientColor->x * m_ambientIntensity);
			fg = dg * (m_ambientColor->y * m_ambientIntensity);
			fb = db * (m_ambientColor->z * m_ambientIntensity);

			if (m_lightingPrecision == eLightingPrecision_vertex)
			{
				//Vertex lighting
				float w0t = wa;
				float w1t = wb;
				float w2t = wc;
				if (lighting == RenderOptions::eLightMode_flat || lighting == RenderOptions::eLightMode_flatPhong)
				{
					w0t = 1.0f / 3.0f;
					w1t = 1.0f / 3.0f;
					w2t = 1.0f / 3.0f;
				}
				fr += (w0t * lDataA->LightingDiffuseR + w1t * lDataB->LightingDiffuseR + w2t * lDataC->LightingDiffuseR) * dr;
				fg += (w0t * lDataA->LightingDiffuseG + w1t * lDataB->LightingDiffuseG + w2t * lDataC->LightingDiffuseG) * dg;
				fb += (w0t * lDataA->LightingDiffuseB + w1t * lDataB->LightingDiffuseB + w2t * lDataC->LightingDiffuseB) * db;
				fr += (w0t * lDataA->LightingSpecularR + w1t * lDataB->LightingSpecularR + w2t * lDataC->LightingSpecularR) * sr;
				fg += (w0t * lDataA->LightingSpecularG + w1t * lDataB->LightingSpecularG + w2t * lDataC->LightingSpecularG) * sg;
				fb += (w0t * lDataA->LightingSpecularB + w1t * lDataB->LightingSpecularB + w2t * lDataC->LightingSpecularB) * sb;
			}
			else if (m_lightingPrecision == eLightingPrecision_pixel)
			{
				ZobVector3 tpos = ZobVector3((t->va->x * wa + t->vb->x * wb + t->vc->x * wc),
					(t->va->y * wa + t->vb->y * wb + t->vc->y * wc),
					(t->va->z * wa + t->vb->z * wb + t->vc->z * wc));
				LightingData pixelLightingData;
				ComputeLightingAtPoint(&tpos, &normal, t, pixelLightingData);
				fr += pixelLightingData.LightingDiffuseR * dr;
				fg += pixelLightingData.LightingDiffuseG * dg;
				fb += pixelLightingData.LightingDiffuseB * db;
				fr += pixelLightingData.LightingSpecularR * sr;
				fg += pixelLightingData.LightingSpecularG * sg;
				fb += pixelLightingData.LightingSpecularB * sb;
			}
		}
		else
		{
			fr = dr;
			fg = dg;
			fb = db;
		}
		fr = clamp2(fr, 0.0f, 1.0f);
		fg = clamp2(fg, 0.0f, 1.0f);
		fb = clamp2(fb, 0.0f, 1.0f);
		z = ComputeFog(z);
		fr = fr * (1.0f - z) + z * m_fogColor->x;
		fg = fg * (1.0f - z) + z * m_fogColor->y;
		fb = fb * (1.0f - z) + z * m_fogColor->z;
		c = ((int)(fr * 255.0f) << 16) + ((int)(fg * 255.0f) << 8) + (int)(fb * 255.0f);
		if (zRatio < m_bufferData->zBuffer[k])
		{
			m_bufferData->buffer[k] = c;
			m_bufferData->zBuffer[k] = zRatio;
		}
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
