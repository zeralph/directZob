#pragma once
#include "Types.h"
#include "MiniFB.h"
#include <math.h>
#include <stdlib.h> 
#include "Color.h"
#include "Triangle.h"
#include <string>
#include <ctime>
#include "Matrix2x2.h"
#include "Matrix4x4.h"
#include <stdarg.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Mesh.h"

#define NB_RASTERIZERS 2

class Engine
{
public:

	template <typename t>
	t clamp2(t x, t min, t max)
	{
		if (x < min) x = min;
		if (x > max) x = max;
		return x;
	}

	enum CullMode
	{
		None = 0,
		CullClockwiseFace,
		CullCounterClockwiseFace
	};

	Engine(int width, int height);
	~Engine();

	void ClearBuffer(const Color* color);
	void CopyBuffer(uint* source, uint * dest);
	void Resize(int width, int height);

	void QueueTriangle(const Triangle* t);
	void DrawGrid(const Camera* camera);

	void ToggleZbufferOutput() { m_showZBuffer = !m_showZBuffer; }
	void GetPixelColor(Color* color, int x, int y);

	void Add(Mesh* mesh);

	void QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c, BufferData* bufferData);
	bool ClipSegment(Vector3* a, Vector3* b);
	int Update(struct Window *window, const Camera* camera);
	inline ulong GetCurrentFrame() { return m_currentFrame; }
	inline const float GetFps() { return m_fps; }
	inline const float GetRenderTime() { return m_renderTimeMS; }
	inline const float GetGeometryTime() { return m_geometryTimeMS;}
	inline const uint Width() const { return m_width; }
	inline const uint Height() const { return m_height; }

	inline const uint GetNbTriangles() const { return m_sceneTriangles; }
	inline const uint GetNbDrawnTriangles() const { return m_drawnTriangles; }

	inline BufferData* GetBufferData() { return &m_bufferData; }

	inline const float GetZNear() { return m_zNear; }
	inline const float GetZFar() { return m_zFar; }
	inline const ulong GetNbPixels() { return m_nbPixels; }
private:

	void DrawHorizontalLine(const float x1, const float x2, const float y, const uint color, BufferData* bufferData);

	std::vector<Mesh*> m_meshes;

	std::vector<const Triangle*> m_rasterTriangleQueues[NB_RASTERIZERS];
	//std::vector<const Triangle*> m_rasterTriangleQueue2;

	std::vector<Line2D> m_rasterLineQueues[NB_RASTERIZERS];
	//std::vector<Line2D> m_rasterLineQueue2;

	Rasterizer* m_rasterizers[NB_RASTERIZERS];
	//Rasterizer* m_rasterizer2;
	std::thread m_rasterThreads[NB_RASTERIZERS];

	int m_width;
	int m_height;
	float m_zNear;
	float m_zFar;
	int m_curBuffer;
	clock_t	m_tick;
	float m_fps;
	float m_geometryTimeMS;
	float m_renderTimeMS;
	ulong m_currentFrame;
	uint* m_buffer;
	float* m_zBuffer;
	BufferData m_bufferData;
	bool m_showZBuffer;
	uint m_sceneTriangles;
	uint m_drawnTriangles;
	ulong m_nbPixels;
};