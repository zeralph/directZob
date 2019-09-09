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
#include "Events.h"

#define Z_NEAR 0.001f
#define Z_FAR 500.0f
namespace Core
{
	class Engine
	{
	public:

		enum CullMode
		{
			None = 0,
			CullClockwiseFace,
			CullCounterClockwiseFace
		};

		Engine(int width, int height, Events* events);
		~Engine();

		inline void Start() { m_started = true; }
		inline void Stop() { m_started = false; }
		void ClearBuffer(const Color* color);
		void CopyBuffer(uint* source, uint* dest);
		void Resize(int width, int height);

		void QueueTriangle(const Triangle* t);
		void DrawGrid(const Camera* camera);

		void ToggleZbufferOutput() { m_showZBuffer = !m_showZBuffer; }

		void QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c);
		bool ClipSegment(Vector3* a, Vector3* b);
		int DrawScene();
		void LoadMesh(std::string& name, std::string& path, const Texture* texture, Vector3& p, Vector3& r, Vector3& s);
		inline ulong GetCurrentFrame() { return m_currentFrame; }
		inline const float GetFps() { return m_fps; }
		inline const float GetRenderTime() { return m_renderTimeMS; }
		inline const float GetGeometryTime() { return m_geometryTimeMS; }

		inline const uint GetNbTriangles() const { return m_sceneTriangles; }
		inline const uint GetNbDrawnTriangles() const { return m_drawnTriangles; }

		inline BufferData* GetBufferData() { return &m_bufferData; }
		//inline Camera* GetCurrentCamera() { return}
		inline const float GetZNear() { return m_zNear; }
		inline const float GetZFar() { return m_zFar; }
		inline const ulong GetNbPixels() { return m_nbPixels; }

		std::string GetObjectList();

	private:

		inline float clamp2(float x, float min, float max) const { if (x < min) x = min; if (x > max) x = max; return x; }
		void DrawHorizontalLine(const float x1, const float x2, const float y, const uint color);
		void WaitForRasterizersEnd() const;

		Events* m_events;
		std::vector<const Triangle*>* m_rasterTriangleQueues;
		std::vector<Line2D>* m_rasterLineQueues;
		std::vector<Rasterizer*>* m_rasterizers;
		uint m_nbRasterizers;
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
		bool m_started = false;
	};
}