#include "ZobSprite.h"
#include "tinyxml.h"
#include "DirectZob.h"

ZobSprite::ZobSprite(const std::string &name, ZobObject*parent):
	ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_mesh, name, nullptr, parent)
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
}

ZobSprite::ZobSprite(TiXmlElement* node, ZobObject* parent)
	:ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_mesh, node, NULL, parent)
{
}

ZobSprite::~ZobSprite()
{
}


TiXmlNode* ZobSprite::SaveUnderNode(TiXmlNode* node)
{
	return node;
}

void ZobSprite::Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix)
{
	ZobObject::Update(parentMatrix, parentRSMatrix);

	//override the rotation matrix
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		Vector3 t = m_modelMatrix.GetTranslation();
		Vector3 l = c->GetLeft();
		Vector3 u = c->GetUp();
		Vector3 f = c->GetForward();
		m_rotationScaleMatrix.FromVectors(l, u, f);
		m_modelMatrix.FromVectors(l, u, f);
		m_rotationScaleMatrix.SetScale(&m_scale);
		m_modelMatrix.AddTranslation(t);
		m_modelMatrix.SetScale(&m_scale);
	}
	
}

void ZobSprite::DrawGizmos(const Camera* camera, Core::Engine* engine)
{

}