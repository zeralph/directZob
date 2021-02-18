#pragma once
#include "../Types.h"
#include "../../dependencies/minifb/include/MiniFB.h"
#include <math.h>
#include <stdlib.h> 
#include "Color.h"
#include "Triangle.h"
#include <string>
#include <ctime>
#include "ZobMatrix2x2.h"
#include "ZobMatrix4x4.h"
#include <stdarg.h>
#include "ZobVector2.h"
#include "ZobVector3.h"
#include "../ZobMaterial.h"
//#include "Camera.h"
#include "../Events.h"

class Rasterizer;
class Camera;
namespace Core
{
	class Engine
	{
	public:
		Engine(int width, int height, Events* events);
		~Engine();

		void											Start();
		const inline bool								Started() const { return m_started; }
		void											Stop();
		void											ClearBuffer(const Color* color);
		void											CopyBuffer(uint* source, uint* dest);
		void											QueueTriangle(const Camera* c, const Triangle* t);
		void											DrawGrid(const Camera* camera);

		void											ToggleZbufferOutput() { m_showZBuffer = !m_showZBuffer; }

		void											QueueLine(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const uint c, bool bold, bool noZ);
		void											QueueEllipse(const Camera* camera, const ZobVector3* center, const ZobVector3* vectorUp, const float r1, const float r2, const uint c, bool bold, bool noZ);
		void											QueueSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ);
		void											QueueWorldBox(const Camera* camera, const Box* box, const uint c, bool bold, bool noZ);
		void											QueueBox(const Camera* camera, const ZobMatrix4x4* mat, const ZobVector3* halfExtends, const ZobVector3* pivot, const uint c, bool bold, bool noZ);
		void											QueueCapsule(const Camera* camera, const ZobMatrix4x4* mat, float radius, float height, const ZobVector3* dir, const uint c, bool bold, bool noZ);
		void											QueueMesh(const Camera* camera, const ZobMatrix4x4* mat, ZobVector3* points, int width, int height, const uint c, bool bold);
		int												StartDrawingScene();
		int												SetDisplayedBuffer();
		inline ulong									GetCurrentFrame() { return m_currentFrame; }
		inline const uint								GetNbDrawnTriangles() const { return m_drawnTriangles; }
		const Rasterizer*								GetRasterizer(int idx) const { return m_rasterizers[idx]; }
		int												GetNbRasterizer() const { return m_nbRasterizers; }
		inline BufferData*								GetBufferData() { return &m_bufferData; }
		inline const float								GetZNear() { return m_zNear; }
		inline const float								GetZFar() { return m_zFar; }
		inline const ulong								GetNbPixels() { return m_nbPixels; }
		inline void										ShowGrid(bool b) { m_showGrid = b; }
		inline void										ShowBBoxes(bool b) { m_showBBoxes = b; }
		inline void										DrawPhysyicsGizmos(bool b) { m_drawPhysicsGizmos = b; }
		inline void										ShowText(bool b) { m_showText = b; }
		inline void										WireFrame(bool b) { m_wireFrame = b; }
		inline void										ShowNormals(bool b) { m_showNormals = b; }
		inline void										DrawGizmos(bool b) { m_drawGizmos = b; }
		inline void										LockFrustrum(bool b) { m_lockFrustrum=b; }
		inline void										DrawCameraGizmos(bool b) { m_drawCameraGizmos = b; }
		inline void										DrawZobObjectGizmos(bool b) { m_drawZobObjectGizmos = b; }
		inline bool										WireFrame() { return m_wireFrame; }
		inline bool										ShowGrid() { return m_showGrid; }
		inline bool										ShowNormals() { return m_showNormals; }
		inline bool										DrawGizmos() { return m_drawGizmos; }
		inline bool										ShowBBoxes() { return m_showBBoxes; }
		inline bool										ShowText() { return m_showText; }
		inline bool										DrawPhysyicsGizmos() { return m_drawPhysicsGizmos; }
		inline bool										DrawCameraGizmos() { return m_drawCameraGizmos; }
		inline bool										DrawZobObjectGizmos() { return m_drawZobObjectGizmos; }
		inline bool										LockFrustrum() { return m_lockFrustrum;  }
		inline void										SetRenderMode(eRenderMode b) { m_renderMode = b; }
		inline const eRenderMode						GetRenderMode() { return m_renderMode; }
		inline void										SetRenderOutput(eRenderOutput r) { m_renderOutput = r; }
		inline const eRenderOutput						GetRenderOutput() { return m_renderOutput; }
		inline const eLightingPrecision					GetLightingPrecision() const  { return m_lightingPrecision ; }
		void											SetLightingPrecision(eLightingPrecision l) { m_lightingPrecision = l; }
		uint											GetObjectIdAtCoords(uint x, uint y);
		float											WaitForRasterizersEnd();
		void											ClearRenderQueues();
		void											Resize(int width, int height);
		bool											GetProjectedCoords(ZobVector3* worldPos);
		float											GetDistanceToCamera(ZobVector3* worldPos);
		bool											LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection, ZobVector3* ret);
		bool											LineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt);
		bool											LineLineIntersection(const ZobVector3* s0, const ZobVector3* s1, const ZobVector3* e0, const ZobVector3* e1, ZobVector3 &out, float& outFactor) const;
		ZobObject*										GetObjectAt2DCoords(float x, float y);
		void											ComputeBoundingBoxes(const ZobMatrix4x4* modelMatrix, const ZobVector3* minBounding, const ZobVector3* maxBounding, Box* obb, Box* aabb) const;
		bool											IsInFrustrum(const Camera* c, const Box* aabb) const;
	private:	
		inline float									clamp2(float x, float min, float max) const { if (x < min) x = min; if (x > max) x = max; return x; }
		void											DrawHorizontalLine(const float x1, const float x2, const float y, const uint color);
		void											ClipSegmentToPlane(ZobVector3 &s0, ZobVector3 &s1, ZobVector3 &pp, ZobVector3 &pn);
		void											QueuePartialSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ, float from, float to);
		uint											SubDivideClippedTriangle(const Camera* c, const Triangle* t);
		void											RecomputeTriangleProj(const Camera* c, Triangle* t);
		uint											ClipTriangle(const Camera* c, const Triangle* t);
		void											QueueTriangleInRasters(const Triangle* t, int idx) const;
		void											QueueLineInRasters(const Line3D* l, int idx) const;
		inline void										RecomputeUv(const ZobVector2* uva, ZobVector2* uvb, float r) const
		{
			uvb->x = uva->x + (uvb->x - uva->x) * r;
			uvb->y = uva->y + (uvb->y - uva->y) * r;
		}

		Rasterizer** m_rasterizers;
		Events* m_events;

		Line3D* m_LineQueue;
		Triangle* m_TrianglesQueue;
		ZobVector3* m_verticesData;
		ZobVector2* m_uvData;

		long m_TriangleQueueSize;
		long m_maxTrianglesQueueSize;

		long m_lineQueueSize;
		long m_maxLineQueueSize;
		
		
		uint m_nbRasterizers;
		float m_zNear;
		float m_zFar;
		int m_curBuffer;
		ulong m_currentFrame;
		uint* m_buffer;
		float* m_zBuffer;
//		uint* m_oBuffer;
		BufferData m_bufferData;
		bool m_showZBuffer;
		uint m_drawnTriangles;
		ulong m_nbPixels;
		volatile bool m_started;
		bool m_wireFrame;
		int m_rasterizerHeight;
		bool m_showNormals;
		bool m_showGrid;
		bool m_drawGizmos;
		bool m_drawPhysicsGizmos;
		bool m_drawCameraGizmos;
		bool m_drawZobObjectGizmos;
		bool m_showBBoxes;
		bool m_showText;
		volatile bool m_lockFrustrum;
		eRenderOutput m_renderOutput;
		eRenderMode m_renderMode = eRenderMode_fullframe;
		eLightingPrecision m_lightingPrecision = eLightingPrecision_vertex;
		const uint oBufferColors[8] = { 0x000000, 0x00FF00, 0x0000FF,
										 0xFFFF00, 0x00FFFF, 0xFF00FF,
										 0xFFFFFF, 0xFF0000, };
	};
}