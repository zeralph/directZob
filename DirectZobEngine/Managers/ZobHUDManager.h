#pragma once
#include "../Types.h"
#include "../ZobObjects/ZobObject.h"
#include <string>
#include <vector>
#include "../ZobObjects/Camera.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/Light.h"
#include "../Rendering/ZobFont.h"
#include "../ZobMaterial.h"

#define HUD_TEXT_BUFFER_SIZE 4096

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
	void Print(eHudUnit u, float x, float y, float fontSize, const char* fontName, const ZobColor* color, const char* fmt, ...);

private:
	void DeleteFonts();
	void Init();
	bool CreateQuad(float xMin, float yMin, float xMax, float yMax, HUDElement* elem);
	void PrintInternal(eHudUnit u, float x, float y, float fontSize, const ZobFont* font, const ZobColor* color, const char* buf, size_t size);
	const ZobFont* GetFont(const std::string& fontName) const;
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
	static char m_buffer[HUD_TEXT_BUFFER_SIZE];
}; 
