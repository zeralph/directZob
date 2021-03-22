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

	enum eHudUnit
	{
		eHudUnit_pixels=0,
		eHudUnit_ratio,
	};

	struct HUDElement
	{
		float x;
		float y;
		float w;
		float h;
		eHudUnit unit;
		char c;
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
	void Print(eHudUnit u, float x, float y, float fontSize, const char* fmt, ...);
	void Print(eHudUnit u, float x, float y, float fontSize, const ZobVector3* color, const char* fmt, ...);

private:
	void Init();
	bool CreateQuad(float xMin, float yMin, float xMax, float yMax, HUDElement* elem);
	void PrintInternal(eHudUnit u, float x, float y, float fontSize, const ZobVector3* color, std::string s);
	Triangle* m_trianglesBuffer;
	ZobVector3* m_vertices;
	ZobVector3* m_projectedVertices;
	ZobVector3* m_normals;
	ZobVector2* m_uvs;
	ZobVector3* m_colors;
	int	m_nbDrawnTriangles;
	DirectZobType::RenderOptions m_renderOptions;
	ZobFont* m_font;
	std::vector<HUDElement> m_hudElements;
	bool m_started;
}; 
