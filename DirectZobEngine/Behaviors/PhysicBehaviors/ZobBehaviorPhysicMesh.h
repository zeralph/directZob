#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehaviorPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobBehaviorPhysicMesh : public ZobBehaviorPhysicShape
{
	friend class ZobBehaviorFactory;
	public:
								~ZobBehaviorPhysicMesh() override;
		void					Init() override;
		void					EditorUpdate() override;

	private:	
								ZobBehaviorPhysicMesh(ZobObject* zobObject);
		bool					LoadMeshInternal();
		void					RemoveCollider() override;

		Mesh*									m_mesh;
		float*									m_concaveMeshVertices;
		uint*									m_concaveMeshIndices;
		int										m_concaveMeshNbTriangles;
		ZobFilePath								m_convexMeshPath;
		reactphysics3d::TriangleVertexArray*	m_triangleVertexArray;
		reactphysics3d::TriangleMesh*			m_triangleMesh;
		reactphysics3d::ConcaveMeshShape*		m_convexMeshShape;

};