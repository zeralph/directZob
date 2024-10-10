#include "Sprite.h"
#include "DirectZob.h"

using namespace std;

std::string emptyStr = std::string("");

ZobSprite::ZobSprite(std::string& name, ZobComponentSprite* zm):Mesh(name, zm)
{
	DirectZob::LogInfo("Sprite %s Creation", m_name.c_str());
	DirectZob::AddIndent();
	m_nbVertices = 4;
	m_nbUvs = 4;
	m_nbNormals = 4;
	m_nbFaces = 2;
	m_nbUvs = 4;

	m_minBoundingBox = ZobVector3(-0.5f, -0.5f, 0.0f);
	m_maxBoundingBox = ZobVector3(0.5f, 0.5f, 0.0f);
	m_indices = NULL;
	m_meshVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_triangleProjectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	//m_projectedVerticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_nbNormals = m_nbVertices;
	m_triangleVerticesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_triangleUvs = (ZobVector2*)malloc(sizeof(ZobVector2) * m_nbUvs);
	m_trianglesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_triangleColors = (ZobColor*)malloc(sizeof(ZobColor) * m_nbVertices);

	m_meshVertices[0] = ZobVector3(-0.5f, 0.5f, 0.0f);
	m_meshVertices[1] = ZobVector3(0.5f, 0.5f, 0.0f);
	m_meshVertices[2] = ZobVector3(0.5f, -0.5f, 0.0f);
	m_meshVertices[3] = ZobVector3(-0.5f, -0.5f, 0.0f);
/*
	m_vertices[0] = ZobVector3(-0.1f, 0.1f, 0.0f);
	m_vertices[1] = ZobVector3(0.1f, 0.1f, 0.0f);
	m_vertices[2] = ZobVector3(0.1f, -0.1f, 0.0f);
	m_vertices[3] = ZobVector3(-0.1f, -0.1f, 0.0f);
*/

	m_triangleColors[0] = ZobColor(255, 255, 255, 255);
	m_triangleColors[1] = ZobColor(255, 255, 255, 255);
	m_triangleColors[2] = ZobColor(255, 255, 255, 255);
	m_triangleColors[3] = ZobColor(255, 255, 255, 255);


	m_triangleVerticesNormals[0] = ZobVector3(0, 0, -1);
	m_triangleVerticesNormals[1] = ZobVector3(0, 0, -1);
	m_triangleVerticesNormals[2] = ZobVector3(0, 0, -1);
	m_triangleVerticesNormals[3] = ZobVector3(0, 0, -1);

	m_trianglesNormals[0] = ZobVector3(0, 0, -1);
	m_trianglesNormals[1] = ZobVector3(0, 0, -1);

	m_triangleUvs[2] = ZobVector2(0, 0);
	m_triangleUvs[1] = ZobVector2(0, 1);
	m_triangleUvs[0] = ZobVector2(1, 1);
	m_triangleUvs[3] = ZobVector2(1, 0);

	m_triangleOptions.zBuffer = Triangle::RenderOptions::buffered;
	m_triangleOptions.lightMode = Triangle::RenderOptions::eLightMode_none;
	m_triangleOptions.cullMode = Triangle::RenderOptions::eCullMode_CounterClockwiseFace;

	Triangle t1;
	t1.va = &m_meshVertices[0];
	t1.vb = &m_meshVertices[1];
	t1.vc = &m_meshVertices[2];
	t1.verticeAIndex = 0;
	t1.verticeBIndex = 1;
	t1.verticeCIndex = 2;
	t1.pa = &m_triangleProjectedVertices[0];
	t1.pb = &m_triangleProjectedVertices[1];
	t1.pc = &m_triangleProjectedVertices[2];
	t1.na = &m_triangleVerticesNormals[0];
	t1.nb = &m_triangleVerticesNormals[1];
	t1.nc = &m_triangleVerticesNormals[2];
	t1.ua = &m_triangleUvs[0];
	t1.ub = &m_triangleUvs[1];
	t1.uc = &m_triangleUvs[2];
	t1.ca = &m_triangleColors[0];
	t1.cb = &m_triangleColors[1];
	t1.cc = &m_triangleColors[2];
	t1.n = &m_triangleVerticesNormals[0];
	t1.material = NULL;
	t1.options = &m_triangleOptions;
	t1.clipMode = Triangle::eClip_3_in;
	t1.area = -100;
	t1.zobObject = NULL;
	m_triangles.push_back(t1);

	Triangle t2;
	t2.va = &m_meshVertices[0];
	t2.vb = &m_meshVertices[2];
	t2.vc = &m_meshVertices[3];
	t2.verticeAIndex = 0;
	t2.verticeBIndex = 2;
	t2.verticeCIndex = 3;
	t2.pa = &m_triangleProjectedVertices[0];
	t2.pb = &m_triangleProjectedVertices[2];
	t2.pc = &m_triangleProjectedVertices[3];
	t2.na = &m_triangleVerticesNormals[0];
	t2.nb = &m_triangleVerticesNormals[2];
	t2.nc = &m_triangleVerticesNormals[3];
	t2.ua = &m_triangleUvs[0];
	t2.ub = &m_triangleUvs[2];
	t2.uc = &m_triangleUvs[3];
	t2.ca = &m_triangleColors[0];
	t2.cb = &m_triangleColors[2];
	t2.cc = &m_triangleColors[3];
	t2.n = &m_triangleVerticesNormals[0];
	t2.material = NULL;
	t2.options = &m_triangleOptions;
	t2.clipMode = Triangle::eClip_3_in;
	t2.area = 100;
	t2.zobObject = NULL;
	m_triangles.push_back(t2);

	memcpy(m_verticesData, m_meshVertices, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesNormalsData, m_triangleVerticesNormals, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_verticesNormalsTmp, m_triangleVerticesNormals, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
	memcpy(m_trianglesNormalsTmp, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
	DirectZob::RemoveIndent();
}

ZobSprite::~ZobSprite()
{
	DirectZob::LogInfo("Delete sprite %s", m_name.c_str());
	DirectZob::AddIndent();
	free(m_indices);
	free(m_meshVertices);
	free(m_verticesData);
	free(m_verticesTmp);
	free(m_triangleVerticesNormals);
	free(m_verticesNormalsData);
	free(m_verticesNormalsTmp);
	free(m_trianglesNormals);
	free(m_trianglesNormalsTmp);
	free(m_trianglesNormalsData);
	free(m_triangleProjectedVertices);
	//free(m_projectedVerticesTmp);
	free(m_triangleUvs);
	m_indices = NULL;
	m_meshVertices = NULL;
	m_verticesData = NULL;
	m_verticesTmp = NULL;
	m_triangleVerticesNormals = NULL;
	m_verticesNormalsData = NULL;
	m_verticesNormalsTmp = NULL;
	m_trianglesNormals = NULL;
	m_trianglesNormalsTmp = NULL;
	m_trianglesNormalsData = NULL;
	m_triangleProjectedVertices = NULL;
	//m_projectedVerticesTmp = NULL;
	m_triangleUvs = NULL;
	DirectZob::RemoveIndent();
}

void ZobSprite::QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const Triangle::RenderOptions* options)
{
	Mesh::QueueForDrawing(z, modelMatrix, rotationMatrix, camera, engine, options);
}

void ZobSprite::Setmaterial(const ZobMaterial* m)
{ 
	m_triangles[0].material = m;  
	m_triangles[1].material = m; 
}