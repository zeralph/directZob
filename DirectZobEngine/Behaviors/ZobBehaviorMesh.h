#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include "../DirectZobEngine/Mesh.h"
#include <string>
#include <vector>

class ZobBehaviorMesh : public ZobBehavior
{
	friend class ZobBehaviorFactory;
public:
	~ZobBehaviorMesh() override;
	void					Init() override;
	void					PreUpdate() override;
	void					PostUpdate() override;
	void					UpdateBeforeObject(float dt) override {}
	void					UpdateAfterObject(float dt) override;
	void					EditorUpdate() override;

private:
							ZobBehaviorMesh(ZobObject* zobObject);
	bool					LoadMeshInternal();
	Mesh*					m_mesh;
	std::string				m_meshName;
	std::string				m_meshPath;
	std::string				m_meshFile;
	int						m_meshNbTriangles;

};