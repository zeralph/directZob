#ifndef DZOB_ZOB_SPRITE_H
#define DZOB_ZOB_SPRITE_H

#include "./Vector3.h"
#include "Sprite.h"
#include "ZobObject.h"
namespace directZob {
class ZobSprite : public ZobObject
{
public:



	ZobSprite(const std::string& name, ZobObject* parent);
	ZobSprite(TiXmlElement* node, ZobObject* parent);
	~ZobSprite() override;

	const std::string	GetMeshName() const override { return std::string(""); };

	void				DrawGizmos(const Camera* camera, Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;
	void				Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix) override;
	void				UpdateMesh(const Camera* camera, Engine* engine) override;
	void				SetTexture(std::string& path);
private:
	Material* m_material;
};
}
#endif
