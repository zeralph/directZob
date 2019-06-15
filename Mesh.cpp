#include "Mesh.h"


using namespace std;

Mesh::Mesh(const std::string* path, const Texture* tex)
{
	std::cout << "Load mesh " << path->c_str();
	m_texture = tex;
	m_nbVertices = 0;
	m_nbUvs = 0;
	m_nbNormals = 0;
	m_nbFaces = 0;
	m_vertices = NULL;
	std::string::size_type sz;
	// Open the file.
	std::ifstream file(path->c_str(), ios::in);
	std::string line;
	if (!file.is_open()) {
		std::cout << " ERROR\n";
		return; 
	}
	while (getline(file, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 't')
				m_nbUvs++;
			else if (line[1] == 'n')
				m_nbNormals++;
			else if (line[1] == ' ')
				m_nbVertices++;
		}
		else if (line[0] == 'f')
		{
			m_nbFaces++;
		}
	}
	file.clear();
	file.seekg(0, ios::beg);

	m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
	m_normals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_normalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_triangles = (Engine::Triangle*)malloc(sizeof(Engine::Triangle) * m_nbFaces);
	
	
	size_t curVertice = 0;
	size_t curNormal = 0;
	size_t curUv = 0;
	size_t curface = 0;
	while (getline(file, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				Vector2 v = Vector2(std::stof(vec[1], &sz), ::stof(vec[2], &sz));
				m_uvs[curUv] = v;
				curUv++;
			}
			else if (line[1] == 'n')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				Vector3 v = Vector3(std::stof(vec[1], &sz), ::stof(vec[2], &sz), ::stof(vec[3], &sz));
				m_normals[curNormal] = v;
				curNormal++;
			}
			else if (line[1] == ' ')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				Vector3 v = Vector3(std::stof(vec[1], &sz), ::stof(vec[2], &sz), ::stof(vec[3], &sz));
				m_vertices[curVertice] = v;
				curVertice++;
			}
		}
		else if (line[0] == 'f')
		{
			std::vector<string> vec;
			std::vector<string> vec2;
			SplitEntry(&line, &vec, ' ');
			Engine::Triangle t;

			SplitEntry(&vec[1], &vec2, '/');
			t.va = &m_vertices[std::stoi(vec2[0], &sz)-1];
			t.ua = &m_uvs[std::stoi(vec2[1], &sz)-1];
			t.na = &m_normals[std::stoi(vec2[2], &sz)-1];

			vec2.clear();
			SplitEntry(&vec[2], &vec2, '/');
			t.vb = &m_vertices[std::stoi(vec2[0], &sz)-1];
			t.ub = &m_uvs[std::stoi(vec2[1], &sz)-1];
			t.nb = &m_normals[std::stoi(vec2[2], &sz)-1];

			vec2.clear();
			SplitEntry(&vec[3], &vec2, '/');
			t.vc = &m_vertices[std::stoi(vec2[0], &sz)-1];
			t.uc = &m_uvs[std::stoi(vec2[1], &sz)-1];
			t.nc = &m_normals[std::stoi(vec2[2], &sz)-1];

			m_triangles[curface] = t;
			curface++;
		}
	}
	memcpy(m_verticesData, m_vertices, sizeof(Vector3) * m_nbVertices);
	memcpy(m_normalsData, m_normals, sizeof(Vector3) * m_nbNormals);

	std::cout << " OK\n";
}

Mesh::~Mesh()
{
}

void Mesh::SplitEntry(std::string* s, std::vector<std::string>* v, char delim)
{
	std::size_t current, previous = 0;
	current = s->find(delim);
	while (current != std::string::npos) {
		v->push_back(s->substr(previous, current - previous));
		previous = current + 1;
		current = s->find(delim, previous);
	}
	v->push_back(s->substr(previous, current - previous));
}

void Mesh::Init()
{
	m_modelMatrix.Identity();
	ReinitVertices();
}

void Mesh::ReinitVertices()
{
	memcpy(m_vertices, m_verticesData, sizeof(Vector3) * m_nbVertices);
	memcpy(m_normals, m_normalsData, sizeof(Vector3) * m_nbNormals);
}

void Mesh::Draw(Engine* engine)
{
	Engine::BufferData* bData = engine->GetBufferData();

	Vector2 a, b, c, uva, uvb, uvc;
	const Camera* cam = engine->GetCamera();
	const Matrix4x4* p = cam->GetProjectionMatrix();
	const Matrix4x4* s = cam->GetScreenMatrix();
	const Vector3* camZ = cam->GetEyeVector();
	float w = (float)bData->width / 2.0f;
	float h = (float)bData->height / 2.0f;
	for (int i = 0; i < m_nbVertices; i++)
	{
		m_modelMatrix.Mul(&m_vertices[i]);
		p->Mul(&m_vertices[i]);
		s->Mul(&m_vertices[i]);
		//m_vertices[i].x = (m_vertices[i].x + 1.0f) * w;
		//m_vertices[i].y = (m_vertices[i].y + 1.0f) * h;
	}
	for (int i = 0; i < m_nbNormals; i++)
	{
		m_modelMatrix.Mul(&m_normals[i]);
	}
	int w2 = bData->width / 2;
	int h2 = bData->height / 2;
	int triangleIndex = 0;
	uint color = 0;
	Vector3 n;
	for (int i = 0; i < m_nbFaces; i ++)
	{
		Engine::Triangle* t = &m_triangles[i];
		//n = t->na + t->nb;
//TODO : only one normal
		if (Vector3::Dot(t->na, camZ) < 0)
		{
			t->ComputeArea();
			engine->DrawTriangle2(t, m_texture, bData);
		}
	}
}