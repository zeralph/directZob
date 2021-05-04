#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Mesh.h"

class Sprite : public Mesh
{
public:

	Sprite(std::string& name);
	~Sprite();
	void QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const RenderOptions* options) override;
	void Setmaterial(const ZobMaterial* m);
private:

	ZobMatrix4x4 m_modelMatrixOverriden;
	ZobMatrix4x4 m_rotationMatrixOverriden;
	DirectZobType::RenderOptions m_triangleOptions;
};

