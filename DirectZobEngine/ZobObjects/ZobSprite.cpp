#include "ZobSprite.h"
#include "tinyxml.h"
#include "DirectZob.h"

ZobSprite::ZobSprite(const std::string &name, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_sprite, name, parent)
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_material = DirectZob::GetInstance()->GetMaterialManager()->CreateMaterial();
	ZobVector3 c = ZobVector3(1, 0, 0);
	m_material->SetDiffuseColor(c);
	Sprite* s = (Sprite*)m_mesh;
	s->Setmaterial(m_material);
}

ZobSprite::ZobSprite(ulong id, TiXmlElement* node, ZobObject* parent)
	:ZobObject(id, node, parent)
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_material = DirectZob::GetInstance()->GetMaterialManager()->CreateMaterial();
	ZobVector3 c = ZobVector3(1, 0, 0);
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
		ZobVector3 t = m_modelMatrix.GetTranslation();
		ZobVector3 l = camera->GetLeft();
		ZobVector3 u = camera->GetUp();
		ZobVector3 f = camera->GetForward();
		m_rotationScaleMatrix.FromVectors(l, u, f);
		m_modelMatrix.FromVectors(l, u, f);
		m_rotationScaleMatrix.SetScale(GetScale());
		m_modelMatrix.AddTranslation(t);
		m_modelMatrix.SetScale(GetScale());

		m_left = ZobVector3(1, 0, 0);
		m_forward = ZobVector3(0, 0, 1);
		m_up = ZobVector3(0, 1, 0);
		m_rotationScaleMatrix.Mul(&m_left);
		m_rotationScaleMatrix.Mul(&m_forward);
		m_rotationScaleMatrix.Mul(&m_up);
	}
	ZobObject::UpdateMesh(camera, engine);
}

void ZobSprite::Update(float dt)
{
	ZobObject::Update(dt);
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