#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include "../Sprite.h"
#include "Misc/ZobFilePath.h"
#include <string>
#include <vector>

class ZobBehaviorSprite : public ZobBehavior
{
	friend class ZobBehaviorFactory;
public:

	~ZobBehaviorSprite() override;
	void						Init() override;
	void						PreUpdate(float dt) override;
	void						PostUpdate() override;
	void						UpdateBeforeObject(float dt) override {}
	void						UpdateAfterObject(float dt) override;
	void						QueueForDrawing(const Camera* camera, Engine* engine) override;
	void						EditorUpdate() override;
	void						Set(ZobFilePath zfp);
	void						SetVisible(bool v) { m_sprite->SetVisible(v); }
	Triangle::RenderOptions*	GetRenderOptions() { return &m_renderOptions; }
	const ZobSprite*			GetMesh() const { return m_sprite; }
	static void					ReloadMaterial(zobId id);
	void						SetForEditor();
private:
								ZobBehaviorSprite(ZobObject* zobObject, bool bEditorZobBehavior);
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