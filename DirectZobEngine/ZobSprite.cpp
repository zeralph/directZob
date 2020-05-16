#include "ZobSprite.h"
#include "tinyxml.h"
#include "DirectZob.h"

ZobSprite::ZobSprite(const std::string &name, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_sprite, name, nullptr, parent)
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_material = DirectZob::GetInstance()->GetMaterialManager()->CreateMaterial();
	Vector3 c = Vector3(1, 0, 0);
	m_material->SetDiffuseColor(c);
	Sprite* s = (Sprite*)m_mesh;
	s->Setmaterial(m_material);
}

ZobSprite::ZobSprite(TiXmlElement* node, ZobObject* parent)
	:ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_sprite, node, NULL, parent)
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_material = DirectZob::GetInstance()->GetMaterialManager()->CreateMaterial();
	Vector3 c = Vector3(1, 0, 0);
	m_material->SetDiffuseColor(c);
	Sprite* s = (Sprite*)m_mesh;
	s->Setmaterial(m_material);
	TiXmlElement* f = node->FirstChildElement("Texture");
	if (f && f->GetText())
	{
		std::string tex = std::string(f->GetText());
		SetTexture(tex);
	}
}

ZobSprite::~ZobSprite()
{
}


TiXmlNode* ZobSprite::SaveUnderNode(TiXmlNode* node)
{
	TiXmlNode* n = ZobObject::SaveUnderNode(node);
	TiXmlElement* ne = (TiXmlElement*)n;
	ne->SetAttribute("type", "sprite");
	if (m_material && m_material->GetDiffuseTexture())
	{
		TiXmlText t(m_material->GetDiffuseTexture()->GetPath().c_str());
		TiXmlElement tex = TiXmlElement("Texture");
		tex.InsertEndChild(t);
		ne->InsertEndChild(tex);
	}
	return n;
}

void ZobSprite::UpdateMesh(const Camera* camera, Core::Engine* engine)
{
	if (camera)
	{
		Vector3 t = m_modelMatrix.GetTranslation();
		Vector3 l = camera->GetLeft();
		Vector3 u = camera->GetUp();
		Vector3 f = camera->GetForward();
		m_rotationScaleMatrix.FromVectors(l, u, f);
		m_modelMatrix.FromVectors(l, u, f);
		m_rotationScaleMatrix.SetScale(&m_scale);
		m_modelMatrix.AddTranslation(t);
		m_modelMatrix.SetScale(&m_scale);

		m_left = Vector3(1, 0, 0);
		m_forward = Vector3(0, 0, 1);
		m_up = Vector3(0, 1, 0);
		m_rotationScaleMatrix.Mul(&m_left);
		m_rotationScaleMatrix.Mul(&m_forward);
		m_rotationScaleMatrix.Mul(&m_up);
	}
	ZobObject::UpdateMesh(camera, engine);
}

void ZobSprite::Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix)
{
	ZobObject::Update(parentMatrix, parentRSMatrix);
}

void ZobSprite::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	ZobObject::DrawGizmos(camera, engine);
}

void ZobSprite::SetTexture(std::string& path)
{
	const Texture* t = DirectZob::GetInstance()->GetMaterialManager()->GetTexture(path);
	m_material->SetDiffuseTexture(t);
}