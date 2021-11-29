#include "ZobBehaviorPhysicMesh.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicMesh::~ZobBehaviorPhysicMesh()
{

}

ZobBehaviorPhysicMesh::ZobBehaviorPhysicMesh(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicMesh;
	m_convexMeshPath.Init();
	m_concaveMeshNbTriangles = 0;
	m_mesh = NULL;
	m_varExposer->WrapVariable<ZobFilePath>("Mesh", &m_convexMeshPath, NULL, false, true);
	m_varExposer->WrapVariable<int>("Nb triangles", &m_concaveMeshNbTriangles, NULL, true, false);
}

void ZobBehaviorPhysicMesh::RemoveCollider()
{
	ZobBehaviorPhysicShape::RemoveCollider();
	m_mesh = NULL;
	free(m_concaveMeshVertices);
	free(m_concaveMeshIndices);
	m_concaveMeshVertices = NULL;
	m_concaveMeshIndices = NULL;
	m_concaveMeshNbTriangles = 0;
}

bool ZobBehaviorPhysicMesh::LoadMeshInternal()
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->GetOrLoadMesh(m_convexMeshPath.name, m_convexMeshPath.path, m_convexMeshPath.file, m_convexMeshPath.bAbsolute);
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
		TriangleVertexArray* triangleArray =
			new TriangleVertexArray(nbVertices, m_concaveMeshVertices, 3 * sizeof(float),
				nbTriangles, m_concaveMeshIndices, 3 * sizeof(uint),
				TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
				TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

		TriangleMesh* triangleMesh = pc->createTriangleMesh();
		triangleMesh->addSubpart(triangleArray);
		ConcaveMeshShape* concaveMesh = pc->createConcaveMeshShape(triangleMesh);
		AddColliderInternal(concaveMesh);
		return true;
	}
	else
	{
		RemoveCollider();
	}
	return false;
}

void ZobBehaviorPhysicMesh::Init()
{
	ReLoadVariables();
	if (m_mesh == NULL && m_convexMeshPath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			m_convexMeshPath.Init();
		}
		else
		{
			ZobBehaviorPhysicShape::Init();
		}
	}
}

void ZobBehaviorPhysicMesh::EditorUpdate()
{
	if (m_isInit)
	{
		ZobBehaviorPhysicShape::EditorUpdate();
		if (m_mesh == NULL)
		{
			if (m_convexMeshPath.IsDefined())
			{
				bool bOK = LoadMeshInternal();
				if (!bOK)
				{
					m_convexMeshPath.Init();
				}
			}
		}
		else
		{
			if (m_mesh->GetName() != m_convexMeshPath.name)
			{
				bool bOK = LoadMeshInternal();
				if (!bOK)
				{
					m_convexMeshPath.Init();
				}
			}
		}
	}
}
