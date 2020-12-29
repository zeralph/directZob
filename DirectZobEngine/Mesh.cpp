#include "Mesh.h"
#include "DirectZob.h"
#include "SceneLoader.h"

using namespace std;
static ZobVector2 vec2zero = ZobVector2(0,0);
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
	m_indices = NULL;
	m_vertices = NULL;
	m_projectedVertices = NULL;
	m_verticesNormals = NULL;
	m_trianglesNormals = NULL;
	m_uvs = NULL;
	m_name = name;
	m_fileName = emptyStr;
	m_path = emptyStr;
	DirectZob::RemoveIndent();
}

Mesh::Mesh(std::string& name, std::string& path, std::string& file):Mesh(name)
{
	DirectZob::AddIndent();
	m_path = path;
	m_fileName = file;
	bool bOK = false;
	std::string fullPath;
	if (m_path.length())
	{ 
		if(m_path[m_path.length() - 1] != '\\')
		{
			m_path.append("\\");
		}
		fullPath = m_path;
		fullPath.append(file);
		std::ifstream f(fullPath.c_str());
		if (f.good())
		{
			bOK = true;
		}
	}
	if(!bOK)
	{
		m_path = std::string(SceneLoader::GetResourcePath());
		fullPath = m_path;
		fullPath.append(file);
	}
	if (fullPath.find(".fbx") != -1)
	{
		LoadFbx(fullPath);
	}
	else if (fullPath.find(".obj") != -1)
	{
		LoadOBJ(fullPath);
	}
	else
	{
		DirectZob::LogError("Mesh %s : bad filename extension. Loading ignored", name.c_str());
	}
	DirectZob::RemoveIndent();
}

Mesh::Mesh(Mesh* m)
{

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
		
		m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
		m_verticesData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
		m_verticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);

		m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
		m_projectedVerticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);

		m_nbNormals = m_nbVertices;
		m_verticesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
		m_verticesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
		m_verticesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);

		m_nbUvs = m_nbVertices;
		m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * m_nbUvs);
		int pc = mesh->GetPolygonCount();
		DirectZob::LogInfo("Object %s, %i polygons", mesh->GetName(), pc);
		int vIdx = 0;
		if (mesh && pc > 0)
		{
			FbxVector4 p = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
			m_pivot.x = p[0];
			m_pivot.y = p[1];
			m_pivot.z = p[2];
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
					m_vertices[vIdx] = ZobVector3(v[0], v[1], v[2]);
					if (vIdx == 0)
					{
						m_minBouding = ZobVector3(v[0], v[1], v[2]);
						m_maxBouding = ZobVector3(v[0], v[1], v[2]);
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
							m_uvs[vIdx] = ZobVector2(uv[0], uv[1]);
						}
						else
						{
							m_uvs[vIdx] = ZobVector2(0, 0);
						}
					}
					else
					{
						m_uvs[vIdx] = ZobVector2(0, 0);
					}
					if (mesh->GetPolygonVertexNormal(j, k, normal))
					{
						//FbxMultT(mesh->GetNode(), normal);
						//normal.Normalize();
						m_verticesNormals[vIdx] = ZobVector3(normal[0], normal[1], normal[2]);
						//todo : compute normal
					}
					else
					{
						m_verticesNormals[vIdx] = ZobVector3(0,1,0);
					}
					vIdx++;
				}

				const ZobMaterial* material = DirectZob::GetInstance()->GetMaterialManager()->LoadFbxMaterial(mesh, m_path);
				Triangle t;
				t.verticeAIndex = startIdx;
				t.verticeBIndex = startIdx + 1;
				t.verticeCIndex = startIdx + 2;
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
					t.verticeAIndex = startIdx + 3;
					t.verticeBIndex = startIdx + 0;
					t.verticeCIndex = startIdx + 2;
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
		m_trianglesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
		m_trianglesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
		m_trianglesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
		for (int i = 0; i < m_nbFaces; i++)
		{
			Triangle* t = &m_triangles[i];
			ZobVector3 v = ZobVector3(0, 0, 0);
			v.Add(t->na);
			v.Add(t->nb);
			v.Add(t->nc);
			v.Div(3.0f);
			m_trianglesNormals[i] = &v;
			t->n = &m_trianglesNormals[i];
			//indices 
			m_indices[i * 3 + 0] = t->verticeAIndex;
			m_indices[i * 3 + 1] = t->verticeBIndex;
			m_indices[i * 3 + 2] = t->verticeCIndex;
		}
		memcpy(m_verticesData, m_vertices, sizeof(ZobVector3) * m_nbVertices);
		memcpy(m_verticesTmp, m_vertices, sizeof(ZobVector3)* m_nbVertices);
		memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(ZobVector3) * m_nbNormals);
		memcpy(m_verticesNormalsTmp, m_verticesNormals, sizeof(ZobVector3)* m_nbNormals);
		memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
		memcpy(m_trianglesNormalsTmp, m_trianglesNormals, sizeof(ZobVector3)* m_nbFaces);
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
	m_indices = (uint*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_verticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_projectedVerticesTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbVertices);
	m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * m_nbUvs);
	m_verticesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_verticesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbNormals);
	m_trianglesNormals = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_trianglesNormalsTmp = (ZobVector3*)malloc(sizeof(ZobVector3) * m_nbFaces);
	m_triangles.clear();

	//if (!m_hasNormals)
	{
		//TODO ... compute !
		m_verticesNormals[0] = ZobVector3(0, 0, 1);
		m_trianglesNormals[0] = ZobVector3(0, 0, 1);
	}

	size_t curVertice = 0;
	size_t curNormal = 0;
	size_t curUv = 0;
	size_t curface = 0;
	const ZobMaterial* tex = NULL;
	while (getline(sfile, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				ZobVector2 v = ZobVector2(std::stof(vec[1], &sz), ::stof(vec[2], &sz));
				m_uvs[curUv] = v;
				curUv++;
			}
			else if (line[1] == 'n')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				ZobVector3 v = ZobVector3(std::stof(vec[1], &sz), ::stof(vec[2], &sz), ::stof(vec[3], &sz));
				m_verticesNormals[curNormal] = v;
				curNormal++;
			}
			else if (line[1] == ' ')
			{
				std::vector<string> vec;
				SplitEntry(&line, &vec, ' ');
				ZobVector3 v = ZobVector3(std::stof(vec[1], &sz), ::stof(vec[2], &sz), ::stof(vec[3], &sz));
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
				std::string matName = m_name;
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
	memcpy(m_verticesData, m_vertices, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesNormalsData, m_verticesNormals, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_trianglesNormalsData, m_trianglesNormals, sizeof(ZobVector3) * m_nbFaces);
	DirectZob::RemoveIndent();
}

void Mesh::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim)
{
	std::size_t current, previous = 0;
	current = s->find(delim);
	while (current != std::string::npos) {
		std::string sub = s->substr(previous, current - previous);
		//if (sub.length() > 0)
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

void Mesh::DrawBoundingBox(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options)
{
	ZobVector3 v = ZobVector3(
		(m_maxBouding.x - m_minBouding.x) / 2.0f,
		(m_maxBouding.y - m_minBouding.y) / 2.0f,
		(m_maxBouding.z - m_minBouding.z) / 2.0f
	);
	ZobVector3 pivot = ZobVector3(m_minBouding.x + v.x, m_minBouding.y + v.y, m_minBouding.z + v.z);
	engine->QueueBox(camera, &modelMatrix, &v, &pivot, 0x00FFFFFF, false, false);
}

void Mesh::Update(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options)
{
	BufferData* bData = engine->GetBufferData();
	const ZobMatrix4x4* view = camera->GetViewMatrix();
	const ZobMatrix4x4* proj = camera->GetProjectionMatrix();
	float w = (float)bData->width / 2.0f;
	float h = (float)bData->height / 2.0f;	

	memcpy(m_vertices, m_verticesData, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesTmp, m_verticesData, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesNormalsTmp, m_verticesNormalsData, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_trianglesNormalsTmp, m_trianglesNormalsData, sizeof(ZobVector3) * m_nbFaces);

	for (uint i = 0; i < m_nbVertices; i++)
	{
		modelMatrix.Mul(&m_verticesTmp[i]);
	}
	int c = 0;
	for (uint i = 0; i < m_triangles.size(); i++)
	{
		Triangle* t = &m_triangles.at(i);
		t->draw = true;
		ZobVector3* va = &m_verticesTmp[t->verticeAIndex];
		ZobVector3* vb = &m_verticesTmp[t->verticeBIndex];
		ZobVector3* vc = &m_verticesTmp[t->verticeCIndex];
			
		bool aIn = camera->PointIsInFrustrum(va);
		bool bIn = camera->PointIsInFrustrum(vb);
		bool cIn = camera->PointIsInFrustrum(vc);
		if(aIn && bIn && cIn)
		{
			t->clipMode = Triangle::eClip_3_in;
		}
		else if(aIn && cIn)
		{
			t->clipMode = Triangle::eClip_AC_in_B_out;
		}
		else if (aIn && bIn)
		{
			t->clipMode = Triangle::eClip_AB_in_C_out;
		}
		else if (aIn)
		{
			t->clipMode = Triangle::eClip_A_in_BC_out;
		}
		else if (bIn && cIn)
		{
			t->clipMode = Triangle::eClip_BC_in_A_out;
		}
		else if (bIn)
		{
			t->clipMode = Triangle::eClip_B_in_AC_out;
		}
		else if(cIn)
		{
			t->clipMode = Triangle::eClip_C_in_AB_out;
		}
		else
		{
			t->clipMode = Triangle::eClip_0_in;
		}
	}
/*
	for (uint i = 0; i < m_nbVertices; i++)
	{
		m_projectedVerticesTmp[i].x = m_verticesTmp[i].x;
		m_projectedVerticesTmp[i].y = m_verticesTmp[i].y;
		m_projectedVerticesTmp[i].z = m_verticesTmp[i].z;
		m_projectedVerticesTmp[i].w = m_verticesTmp[i].w;
		view->Mul(&m_projectedVerticesTmp[i]);
		proj->Mul(&m_projectedVerticesTmp[i]);
		//static float kk = 1.0f; // sinf(m_projectedVerticesTmp[i].y) * kk
		m_projectedVerticesTmp[i].x = ((m_projectedVerticesTmp[i].x )/ m_projectedVerticesTmp[i].z + 1) * w;
		m_projectedVerticesTmp[i].y = (m_projectedVerticesTmp[i].y / m_projectedVerticesTmp[i].z + 1) * h;
	}
*/	
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
void Mesh::QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, RenderOptions* options)
{
	memcpy(m_vertices, m_verticesTmp, sizeof(ZobVector3) * m_nbVertices);
	memcpy(m_verticesNormals, m_verticesNormalsTmp, sizeof(ZobVector3) * m_nbNormals);
	memcpy(m_trianglesNormals, m_trianglesNormalsTmp, sizeof(ZobVector3) * m_nbFaces);
	memcpy(m_projectedVertices, m_projectedVerticesTmp, sizeof(ZobVector3) * m_nbVertices);

	if (engine->DrawGizmos())// && engine->ShowBBoxes() )
	{
		DrawBoundingBox(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
	}
	//Update(modelMatrix, rotationMatrix, camera, engine, ownerId, options);
	BufferData* bData = engine->GetBufferData();
	float znear = bData->zNear;
	float zfar = bData->zFar;
	ZobVector3 n;
	for (int i = 0; i < m_nbFaces; i++)
	{
		Triangle* t = &m_triangles[i];
		t->options = options;
		t->zobObject = z;
		t->ca = 0xFF0000;
		t->cb = 0x00FF00;
		t->cc = 0x0000FF;
		if(t->draw)
		{
			bool bCull = false;
			t->ComputeArea();
			static float sAreaMin = 0.0f;
			static float sAreaMax = 50000.0f;
			if (t->options->cullMode == eCullMode_CounterClockwiseFace)
			{
				t->area = -t->area;
			}
			//if (t->area > sAreaMin)
			{
				engine->QueueTriangle(camera, t);
				static bool bShowNormal = true;
				if (engine->ShowNormals())
				{
					ZobVector3 v = t->na;
					v = v + t->va;
					engine->QueueLine(camera, t->va, &v, 0xFF00FF, false, false);
					v = t->nb;
					v = v + t->vb;
					engine->QueueLine(camera, t->vb, &v, 0xFF00FF, false, false);
					v = t->nb;
					v = v + t->vc;
					engine->QueueLine(camera, t->vc, &v, 0xFF00FF, false, false);
				}
			}
		}
	}
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i]->QueueForDrawing(z, modelMatrix, rotationMatrix, camera,  engine, ownerId, options);
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

void Mesh::CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* tList, size_t &tArrayIdx, const ZobMaterial* tex)
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
		t.verticeAIndex = std::stoi(vec[0], &sz) - 1;
		t.va = &m_vertices[std::stoi(vec[0], &sz) - 1];
		t.pa = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ua = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ua = &vec2zero;
		if (m_hasNormals && vec.size() > 2)
		{
			t.na = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.na = &m_verticesNormals[0];
		}

		vec.clear();
		SplitEntry(&line->at(b), &vec, '/');
		t.verticeBIndex = std::stoi(vec[0], &sz) - 1;
		t.vb = &m_vertices[std::stoi(vec[0], &sz) - 1];
		t.pb = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.ub = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.ub = &vec2zero;
		if (m_hasNormals && vec.size() >2)
		{
			t.nb = &m_verticesNormals[std::stoi(vec[2], &sz) - 1];
		}
		else
		{
			t.nb = &m_verticesNormals[0];
		}

		vec.clear();
		SplitEntry(&line->at(c), &vec, '/');
		t.verticeCIndex = std::stoi(vec[0], &sz) - 1;
		t.vc = &m_vertices[std::stoi(vec[0], &sz) - 1];
		t.pc = &m_projectedVertices[std::stoi(vec[0], &sz) - 1];
		if (vec[1].size() > 0)
			t.uc = &m_uvs[std::stoi(vec[1], &sz) - 1];
		else
			t.uc = &vec2zero;
		if (m_hasNormals && vec.size() > 2)
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
		m_trianglesNormals[tArrayIdx] = ZobVector3(nx, ny, nz);
		t.n = &m_trianglesNormals[tArrayIdx];
		tList->push_back(t);

		//indices 
		m_indices[tArrayIdx * 3 + 0] = t.verticeAIndex;
		m_indices[tArrayIdx * 3 + 1] = t.verticeBIndex;
		m_indices[tArrayIdx * 3 + 2] = t.verticeCIndex;

		tArrayIdx++;	
	}
}
