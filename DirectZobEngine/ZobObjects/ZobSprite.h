#pragma once
#include "../Rendering/ZobVector3.h"
#include "../Sprite.h"
#include "ZobObject.h"

class ZobSprite : public ZobObject
{
public:



	ZobSprite(const std::string& name, ZobObject* parent);
	ZobSprite(ulong id, TiXmlElement* node, ZobObject* parent);
	~ZobSprite() override;

	const std::string	GetMeshName() const override { return std::string(""); };

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;
	void				Update() override;
	void				UpdateMesh(const Camera* camera, Core::Engine* engine) override;
	void				SetTexture(std::string& path);
private:
	ZobMaterial* m_material;
};

