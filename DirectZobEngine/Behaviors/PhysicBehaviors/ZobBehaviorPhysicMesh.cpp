#include "ZobBehaviorPhysicMesh.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicMesh::~ZobBehaviorPhysicMesh()
{

}

ZobBehaviorPhysicMesh::ZobBehaviorPhysicMesh(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicMesh;
	m_convexMeshName = "";
	m_convexMeshPath = "";
	m_convexMeshFile = "";
	m_concaveMeshNbTriangles = 0;
	m_mesh = NULL;
	WrapPath("Mesh", &m_convexMeshName, &m_convexMeshPath,& m_convexMeshFile, false, true);
	WrapVariable(eWrapperType_int, "Nb triangles", &m_concaveMeshNbTriangles, true, false);
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
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->GetOrLoadMesh(m_convexMeshName, m_convexMeshPath, m_convexMeshFile);
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
	if (m_mesh == NULL && m_convexMeshName.size() && m_convexMeshPath.size() && m_convexMeshFile.size())
	{
		if (!LoadMeshInternal())
		{
			m_convexMeshName = "";
			m_convexMeshPath = "";
			m_convexMeshFile = "";
		}
		else
		{
			ZobBehaviorPhysicShape::Init();
		}
	}
}

void ZobBehaviorPhysicMesh::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	if (m_mesh == NULL)
	{
		if (m_convexMeshName.size() && m_convexMeshPath.size() && m_convexMeshFile.size())
		{
			bool bOK = LoadMeshInternal();
			if (!bOK)
			{
				m_convexMeshName = "";
				m_convexMeshPath = "";
				m_convexMeshFile = "";
			}
		}
	}
	else
	{
		if (m_mesh->GetName() != m_convexMeshName)
		{
			bool bOK = LoadMeshInternal();
			if (!bOK)
			{
				m_convexMeshName = "";
				m_convexMeshPath = "";
				m_convexMeshFile = "";
			}
		}
	}
}
