#include "Mesh.h"
#include "DirectZob.h"
using namespace std;

Mesh::Mesh(std::string& name, std::string& path, const Texture* tex)
{
	m_name = name.c_str();
	m_texture = tex;
	m_nbVertices = 0;
	m_nbUvs = 0;
	m_nbNormals = 0;
	m_nbFaces = 0;
	m_hasNormals = false;
	m_vertices = NULL;
	m_normals = NULL;
	m_uvs = NULL;
	std::string s = "Load mesh " + std::string(path);
	DirectZob::Log(s);

	std::string::size_type sz;
	// Open the file.
	std::ifstream file(path, ios::in);
	std::string line;
	if (!file.is_open()) 
	{
		s = "ERROR";
		DirectZob::Log(s);
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
			vector<string> v;
			SplitEntry(&line, &v, ' ');
			m_nbFaces+= (uint)v.size() -3;
		}
	}
	file.clear();
	file.seekg(0, ios::beg);
	if (m_nbNormals > 0)
	{
		m_hasNormals = true;
	}
	else
	{
		m_nbNormals = 1;
	}
	m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
	m_normals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_normalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_triangles.clear();
	
	if (!m_hasNormals)
	{
		m_normals[0] = Vector3(0, 0, 1);
	}
	
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
			vector<string> v;
			SplitEntry(&line, &v, ' ');
			v.erase(v.begin());
			CreateTriangles(&v, &m_triangles, curface, tex);
		}
	}
	memcpy(m_verticesData, m_vertices, sizeof(Vector3) * m_nbVertices);
	memcpy(m_normalsData, m_normals, sizeof(Vector3) * m_nbNormals);

	s = "Mesh " + std::string(path) + " loaded";
	DirectZob::Log(s);
}

Mesh::~Mesh()
{
}

void Mesh::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim)
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
	m_modelMatrixRotationOnly.Identity();
	ReinitVertices();
}

void Mesh::ReinitVertices()
{
	memcpy(m_vertices, m_verticesData, sizeof(Vector3) * m_nbVertices);
	memcpy(m_normals, m_normalsData, sizeof(Vector3) * m_nbNormals);
}

void Mesh::Update(const Camera* camera, const BufferData* bData)
{
	Vector2 a, b, c, uva, uvb, uvc;
	const Matrix4x4* view = camera->GetViewMatrix();
	const Matrix4x4* proj = camera->GetProjectionMatrix();
	const Vector3* camZ = camera->GetForward();
	float w = (float)bData->width / 2.0f;
	float h = (float)bData->height / 2.0f;
	float znear = bData->zNear;
	float zfar = bData->zFar;
	for (uint i = 0; i < m_nbVertices; i++)
	{
		m_modelMatrix.Mul(&m_vertices[i]);
		view->Mul(&m_vertices[i]);
		proj->Mul(&m_vertices[i]);
		m_vertices[i].x = (m_vertices[i].x / m_vertices[i].z + 1) * w;
		m_vertices[i].y = (m_vertices[i].y / m_vertices[i].z + 1) * h;

	}
	for (uint i = 0; i < m_nbNormals; i++)
	{
		m_modelMatrixRotationOnly.Mul(&m_normals[i]);
		m_normals[i].Normalize();
	}
	int w2 = bData->width / 2;
	int h2 = bData->height / 2;
	int triangleIndex = 0;
	uint color = 0;
	Vector3 n;

	Vector3 light = Vector3(0.1f, 1.0f, 0.1f);
	light.Normalize();
	uint drawnFaces = 0;
	for (int i = 0; i < m_nbFaces; i ++)
	{
		Triangle* t = &m_triangles[i];
		n.Set(t->na);
		n.Add(t->nb);
		n.Add(t->nc);
		n.Div(3.0f);
		t->draw = false;
		if (!RejectTriangle(t, znear, zfar, (float)bData->width, (float)bData->height))
		{
			if (Vector3::Dot(&n, camZ) < 0.5f)
			{
				t->ComputeArea();
				static float a = 50000.0f;
				if (t->area > 0 && t->area < a)
				{
					t->ComputeLighting(&light);
					//engine->QueueTriangle(t);
					t->draw = true;
					drawnFaces++;
				}
				else
				{
					int tt = 0;
					tt++;;
				}
			}
		}
	}
	drawnFaces;
}

inline bool Mesh::RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height)
{
	if (t->va->x < 0 && t->vb->x < 0 && t->vc->x < 0)
	{
		return true;
	}
	if (t->va->x > width && t->vb->x > width && t->vc->x > width)
	{
		return true;
	}
	if (t->va->y > height && t->vb->y > height && t->vc->y > height)
	{
		return true;
	}
	if (t->va->y < 0 && t->vb->y < 0 && t->vc->y < 0)
	{
		return true;
	}
	if (t->va->w < znear || t->vb->w < znear || t->vc->w < znear)
	{
		return true;
	}
	if (t->va->w > zfar || t->vb->w > zfar || t->vc->w > zfar)
	{
		return true;
	}
	//if ((t->va->w > znear && t->vb->w > znear && t->vc->w > znear) && (t->va->w < zfar && t->vb->w < zfar && t->vc->w < zfar))
	return false;
}

void Mesh::CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* tList, size_t &tArrayIdx, const Texture* tex)
{
	size_t nbFaces = line->size() - 2;
	int a, b, c = 0;
	std::vector<std::string> vec;
	for (size_t i = 0; i < nbFaces; i++)
	{
		if (i % 2 == 0)
		{
			a = i + 0;
			b = i + 1;
			c = i + 2;
		}
		else
		{
			a = i - 1;
			b = i + 1;
			c = i + 2;
		}

		Triangle t;
		std::string::size_type sz;

		vec.clear();
		SplitEntry(&line->at(a), &vec, '/');
		t.va = &m_vertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ua = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ua = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.na = &m_normals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.na = &m_normals[0];
		}

		vec.clear();
		SplitEntry(&line->at(b), &vec, '/');
		t.vb = &m_vertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ub = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ub = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.nb = &m_normals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nb = &m_normals[0];
		}

		vec.clear();
		SplitEntry(&line->at(c), &vec, '/');
		t.vc = &m_vertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.uc = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.uc = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.nc = &m_normals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nc = &m_normals[0];
		}
		t.tex = tex;
		//
		tList->push_back(t);
		tArrayIdx++;
		
	}
}