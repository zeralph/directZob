#pragma once
#include "../Types.h"
#include <string>
#include <vector>
#include "../Rendering/ZobFont.h"

#define HUD_TEXT_BUFFER_SIZE 4096

class Engine;
class Camera;
class ZobComponentText;
class HUDElement;
class ZobMaterial;
class ZobVector2;
class ZobVector3;
class ZobColor;
class ZobObject;
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
		float z;
		float scaleX;
		float scaleY;
		eHudUnit unit;
		char c;
		ZobColor color;
		const ZobMaterial* mat;
		const ZobFont::FontGlyphe* glyphe;
		ZobObject* zo;
	};

	ZobHUDManager();
	~ZobHUDManager();

	void Stop();
	void Start();
	
	void UpdateNavigation(float dt);
	void UpdateObjects(const Camera* camera, Engine* engine, float dt);
	void QueueForDrawing(const Camera* camera, Engine* engine);
	void Print(ZobObject* zobObj, eHudUnit u, float x, float y, float z, float fontSize, const ZobFont* font, const ZobColor* color, const char* fmt, ...);
	void Print(ZobObject* zobObj, eHudUnit u, float x, float y, float z, float w, float h, const ZobMaterial* mat);
	const ZobFont* CreateOrGetZobFont(ZobFilePath zfpXml, ZobFilePath zfpTexture);
	void AddToMenu(ZobComponentText* zct);
	void RemoveFromMenu(ZobComponentText* zct);
	void Init();
private:
	void DeleteFonts();
	bool CreateQuad(ZobObject* zobObj, float xMin, float yMin, float xMax, float yMax, HUDElement* elem);
	void PrintInternal(ZobObject* zobObj, eHudUnit u, float x, float y, float z, float fontSize, const ZobFont* font, const ZobColor* color, const char* buf, size_t size);
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
	std::vector<ZobComponentText*> m_menuItems;
	int m_menuCurrentIndex;
}; 
