#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Mesh.h"
class ZobComponentMesh;
class ZobSprite : public Mesh
{
public:

	ZobSprite(std::string& name, ZobComponentMesh* zm);
	~ZobSprite();
	void QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const Triangle::RenderOptions* options) override;
	void Setmaterial(const ZobMaterial* m);

private:

	ZobMatrix4x4 m_modelMatrixOverriden;
	ZobMatrix4x4 m_rotationMatrixOverriden;
	Triangle::RenderOptions m_triangleOptions;
};

