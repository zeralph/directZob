#include "ZobBehaviorMesh.h"
#include "../DirectZob.h"

ZobBehaviorMesh::~ZobBehaviorMesh()
{

}

ZobBehaviorMesh::ZobBehaviorMesh(ZobObject* zobObject, bool bEditorZobBehavior) : ZobBehavior(zobObject, bEditorZobBehavior)
{
	m_type = eBehavior_mesh;
	m_meshPath.Init();
	m_mesh = NULL;

	m_varExposer->WrapVariable<ZobFilePath>("File", &m_meshPath, NULL, false, true);
	
	Triangle::RenderOptions::eCullMode cm[3] = { Triangle::RenderOptions::eCullMode_None, Triangle::RenderOptions::eCullMode_ClockwiseFace, Triangle::RenderOptions::eCullMode_CounterClockwiseFace};
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, NULL, false, true);

	Triangle::RenderOptions::eLightMode lm[5] = { Triangle::RenderOptions::eLightMode_none, Triangle::RenderOptions::eLightMode_flat, Triangle::RenderOptions::eLightMode_gouraud, Triangle::RenderOptions::eLightMode_phong , Triangle::RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, NULL, false, true);

	m_varExposer->WrapVariable<bool>("ZBuffered", &m_renderOptions.zBuffered, NULL, false, true);
	
	m_varExposer->WrapVariable<bool>("Transparent", &m_renderOptions.bTransparency, NULL, false, true);
	
	m_varExposer->WrapVariable<int>("Nb triangles", &m_meshNbTriangles, NULL, true, false);
}

void ZobBehaviorMesh::Init()
{
	m_meshNbTriangles = 0;
	ReLoadVariables();
	if (m_mesh == NULL && m_meshPath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			DirectZob::LogError("Mesh loading error %s %s %s", m_meshPath.file.c_str(), m_meshPath.path.c_str(), m_meshPath.name.c_str());
			m_meshPath.Init();
			m_meshNbTriangles = 0;
		}
		else
		{
			m_meshNbTriangles = m_mesh->GetNbTriangles();
		}
	}
}

void ZobBehaviorMesh::Set(ZobFilePath zfp) 
{ 
	m_meshPath.file = zfp.file; 
	m_meshPath.name = zfp.name;
	m_meshPath.path = zfp.path;
	m_meshPath.bAbsolute = zfp.bAbsolute;
	LoadMeshInternal();
}

void ZobBehaviorMesh::PreUpdate(float dt)
{
}

void ZobBehaviorMesh::UpdateAfterObject(float dt)
{

}

void ZobBehaviorMesh::PostUpdate()
{
	/*if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_mesh->QueueForDrawing(m_zobObject, mm, rs, c, e, ro);
	}*/
	if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		const Triangle::RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_mesh->Update(mm, rs, c, e, ro);
	}
}

void ZobBehaviorMesh::QueueForDrawing(const Camera* camera, Engine* engine)
{
	if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		Triangle::RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_mesh->QueueForDrawing(m_zobObject, mm, rs, c, e, ro);
	}
}

void ZobBehaviorMesh::EditorUpdate()
{
	if (!m_mesh)
	{
		if (m_meshPath.IsDefined())
		{
			LoadMeshInternal();
		}
	}
	else
	{
		if (m_mesh->GetName() != m_meshPath.name)
		{
			LoadMeshInternal();
		}
	}
}

bool ZobBehaviorMesh::LoadMeshInternal()
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(m_meshPath.name, m_meshPath.path, m_meshPath.file, m_meshPath.bAbsolute);
	if (m_mesh)
	{
		m_meshNbTriangles = m_mesh->GetNbTriangles();
	}
	else
	{
		m_meshNbTriangles = 0;
	}
	return m_mesh != NULL;
}