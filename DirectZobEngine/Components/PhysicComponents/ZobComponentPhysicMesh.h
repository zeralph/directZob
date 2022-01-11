#pragma once

#undef min
#undef max
#undef None

#include "ZobComponentPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobComponentPhysicMesh : public ZobComponentPhysicShape
{
	friend class ZobComponentFactory;
	public:
								~ZobComponentPhysicMesh() override;
		void					Init(DirectZobType::sceneLoadingCallback cb) override;
		void					EditorUpdate() override;

	private:	
								ZobComponentPhysicMesh(ZobObject* zobObject);
		bool					LoadMeshInternal();
		void					RemoveCollider() override;

		Mesh*									m_mesh;
		float*									m_concaveMeshVertices;
		uint*									m_concaveMeshIndices;
		int										m_concaveMeshNbTriangles;
		ZobFilePath								m_convexMeshPath;
		reactphysics3d::TriangleVertexArray*	m_triangleVertexArray;
		reactphysics3d::TriangleMesh*			m_triangleMesh;
		reactphysics3d::ConcaveMeshShape*		m_concaveMeshShape;

};