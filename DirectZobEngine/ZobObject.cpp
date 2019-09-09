#include "ZobObject.h"

void ZobObject::Update(const Matrix4x4& parentMatrix)
{
	m_modelMatrix.Identity();
	m_modelMatrix.SetScale(&m_scale);
	m_modelMatrix.SetRotation(&m_rotation);
	m_modelMatrix.SetTranslation(&m_translation);
	m_modelMatrix.Mul(&parentMatrix);
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->Update(m_modelMatrix);
	}
}

void ZobObject::Draw(const Camera* camera, Core::Engine* engine)
{
	if (m_mesh)
	{
		m_mesh->Draw(m_modelMatrix, camera, engine);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->Draw(camera, engine);
	}
}