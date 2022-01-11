#include "ZobComponentMesh.h"
#include "../../DirectZob.h"
#include "../../Mesh.h"
#include "../../ZobObjects/Camera.h"
#include "../../Managers/CameraManager.h"
#include "../../Managers/MeshManager.h"
#include "../../Managers/MaterialManager.h"
#include "../ZobComponentFactory.h"
ZobComponentMesh::~ZobComponentMesh()
{

}

ZobComponentMesh::ZobComponentMesh(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	SET_CLASS_AND_NAME
	m_componentType = ZobComponentFactory::eComponent_mesh;
	m_meshPath.Reset();
	m_mesh = NULL;
	m_meshPath.SetFileType(ZobFilePath::eFileType_mesh);
	m_varExposer->WrapVariable<ZobFilePath>("File", &m_meshPath, NULL, false, true);

	Triangle::RenderOptions::eCullMode cm[3] = { Triangle::RenderOptions::eCullMode_None, Triangle::RenderOptions::eCullMode_ClockwiseFace, Triangle::RenderOptions::eCullMode_CounterClockwiseFace};
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, NULL, false, false, true);

	Triangle::RenderOptions::eLightMode lm[5] = { Triangle::RenderOptions::eLightMode_none, Triangle::RenderOptions::eLightMode_flat, Triangle::RenderOptions::eLightMode_gouraud, Triangle::RenderOptions::eLightMode_phong , Triangle::RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, NULL, false, false, true);

	m_varExposer->WrapVariable<bool>("ZBuffered", &m_renderOptions.zBuffered, NULL, false, true);
	
	m_varExposer->WrapVariable<bool>("Transparent", &m_renderOptions.bTransparency, NULL, false, true);
	
	m_varExposer->WrapVariable<bool>("Force rendering", &m_renderOptions.bForceRender, NULL, false, true);

	m_varExposer->WrapVariable<int>("Nb triangles", &m_meshNbTriangles, NULL, true, false);
	m_varExposer->WrapVariable<uint>("Size", &m_meshSize, NULL, true, false);
}

void ZobComponentMesh::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (cb)
	{
		cb(0, 0, m_name);
	}
	m_meshNbTriangles = 0;
	m_meshSize = 0;
	ReLoadVariables();
	if (m_mesh == NULL && m_meshPath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			DirectZob::LogError("Error loading mesh %s at %s", m_meshPath.GetName().c_str(), m_meshPath.GetFullPath().c_str());
			m_meshPath.Reset();
			m_meshNbTriangles = 0;
		}
		else
		{
			m_meshNbTriangles = m_mesh->GetNbTriangles();
			m_meshSize = m_mesh->GetSize();
		}
	}
}

void ZobComponentMesh::Set(ZobFilePath zfp) 
{ 
	m_meshPath = zfp;
	m_meshPath.SetFileType(ZobFilePath::eFileType_mesh);
	Init(NULL);
}

void ZobComponentMesh::PreUpdate(float dt)
{
}

void ZobComponentMesh::UpdateAfterObject(float dt)
{

}

void ZobComponentMesh::SetVisible(bool v)
{ 
	m_mesh->SetVisible(v); 
}

void ZobComponentMesh::PostUpdate()
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

void ZobComponentMesh::QueueForDrawing(const Camera* camera, Engine* engine)
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

void ZobComponentMesh::EditorUpdate()
{
	if (!m_mesh)
	{
		if (m_meshPath.IsDefined())
		{
			Init(NULL);
		}
	}
	else
	{
		if (m_mesh->GetName() != m_meshPath.GetName())
		{
			LoadMeshInternal();
		}
	}
}

bool ZobComponentMesh::LoadMeshInternal()
{
	m_mesh = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(m_meshPath);
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