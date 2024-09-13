#pragma once

#undef min
#undef max
#undef None

#include "../ZobComponent.h"
#include "../../Misc/ZobFilePath.h"
#include "../../Rendering/Triangle.h"
#include <string>
#include <vector>

class ZobSprite;
class ZobComponentSprite : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentSprite() override;
	void						Init(DirectZobType::sceneLoadingCallback cb) override;
	void						PreUpdate(float dt) override;
	void						PostUpdate() override;
	void						QueueForDrawing(const Camera* camera, Engine* engine) override;
	void						EditorUpdate() override;
	void						Set(ZobFilePath zfp);
	void						SetVisible(bool v);
	Triangle::RenderOptions*	GetRenderOptions() { return &m_renderOptions; }
	const ZobSprite*			GetMesh() const { return m_sprite; }
	static void					ReloadMaterial(zobId id);
	void						SetForEditor();
private:
								ZobComponentSprite(ZobObject* zobObject, bool bEditorZobComponent);
	bool						LoadMeshInternal();
	
	ZobSprite*					m_sprite;
	ZobObject*					m_zobObject;
	const ZobMaterial*			m_material;
	Triangle::RenderOptions		m_renderOptions;
	ZobFilePath					m_texturePath;
	ZobColor					m_ambientColor;
	ZobColor					m_diffuseColor;
	ZobColor					m_specularColor;
};