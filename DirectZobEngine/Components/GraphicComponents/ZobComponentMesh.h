#pragma once

#undef min
#undef max
#undef None

#include "../ZobComponent.h"
#include "../../Rendering/Triangle.h"
#include "../../Misc/ZobFilePath.h"
#include <string>
#include <vector>

class Mesh;
class ZobComponentMesh : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentMesh() override;
	void						Init(DirectZobType::sceneLoadingCallback cb) override;
	void						PreUpdate(float dt) override;
	void						PostUpdate() override;
	void						QueueForDrawing(const Camera* camera, Engine* engine) override;
	void						EditorUpdate() override;
	void						Set(ZobFilePath& zfp);
	void						SetVisible(bool v);
	Triangle::RenderOptions*	GetRenderOptions() { return &m_renderOptions; }
	const Mesh*					GetMesh() const { return m_mesh; }
private:
								ZobComponentMesh(ZobObject* zobObject, bool bEditorZobComponent);
	bool						LoadMeshInternal();
	Mesh*						m_mesh;
	Triangle::RenderOptions		m_renderOptions;
	ZobFilePath					m_meshPath;
	int							m_meshNbTriangles;
	uint						m_meshSize;

};