#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Rendering/ZobVector3.h" 
#include "Rendering/Color.h" 
#include "Events.h"
#include "Rendering/ZobTexture.h"

class ZobMaterial
{
public:

	//Material(const std::string& path, const std::string& file, const std::string& name);
	ZobMaterial();
	ZobMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor, float specularExponent, const ZobTexture* texture);
	~ZobMaterial();
	inline const ZobTexture*		GetDiffuseTexture() const { return m_diffuseTexture; }
	void						SetDiffuseTexture(const ZobTexture* t) { m_diffuseTexture = t; }
	inline const ZobColor* 		GetAmbientColor() const { return &m_ambientColor; }
	inline const ZobColor* 		GetDiffuseColor() const { return &m_diffuseColor; }
	inline const ZobColor*		GetSpecularColor() const { return &m_specularColor; }
	inline void					SetDiffuseColor(const ZobColor& c) { m_diffuseColor = c; }
	inline float				GetSpecularExponent() const { return m_specularExponent; }
	inline const std::string& 	GetName() const { return m_name; }

private:

	const ZobTexture* m_diffuseTexture;
	std::string m_name;
	ZobColor m_ambientColor;
	ZobColor m_diffuseColor;
	ZobColor m_specularColor;
	float m_specularExponent;
};
