#pragma once
#include "../Rendering/ZobVector3.h"
#include "../Sprite.h"
#include "ZobObject.h"

class ZobSprite : public ZobObject
{
public:



	ZobSprite(const std::string& name, ZobObject* parent);
	ZobSprite(std::string id, TiXmlElement* node, ZobObject* parent);
	~ZobSprite() override;

	void				DrawGizmos(const Camera* camera, Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;
	void				Update(float dt) override;
	//void				UpdateMesh(const Camera* camera, Engine* engine) override;
	void				SetTexture(std::string& path);
private:
	ZobMaterial* m_material;
};


