#pragma once
#include "Vector3.h"
#include "ZobObject.h"

class ZobSprite : public ZobObject
{
public:



	ZobSprite(const std::string& name, ZobObject* parent);
	ZobSprite(TiXmlElement* node, ZobObject* parent);
	~ZobSprite() override;

	void				DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*			SaveUnderNode(TiXmlNode* node) override;
	void				Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix) override;
private:

};


