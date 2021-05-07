#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include "../Mesh.h"
#include <string>
#include <vector>

class ZobBehaviorMesh : public ZobBehavior
{
	friend class ZobBehaviorFactory;
public:

	~ZobBehaviorMesh() override;
	void					Init() override;
	void					PreUpdate(float dt) override;
	void					PostUpdate() override;
	void					UpdateBeforeObject(float dt) override {}
	void					UpdateAfterObject(float dt) override;
	void					QueueForDrawing(const Camera* camera, Engine* engine) override;
	void					EditorUpdate() override;
	void					Set(ZobFilePath zfp);
	RenderOptions*			GetRenderOptions() { return &m_renderOptions; }
private:
							ZobBehaviorMesh(ZobObject* zobObject);
	bool					LoadMeshInternal();
	Mesh*					m_mesh;
	RenderOptions			m_renderOptions;
	ZobFilePath				m_meshPath;
	int						m_meshNbTriangles;

};