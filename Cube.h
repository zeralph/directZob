#pragma once
#include "Types.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Engine.h"

class Cube
{
public:
	Cube();
	~Cube();

	void Draw(Engine* engine);

private:

	Matrix4x4	m_modelMatrix;

	Vector3 m_vertices[8] = {	Vector3(-1.0f, 1.0f, -1.0f), 
								Vector3(1.0f, 1.0f, -1.0f), 
								Vector3(1.0f, -1.0f, -1.0f), 
								Vector3(-1.0f, -1.0f, -1.0f), 
								Vector3(-1.0f, 1.0f, 1.0f), 
								Vector3(1.0f, 1.0f, 1.0f), 
								Vector3(1.0f, -1.0f, 1.0f), 
								Vector3(-1.0f, -1.0f, 1.0f) 
							};
	uint m_verticesIndex[36] = {0, 1, 3, 1, 2, 3, 
								1, 5, 2, 5, 6, 2,
								5, 4, 6, 4, 7, 6,
								4, 0, 7, 0, 3, 7,
								4, 5, 0, 5, 1, 0,
								7, 6, 3, 6, 2, 3,
							};
};