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


		float* m_concaveMeshVertices;
		uint* m_concaveMeshIndices;
		int m_concaveMeshNbTriangles;
		std::string m_convexMeshName;
		std::string m_convexMeshPath;
		std::string m_convexMeshFile;

};