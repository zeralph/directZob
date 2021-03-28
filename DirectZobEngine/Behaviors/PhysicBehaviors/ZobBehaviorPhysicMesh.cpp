#include "ZobBehaviorPhysicMesh.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicMesh::~ZobBehaviorPhysicMesh()
{

}

ZobBehaviorPhysicMesh::ZobBehaviorPhysicMesh(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicMesh;
	Init();
}

void ZobBehaviorPhysicMesh::Init()
{
	ZobBehaviorPhysicShape::Init();
	const Mesh* m = DirectZob::GetInstance()->GetMeshManager()->GetOrLoadMesh(m_convexMeshName, m_convexMeshPath, m_convexMeshFile);
	if (m)
	{
		PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
		const int nbVertices = m->GetNbVertices();
		const int nbTriangles = m->GetNbTriangles();
		m_concaveMeshVertices = (float*)malloc(sizeof(float) * nbVertices * 3);
		m_concaveMeshIndices = (uint*)malloc(sizeof(uint) * nbTriangles * 3);
		m_concaveMeshNbTriangles = nbTriangles;
		int idx = 0;
		for (int i = 0; i < nbVertices; i++)
		{
			m_concaveMeshVertices[idx] = m->GetVertices()[i].x;
			idx++;
			m_concaveMeshVertices[idx] = m->GetVertices()[i].y;
			idx++;
			m_concaveMeshVertices[idx] = m->GetVertices()[i].z;
			idx++;
		}
		assert(idx == nbVertices * 3);
		idx = 0;
		//for (std::vector<Triangle>::const_iterator iter = m->GetTriangles().begin(); iter != m->GetTriangles().end(); iter++)
		for (int i = 0; i < nbTriangles; i++)
		{
			const Triangle* t = &m->GetTriangles()->at(i);
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
	}
	else
	{
		RemoveCollider();
	}
}

void ZobBehaviorPhysicMesh::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
}
