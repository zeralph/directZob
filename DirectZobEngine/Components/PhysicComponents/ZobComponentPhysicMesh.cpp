#include "ZobComponentPhysicMesh.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"
#include "../../Mesh.h"
#include "../../Managers/MeshManager.h"

ZobComponentPhysicMesh::~ZobComponentPhysicMesh()
{
	RemoveCollider();
}

ZobComponentPhysicMesh::ZobComponentPhysicMesh(ZobObject* zobObject) : ZobComponentPhysicShape(zobObject)
{
	SET_CLASS_AND_NAME
	m_componentType = ZobComponentFactory::eComponent_physicMesh;
	m_convexMeshPath.Reset();
	m_concaveMeshNbTriangles = 0;
	m_mesh = NULL;
	m_varExposer->WrapVariable<ZobFilePath>("Mesh", &m_convexMeshPath, NULL, false, true);
	m_varExposer->WrapVariable<int>("Nb triangles", &m_concaveMeshNbTriangles, NULL, true, false);
}

void ZobComponentPhysicMesh::RemoveCollider()
{
	ZobComponentPhysicShape::RemoveCollider();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	pc->destroyConcaveMeshShape(m_concaveMeshShape);
	pc->destroyTriangleMesh(m_triangleMesh);
	delete m_triangleVertexArray;
	m_mesh = NULL;
	free(m_concaveMeshVertices);
	free(m_concaveMeshIndices);
	m_concaveMeshVertices = NULL;
	m_concaveMeshIndices = NULL;
	m_concaveMeshNbTriangles = 0;
}

bool ZobComponentPhysicMesh::LoadMeshInternal()
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->GetOrLoadMesh(m_convexMeshPath);
	if (m_mesh)
	{
		PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
		const int nbVertices = m_mesh->GetNbVertices();
		const int nbTriangles = m_mesh->GetNbTriangles();
		m_concaveMeshVertices = (float*)malloc(sizeof(float) * nbVertices * 3);
		m_concaveMeshIndices = (uint*)malloc(sizeof(uint) * nbTriangles * 3);
		m_concaveMeshNbTriangles = nbTriangles;
		int idx = 0;
		for (int i = 0; i < nbVertices; i++)
		{
			m_concaveMeshVertices[idx] = m_mesh->GetVertices()[i].x;
			idx++;
			m_concaveMeshVertices[idx] = m_mesh->GetVertices()[i].y;
			idx++;
			m_concaveMeshVertices[idx] = m_mesh->GetVertices()[i].z;
			idx++;
		}
		assert(idx == nbVertices * 3);
		idx = 0;
		//for (std::vector<Triangle>::const_iterator iter = m->GetTriangles().begin(); iter != m->GetTriangles().end(); iter++)
		for (int i = 0; i < nbTriangles; i++)
		{
			const Triangle* t = &m_mesh->GetTriangles()->at(i);
			m_concaveMeshIndices[idx] = (uint)t->verticeAIndex;
			idx++;
			m_concaveMeshIndices[idx] = (uint)t->verticeBIndex;
			idx++;
			m_concaveMeshIndices[idx] = (uint)t->verticeCIndex;
			idx++;
		}
		assert(idx == nbTriangles * 3);
		m_triangleVertexArray =
			new TriangleVertexArray(nbVertices, m_concaveMeshVertices, 3 * sizeof(float),
				nbTriangles, m_concaveMeshIndices, 3 * sizeof(uint),
				TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
				TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

		m_triangleMesh = pc->createTriangleMesh();
		m_triangleMesh->addSubpart(m_triangleVertexArray);
		m_concaveMeshShape = pc->createConcaveMeshShape(m_triangleMesh);
		AddColliderInternal(m_concaveMeshShape);
		return true;
	}
	else
	{
		RemoveCollider();
	}
	return false;
}

void ZobComponentPhysicMesh::Init(DirectZobType::sceneLoadingCallback cb)
{
	ReLoadVariables();
	if (m_mesh == NULL && m_convexMeshPath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			m_convexMeshPath.Reset();
		}
		else
		{
			ZobComponentPhysicShape::Init(cb);
		}
	}
}

void ZobComponentPhysicMesh::EditorUpdate()
{
	if (m_isInit)
	{
		ZobComponentPhysicShape::EditorUpdate();
		if (m_mesh == NULL)
		{
			if (m_convexMeshPath.IsDefined())
			{
				bool bOK = LoadMeshInternal();
				if (!bOK)
				{
					m_convexMeshPath.Reset();
				}
			}
		}
		else
		{
			if (m_mesh->GetName() != m_convexMeshPath.GetName())
			{
				bool bOK = LoadMeshInternal();
				if (!bOK)
				{
					m_convexMeshPath.Reset();
				}
			}
		}
	}
}
