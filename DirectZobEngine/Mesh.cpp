#include "Mesh.h"
#include "DirectZob.h"
using namespace std;

Mesh::Mesh(std::string& name, std::string& path, std::string& file)
{	
	m_nbVertices = 0;
	m_nbUvs = 0;
	m_nbNormals = 0;
	m_nbFaces = 0;
	m_hasNormals = false;
	m_vertices = NULL;
	m_projectedVertices = NULL;
	m_verticesNormals = NULL;
	m_trianglesNormals = NULL;
	m_uvs = NULL;
	m_name = name;
	m_file = file;
	std::string fullPath = path;
	fullPath.append(file);
	std::string s = "Load mesh " + std::string(fullPath);
	DirectZob::LogInfo(s.c_str());

	static std::string sMtllib = std::string("mtllib");

	std::string::size_type sz;
	// Open the file.
	std::ifstream sfile(fullPath, ios::in);
	std::string line;
	if (!sfile.is_open()) 
	{
		s = "Error opening ";
		s.append(fullPath);
		DirectZob::LogError(s.c_str());
		return;
	}
	while (getline(sfile, line))
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
		else if (line.rfind("mtllib",0) == 0)
		{
			vector<string> v;
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				DirectZob::GetInstance()->GetmaterialManager()->LoadMaterials(path, v[1]);
			}
		}
	}
	sfile.clear();
	sfile.seekg(0, ios::beg);
	if (m_nbNormals > 0)
	{
		m_hasNormals = true;
	}
	else
	{
		m_nbNormals = 1;
	}
	m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_projectedVertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
	m_verticesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_verticesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_trianglesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_trianglesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_triangles.clear();
	
	if (!m_hasNormals)
	{
		//TODO ... compute !
		m_verticesNormals[0] = Vector3(0, 0, 1);
		m_trianglesNormals[0] = Vector3(0, 0, 1);
	}
	
	size_t curVertice = 0;
	size_t curNormal = 0;
	size_t curUv = 0;
	size_t curface = 0;
	const Material* tex = NULL;
	while (getline(sfile, line))
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
				m_verticesNormals[curNormal] = v;
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
		else if (line.rfind("usemtl", 0) == 0)
		{
			vector<string> v;
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				std::string matName = file;
				matName = matName.substr(0, matName.size() - 4);
				matName.append(".");
				matName.append(v[1]);
				tex = DirectZob::GetInstance()->GetmaterialManager()->GetTexture(matName);
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
	memcpy(m_verticesData, m_vertices, sizeof(Vector3)* m_nbVertices);
	memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(Vector3)* m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(Vector3)* m_nbFaces);
	s = "Mesh loaded : " + std::string(fullPath);
	DirectZob::LogInfo(s.c_str());
}

Mesh::~Mesh()
{
	m_triangles.clear();
	delete m_vertices;
	delete m_verticesData;
	delete m_uvs;
	delete m_verticesNormals;
	delete m_verticesNormalsData;
	delete m_trianglesNormals;
	delete m_trianglesNormalsData;
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

/*void Mesh::Update()
{
	m_modelMatrix.Identity();
	m_modelMatrixRotationOnly.Identity();
	m_modelMatrix.SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_modelMatrix.SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);
	m_modelMatrix.SetTranslation(m_translation.x, m_translation.y, m_translation.z);
	m_modelMatrixRotationOnly.SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);
	ReinitVertices();
}*/

void Mesh::ReinitVertices()
{
	memcpy(m_vertices, m_verticesData, sizeof(Vector3) * m_nbVertices);
	memcpy(m_verticesNormals, m_verticesNormalsData, sizeof(Vector3) * m_nbNormals);
	memcpy(m_trianglesNormals, m_trianglesNormalsData, sizeof(Vector3) * m_nbFaces);
}

void Mesh::Draw(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions options)
{
	ReinitVertices();
	BufferData* bData = engine->GetBufferData();
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
		modelMatrix.Mul(&m_vertices[i]);
		
		m_projectedVertices[i].x = m_vertices[i].x;
		m_projectedVertices[i].y = m_vertices[i].y;
		m_projectedVertices[i].z = m_vertices[i].z;
		m_projectedVertices[i].w = m_vertices[i].w;

		view->Mul(&m_projectedVertices[i]);
		proj->Mul(&m_projectedVertices[i]);
		m_projectedVertices[i].x = (m_projectedVertices[i].x / m_projectedVertices[i].z + 1) * w;
		m_projectedVertices[i].y = (m_projectedVertices[i].y / m_projectedVertices[i].z + 1) * h;
		//m_projectedVertices[i].z = m_vertices[i].z;
		//m_projectedVertices[i].w = m_vertices[i].w;
	}
	for (uint i = 0; i < m_nbNormals; i++)
	{
		rotationMatrix.Mul(&m_verticesNormals[i]);
		m_verticesNormals[i].Normalize();
	}
	for (uint i = 0; i < m_nbFaces; i++)
	{
		rotationMatrix.Mul(&m_trianglesNormals[i]);
		m_trianglesNormals[i].Normalize();
	}
	int w2 = bData->width / 2;
	int h2 = bData->height / 2;
	int triangleIndex = 0;
	uint color = 0;
	Vector3 n;
	Vector3 light = Vector3(0.1f, 1.0f, 0.1f);
	light.Normalize();
	uint drawnFaces = 0;
	for (int i = 0; i < m_nbFaces; i++)
	{
		Triangle* t = &m_triangles[i];
		t->draw = false;
		t->options.ZBuffered(options.ZBuffered());
		t->options.LightMode(options.LightMode());
		if (!RejectTriangle(t, znear, zfar, (float)bData->width, (float)bData->height))
		{
			//t->n->Set(t->na);
			//t->n->Add(t->nb);
			//t->n->Add(t->nc);
			//t->n->Div(3.0f);
			bool bCull = false;
			if (false)	//draw normals
			{
				Vector3 p1 = Vector3((t->va->x + t->vb->x + t->vc->x) / 3.0f,
					(t->vb->y + t->vb->y + t->vc->y) / 3.0f,
					(t->va->z + t->vb->z + t->vc->z) / 3.0f);
				Vector3 p2 = p1 + n;
				engine->QueueLine(camera, &p1, &p2, 0xFFFFFFFF);
			}


			t->ComputeArea();
			static float sAreaMin = 0.0f;
			static float sAreaMax = 50000.0f;
			if (engine->GetCullMode() == Engine::CullCounterClockwiseFace)
			{
				t->area = -t->area;
			}
			if ((engine->GetCullMode() == Engine::None || t->area > sAreaMin) && t->area < sAreaMax)
			{
				t->owner = ownerId;
				t->ComputeLighting(&light);
				t->draw = true;
				engine->QueueTriangle(t);
				drawnFaces++;
			}
		}
	}
	drawnFaces;
}

inline bool Mesh::RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height)
{
	//return false;
	if (t->pa->x < 0 && t->pb->x < 0 && t->pc->x < 0)
	{
		return true;
	}
	if (t->va->x > width && t->vb->x > width && t->vc->x > width)
	{
		return true;
	}
	if (t->pa->y > height && t->pb->y > height && t->pc->y > height)
	{
		return true;
	}
	if (t->pa->y < 0 && t->pb->y < 0 && t->pc->y < 0)
	{
		return true;
	}
	if (t->pa->w < znear || t->pb->w < znear || t->pc->w < znear)
	{
		return true;
	}
	if (t->pa->w > zfar || t->pb->w > zfar || t->pc->w > zfar)
	{
		return true;
	}
	//if ((t->va->w > znear && t->vb->w > znear && t->vc->w > znear) && (t->va->w < zfar && t->vb->w < zfar && t->vc->w < zfar))
	return false;
}

void Mesh::CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* tList, size_t &tArrayIdx, const Material* tex)
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
		t.pa = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ua = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ua = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.na = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.na = &m_verticesNormals[0];
		}

		vec.clear();
		SplitEntry(&line->at(b), &vec, '/');
		t.vb = &m_vertices[std::stoi(vec[0], &sz) - 1];
		t.pb = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ub = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ub = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.nb = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nb = &m_verticesNormals[0];
		}

		vec.clear();
		SplitEntry(&line->at(c), &vec, '/');
		t.vc = &m_vertices[std::stoi(vec[0], &sz) - 1];
		t.pc = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.uc = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.uc = &Vector2(0, 0);
		if (m_hasNormals)
		{
			t.nc = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nc = &m_verticesNormals[0];
		}
		t.tex = tex;
		float nx = (t.na->x + t.nb->x + t.nc->x) / 3.0f;
		float ny = (t.na->y + t.nb->y + t.nc->y) / 3.0f;
		float nz = (t.na->z + t.nb->z + t.nc->z) / 3.0f;
		m_trianglesNormals[tArrayIdx] = Vector3(nx, ny, nz);
		t.n = &m_trianglesNormals[tArrayIdx];
		tList->push_back(t);
		tArrayIdx++;	
	}
}