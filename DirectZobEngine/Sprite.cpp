#include "Sprite.h"
#include "DirectZob.h"

using namespace std;

std::string emptyStr = std::string("");

Sprite::Sprite(std::string& name):Mesh(name, emptyStr, emptyStr)
{
	DirectZob::LogInfo("Sprite %s Creation", m_name.c_str());
	DirectZob::AddIndent();
	m_nbVertices = 4;
	m_nbUvs = 4;
	m_nbNormals = 4;
	m_nbFaces = 2;
	m_nbUvs = 4;
	m_material = NULL;

	m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_projectedVertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_projectedVerticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_nbNormals = m_nbVertices;
	m_verticesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_verticesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_verticesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
	m_trianglesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_trianglesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_trianglesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);

	m_vertices[0] = Vector3(-0.5f, 0.5f, 0.0f);
	m_vertices[1] = Vector3(0.5f, 0.5f, 0.0f);
	m_vertices[2] = Vector3(0.5f, -0.5f, 0.0f);
	m_vertices[3] = Vector3(-0.5f, -0.5f, 0.0f);

	m_verticesNormals[0] = Vector3(0, 0, -1);
	m_verticesNormals[1] = Vector3(0, 0, -1);
	m_verticesNormals[2] = Vector3(0, 0, -1);
	m_verticesNormals[3] = Vector3(0, 0, -1);

	m_trianglesNormals[0] = Vector3(0, 0, -1);
	m_trianglesNormals[1] = Vector3(0, 0, -1);

	m_uvs[0] = Vector2(0, 0);
	m_uvs[1] = Vector2(0, 1);
	m_uvs[2] = Vector2(1, 1);
	m_uvs[3] = Vector2(1, 0);

	Triangle t1;
	t1.va = &m_vertices[0];
	t1.vb = &m_vertices[1];
	t1.vc = &m_vertices[2];
	t1.pa = &m_projectedVertices[0];
	t1.pb = &m_projectedVertices[1];
	t1.pc = &m_projectedVertices[2];
	t1.na = &m_verticesNormals[0];
	t1.nb = &m_verticesNormals[1];
	t1.nc = &m_verticesNormals[2];
	t1.ua = &m_uvs[0];
	t1.ub = &m_uvs[1];
	t1.uc = &m_uvs[2];
	t1.material = NULL;
	m_triangles.push_back(t1);

	Triangle t2;
	t2.va = &m_vertices[0];
	t2.vb = &m_vertices[2];
	t2.vc = &m_vertices[3];
	t2.pa = &m_projectedVertices[0];
	t2.pb = &m_projectedVertices[2];
	t2.pc = &m_projectedVertices[3];
	t2.na = &m_verticesNormals[0];
	t2.nb = &m_verticesNormals[2];
	t2.nc = &m_verticesNormals[3];
	t2.ua = &m_uvs[0];
	t2.ub = &m_uvs[2];
	t2.uc = &m_uvs[3];
	t2.material = NULL;
	m_triangles.push_back(t2);

	memcpy(m_verticesData, m_vertices, sizeof(Vector3) * m_nbVertices);
	memcpy(m_verticesTmp, m_vertices, sizeof(Vector3) * m_nbVertices);
	memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(Vector3) * m_nbNormals);
	memcpy(m_verticesNormalsTmp, m_verticesNormals, sizeof(Vector3) * m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(Vector3) * m_nbFaces);
	memcpy(m_trianglesNormalsTmp, m_trianglesNormals, sizeof(Vector3) * m_nbFaces);
	DirectZob::RemoveIndent();
}

Sprite::~Sprite()
{

}
/*
void Sprite::QueueForDrawing(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options)
{
	Mesh::QueueForDrawing(m_modelMatrixOverriden, m_rotationMatrixOverriden, camera, engine, ownerId, options);
}


void Sprite::Update(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options)
{
	Mesh::Update(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
}
*/
void Sprite::Setmaterial(const Material* m) 
{ 
	m_material = m; 
	m_triangles[0].material = m;  
	m_triangles[1].material = m; 
}