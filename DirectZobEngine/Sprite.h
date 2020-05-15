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

	//void Update(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options) override;
	//void QueueForDrawing(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options) override;
	void Setmaterial(const Material* m);
private:
	const Material* m_material = NULL;
	Matrix4x4 m_modelMatrixOverriden;
	Matrix4x4 m_rotationMatrixOverriden;
};

