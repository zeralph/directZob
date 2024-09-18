#include "Rasterizer.h"
#include "DirectZob.h"
#include "ZobObjects/Light.h"
#include "../Managers/LightManager.h"
#include "../Managers/CameraManager.h"
#include "Texture.h"
#include "../ZobObjects/Camera.h"
#include <cstring>
#undef None
#include "../../dependencies/optick/include/optick.h"
#include "../Misc/ZobGeometryHelper.h"

static ZobVector3 sFog = ZobVector3(1.0f, 1.0f, 0.95f);
static float fogDecal = -0.6f;

const int BAYER_PATTERN_16X16[16][16] =   {   //  16x16 Bayer Dithering Matrix.  Color levels: 256
	{     0, 191,  48, 239,  12, 203,  60, 251,   3, 194,  51, 242,  15, 206,  63, 254  }, 
	{   127,  64, 175, 112, 139,  76, 187, 124, 130,  67, 178, 115, 142,  79, 190, 127  },
	{    32, 223,  16, 207,  44, 235,  28, 219,  35, 226,  19, 210,  47, 238,  31, 222  },
	{   159,  96, 143,  80, 171, 108, 155,  92, 162,  99, 146,  83, 174, 111, 158,  95  },
	{     8, 199,  56, 247,   4, 195,  52, 243,  11, 202,  59, 250,   7, 198,  55, 246  },
	{   135,  72, 183, 120, 131,  68, 179, 116, 138,  75, 186, 123, 134,  71, 182, 119  },
	{    40, 231,  24, 215,  36, 227,  20, 211,  43, 234,  27, 218,  39, 230,  23, 214  },
	{   167, 104, 151,  88, 163, 100, 147,  84, 170, 107, 154,  91, 166, 103, 150,  87  },
	{     2, 193,  50, 241,  14, 205,  62, 253,   1, 192,  49, 240,  13, 204,  61, 252  },
	{   129,  66, 177, 114, 141,  78, 189, 126, 128,  65, 176, 113, 140,  77, 188, 125  },
	{    34, 225,  18, 209,  46, 237,  30, 221,  33, 224,  17, 208,  45, 236,  29, 220  },
	{   161,  98, 145,  82, 173, 110, 157,  94, 160,  97, 144,  81, 172, 109, 156,  93  },
	{    10, 201,  58, 249,   6, 197,  54, 245,   9, 200,  57, 248,   5, 196,  53, 244  },
	{   137,  74, 185, 122, 133,  70, 181, 118, 136,  73, 184, 121, 132,  69, 180, 117  },
	{    42, 233,  26, 217,  38, 229,  22, 213,  41, 232,  25, 216,  37, 228,  21, 212  },
	{   169, 106, 153,  90, 165, 102, 149,  86, 168, 105, 152,  89, 164, 101, 148,  85  }
};
static ZobVector3 colors[16] =
{
	ZobVector3(1,0,0),
	ZobVector3(1,1,0),
	ZobVector3(1,0,1),

	ZobVector3(0,1,0),
	ZobVector3(0,1,1),
	ZobVector3(1,1,0),
	
	ZobVector3(0,1,1),
	ZobVector3(1,0,1),
	ZobVector3(0,1,1),

	ZobVector3(1,1,1),
	ZobVector3(0,0,0),

	ZobVector3(0.5,0,0),
	ZobVector3(0,0.5,0),
	ZobVector3(0,0,0.5),
	ZobVector3(0.5,0,0.5),
	ZobVector3(0,0.5,0.5),
};

Rasterizer::Rasterizer(uint width, uint height, uint startHeight, uint endHeight, uint MaxQueueSize, uint rasterizerNumber, BufferData* bufferData)
{
	m_startHeight = startHeight;
	m_bufferData = bufferData;
	m_endHeight = endHeight;
	m_width = width;
	m_height = height;
	m_maxTriangleQueueSize = MaxQueueSize;
	m_lines.clear();
	m_triangles = (const Triangle**)malloc(sizeof(Triangle*) * m_maxTriangleQueueSize);
	memset(m_triangles, 0, sizeof(Triangle*) * m_maxTriangleQueueSize);
	m_nbTriangles = 0;
	m_time = 0.0f;
	m_rasterizerNumber = rasterizerNumber;
	m_runThread = true;
	m_bStartDraw = false;

}

void Rasterizer::Init(std::condition_variable* cv, std::mutex *m)
{
	//m_thread = std::thread(&Rasterizer::Run, this);
	//m_thread.detach();
	//m_drawMutex = drawMutex;
	m_startConditionVariable = cv;
	m_drawMutex = m;
	m_bStartDraw = false;
	m_thread = std::thread(&Rasterizer::Render, this);
	m_thread.detach();
}

void Rasterizer::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
	const Engine* e = DirectZob::GetInstance()->GetEngine();
	int h = e->GetRasterizerHeight();
	m_startHeight = (m_rasterizerNumber-1) * h;
	m_endHeight = fminf(m_startHeight + h, height);
}

void Rasterizer::Start()
{
	//OPTICK_EVENT();
	//const bool wireFrame, const eRenderMode renderMode, const bool bEvenFrame, const eLightingPrecision lp, ZobVector3 camForward)

	const Engine* e = DirectZob::GetInstance()->GetEngine();
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	m_bStartDraw = true;
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
	m_nbTriangles = 0;
	//m_triangles.clear();
}

void Rasterizer::QueueTriangle(const Triangle* t)
{
	if (m_nbTriangles < m_maxTriangleQueueSize)
	{
		m_triangles[m_nbTriangles] = t;
		m_nbTriangles++;
	}
	//m_triangles.push_back(t);
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
		{
			std::unique_lock<std::mutex> g(*m_drawMutex);
			{
				(*m_startConditionVariable).wait(g, [this] { return m_bStartDraw; });
				if (DirectZob::GetInstance()->GetEngine()->RayTracingEnabled())
				{
					RenderInternalRayTrace();
				}
				else
				{
					RenderInternal();
				}
				m_bStartDraw = false;
			}
		}
	}
}
void Rasterizer::RenderInternalRayTrace()
{
	OPTICK_CATEGORY("Rendering", Optick::Category::Rendering);
	m_tick = clock();
	Camera* camera = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (camera)
	{
		float texPixelData[4];
		ZobVector3 intersection;
		float fr, fb, fg, fa;
		float fx, fy;
		uint c, k;
		uint startH = m_startHeight * m_width;
		uint rasterHeigth = m_endHeight - m_startHeight;
		float z, lastZ;
		float u, v;
		Ray ray;
		float fov = camera->GetFov();
		ZobVector3 camPos = camera->GetWorldPosition();
		ZobVector3 camFw = camera->GetForward();
		ZobMatrix4x4 mat = camera->GetViewMatrix();
		mat.FromVectors(camera->GetLeft(), camera->GetUp(), camera->GetForward());
		//mat.SetPosition(0, 0, 0);
		ray.p = camPos;
		static float zzz = -0.1f;
		float dx = fov / 90.0f;
		float dy = dx * (float)m_height / (float)m_width;
		ZobVector3 fw;
		const Triangle* t;
		const ZobMaterial* m;
		for (int y = m_startHeight; y < m_endHeight; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				fx = (x / (float)m_width * 2.0f - 1.0f) * dx;
				fy = (y / (float)m_height * 2.0f - 1.0f)* dy;
				fw = ZobVector3(-fx, -fy, 1.f);
				mat.Mul(&fw);
				fw.Normalize();
				ray.n = fw;
				k = y * m_width + x;
				lastZ = 1000000.0f;
				for (int l = 0; l < m_nbTriangles; l++)
				{
					t = m_triangles[l];
					bool ret = ZobGeometryHelper::RayTriangleIntersect(&ray, t, z, u, v);
					if (ret && z < lastZ)
					{					
						m = t->material;
						if (m)
						{
							const Texture* texture = m->GetDiffuseTexture();
							if (texture && texture->GetData())
							{
								float wa = u;
								float wb = v;
								float wc = 1 - u - v;
								float su = wa * t->ua->x + wb * t->ub->x + wc * t->uc->x;
								float tu = wa * t->ua->y + wb * t->ub->y + wc * t->uc->y;
								su = (su * (float)texture->GetWidth());
								tu = (tu * (float)texture->GetHeight());
								su = (int)floor(su) % texture->GetWidth();
								tu = (int)floor(tu) % texture->GetHeight();

								c = (int)(((int)tu * (int)texture->GetWidth() + (int)su) * 4);
								const float* d = texture->GetData();
								std::memcpy(texPixelData, &d[c], sizeof(float) * 4);
								fr = texPixelData[0] * 255.0f;
								fg = texPixelData[1] * 255.0f;
								fb = texPixelData[2] * 255.0f;
								fa = texPixelData[3] * 255.0f;
								if (fa == 0)
								{
									continue;
								}
							}
							else
							{
								fr = m->GetDiffuseColor()->GetRed();
								fg = m->GetDiffuseColor()->GetGreen();
								fb = m->GetDiffuseColor()->GetBlue();
								fr = fabsf(t->n->x) * 255.0f;
								fg = fabsf(t->n->y) * 255.0f;
								fb = fabsf(t->n->z) * 255.0f;
							}
							c = ((int)fr << 16) + ((int)fg << 8) + (int)fb;
							WriteInBuffer(k, c, 1);
						}
						lastZ = z;
					}
				}
			}
		}
	}
	m_time = (float)(clock() - m_tick) / CLOCKS_PER_SEC * 1000;
}
void Rasterizer::RenderInternal()
{
	OPTICK_CATEGORY("Rendering", Optick::Category::Rendering);
	m_tick = clock();
	//warning : invert lightdir ! https://fr.wikipedia.org/wiki/Ombrage_de_Phong
	LightManager* lm = DirectZob::GetInstance()->GetLightManager();

	if (lm)
	{
		m_activeLights = lm->GetActiveLights();
		m_editorLights = lm->GetEditorLight();
		m_ambientColor = lm->GetAmbientColor();
		m_ambientIntensity = lm->GetAmbientColorIntensity();
		m_fogColor = lm->GetFogColor();
		m_fogType = lm->GetFogType();
		m_fogDensity = lm->GetFogDensity();
		m_fogDistance = lm->GetFogDistance();
	}
	OPTICK_PUSH("triangles");
	for (int i = 0; i < m_nbTriangles; i++)
	{
		const Triangle* t = m_triangles[i];
		DrawTriangle(t);
	}
	OPTICK_POP();
	OPTICK_PUSH("Lines");
	for (int i = 0; i < m_lines.size(); i++)
	{
		const Line3D* l = m_lines.at(i);
		DrawLine(l);
	}
	OPTICK_POP();
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
		for (int i = 0; i != longLen; i += incrementVal) 
		{
			px = x + (int)((double)i * multDiff);
			py = y + i;
			if (px < m_bufferData->width && py < m_bufferData->height)
			{
				zRatio =  (sz);// / m_bufferData->zNear + (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				if (l->noZ)
				{
					zRatio = m_bufferData->zNear;
				}
				k = py * m_bufferData->width + px;
				if (m_bufferData->zBuffer[k] == 0 || m_bufferData->zBuffer[k] > zRatio )
				{
					WriteInBuffer(k, l->c, zRatio);
					if (l->bold)
					{
						WriteInBuffer(k, l->c, zRatio);
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
				zRatio = sz;// m_bufferData->zNear + (sz - m_bufferData->zNear) / (m_bufferData->zFar - m_bufferData->zNear);
				if (l->noZ)
				{
					zRatio = zRatio = m_bufferData->zNear;;
				}
				k = py * m_bufferData->width + px;
				if (m_bufferData->zBuffer[k]==0 || m_bufferData->zBuffer[k] > zRatio)
				{
					WriteInBuffer(k, l->c, zRatio);
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
		const std::vector<const Light*>* lightsVec = m_activeLights;
		if (t->zobObject->IsEditorObject())
		{
			lightsVec = m_editorLights;
		}
		for (std::vector<const Light*>::const_iterator iter = lightsVec->begin(); iter != lightsVec->end(); iter++)
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
					cl = 0.0f;
					sl = 0.0f;
					if (t->options->lightMode != Triangle::RenderOptions::eLightMode_none)
					{
						cl = CLAMP(-ZobVector3::Dot(normal, &lightDir), 0.0f, 1.0f);
						sl = 0.0f;
						if (t->options->lightMode == Triangle::RenderOptions::eLightMode_phong || t->options->lightMode == Triangle::RenderOptions::eLightMode_flatPhong)
						{
							sl = computeSpecular(normal, &lightPos, position, t);
						}
					}
					float a = l->GetColor()->GetRedNormalized() * lightPower;
					float b = l->GetColor()->GetGreenNormalized() * lightPower;
					float c = l->GetColor()->GetBlueNormalized() * lightPower;
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
	const Engine* engine = DirectZob::GetInstance()->GetEngineConst();

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
	ZobVector2 p;
	float invslope1 = (v2->x - v1->x) / (v2->y - v1->y);
	float invslope2 = (v3->x - v1->x) / (v3->y - v1->y);
	float curx1 = v1->x;
	float curx2 = v1->x;
	float a, b;
	for (int scanlineY = v1->y; scanlineY <= v2->y; scanlineY++)
	{
		int k;
		if (RenderLine(scanlineY))
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
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
						p.x = b - 1;
						p.y = scanlineY;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
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
	ZobVector2 p;
	float invslope1 = (v3->x - v1->x) / (v3->y - v1->y);
	float invslope2 = (v3->x - v2->x) / (v3->y - v2->y);
	float curx1 = v3->x;
	float curx2 = v3->x;
	float a, b;
	for (int scanlineY = v3->y; scanlineY > v1->y; scanlineY--)
	{
		if (RenderLine(scanlineY))
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
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
						p.x = b - 1;
						p.y = scanlineY;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
				else
				{
					for (int i = a; i < b; i++)
					{
						p.x = i;
						p.y = scanlineY;
						FillBufferPixel(&p, t, lDataA, lDataB, lDataC);
					}
				}
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

inline const void Rasterizer::FillBufferPixel(const ZobVector2* screenCoord, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const
{
	float wa, wb, wc, su, tu, cl, sl, dr, dg, db, sr, sg, sb, a, z, fr, fg, fb, lightPower;
	float texPixelData[4];
	int c, k;
	ZobVector3 normal, lightDir;

	const Engine* engine = DirectZob::GetInstance()->GetEngineConst();

	k = screenCoord->y * m_width + screenCoord->x;

	wa = edgeFunction(t->pb, t->pc, screenCoord);
	wb = edgeFunction(t->pc, t->pa, screenCoord);
	wc = edgeFunction(t->pa, t->pb, screenCoord);
	wa /= t->area;
	wb /= t->area;
	wc /= t->area;
	
	float w = (t->pa->w * wa + t->pb->w * wb + t->pc->w * wc);

	switch (t->options->zBuffer)
	{
		case Triangle::RenderOptions::buffered:
		{
			z = t->pa->z * wa + t->pb->z * wb + t->pc->z * wc;
			break;
		}
		case Triangle::RenderOptions::unBuffered:
		{
			z = m_bufferData->zNear;
			break;
		}
		case Triangle::RenderOptions::halfBuffered:
		{
			if (((int)screenCoord->x + (int)screenCoord->y) % 2 == 0)
			{
				z = m_bufferData->zNear;
			}
			else
			{
				z = t->pa->z * wa + t->pb->z * wb + t->pc->z * wc;
			}
			break;
		}
		case Triangle::RenderOptions::noZFar:
		{
			z = m_bufferData->zFar - 0.0001f;
		}
	}
	const ZobMaterial* material = t->material;
	float zf = m_bufferData->zBuffer[k] == 0.0f ? 10000.0f: m_bufferData->zBuffer[k];
	float transparency = (t->options->bTransparency) ? 0.5f: 1.0f;
	if(z>0 && z < zf)
	{
		//m_bufferData->zBuffer[k] = -1.0f;
		cl = 1.0f;
		Triangle::RenderOptions::eLightMode lighting = t->options->lightMode;
		if (lighting == Triangle::RenderOptions::eLightMode_flat || lighting == Triangle::RenderOptions::eLightMode_flatPhong || lighting == Triangle::RenderOptions::eLightMode_none)
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
		dr = (wa * t->ca->GetRedNormalized() + wb * t->cb->GetRedNormalized() + wc * t->cc->GetRedNormalized());
		dg = (wa * t->ca->GetGreenNormalized() + wb * t->cb->GetGreenNormalized() + wc * t->cc->GetGreenNormalized());
		db = (wa * t->ca->GetBlueNormalized() + wb * t->cb->GetBlueNormalized() + wc * t->cc->GetBlueNormalized());
		sr = 0.0f;
		sg = 0.0f;
		sb = 0.0f;
		a = 1.0f;
		if (material)
		{
			//transparency *= material->GetTransparency();
			const Texture* texture = material->GetDiffuseTexture();
			if (texture && texture->GetData())
			{
				su = wa * t->ua->x + wb * t->ub->x + wc * t->uc->x;
				tu = wa * t->ua->y + wb * t->ub->y + wc * t->uc->y;
				if(engine->UsePerspectiveCorrection())
				{
					float zu = 1.0f / (wa * 1.0f / t->pa->z + wb * 1.0f / t->pb->z + wc * 1.0f / t->pc->z); 
					//t->pa->z = 1.0f / t->pa->z;
					//t->pb->z = 1.0f / t->pb->z;
					//t->pc->z = 1.0f / t->pc->z;	
					su *= zu;
					tu *= zu;	
					tu = 1.0f - tu;				
				}	
				else
				{
					tu = 1.0f - tu;
				}			
				su = (su * (float)texture->GetWidth());
				tu = (tu * (float)texture->GetHeight());
				su = (int)floor(su) % texture->GetWidth();
				tu = (int)floor(tu) % texture->GetHeight();

				c = (int)(((int)tu * (int)texture->GetWidth() + (int)su) * 4);
				const float* d = texture->GetData();
				std::memcpy(texPixelData, &d[c], sizeof(float) * 4);
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
				dr *= material->GetDiffuseColor()->GetRedNormalized();
				dg *= material->GetDiffuseColor()->GetGreenNormalized();
				db *= material->GetDiffuseColor()->GetBlueNormalized();
				a = 1.0f;
			}
			sr = material->GetSpecularColor()->GetRedNormalized();
			sg = material->GetSpecularColor()->GetGreenNormalized();
			sb = material->GetSpecularColor()->GetBlueNormalized();
		}
		if (lighting != Triangle::RenderOptions::eLightMode_none)
		{
			//lighting ambient
			fr = dr * (m_ambientColor->GetRedNormalized() * m_ambientIntensity);
			fg = dg * (m_ambientColor->GetGreenNormalized() * m_ambientIntensity);
			fb = db * (m_ambientColor->GetBlueNormalized() * m_ambientIntensity);

			if (m_lightingPrecision == eLightingPrecision_vertex)
			{
				//Vertex lighting
				float w0t = wa;
				float w1t = wb;
				float w2t = wc;
				if (lighting == Triangle::RenderOptions::eLightMode_flat || lighting == Triangle::RenderOptions::eLightMode_flatPhong)
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
		fr = CLAMP(fr, 0.0f, 1.0f) * t->options->color.GetRedNormalized();
		fg = CLAMP(fg, 0.0f, 1.0f) * t->options->color.GetGreenNormalized();
		fb = CLAMP(fb, 0.0f, 1.0f) * t->options->color.GetBlueNormalized();
		float fog = ComputeFog(z);
		fr = fr * (1.0f - fog) + fog * m_fogColor->GetRedNormalized();
		fg = fg * (1.0f - fog) + fog * m_fogColor->GetGreenNormalized();
		fb = fb * (1.0f - fog) + fog * m_fogColor->GetBlueNormalized();
		static int modulo = 2;
		if (transparency < 1.0f)
		{
			//int t = (int)(0.25f * p->x + 0.5f * p->y) % modulo * (int)(p->y) % modulo;
			//if(!t)
			if (((int)screenCoord->x  + (int)screenCoord->y) % 2 == 0)
			{
				return;
			}
		}

		const int ncolors = (int)engine->GetNbBitsPerColorDepth();
		if(ncolors != 0)
		{
			int divider = 256 / ncolors;
			int corr = divider;
			if (engine->UseDithering())
			{
				const int   row = (int)screenCoord->y & 15;   //  y % 16
				const int   col = (int)screenCoord->x & 15;   //  x % 16
				const int   t       = BAYER_PATTERN_16X16[col][row];
				corr    = (t / ncolors);
			}
			const int   blue = (int)(fb * 255.0f);
			const int   green = (int)(fg * 255.0f);
			const int   red = (int)(fr * 255.0f);
			int i1  = (blue  + corr) / divider; i1 = CLAMP( i1, 0, ncolors );
			int i2  = (green + corr) / divider; i2 = CLAMP( i2, 0, ncolors );
			int i3  = (red   + corr) / divider; i3 = CLAMP( i3, 0, ncolors );

			//  If you want to compress the image, use the values of i1,i2,i3
			//  they have values in the range 0..ncolors
			//  So if the ncolors is 4 - you have values: 0,1,2,3 which is encoded in 2 bits
			//  2 bits for 3 planes == 6 bits per pixel

			fb   = CLAMP( i1 * divider, 0, 255 );  //  blue
			fg   = CLAMP( i2 * divider, 0, 255 );  //  green
			fr  = CLAMP( i3 * divider, 0, 255 );  //  red
		}
		else
		{
			fb = (int)(fb * 255.0f);
			fg = (int)(fg * 255.0f);
			fr = (int)(fr * 255.0f);
		}
		
		if (engine->DebugCulling() && t->clipMode != Triangle::eClip::eClip_3_in)
		{
			ZobColor cc = ZobColor::White;
			if (t->clipMode == Triangle::eClip::eClip_A_in_BC_out) { cc = ZobColor::Blue; };
			if (t->clipMode == Triangle::eClip::eClip_B_in_AC_out) { cc = ZobColor::Cyan; };
			if (t->clipMode == Triangle::eClip::eClip_C_in_AB_out) { cc = ZobColor::Green; };
			if (t->clipMode == Triangle::eClip::eClip_AB_in_C_out) { cc = ZobColor::Yellow; };
			if (t->clipMode == Triangle::eClip::eClip_AC_in_B_out) { cc = ZobColor::Magenta; };
			if (t->clipMode == Triangle::eClip::eClip_BC_in_A_out) { cc = ZobColor::White; };
			if (t->clipMode == Triangle::eClip::eClip_3_out_corner) 
			{ 
				cc = ZobColor::Red; 
			};
			fr = cc.GetRed();
			fg = cc.GetGreen();
			fb = cc.GetBlue();
		}
		
		c = ((int)fr << 16) + ((int)fg << 8) + (int)fb;
		WriteInBuffer(k, c, z);
	}
}

void Rasterizer::WriteInBuffer(int k, int c, float z) const
{
	//if (k < m_bufferData->width * m_bufferData->height)
	{
		m_bufferData->buffer[k] = c;
		m_bufferData->zBuffer[k] = z;
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

bool Rasterizer::RenderLine(int line) const
{
	bool bRender = (m_renderMode == eRenderMode_fullframe) ||
		(m_renderMode == eRenderMode_interlaced && line % 2 == m_bEvenFrame) ||
		(m_renderMode == eRenderMode_scanline && line % 2 == 0);
	const Engine* e = DirectZob::GetInstance()->GetEngineConst();
	if ( e->EqualizeTriangleQueues())
	{
		bRender &= (line >= 0 && line < m_height);
	}
	else
	{
		bRender &= (line >= m_startHeight && line < m_endHeight);
	}
	return bRender;
}