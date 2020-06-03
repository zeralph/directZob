#include "Mesh.h"
#include "DirectZob.h"
using namespace directZob;
using namespace std;
static Vector2 vec2zero = Vector2(0,0);
static std::string emptyStr = std::string("");
Mesh::Mesh(std::string& name)
{	
	DirectZob::LogInfo("Mesh %s Creation", m_name.c_str());
	DirectZob::AddIndent();
	m_subMeshes.clear();
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
	m_file = emptyStr;
	m_path = emptyStr;
	DirectZob::RemoveIndent();
}

Mesh::Mesh(std::string& name, std::string& path, std::string& file):Mesh(name)
{
	DirectZob::AddIndent();
	m_file = file;
	m_path = path;
	std::string fullPath = path;
	fullPath.append(file);
	if (fullPath.find(".fbx") != -1)
	{
		LoadFbx(fullPath);
	}
	else
	{
		LoadOBJ(fullPath);
	}
	DirectZob::RemoveIndent();
}

Mesh::Mesh(std::string &parentName, std::string& path, fbxsdk::FbxMesh* mesh)
{
	if (mesh)
	{
		m_path = path;
		m_name = parentName+"."+mesh->GetName();

		DirectZob::LogInfo("Mesh %s Creation", m_name.c_str());
		DirectZob::AddIndent();

		m_nbVertices += mesh->GetPolygonVertexCount();
		m_nbFaces = 0;
		m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_verticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);

		m_projectedVertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_projectedVerticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);

		m_nbNormals = m_nbVertices;
		m_verticesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
		m_verticesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
		m_verticesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);

		m_nbUvs = m_nbVertices;
		m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
		int pc = mesh->GetPolygonCount();
		DirectZob::LogInfo("Object %s, %i polygons", mesh->GetName(), pc);
		int vIdx = 0;
		if (mesh && pc > 0)
		{
			fbxsdk::FbxAMatrix mat;
			mesh->GetPivot(mat);
			for (int j = 0; j < pc; j++)
			{
				int pSize = mesh->GetPolygonSize(j);
				int startIdx = vIdx;
				FbxVector4 normal = FbxVector4(0, 0, 0);
				for (int k = 0; k < pSize; k++)
				{
					int polyGroup = mesh->GetPolygonGroup(j);
					int ctrlIdx = mesh->GetPolygonVertex(j, k);
					FbxVector4 v = mesh->GetControlPointAt(ctrlIdx);
					FbxMultT(mesh->GetNode(), v);
					m_vertices[vIdx] = Vector3(v[0], v[1], v[2]);
					if (vIdx == 0)
					{
						m_minBouding = Vector3(v[0], v[1], v[2]);
						m_maxBouding = Vector3(v[0], v[1], v[2]);
					}
					else
					{
						m_minBouding.x = min(m_minBouding.x, (float)v[0]);
						m_minBouding.y = min(m_minBouding.y, (float)v[1]);
						m_minBouding.z = min(m_minBouding.z, (float)v[2]);
						m_maxBouding.x = max(m_maxBouding.x, (float)v[0]);
						m_maxBouding.y = max(m_maxBouding.y, (float)v[1]);
						m_maxBouding.z = max(m_maxBouding.z, (float)v[2]);
					}
					//UVs
					fbxsdk::FbxStringList uvsNames;
					mesh->GetUVSetNames(uvsNames);
					if (uvsNames.GetCount() > 0)
					{
						fbxsdk::FbxVector2 uv;
						bool unmapped = true;
						char* uvSetName;
						mesh->GetPolygonVertexUV(j, k, uvsNames[0], uv, unmapped);
						if (!unmapped)
						{
							m_uvs[vIdx] = Vector2(uv[0], uv[1]);
						}
						else
						{
							m_uvs[vIdx] = Vector2(0, 0);
						}
					}
					else
					{
						m_uvs[vIdx] = Vector2(0, 0);
					}
					if (mesh->GetPolygonVertexNormal(j, k, normal))
					{
						FbxMultT(mesh->GetNode(), normal);
						normal.Normalize();
						m_verticesNormals[vIdx] = Vector3(normal[0], normal[1], normal[2]);
						//todo : compute normal
					}
					else
					{
						m_verticesNormals[vIdx] = Vector3(0,1,0);
					}
					vIdx++;
				}

				const Material* material = DirectZob::GetInstance()->GetMaterialManager()->LoadFbxMaterial(mesh, m_path);
				Triangle t;
				t.va = &m_vertices[startIdx];
				t.vb = &m_vertices[startIdx + 1];
				t.vc = &m_vertices[startIdx + 2];
				t.pa = &m_projectedVertices[startIdx];
				t.pb = &m_projectedVertices[startIdx + 1];
				t.pc = &m_projectedVertices[startIdx + 2];
				t.na = &m_verticesNormals[startIdx];
				t.nb = &m_verticesNormals[startIdx + 1];
				t.nc = &m_verticesNormals[startIdx + 2];
				t.ua = &m_uvs[startIdx];
				t.ub = &m_uvs[startIdx + 1];
				t.uc = &m_uvs[startIdx + 2];
				t.material = material;
				m_triangles.push_back(t);
				m_nbFaces++;
				if (pSize == 4)
				{
					Triangle t;
					t.va = &m_vertices[startIdx + 3];
					t.vb = &m_vertices[startIdx + 0];
					t.vc = &m_vertices[startIdx + 2];
					t.pa = &m_projectedVertices[startIdx + 3];
					t.pb = &m_projectedVertices[startIdx + 0];
					t.pc = &m_projectedVertices[startIdx + 2];
					t.na = &m_verticesNormals[startIdx + 3];
					t.nb = &m_verticesNormals[startIdx + 0];
					t.nc = &m_verticesNormals[startIdx + 2];
					t.ua = &m_uvs[startIdx + 3];
					t.ub = &m_uvs[startIdx + 0];
					t.uc = &m_uvs[startIdx + 2];
					t.material = material;
					m_triangles.push_back(t);
					m_nbFaces++;
				}
			}
		}
		m_trianglesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
		m_trianglesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
		m_trianglesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
		for (int i = 0; i < m_nbFaces; i++)
		{
			Triangle* t = &m_triangles[i];
			Vector3 v = Vector3(0, 0, 0);
			v.Add(t->na);
			v.Add(t->nb);
			v.Add(t->nc);
			v.Div(3.0f);
			m_trianglesNormals[i] = v;
			t->n = &m_trianglesNormals[i];
		}
		memcpy(m_verticesData, m_vertices, sizeof(Vector3) * m_nbVertices);
		memcpy(m_verticesTmp, m_vertices, sizeof(Vector3)* m_nbVertices);
		memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(Vector3) * m_nbNormals);
		memcpy(m_verticesNormalsTmp, m_verticesNormals, sizeof(Vector3)* m_nbNormals);
		memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(Vector3) * m_nbFaces);
		memcpy(m_trianglesNormalsTmp, m_trianglesNormals, sizeof(Vector3)* m_nbFaces);
	}
	DirectZob::RemoveIndent();
}

void Mesh::LoadFbx(const std::string& fullPath)
{
	DirectZob::LogInfo("Load FBX %s", fullPath.c_str());
	DirectZob::AddIndent();
	FbxManager* fbxManag = DirectZob::GetInstance()->GetMeshManager()->GetFbxManager();
	FbxImporter* importer = FbxImporter::Create(fbxManag, "");
	//importer->SetEmbeddingExtractionFolder("C:\\_GIT\\directZob\\resources\\test");
	if (importer->Initialize(fullPath.c_str(), -1, fbxManag->GetIOSettings()))
	{
		FbxScene* lScene = FbxScene::Create(fbxManag, "myScene");
		importer->Import(lScene);
		importer->Destroy(true);
		if (lScene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::cm)
		{
			const FbxSystemUnit::ConversionOptions lConversionOptions = {
			  false, /* mConvertRrsNodes */
			  true, /* mConvertLimits */
			  true, /* mConvertClusters */
			  true, /* mConvertLightIntensity */
			  true, /* mConvertPhotometricLProperties */
			  true  /* mConvertCameraClipPlanes */
			};
			// Convert the scene to meters using the defined options.
			FbxSystemUnit::m.ConvertScene(lScene, lConversionOptions);
		}
		int gc = lScene->GetGeometryCount();
		m_nbFaces = 0;
		int i = 0;
		//first pass, allocations
		m_nbVertices = 0;
		m_nbFaces = 0;
		m_nbNormals = 0;
		m_nbUvs = 0;
		for (int i = 0; i < gc; i++)
		{
			FbxMesh* mesh = (FbxMesh*)lScene->GetGeometry(i);
			Mesh* m = new Mesh(m_name, m_path, mesh);
			m_subMeshes.push_back(m);
		}
		lScene->Destroy(true);
	}
	DirectZob::RemoveIndent();
}

void Mesh::FbxMultT(FbxNode* node, FbxVector4 &vector) 
{
	fbxsdk::FbxAMatrix matrixGeo;
	matrixGeo.SetIdentity();
	if (node->GetNodeAttribute())
	{
		FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
		matrixGeo.SetT(lT);
		matrixGeo.SetR(lR);
		matrixGeo.SetS(lS);
	}
	fbxsdk::FbxAMatrix globalMatrix = node->EvaluateLocalTransform();
	fbxsdk::FbxAMatrix matrix = globalMatrix * matrixGeo;
	vector = matrix.MultT(vector);
}

Mesh::~Mesh()
{
	DirectZob::LogInfo("Delete mesh %s", m_name.c_str());
	DirectZob::AddIndent();
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		Mesh* m = m_subMeshes[i];
		m_subMeshes[i] = NULL;
		delete m;
	}
	m_subMeshes.clear();
	m_triangles.clear();
	delete m_vertices;
	delete m_verticesData;
	delete m_verticesTmp;
	delete m_verticesNormals;
	delete m_verticesNormalsData;
	delete m_verticesNormalsTmp;
	delete m_trianglesNormals;
	delete m_trianglesNormalsTmp;
	delete m_trianglesNormalsData;
	delete m_projectedVertices;
	delete m_projectedVerticesTmp;
	delete m_uvs;
	DirectZob::RemoveIndent();
}

void Mesh::LoadOBJ(const std::string& fullPath)
{
	static std::string sMtllib = std::string("mtllib");
	DirectZob::LogInfo("Load OBJ %s", fullPath.c_str());
	DirectZob::AddIndent();
	std::string::size_type sz;
	// Open the file.
	std::ifstream sfile(fullPath, ios::in);
	std::string line;
	if (!sfile.is_open())
	{
		DirectZob::LogError("Error opening %s", fullPath.c_str());
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
			m_nbFaces += (uint)v.size() - 3;
		}
		else if (line.rfind("mtllib", 0) == 0)
		{
			vector<string> v;
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				DirectZob::GetInstance()->GetMaterialManager()->LoadOBJMaterials(m_path, v[1]);
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
	m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_verticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_projectedVertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_projectedVerticesTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
	m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
	m_verticesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_verticesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_verticesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
	m_trianglesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_trianglesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
	m_trianglesNormalsTmp = (Vector3*)malloc(sizeof(Vector3) * m_nbFaces);
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
				std::string matName = m_file;
				matName = matName.substr(0, matName.size() - 4);
				matName.append(".");
				matName.append(v[1]);
				tex = DirectZob::GetInstance()->GetMaterialManager()->GetMaterial(matName);
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
	memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(Vector3) * m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(Vector3) * m_nbFaces);
	DirectZob::RemoveIndent();
}

void Mesh::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim)
{
	std::size_t current, previous = 0;
	current = s->find(delim);
	while (current != std::string::npos) {
		std::string sub = s->substr(previous, current - previous);
		if (sub.length() > 0)
		{
			v->push_back(sub);
		}
		previous = current + 1;
		current = s->find(delim, previous);
	}
	std::string sub = s->substr(previous, current - previous);
	if (sub.length() > 0)
	{
		v->push_back(sub);
	}
}

void Mesh::DrawBoundingBox(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const uint ownerId, const RenderOptions* options)
{
	Vector3 v0 = Vector3(m_minBouding.x, m_minBouding.y, m_minBouding.z);
	Vector3 v1 = Vector3(m_minBouding.x, m_maxBouding.y, m_minBouding.z);
	Vector3 v2 = Vector3(m_maxBouding.x, m_maxBouding.y, m_minBouding.z);
	Vector3 v3 = Vector3(m_maxBouding.x, m_minBouding.y, m_minBouding.z);

	Vector3 v4 = Vector3(m_minBouding.x, m_minBouding.y, m_maxBouding.z);
	Vector3 v5 = Vector3(m_minBouding.x, m_maxBouding.y, m_maxBouding.z);
	Vector3 v6 = Vector3(m_maxBouding.x, m_maxBouding.y, m_maxBouding.z);
	Vector3 v7 = Vector3(m_maxBouding.x, m_minBouding.y, m_maxBouding.z);
	modelMatrix.Mul(&v0);
	modelMatrix.Mul(&v1);
	modelMatrix.Mul(&v2);
	modelMatrix.Mul(&v3);
	modelMatrix.Mul(&v4);
	modelMatrix.Mul(&v5);
	modelMatrix.Mul(&v6);
	modelMatrix.Mul(&v7);
	engine->QueueLine(camera, &v0, &v1, 0xFF00FF00, true);
	engine->QueueLine(camera, &v1, &v2, 0xFF00FF00, true);
	engine->QueueLine(camera, &v2, &v3, 0xFF00FF00, true);
	engine->QueueLine(camera, &v3, &v0, 0xFF00FF00, true);

	engine->QueueLine(camera, &v4, &v5, 0xFF00FF00, true);
	engine->QueueLine(camera, &v5, &v6, 0xFF00FF00, true);
	engine->QueueLine(camera, &v6, &v7, 0xFF00FF00, true);
	engine->QueueLine(camera, &v7, &v4, 0xFF00FF00, true);

	engine->QueueLine(camera, &v1, &v5, 0xFF00FF00, true);
	engine->QueueLine(camera, &v2, &v6, 0xFF00FF00, true);
	engine->QueueLine(camera, &v3, &v7, 0xFF00FF00, true);
	engine->QueueLine(camera, &v0, &v4, 0xFF00FF00, true);
}

void Mesh::Update(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const uint ownerId, const RenderOptions* options)
{
	BufferData* bData = engine->GetBufferData();
	const Matrix4x4* view = camera->GetViewMatrix();
	const Matrix4x4* proj = camera->GetProjectionMatrix();
	float w = (float)bData->width / 2.0f;
	float h = (float)bData->height / 2.0f;	

	memcpy(m_verticesTmp, m_verticesData, sizeof(Vector3) * m_nbVertices);
	memcpy(m_verticesNormalsTmp, m_verticesNormalsData, sizeof(Vector3) * m_nbNormals);
	memcpy(m_trianglesNormalsTmp, m_trianglesNormalsData, sizeof(Vector3) * m_nbFaces);

	for (uint i = 0; i < m_nbVertices; i++)
	{
		modelMatrix.Mul(&m_verticesTmp[i]);
		m_projectedVerticesTmp[i].x = m_verticesTmp[i].x;
		m_projectedVerticesTmp[i].y = m_verticesTmp[i].y;
		m_projectedVerticesTmp[i].z = m_verticesTmp[i].z;
		m_projectedVerticesTmp[i].w = m_verticesTmp[i].w;
		camera->ToViewSpace(&m_projectedVerticesTmp[i]);
		proj->Mul(&m_projectedVerticesTmp[i]);
		m_projectedVerticesTmp[i].x = (m_projectedVerticesTmp[i].x / m_projectedVerticesTmp[i].z + 1) * w;
		m_projectedVerticesTmp[i].y = (m_projectedVerticesTmp[i].y / m_projectedVerticesTmp[i].z + 1) * h;
	}
	for (uint i = 0; i < m_nbNormals; i++)
	{
		rotationMatrix.Mul(&m_verticesNormalsTmp[i]);
		m_verticesNormalsTmp[i].Normalize();
	}
	for (uint i = 0; i < m_nbFaces; i++)
	{
		rotationMatrix.Mul(&m_trianglesNormalsTmp[i]);
		m_trianglesNormalsTmp[i].Normalize();
	}
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i]->Update(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
	}
}
void Mesh::QueueForDrawing(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const uint ownerId, const RenderOptions* options)
{
	memcpy(m_vertices, m_verticesTmp, sizeof(Vector3) * m_nbVertices);
	memcpy(m_verticesNormals, m_verticesNormalsTmp, sizeof(Vector3) * m_nbNormals);
	memcpy(m_trianglesNormals, m_trianglesNormalsTmp, sizeof(Vector3) * m_nbFaces);
	memcpy(m_projectedVertices, m_projectedVerticesTmp, sizeof(Vector3) * m_nbVertices);

	if (engine->DrawGizmos())// && engine->ShowBBoxes() )
	{
		DrawBoundingBox(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
	}
	//Update(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
	BufferData* bData = engine->GetBufferData();
	float znear = bData->zNear;
	float zfar = bData->zFar;
	Vector3 n;
	for (int i = 0; i < m_nbFaces; i++)
	{
		Triangle* t = &m_triangles[i];
		t->draw = false;
		t->options = options;
		if (!RejectTriangle(t, znear, zfar, (float)bData->width, (float)bData->height))
		{
			bool bCull = false;
			t->ComputeArea();
			static float sAreaMin = 0.0f;
			static float sAreaMax = 50000.0f;
			if (t->options->cullMode == eCullMode_CounterClockwiseFace)
			{
				t->area = -t->area;
			}
			//if (engine->GetCullMode() == Engine::None )// ( || t->area > sAreaMin && t->area < sAreaMax))
			{
				t->owner = ownerId;
				t->draw = true;
				engine->QueueTriangle(t);
				static bool bShowNormal = true;
				if (engine->ShowNormals())
				{
					Vector3 v = t->na;
					v = v + t->va;
					engine->QueueLine(camera, t->va, &v, 0xFF00FF, false);
					v = t->nb;
					v = v + t->vb;
					engine->QueueLine(camera, t->vb, &v, 0xFF00FF, false);
					v = t->nb;
					v = v + t->vc;
					engine->QueueLine(camera, t->vc, &v, 0xFF00FF, false);
				}
			}
		}
	}
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i]->QueueForDrawing(modelMatrix, rotationMatrix, camera,  engine, ownerId, options);
	}
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
	if (t->pa->z < znear || t->pb->z < znear || t->pc->z < znear)
	{
		return true;
	}
	if (t->pa->z > zfar || t->pb->z > zfar || t->pc->z > zfar)
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
			t.ua = &vec2zero;
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
			t.ub = &vec2zero;
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
			t.uc = &vec2zero;
		if (m_hasNormals)
		{
			t.nc = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nc = &m_verticesNormals[0];
		}
		t.material = tex;
		float nx = (t.na->x + t.nb->x + t.nc->x) / 3.0f;
		float ny = (t.na->y + t.nb->y + t.nc->y) / 3.0f;
		float nz = (t.na->z + t.nb->z + t.nc->z) / 3.0f;
		m_trianglesNormals[tArrayIdx] = Vector3(nx, ny, nz);
		t.n = &m_trianglesNormals[tArrayIdx];
		tList->push_back(t);
		tArrayIdx++;	
	}
}
