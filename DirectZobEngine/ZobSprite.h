#pragma once
#include "ZobVector3.h"
#include "Sprite.h"
#include "ZobObject.h"

class ZobSprite : public ZobObject
{
public:



	ZobSprite(const std::string& name, ZobObject* parent);
	ZobSprite(TiXmlElement* node, ZobObject* parent);
	~ZobSprite() override;

	const std::string	GetMeshName() const override { return std::string(""); };

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;
	void				Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix) override;
	void				UpdateMesh(const Camera* camera, Core::Engine* engine) override;
	void				SetTexture(std::string& path);
private:
	ZobMaterial* m_material;
};


