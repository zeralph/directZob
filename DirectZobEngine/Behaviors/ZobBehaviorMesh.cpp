#include "ZobBehaviorMesh.h"
#include "../DirectZob.h"

ZobBehaviorMesh::~ZobBehaviorMesh()
{

}

ZobBehaviorMesh::ZobBehaviorMesh(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_type = eBehavior_mesh;
	m_meshPath.Init();
	m_mesh = NULL;

	m_varExposer->WrapVariable<ZobFilePath>("File", &m_meshPath, false, true);
	
	RenderOptions::eCullMode cm[3] = { RenderOptions::eCullMode_None, RenderOptions::eCullMode_ClockwiseFace, RenderOptions::eCullMode_CounterClockwiseFace};
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, false, true);

	RenderOptions::eLightMode lm[5] = { RenderOptions::eLightMode_none, RenderOptions::eLightMode_flat, RenderOptions::eLightMode_gouraud, RenderOptions::eLightMode_phong , RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, false, true);

	m_varExposer->WrapVariable<bool>("ZBuffered", &m_renderOptions.zBuffered, false, true);
	
	m_varExposer->WrapVariable<bool>("Transparent", &m_renderOptions.bTransparency, false, true);
	
	m_varExposer->WrapVariable<int>("Nb triangles", &m_meshNbTriangles, true, false);
}

void ZobBehaviorMesh::Init()
{
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

void ZobBehaviorMesh::PreUpdate()
{
}

void ZobBehaviorMesh::UpdateAfterObject(float dt)
{
	if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();	
		const RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_mesh->Update(mm, rs, c, e, ro);
	}
}

void ZobBehaviorMesh::PostUpdate()
{
	if (m_mesh)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		RenderOptions* ro = &this->m_renderOptions;
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
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(m_meshPath.name, m_meshPath.path, m_meshPath.file);
	if (m_mesh)
	{
	}
	return m_mesh != NULL;
}