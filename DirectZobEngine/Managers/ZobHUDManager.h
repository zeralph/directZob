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
		float scaleX;
		float scaleY;
		eHudUnit unit;
		char c;
		ZobColor color;
		const ZobMaterial* mat;
		const ZobFont::FontGlyphe* glyphe;
	};

	ZobHUDManager();
	~ZobHUDManager();

	void Stop();
	void Start();
	
	void PreUpdate(float dt);
	void UpdateBehavior(float dt);
	void UpdateObjects(const Camera* camera, Engine* engine, float dt);
	void QueueForDrawing(const Camera* camera, Engine* engine);
	void Print(eHudUnit u, float x, float y, float fontSize, const char* fontName, const ZobVector3* color, const char* fmt, ...);

private:
	void DeleteFonts();
	void Init();
	bool CreateQuad(float xMin, float yMin, float xMax, float yMax, HUDElement* elem);
	void PrintInternal(eHudUnit u, float x, float y, float fontSize, const ZobFont* font, const ZobVector3* color, std::string s);
	const ZobFont* GetFont(const char* fontName) const;
	Triangle* m_trianglesBuffer;
	ZobVector3* m_vertices;
	ZobVector3* m_projectedVertices;
	ZobVector3* m_normals;
	ZobVector2* m_uvs;
	ZobColor* m_colors;
	int	m_nbDrawnTriangles;
	Triangle::RenderOptions m_renderOptions;
	std::vector <const ZobFont*> m_fonts;
	std::vector<HUDElement> m_hudElements;
	bool m_started;
}; 
