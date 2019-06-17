#pragma once
#include "Types.h"
#include "MiniFB.h"
#include <math.h>
#include <stdlib.h> 
#include "Color.h"
#include "text2D.h"
#include <string>
#include <ctime>
#include "Matrix2x2.h"
#include "Matrix4x4.h"
#include <stdarg.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Camera.h"
class Engine
{
public:

	struct Triangle
	{
		Vector3* va;
		Vector3* vb;
		Vector3* vc;

		Vector3* na;
		Vector3* nb;
		Vector3* nc;

		Vector2* ua;
		Vector2* ub;
		Vector2* uc;

		float area;

		float la;
		float lb;
		float lc;

		void ComputeArea() 
		{ 
			area = (vc->x - va->x) * (vb->y - va->y) - (vc->y - va->y) * (vb->x - va->x); 
		}

		void ComputeLighting(Vector3* v)
		{
			la = fmax(0.0f, Vector3::Dot(na, v));
			lb = fmax(0.0f, Vector3::Dot(na, v));
			lc = fmax(0.0f, Vector3::Dot(na, v));
		}
	};

	struct BufferData
	{
		uint width;
		uint height;
		uint size;
		uint* buffer;
		float* zBuffer;
	};

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
	void DrawPixel(const Matrix2x2* matrix, const uint color, const float dx, const float dy, BufferData* bufferData);
	void DrawTexture(const Matrix2x2* matrix, const Texture* texture, BufferData* bufferData);
	void DrawLine(const float xa, const float  ya, const float xb, const float yb, const uint c, BufferData* bufferData);
	void DrawCircle(const float xc, const float yc, const float r, const uint c, BufferData* bufferData);
	void DrawString(const Matrix2x2* matrix, std::string s, const uint color, BufferData* bufferData);
	void DrawChar(const Matrix2x2* matrix, char c, const uint color, BufferData* bufferData);
	void DrawBuffer(const Matrix2x2* matrix, const uint color, const uint* buffer, const int bufferWidth, const int bufferHeight, BufferData* bufferData);

	void DrawTriangle(const Vector2* va, const Vector2* vb, const Vector2* vc, const uint color, BufferData* bufferData);
	void DrawTriangle2(const Triangle* t, const Texture* tex, BufferData* bufferData);


	void GetPixelColor(Color* color, int x, int y);

	Camera* GetCamera() { return m_camera; }

	int Update();
	inline ulong GetCurrentFrame() {return m_currentFrame;}
	inline float GetFps() { return m_fps; }
	inline int Width() { return m_width; }
	inline int Height() { return m_height; }

	

	inline BufferData* GetBufferData() { return &m_bufferData; }

private:

	void FillBottomFlatTriangle(Vector2* v1, Vector2* v2, Vector2* v3, const uint color, BufferData* bufferData);
	void FillTopFlatTriangle(Vector2* v1, Vector2* v2, Vector2* v3, const uint color, BufferData* bufferData);
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3);
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3);

	void DrawHorizontalLine(const float x1, const float x2, const float y, const uint color, BufferData* bufferData);

	void FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Texture* tex, BufferData* bufferData);
	void FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Texture* tex, BufferData* bufferData);
	void FillBufferPIxel(const Vector3* p, const Triangle* t, const Texture* texData, BufferData* bufferData);

	inline float edgeFunction(const Vector3* a, const Vector3* b, const Vector3* c) {return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x);}



	int m_width;
	int m_height;
	float m_zNear;
	float m_zFar;
	int m_curBuffer;
	clock_t	m_tick;
	float m_fps;
	ulong m_currentFrame;
	uint* m_buffer;
	float* m_zBuffer;
	Camera* m_camera;
	BufferData m_bufferData;

};