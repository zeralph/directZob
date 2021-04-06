#include "ZobBehaviorMesh.h"
#include "../DirectZob.h"

ZobBehaviorMesh::~ZobBehaviorMesh()
{

}

ZobBehaviorMesh::ZobBehaviorMesh(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_type = eBehavior_mesh;
	m_meshName = "";
	m_meshPath = "";
	m_meshFile = "";
	m_mesh = NULL;
	m_varExposer->WrapPath("File", &m_meshName, &m_meshPath, &m_meshFile, false, true);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_bool, "ZBuffered", &m_renderOptions.zBuffered, false, true);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_bool, "Transparent", &m_renderOptions.bTransparency, false, true);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_int, "Nb triangles", &m_meshNbTriangles, true, false);
}

void ZobBehaviorMesh::Init()
{
	ReLoadVariables();
	if (m_mesh == NULL && m_meshName.size() && m_meshPath.size() && m_meshFile.size())
	{
		if (!LoadMeshInternal())
		{
			DirectZob::LogError("Mesh loading error %s %s %s", m_meshName.c_str(), m_meshPath.c_str(), m_meshFile.c_str());
			m_meshName = "";
			m_meshPath = "";
			m_meshFile = "";
			m_meshNbTriangles = 0;
		}
		else
		{
			m_meshNbTriangles = m_mesh->GetNbTriangles();
		}
	}
}

void ZobBehaviorMesh::PreUpdate()
{
}

void ZobBehaviorMesh::UpdateAfterObject(float dt)
{
	if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationScaleMatrix();
		const RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_mesh->Update(mm, rs, c, e, ro);
	}
}

void ZobBehaviorMesh::PostUpdate()
{
	const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
	const ZobMatrix4x4* rs = m_zobObject->GetRotationScaleMatrix();
	RenderOptions* ro = &this->m_renderOptions;
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	Engine* e = DirectZob::GetInstance()->GetEngine();
	m_mesh->QueueForDrawing(m_zobObject, mm, rs, c, e, ro);
}

void ZobBehaviorMesh::EditorUpdate()
{

}

bool ZobBehaviorMesh::LoadMeshInternal()
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(m_meshName, m_meshPath, m_meshFile);
	if (m_mesh)
	{
	}
	return m_mesh != NULL;
}