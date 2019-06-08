#include "Cube.h"


Cube::Cube()
{
	m_modelMatrix.Identity();
}

Cube::~Cube()
{

}

void Cube::Init()
{
	m_modelMatrix.Identity();
	ReinitVertices();
}

void Cube::Draw(Engine* engine)
{
	Engine::BufferData* bData = engine->GetBufferData();

	Vector2 a, b, c;
	Matrix4x4 m = m_modelMatrix;
	const Camera* cam = engine->GetCamera();
	const Matrix4x4* p = cam->GetProjectionMatrix();
	for (int i = 0; i < NB_VERTICES; i++)
	{
		m_modelMatrix.Mul(&m_vertices[i]);
		p->Mul(&m_vertices[i]);
		//m_vertices[i].Normalize();
	}
	int w2 = bData->width / 2;
	int h2 = bData->height/ 2;
	const uint ca = Color::Cyan.GetRawValue();
	const uint cb = Color::Red.GetRawValue();
	for (int i = 0; i < NB_VERTICES_INDEX; i+=3)
	{
		int ia = m_verticesIndex[i];
		int ib = m_verticesIndex[i+1];
		int ic = m_verticesIndex[i+2];
		a.x = (m_vertices[ia].x + 1.0f) * (float)w2;
		a.y = (m_vertices[ia].y + 1.0f) * (float)h2;

		b.x = (m_vertices[ib].x + 1.0f) * (float)w2;
		b.y = (m_vertices[ib].y + 1.0f) * (float)h2;

		c.x = (m_vertices[ic].x + 1.0f) * (float)w2;
		c.y = (m_vertices[ic].y + 1.0f) * (float)h2;

		engine->DrawTriangle(&a, &b, &c, (i%2==0)?ca:cb, bData);
	}
}


const Vector3 Cube::m_verticesData[NB_VERTICES] = {	Vector3(-1.0f, 1.0f, -1.0f),
													Vector3(1.0f, 1.0f, -1.0f),
													Vector3(1.0f, -1.0f, -1.0f),
													Vector3(-1.0f, -1.0f, -1.0f),
													Vector3(-1.0f, 1.0f, 1.0f),
													Vector3(1.0f, 1.0f, 1.0f),
													Vector3(1.0f, -1.0f, 1.0f),
													Vector3(-1.0f, -1.0f, 1.0f)
												};
const uint Cube::m_verticesIndex[NB_VERTICES_INDEX] = {	0, 1, 3, 1, 2, 3,
														1, 5, 2, 5, 6, 2,
														5, 4, 6, 4, 7, 6,
														4, 0, 7, 0, 3, 7,
														4, 5, 0, 5, 1, 0,
														7, 6, 3, 6, 2, 3,
													};