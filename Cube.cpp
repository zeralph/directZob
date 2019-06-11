#include "Cube.h"


Cube::Cube(const Texture *tex)
{
	m_texture = tex;
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

inline void Cube::ReinitVertices() 
{ 
	memcpy(&m_vertices, &m_verticesData, sizeof(Vector3) * NB_VERTICES);
	memcpy(&m_normals, &m_normalsData, sizeof(Vector3) * NB_TRIANGLES);
};

void Cube::Draw(Engine* engine)
{
	Engine::BufferData* bData = engine->GetBufferData();

	Vector2 a, b, c;
	Matrix4x4 m = m_modelMatrix;
	const Camera* cam = engine->GetCamera();
	const Matrix4x4* p = cam->GetProjectionMatrix();
	const Vector3* camZ = cam->GetEyeVector();
	for (int i = 0; i < NB_VERTICES; i++)
	{
		m_modelMatrix.Mul(&m_vertices[i]);
		p->Mul(&m_vertices[i]);
	}
	for (int i = 0; i < NB_TRIANGLES; i++)
	{
		m_modelMatrix.Mul(&m_normals[i]);
	}
	int w2 = bData->width / 2;
	int h2 = bData->height/ 2;
	int triangleIndex = 0;
	uint color = 0;
	for (int i = 0; i < NB_VERTICES_INDEX; i+=3)
	//for (int i = 0; i < 3; i += 3)
	{
		triangleIndex = i / 3;
		if (Vector3::Dot(&m_normals[triangleIndex], camZ) < 0)
		{
			color = m_colorsData[triangleIndex];
			int ia = m_verticesIndex[i];
			int ib = m_verticesIndex[i + 1];
			int ic = m_verticesIndex[i + 2];
			a.x = (m_vertices[ia].x + 1.0f) * (float)w2;
			a.y = (m_vertices[ia].y + 1.0f) * (float)h2;

			b.x = (m_vertices[ib].x + 1.0f) * (float)w2;
			b.y = (m_vertices[ib].y + 1.0f) * (float)h2;

			c.x = (m_vertices[ic].x + 1.0f) * (float)w2;
			c.y = (m_vertices[ic].y + 1.0f) * (float)h2;

			engine->DrawTriangle2(&a, &b, &c, m_texture, bData);

			engine->DrawLine(a.x, a.y, b.x, b.y, 0xFF0000, bData);
			engine->DrawLine(b.x, b.y, c.x, c.y, 0x00FF00, bData);
			engine->DrawLine(c.x, c.y, a.x, a.y, 0x0000FF, bData);
		}
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

const Vector3 Cube::m_normalsData[NB_TRIANGLES] = {	Vector3(0.0f, 0.0f, -1.0f),
													Vector3(0.0f, 0.0f, -1.0f),
													Vector3(1.0f, 0.0f, 0.0f),
													Vector3(1.0f, 0.0f, 0.0f),
													Vector3(0.0f, 0.0f, 1.0f),
													Vector3(0.0f, 0.0f, 1.0f),
													Vector3(-1.0f, 0.0f, 0.0f),
													Vector3(-1.0f, 0.0f, 0.0f),
													Vector3(0.0f, 1.0f, 0.0f),
													Vector3(0.0f, 1.0f, 0.0f),
													Vector3(0.0f, -1.0f, 0.0f),
													Vector3(0.0f, -1.0f, 0.0f)
												};

const uint Cube::m_colorsData[NB_TRIANGLES] =	{	Color::Red.GetRawValue(),
													Color::Red.GetRawValue(),
													Color::Green.GetRawValue(),
													Color::Green.GetRawValue(),
													Color::Blue.GetRawValue(),
													Color::Blue.GetRawValue(),
													Color::Yellow.GetRawValue(),
													Color::Yellow.GetRawValue(),
													Color::Cyan.GetRawValue(),
													Color::Cyan.GetRawValue(),
													Color::Magenta.GetRawValue(),
													Color::Magenta.GetRawValue()
												};