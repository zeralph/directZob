#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"
#include "nanojpeg.h"
#include "tga.h"

Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string &textureFile)
{
	m_name = name.c_str();
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	if (textureFile.size() > 0)
	{
		std::vector<unsigned char> image; //the raw pixels
		unsigned width, height;
		uint8_t error = 0;
		if (textureFile.find(".png") != -1)
		{
			error = lodepng::decode(image, width, height, textureFile);
			if (error)
			{
				OnError(error, name.c_str(), textureFile.c_str());
			}
			else
			{
				m_width = width;
				m_height = height;
				m_data = (float*)malloc(sizeof(float) * 4 * image.size());
				for (int i = 0; i < image.size(); i += 4)
				{
					float r = (float)image[i] / 255.0f;
					float g = (float)image[i + 1] / 255.0f;
					float b = (float)image[i + 2] / 255.0f;
					float a = (float)image[i + 3] / 255.0f;
					m_data[i] = r;
					m_data[i + 1] = g;
					m_data[i + 2] = b;
					m_data[i + 3] = a;
				}
				m_dataSize = width * height * 4;
				DirectZob::LogInfo("Loaded texture %s", textureFile);
			}
		}
		else if (textureFile.find(".jpg") != -1 || textureFile.find(".jpeg") != -1)
		{
			FILE* f;
			int err = (int)fopen_s(&f, textureFile.c_str(), "rb");
			if (err == 0)
			{
				char* buf;
				fseek(f, 0, SEEK_END);
				int size = (int)ftell(f);
				buf = (char*)malloc(size);
				fseek(f, 0, SEEK_SET);
				size = (int)fread(buf, 1, size, f);
				fclose(f);

				nanojpeg::njInit();
				error = nanojpeg::njDecode(buf, size);
				if (error == NJ_OK) {
					int s = nanojpeg::njGetImageSize();
					for (int i = 0; i < s; i++)
					{
						image.push_back(nanojpeg::njGetImage()[i]);
					}
					width = nanojpeg::njGetWidth();
					height = nanojpeg::njGetHeight();
					m_width = width;
					m_height = height;
					m_data = (float*)malloc(sizeof(float) * 4 * image.size());
					int j = 0;
					for (int i = 0; i < image.size(); i += 3)
					{
						float r = (float)image[i] / 255.0f;
						float g = (float)image[i + 1] / 255.0f;
						float b = (float)image[i + 2] / 255.0f;
						float a = 1.0f;
						m_data[j] = r;
						m_data[j + 1] = g;
						m_data[j + 2] = b;
						m_data[j + 3] = a;
						j += 4;
					}
					m_dataSize = width * height * 4;
					DirectZob::LogInfo("Loaded texture %s", textureFile);
				}
				else
				{
					OnError(error, name.c_str(), textureFile.c_str());
				}
				free((void*)buf);
			}
			else
			{
				OnError(error, name.c_str(), textureFile.c_str());
			}
		}
		else if (textureFile.find(".tga") != -1)
		{
			int w, h, bpp = 0;
			tga::ImageFormat format = tga::ImageFormat::Undefined;
			tga::TGA tgaTex;
			if(tgaTex.Load(textureFile))
			{
				w = tgaTex.GetWidth();
				h = tgaTex.GetHeight();
				format = tgaTex.GetFormat();
				if (format == tga::ImageFormat::RGB)
				{
					bpp = 3;
				}
				else if (format == tga::ImageFormat::RGBA)
				{
					bpp = 4;
				}
				else if (format == tga::ImageFormat::Monochrome)
				{
					return;// bpp = 1;
				}
				else
				{
					return;
				}
				uint8_t* d = tgaTex.GetData();
				uint64_t s = tgaTex.GetSize();
				m_data = (float*)malloc(sizeof(float) * w*h*4);
				int j = 0;
				for (int i = 0; i < s; i += bpp)
				{
					float r = (float)d[i] / 255.0f;
					float g = (float)d[i + 1] / 255.0f;
					float b = (float)d[i + 2] / 255.0f;
					float a = 1.0f;
					if (r == 0.0f || g == 0.0f || b == 0.0f)
					{
						int hh = 0;
						hh++;
					}
					if (bpp == 4)
					{
						a = (float)image[i + 3] / 255.0f;
					}
					m_data[j] = b;
					m_data[j + 1] =r;
					m_data[j + 2] = g;
					m_data[j + 3] = a;
					j += 4;
				}
				m_width = w;
				m_height = h;
				m_dataSize = width * height * 4;
				DirectZob::LogInfo("Loaded texture %s", textureFile.c_str());
			}
			else
			{
				OnError(error, name.c_str(), textureFile.c_str());
			}
		}
		else
		{
			DirectZob::LogError("no decoder for file %s", textureFile.c_str());
		}
		image.clear();
	}
	DirectZob::LogInfo("Loaded material %s", name.c_str());
}

const Material* MaterialManager::LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, const std::string &path)
{
	const Material* finalMaterial = NULL;
	MaterialManager* materialMgr = DirectZob::GetInstance()->GetMaterialManager();
	int mcount = mesh->GetElementMaterialCount();
	for (int index = 0; index < mcount; index++)
	{
		//fbxsdk::FbxGeometryElementMaterial* material = mesh->GetElementMaterial(0);
		const fbxsdk::FbxGeometryElementMaterial* gMaterial = mesh->GetElementMaterial(index);
		if (gMaterial)
		{
			fbxsdk::FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(gMaterial->GetIndexArray().GetAt(0));
			int propCount = material->GetSrcPropertyCount();
			//if (propCount > 0)
			{
				const char* matName = material->GetName();
				finalMaterial = materialMgr->GetMaterial(matName);
				if (finalMaterial)
				{
					return finalMaterial;
				}
				else
				{
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					fbxsdk::FbxProperty prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					// Check if it's layeredtextures
					FbxDouble3 c = prop.Get<FbxDouble3>();
					Vector3 diffuse = Vector3(c[0], c[1], c[2]);
					const char* texture_name = NULL;
					const char* texture_name2 = NULL;
					const char* texture_name3 = NULL;
					int file_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
					if (file_texture_count > 0)
					{
						for (int j = 0; j < file_texture_count; j++)
						{
							FbxFileTexture* file_texture = FbxCast<fbxsdk::FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
							texture_name = file_texture->GetFileName();
							texture_name2 = file_texture->GetRelativeFileName();
							texture_name3 = file_texture->GetMediaName();
						}
					}
					/*int layered_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
					if (layered_texture_count > 0)
					{
						for (int j = 0; j < layered_texture_count; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<fbxsdk::FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<fbxsdk::FbxTexture>();
							for (int k = 0; k < lcount; k++)
							{
								FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(layered_texture->GetSrcObject<fbxsdk::FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								texture_name = texture->GetName();
								//texture_name2 = texture->GetRelativeFileName();
								//texture_name3 = texture->GetMediaName();
							}
						}
					}
					else
					{
						// Directly get textures
						int texture_count = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();
						for (int j = 0; j < texture_count; j++)
						{
							const fbxsdk::FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(prop.GetSrcObject<fbxsdk::FbxTexture>(j));
							//texture->FindSrcObject
							// Then, you can get all the properties of the texture, include its name
							texture_name = texture->GetName();
							//texture_name2 = texture->GetRelativeFileName();
							//texture_name3 = texture->GetMediaName();
						}
					}*/
					float f;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);
					f = prop.Get<FbxDouble>();
					
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
					f = prop.Get<FbxDouble>();
					Vector3 ambient;
					std::string texFullPath = "";
					if (texture_name2)
					{
						texFullPath = path + std::string(texture_name2);
					}
					finalMaterial = LoadMaterial(matName, &ambient, &diffuse, texFullPath);
				}
			}
		}
	}
	return finalMaterial;
}


void Material::OnError(int error, const char* material, const char* texture)
{
	m_data = NULL;
	m_dataSize = 0;
	m_width = m_height = 0;
	DirectZob::LogError("Error %i occured when loading texture %s for material %s : format not supported", error, texture, material);

}

Material::~Material()
{
	delete m_data;
	m_data = NULL;
	m_name = "deleted";
	m_ambientColor = Vector3(0, 0, 0);
	m_diffuseColor = Vector3(0,0,0);
	m_width = 0;
	m_height = 0;
	m_dataSize = 0;
}
