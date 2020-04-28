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
#include "Material.h"
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

		enum RenderOutput
		{
			RenderOutput_render = 0,
			RenderOutput_zBuffer,
			RenderOutput_oBuffer,
			RenderOutputMAX
		};
		enum CullMode
		{
			None = 0,
			CullClockwiseFace,
			CullCounterClockwiseFace
		};

		Engine(int width, int height, Events* events);
		~Engine();

		inline void Start() { m_started = true; }
		const inline bool Started() const { return m_started; }
		void Stop();
		void ClearBuffer(const Color* color);
		void CopyBuffer(uint* source, uint* dest);
		void Resize(int width, int height);

		void QueueTriangle(const Triangle* t);
		void DrawGrid(const Camera* camera);

		void ToggleZbufferOutput() { m_showZBuffer = !m_showZBuffer; }

		void QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c);
		bool ClipSegment(Vector3* a, Vector3* b);
		int StartDrawingScene();
		int EndDrawingScene();
		void LoadMesh(std::string& name, std::string& path, const Material* texture, Vector3& p, Vector3& r, Vector3& s);
		inline ulong GetCurrentFrame() { return m_currentFrame; }
		inline const float GetFps() { return m_fps; }
		inline const float GetRenderTime() { return m_renderTimeMS; }
		inline const float GetGeometryTime() { return m_geometryTimeMS; }
		inline const  float GetFrameTime() { return m_frameTimeMS; }
		inline void SetGeometryTime(float f) { m_geometryTimeMS = f; }
		inline const uint GetNbTriangles() const { return m_sceneTriangles; }
		inline const uint GetNbDrawnTriangles() const { return m_drawnTriangles; }

		inline void SetRenderOutput(RenderOutput r) { m_renderOutput = r; }

		inline BufferData* GetBufferData() { return &m_bufferData; }
		//inline Camera* GetCurrentCamera() { return}
		inline const float GetZNear() { return m_zNear; }
		inline const float GetZFar() { return m_zFar; }
		inline const ulong GetNbPixels() { return m_nbPixels; }
		inline void ShowGrid(bool b) { m_showGrid = b; }
		inline void WireFrame(bool b) { m_wireFrame = b; }
		inline void ShowNormals(bool b) { m_showNormals = b; }
		inline bool ShowGrid() { return m_showGrid; }
		inline bool WireFrame() { return m_wireFrame; }
		inline bool ShowNormals() { return m_showNormals; }
		uint GetObjectIdAtCoords(uint x, uint y);
		const CullMode GetCullMode() const { return m_cullMode; }
		void SetCullMode(const CullMode c) { m_cullMode = c; }
		void WaitForRasterizersEnd();
		void ClearRenderQueues();
	private:

		inline float clamp2(float x, float min, float max) const { if (x < min) x = min; if (x > max) x = max; return x; }
		void DrawHorizontalLine(const float x1, const float x2, const float y, const uint color);
		void ClipSegmentToPlane(Vector3 &s0, Vector3 &s1, Vector3 &pp, Vector3 &pn);

		Events* m_events;
		Triangle** m_rasterTriangleQueues;
		long* m_rasterNbTriangleQueues;
		std::vector<Line3D>* m_rasterLineQueues;
		std::vector<Rasterizer*>* m_rasterizers;
		uint m_nbRasterizers;
		uint m_triangleQueueSize;
		float m_zNear;
		float m_zFar;
		int m_curBuffer;
		clock_t	m_tick;
		clock_t	m_drawTick;
		float m_fps;
		float m_geometryTimeMS;
		float m_renderTimeMS;
		float m_frameTimeMS;
		ulong m_currentFrame;
		uint* m_buffer;
		float* m_zBuffer;
//		uint* m_oBuffer;
		BufferData m_bufferData;
		bool m_showZBuffer;
		uint m_sceneTriangles;
		uint m_drawnTriangles;
		ulong m_nbPixels;
		bool m_started = false;
		bool m_wireFrame = false;
		bool m_showNormals = false;
		bool m_showGrid = true;
		RenderOutput m_renderOutput;
		CullMode m_cullMode;

		const uint oBufferColors[8] = { 0x000000, 0x00FF00, 0x0000FF,
										 0xFFFF00, 0x00FFFF, 0xFF00FF,
										 0xFFFFFF, 0xFF0000, };
	};
}