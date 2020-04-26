#include "Mesh.h"
#include "DirectZob.h"

using namespace std;


static Vector2 vec2zero = Vector2(0,0);

Mesh::Mesh(std::string& name, std::string& path, std::string& file)
{	
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
}

Mesh::Mesh(std::string &parentName, fbxsdk::FbxMesh* mesh)
{
	if (mesh)
	{
		char buf[256];
		m_name = parentName+"."+mesh->GetName();
		m_nbVertices += mesh->GetPolygonVertexCount();
		_snprintf_s(buf, 256, "Load sub mesh %s, %i vertices", mesh->GetName(), m_nbVertices);
		DirectZob::LogInfo(buf);
		m_nbFaces = 0;
		m_vertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_projectedVertices = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_verticesData = (Vector3*)malloc(sizeof(Vector3) * m_nbVertices);
		m_nbNormals = m_nbVertices;
		m_verticesNormals = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
		m_verticesNormalsData = (Vector3*)malloc(sizeof(Vector3) * m_nbNormals);
		m_nbUvs = m_nbVertices;
		m_uvs = (Vector2*)malloc(sizeof(Vector2) * m_nbUvs);
		int pc = mesh->GetPolygonCount();
		_snprintf_s(buf, 256, "Object %s, %i polygons", mesh->GetName(), pc);
		DirectZob::LogInfo(buf);
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

				const Material* material = LoadFbxMaterial(mesh);
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
		memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(Vector3) * m_nbNormals);
		memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(Vector3) * m_nbFaces);
	}
}

void Mesh::LoadFbx(const std::string& fullPath)
{
	char buf[256];
	_snprintf_s(buf, 256, "Load mesh %s", fullPath.c_str());
	DirectZob::LogInfo(buf);
	FbxManager* fbxManag = DirectZob::GetInstance()->GetMeshManager()->GetFbxManager();
	FbxImporter* importer = FbxImporter::Create(fbxManag, "");
	importer->SetEmbeddingExtractionFolder("C:\\_GIT\\directZob\\resources\\test");
	if (importer->Initialize(fullPath.c_str(), -1, fbxManag->GetIOSettings()))
	{
		FbxScene* lScene = FbxScene::Create(fbxManag, "myScene");
		importer->Import(lScene);
		importer->Destroy();
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
			Mesh* m = new Mesh(m_name, mesh);
			m_subMeshes.push_back(m);
		}
	}
}

const Material* Mesh::LoadFbxMaterial(fbxsdk::FbxMesh* mesh)
{
	const Material* finalMaterial = NULL;
	MaterialManager* materialMgr = DirectZob::GetInstance()->GetMaterialManager();
	int mcount = mesh->GetElementMaterialCount();
	for (int index = 0; index < mcount; index++)
	{
		//fbxsdk::FbxGeometryElementMaterial* material = mesh->GetElementMaterial(0);
		fbxsdk::FbxGeometryElementMaterial* gMaterial = mesh->GetElementMaterial(index);
		if (gMaterial)
		{
			fbxsdk::FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(gMaterial->GetIndexArray().GetAt(0));
			int propCount = material->GetSrcPropertyCount();
			//if (propCount > 0)
			{
				const char* matName = material->GetName();
				finalMaterial = materialMgr->GetMaterial(matName);
				if (finalMaterial)
				{
					return finalMaterial;
				}
				else
				{
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					fbxsdk::FbxProperty prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					// Check if it's layeredtextures
					FbxDouble3 c = prop.Get<FbxDouble3>();
					Vector3 diffuse = Vector3(c[0], c[1], c[2]);
					const char* texture_name = NULL;
					const char* texture_name2 = NULL;
					const char* texture_name3 = NULL;
					int file_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
					if (file_texture_count > 0)
					{
						for (int j = 0; j < file_texture_count; j++)
						{
							FbxFileTexture* file_texture = FbxCast<fbxsdk::FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
							texture_name = file_texture->GetFileName();
							texture_name2 = file_texture->GetRelativeFileName();
							texture_name3 = file_texture->GetMediaName();
						}
					}
					/*int layered_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
					if (layered_texture_count > 0)
					{
						for (int j = 0; j < layered_texture_count; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<fbxsdk::FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<fbxsdk::FbxTexture>();
							for (int k = 0; k < lcount; k++)
							{
								FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(layered_texture->GetSrcObject<fbxsdk::FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								texture_name = texture->GetName();
								//texture_name2 = texture->GetRelativeFileName();
								//texture_name3 = texture->GetMediaName();
							}
						}
					}
					else
					{
						// Directly get textures
						int texture_count = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();
						for (int j = 0; j < texture_count; j++)
						{
							const fbxsdk::FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(prop.GetSrcObject<fbxsdk::FbxTexture>(j));
							//texture->FindSrcObject
							// Then, you can get all the properties of the texture, include its name
							texture_name = texture->GetName();
							//texture_name2 = texture->GetRelativeFileName();
							//texture_name3 = texture->GetMediaName();
						}
					}*/
					float f;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);
					f = prop.Get<FbxDouble>();
					
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
					f = prop.Get<FbxDouble>();
					Vector3 ambient;
					//texture_name = "C:\\_GIT\\directZob\\resources\\earth_256.png";
					finalMaterial = materialMgr->LoadMaterial(matName, &ambient, &diffuse, texture_name);
				}
			}
		}
	}
	return finalMaterial;
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
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		delete m_subMeshes[i];
		m_subMeshes[i] = NULL;
	}
	m_subMeshes.clear();
	m_triangles.clear();
	delete m_vertices;
	delete m_verticesData;
	delete m_uvs;
	delete m_verticesNormals;
	delete m_verticesNormalsData;
	delete m_trianglesNormals;
	delete m_trianglesNormalsData;
}

void Mesh::LoadOBJ(const std::string& fullPath)
{
	static std::string sMtllib = std::string("mtllib");
	std::string s = "Load mesh " + fullPath;
	DirectZob::LogInfo(s.c_str());
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
				DirectZob::GetInstance()->GetMaterialManager()->LoadMaterials(m_path, v[1]);
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
	s = "Mesh loaded : " + std::string(fullPath);
	DirectZob::LogInfo(s.c_str());
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

void Mesh::DrawBoundingBox(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions options)
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
	engine->QueueLine(camera, &v0, &v1, 0xFF00FF00);
	engine->QueueLine(camera, &v1, &v2, 0xFF00FF00);
	engine->QueueLine(camera, &v2, &v3, 0xFF00FF00);
	engine->QueueLine(camera, &v3, &v0, 0xFF00FF00);

	engine->QueueLine(camera, &v4, &v5, 0xFF00FF00);
	engine->QueueLine(camera, &v5, &v6, 0xFF00FF00);
	engine->QueueLine(camera, &v6, &v7, 0xFF00FF00);
	engine->QueueLine(camera, &v7, &v4, 0xFF00FF00);

	engine->QueueLine(camera, &v1, &v5, 0xFF00FF00);
	engine->QueueLine(camera, &v2, &v6, 0xFF00FF00);
	engine->QueueLine(camera, &v3, &v7, 0xFF00FF00);
	engine->QueueLine(camera, &v0, &v4, 0xFF00FF00);
}

void Mesh::Draw(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions options)
{
	DrawBoundingBox(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
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
			if (true)	//draw normals
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
			//if (engine->GetCullMode() == Engine::None )// ( || t->area > sAreaMin && t->area < sAreaMax))
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
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i]->Draw(modelMatrix, rotationMatrix, camera,  engine, ownerId, options);
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
