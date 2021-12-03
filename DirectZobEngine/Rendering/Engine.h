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
#include "../Events.h"
#include "../Misc/ZobVariablesExposer.h"

class Rasterizer;
class Camera;
class Engine
{
public:

	Engine(int width, int height, Events* events);
	~Engine();

	void											Start();
	const inline bool								Started() const { return m_started; }
	void											Stop();
	void											ClearBuffer(const ZobColor* color);
	void											CopyBuffer(uint* source, uint* dest);
	void											QueueWorldTriangle(const Camera* c, const Triangle* t);
	void											QueueProjectedTriangle(const Camera* c, const Triangle* t);
	void											DrawGrid(const Camera* camera);

	void											ToggleZbufferOutput() { m_showZBuffer = !m_showZBuffer; }

	void											QueueLine(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const uint c, bool bold, bool noZ);
	void											QueueTriangle(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const ZobVector3* v3, const uint c, bool transparent, bool noZ);
	void											QueueEllipse(const Camera* camera, const ZobVector3* center, const ZobVector3* vectorUp, const float r1, const float r2, const uint c, bool bold, bool noZ);
	void											QueueSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ);
	void											QueueWorldBox(const Camera* camera, const Box* box, const uint c, bool bold, bool noZ);
	void											QueueBox(const Camera* camera, const ZobMatrix4x4* mat, const ZobVector3* halfExtends, const ZobVector3* pivot, const uint c, bool bold, bool noZ);
	void											QueueCapsule(const Camera* camera, const ZobMatrix4x4* mat, float radius, float height, const ZobVector3* dir, const uint c, bool bold, bool noZ);
	void											QueueMesh(const Camera* camera, const ZobMatrix4x4* mat, ZobVector3* points, int width, int height, const uint c, bool bold);
	int												StartDrawingScene();
	int												SetDisplayedBuffer();
	inline ulong									GetCurrentFrame() const { return m_currentFrame; }
	inline const uint								GetNbDrawnTriangles() const { return m_drawnTriangles; }
	const Rasterizer*								GetRasterizer(int idx) const { return m_rasterizers[idx]; }
	const int										GetNbRasterizer() const { return m_nbRasterizers; }
	const int										GetRasterizerHeight() const { return m_rasterizerHeight; }
	inline BufferData*								GetBufferData() { return &m_bufferData; }
	inline const float								GetZNear() const { return m_zNear; }
	inline const float								GetZFar() const { return m_zFar; }
	inline const ulong								GetNbPixels() const { return m_nbPixels; }
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
	inline bool										WireFrame() const { return m_wireFrame; }
	inline bool										ShowGrid() const { return m_showGrid; }
	inline bool										ShowNormals() const { return m_showNormals; }
	inline bool										DrawGizmos() const { return m_drawGizmos; }
	inline bool										ShowBBoxes() const { return m_showBBoxes; }
	inline bool										ShowText() const { return m_showText; }
	inline bool										DrawPhysyicsGizmos() const { return m_drawPhysicsGizmos; }
	inline bool										DrawCameraGizmos() const { return m_drawCameraGizmos; }
	inline bool										DrawZobObjectGizmos() const { return m_drawZobObjectGizmos; }
	inline bool										LockFrustrum() const { return m_lockFrustrum;  }
	inline void										SetRenderMode(eRenderMode b) { m_renderMode = b; }
	inline const eRenderMode						GetRenderMode() const { return m_renderMode; }
	inline const eLightingPrecision					GetLightingPrecision() const  { return m_lightingPrecision ; }
	void											SetLightingPrecision(eLightingPrecision l) { m_lightingPrecision = l; }
	uint											GetObjectIdAtCoords(uint x, uint y);
	float											WaitForRasterizersEnd();
	void											StopRasterizers();
	void											ClearRenderQueues();
	bool											Resize(int width, int height);
	bool											GetProjectedCoords(ZobVector3* worldPos);
	float											GetDistanceToCamera(ZobVector3* worldPos);
	bool											LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection, ZobVector3* ret);
	bool											LineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt);
	bool											LineLineIntersection(const ZobVector3* s0, const ZobVector3* s1, const ZobVector3* e0, const ZobVector3* e1, ZobVector3 &out, float& outFactor) const;
	ZobObject*										GetObjectAt2DCoords(float x, float y);
	void											ComputeBoundingBoxes(const ZobMatrix4x4* modelMatrix, const ZobVector3* minBounding, const ZobVector3* maxBounding, Box* obb, Box* aabb) const;
	bool											IsInFrustrum(const Camera* c, const Box* aabb) const;
	void											SwapBuffers();
	void											UpdateEditorBitmapData();
	void											PrintRasterizersInfos();
	inline const bool								EqualizeTriangleQueues() const {return m_EqualizeTriangleQueues;}
	void 											EnablePerspectiveCorrection(bool enable);
	inline const eBitsPerColor						GetNbBitsPerColorDepth() const {return m_nbBitsPerColorDepth;}
	inline const bool								UsePerspectiveCorrection() const {return m_perspCorrection;}
	void											SaveUnderNode(TiXmlElement* node);
	void											LoadFromNode(TiXmlElement* node);
	inline const bool								UseDithering() const { return m_dithering; }
	ZobVariablesExposer*							GetVariablesExposer() { return m_varExposer; }
private:	
	void											DrawHorizontalLine(const float x1, const float x2, const float y, const uint color);
	void											ClipSegmentToPlane(ZobVector3 &s0, ZobVector3 &s1, ZobVector3 &pp, ZobVector3 &pn);
	void											QueuePartialSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ, float from, float to);
	uint											SubDivideClippedTriangle(const Camera* c, const Triangle* t);
	void											RecomputeTriangleProj(const Camera* c, Triangle* t);
	uint											ClipTriangle(const Camera* c, Triangle* t);
	int												QueueTriangleInRasters(const Triangle* t, int idx) const;
	void											QueueLineInRasters(const Line3D* l, int idx) const;
	bool											ResizeInternal();
	inline void										RecomputeUv(const ZobVector2* uva, ZobVector2* uvb, float r) const
	{
		uvb->x = uva->x + (uvb->x - uva->x) * r;
		uvb->y = uva->y + (uvb->y - uva->y) * r;
	}
	inline void										RecomputeNormal(const ZobVector3* na, ZobVector3* nb, float r) const
	{
		static bool  titi = true;
		if (titi)
		{
			nb->x = na->x + (nb->x - na->x) * r;
			nb->y = na->y + (nb->y - na->y) * r;
			nb->z = na->z + (nb->z - na->z) * r;
		}
	}
	inline void										RecomputeColor(const ZobVector3* ca, ZobVector3* cb, float r) const
	{
		cb->x = ca->x + (cb->x - ca->x) * r;
		cb->y = ca->y + (cb->y - ca->y) * r;
		cb->z = ca->z + (cb->z - ca->z) * r;
	}

	uint** m_buffer;
	float** m_zBuffer;
	Rasterizer** m_rasterizers;
	Events* m_events;
	float* m_zBufferClearArray;
	uint* m_bufferClearArray;
	Line3D* m_LineQueue;
	Triangle* m_TrianglesQueue;
	ZobVector3* m_verticesData;
	ZobVector2* m_uvData;
	ZobVector3* m_colorData;
	RenderOptions* m_renderOptions;
	ZobVariablesExposer* m_varExposer;
	int m_usedRenderOptions;
	long m_TriangleQueueSize;
	long m_maxTrianglesQueueSize;
	long m_lineQueueSize;
	long m_maxLineQueueSize;	
	long m_LastTriangleQueueSize;	
	ulong m_currentFrame;
	int m_currentBuffer;
	BufferData m_bufferData;
	ulong m_nbPixels;
	volatile bool m_started;
	int m_nextWidth;
	int m_nextHeight;
	bool m_doResize;

	eRenderMode m_renderMode = eRenderMode_fullframe;
	eLightingPrecision m_lightingPrecision = eLightingPrecision_vertex;
	float m_zNear;
	float m_zFar;
	bool m_showZBuffer;
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
	bool m_EqualizeTriangleQueues;
	bool m_perspCorrection;
	eBitsPerColor m_nbBitsPerColorDepth;
	bool m_dithering;
	uint m_nbRasterizers;
	uint m_drawnTriangles;
};