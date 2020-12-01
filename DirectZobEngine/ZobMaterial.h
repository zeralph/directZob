#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Rendering/ZobVector3.h" 
#include "Events.h"
#include "Rendering/Texture.h"

class ZobMaterial
{
public:

	//Material(const std::string& path, const std::string& file, const std::string& name);
	ZobMaterial();
	ZobMaterial(const std::string& name, const ZobVector3* ambientColor, const ZobVector3* diffuseColor, const Texture* texture);
	~ZobMaterial();
	inline const Texture*		GetDiffuseTexture() const { return m_diffuseTexture; }
	void						SetDiffuseTexture(const Texture* t) { m_diffuseTexture = t; }
	inline const ZobVector3* 		GetAmbientColor() const { return &m_ambientColor; }
	inline const ZobVector3* 		GetDiffuseColor() const { return &m_diffuseColor; }
	inline void					SetDiffuseColor(const ZobVector3& c) { m_diffuseColor = c; }
	inline const std::string& 	GetName() const { return m_name; }

private:

	const Texture* m_diffuseTexture;
	std::string m_name;
	ZobVector3 m_ambientColor;
	ZobVector3 m_diffuseColor;
	//const uint* data = &internal_data[0];
};
