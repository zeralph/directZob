#pragma once
#include "../Types.h"
#include "../ZobObjects/ZobObject.h"
#include <string>
#include <vector>
#include "../ZobObjects/Camera.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/ZobSprite.h"
#include "../Rendering/ZobFont.h"
#include "../ZobMaterial.h"

class ZobHUDManager
{
public:

	struct HUDElement
	{
		float x;
		float y;
		float w;
		float h;
		ZobVector3 color;
		const ZobMaterial* mat;
	};

	ZobHUDManager();
	~ZobHUDManager();

	void Stop();
	void Start();
	
	void PreUpdate();
	void UpdateBehavior(float dt);
	void UpdateObjects(const Camera* camera, Core::Engine* engine, float dt);
	void QueueForDrawing(const Camera* camera, Core::Engine* engine);
	void Print(float x, float y, float w, float h, const char* fmt, ...);
	void Print(float x, float y, float w, float h, const ZobVector3* color, const char* fmt, ...);

private:
	void Init();
	bool CreateQuad(float xMin, float yMin, float xMax, float yMax, const HUDElement* elem);
	void PrintInternal(float x, float y, float w, float h, const ZobVector3* color, std::string s);
	Triangle* m_trianglesBuffer;
	ZobVector3* m_vertices;
	ZobVector3* m_projectedVertices;
	ZobVector3* m_normals;
	ZobVector2* m_uvs;
	int	m_nbDrawnTriangles;
	DirectZobType::RenderOptions m_renderOptions;
	ZobFont* m_font;
	std::vector<HUDElement> m_hudElements;
	bool m_started;
}; 
