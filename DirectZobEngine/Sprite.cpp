#include "Sprite.h"
#include "DirectZob.h"

using namespace std;

std::string emptyStr = std::string("");

Sprite::Sprite(std::string& name):Mesh(name)
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
	m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_projectedVerticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_nbNormals = m_nbVertices;
	m_verticesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * m_nbUvs);
	m_trianglesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);

	m_vertices[0] = ZobVector3(-0.5f, 0.5f, 0.0f);
	m_vertices[1] = ZobVector3(0.5f, 0.5f, 0.0f);
	m_vertices[2] = ZobVector3(0.5f, -0.5f, 0.0f);
	m_vertices[3] = ZobVector3(-0.5f, -0.5f, 0.0f);

	m_vertices[0] = ZobVector3(-0.1f, 0.1f, 0.0f);
	m_vertices[1] = ZobVector3(0.1f, 0.1f, 0.0f);
	m_vertices[2] = ZobVector3(0.1f, -0.1f, 0.0f);
	m_vertices[3] = ZobVector3(-0.1f, -0.1f, 0.0f);

	m_verticesNormals[0] = ZobVector3(0, 0, -1);
	m_verticesNormals[1] = ZobVector3(0, 0, -1);
	m_verticesNormals[2] = ZobVector3(0, 0, -1);
	m_verticesNormals[3] = ZobVector3(0, 0, -1);

	m_trianglesNormals[0] = ZobVector3(0, 0, -1);
	m_trianglesNormals[1] = ZobVector3(0, 0, -1);

	m_uvs[0] = ZobVector2(0, 0);
	m_uvs[1] = ZobVector2(0, 1);
	m_uvs[2] = ZobVector2(1, 1);
	m_uvs[3] = ZobVector2(1, 0);

	m_triangleOptions.zBuffered = false;
	m_triangleOptions.lightMode = RenderOptions::eLightMode_none;

	Triangle t1;
	t1.va = &m_vertices[0];
	t1.vb = &m_vertices[1];
	t1.vc = &m_vertices[2];
	t1.verticeAIndex = 0;
	t1.verticeBIndex = 1;
	t1.verticeCIndex = 2;
	t1.pa = &m_projectedVertices[0];
	t1.pb = &m_projectedVertices[1];
	t1.pc = &m_projectedVertices[2];
	t1.na = &m_verticesNormals[0];
	t1.nb = &m_verticesNormals[1];
	t1.nc = &m_verticesNormals[2];
	t1.ua = &m_uvs[0];
	t1.ub = &m_uvs[1];
	t1.uc = &m_uvs[2];
	t1.n = &m_verticesNormals[0];
	t1.material = NULL;
	t1.options = &m_triangleOptions;
	t1.clipMode = Triangle::eClip_3_in;
	t1.area = -100;
	m_triangles.push_back(t1);

	Triangle t2;
	t2.va = &m_vertices[0];
	t2.vb = &m_vertices[2];
	t2.vc = &m_vertices[3];
	t2.verticeAIndex = 0;
	t2.verticeBIndex = 2;
	t2.verticeCIndex = 3;
	t2.pa = &m_projectedVertices[0];
	t2.pb = &m_projectedVertices[2];
	t2.pc = &m_projectedVertices[3];
	t2.na = &m_verticesNormals[0];
	t2.nb = &m_verticesNormals[2];
	t2.nc = &m_verticesNormals[3];
	t2.ua = &m_uvs[0];
	t2.ub = &m_uvs[2];
	t2.uc = &m_uvs[3];
	t2.n = &m_verticesNormals[0];
	t2.material = NULL;
	t2.options = &m_triangleOptions;
	t2.clipMode = Triangle::eClip_3_in;
	t2.area = 100;
	m_triangles.push_back(t2);

	memcpy(m_verticesData, m_vertices, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_verticesNormalsTmp, m_verticesNormals, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
	memcpy(m_trianglesNormalsTmp, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
	DirectZob::RemoveIndent();
}

Sprite::~Sprite()
{
	DirectZob::LogInfo("Delete sprite %s", m_name.c_str());
	DirectZob::AddIndent();
	free(m_indices);
	free(m_vertices);
	free(m_verticesData);
	free(m_verticesTmp);
	free(m_verticesNormals);
	free(m_verticesNormalsData);
	free(m_verticesNormalsTmp);
	free(m_trianglesNormals);
	free(m_trianglesNormalsTmp);
	free(m_trianglesNormalsData);
	free(m_projectedVertices);
	free(m_projectedVerticesTmp);
	free(m_uvs);
	m_indices = NULL;
	m_vertices = NULL;
	m_verticesData = NULL;
	m_verticesTmp = NULL;
	m_verticesNormals = NULL;
	m_verticesNormalsData = NULL;
	m_verticesNormalsTmp = NULL;
	m_trianglesNormals = NULL;
	m_trianglesNormalsTmp = NULL;
	m_trianglesNormalsData = NULL;
	m_projectedVertices = NULL;
	m_projectedVerticesTmp = NULL;
	m_uvs = NULL;
	DirectZob::RemoveIndent();
}

void Sprite::QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const RenderOptions* options)
{
	Mesh::QueueForDrawing(z, modelMatrix, rotationMatrix, camera, engine, options);
}

void Sprite::Setmaterial(const ZobMaterial* m)
{ 
	m_triangles[0].material = m;  
	m_triangles[1].material = m; 
}