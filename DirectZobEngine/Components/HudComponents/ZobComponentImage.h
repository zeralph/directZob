#pragma once
#include "../../Types.h"
#include "../../tinyxml/tinyxml.h"
#include "../ZobComponent.h"
#include "../../Managers/ZobHUDManager.h"
#include <string>
#include <vector>

class ZobComponentImage : public ZobComponent
{
	friend class ZobComponentFactory;
public:

	~ZobComponentImage() override;
	void					Init() override;
	void					PreUpdate(float dt) override;
	void					UpdateAfterObject(float dt) override {}
	void					UpdateBeforeObject(float dt) override;
	void					PostUpdate() override {}
	void					EditorUpdate() override;
	void					DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
private:
	ZobComponentImage(ZobObject* zobObject, bool bEditorZobComponent);
	void					LoadMaterialInternal();
	float m_x;
	float m_y;
	float m_w;
	float m_h;
	ZobFilePath m_texture;
	const ZobMaterial* m_image;
	float m_z;
	bool m_visible;
};