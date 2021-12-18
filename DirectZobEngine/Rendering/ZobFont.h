#pragma once
#include "../Types.h"
#include "../ZobMaterial.h"
#include "ZobMatrix2x2.h"
#include "Engine.h"
#include "../Events.h"
#include "../ZobMaterial.h"
#include "../Misc/ZobFilePath.h"

class ZobFont
{
public:

	struct FontGlyphe
	{
		const ZobMaterial* mat;
		char c;
		int charcode;
		float width;
		float offsetX;
		float offsetY;
		float x;
		float y;
		float w;
		float h;
		float uv_min_x;
		float uv_min_y;
		float uv_max_x;
		float uv_max_y;
	};

	ZobFont(ZobFilePath zfpTexture, ZobFilePath zfpXml);
	~ZobFont();
	const FontGlyphe* GetChar(char c) const;
	inline float GetHeight() const { return m_height; }
	inline const char* GetName() const { return m_name.c_str(); }
private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim) const;
	ZobColor m_color;
	std::string m_name;
	const ZobMaterial* m_charMaterial;
	std::vector<FontGlyphe> m_glyphes;
	float m_height;
};
