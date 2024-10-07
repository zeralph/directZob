#include "ZobComponentSkybox.h"
#include "../../DirectZob.h"
#include "../ZobComponentFactory.h"
#include "../../ZobObjects/Camera.h"
#include "../../Managers/CameraManager.h"
#include "../../Managers/MeshManager.h"
#include "../../Managers/MaterialManager.h"
#include "../../Sprite.h"

ZobComponentSkybox::~ZobComponentSkybox()
{

}

ZobComponentSkybox::ZobComponentSkybox(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	SET_CLASS_AND_NAME
	m_componentType = ZobComponentFactory::eComponent_skybox;
	m_texturePath.Reset();
	m_sprite = NULL;
	m_material = NULL;
	m_zobObject = zobObject;
	m_ambientColor = &ZobColor::White;
	m_diffuseColor = &ZobColor::White;
	m_specularColor = &ZobColor::White;
	m_texturePath.SetFileType(ZobFilePath::eFileType_texture);
	m_varExposer->WrapVariable<ZobFilePath>("File", &m_texturePath, &ZobComponentSkybox::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("Ambient color", &m_ambientColor, &ZobComponentSkybox::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("diffuse color", &m_diffuseColor, &ZobComponentSkybox::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("specular color", &m_specularColor, &ZobComponentSkybox::ReloadMaterial, false, true);

	Triangle::RenderOptions::eCullMode cm[3] = { Triangle::RenderOptions::eCullMode_None, Triangle::RenderOptions::eCullMode_ClockwiseFace, Triangle::RenderOptions::eCullMode_CounterClockwiseFace };
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, NULL, false, false, true);

	Triangle::RenderOptions::eLightMode lm[5] = { Triangle::RenderOptions::eLightMode_none, Triangle::RenderOptions::eLightMode_flat, Triangle::RenderOptions::eLightMode_gouraud, Triangle::RenderOptions::eLightMode_phong , Triangle::RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, NULL, false, false, true);

	Triangle::RenderOptions::eZBufferMode zm[4] = { Triangle::RenderOptions::buffered, Triangle::RenderOptions::unBuffered, Triangle::RenderOptions::halfBuffered, Triangle::RenderOptions::noZFar };
	const char* zmStr[4] = { "Buffered", "Unbuffered", "Half buffered", "No Z-far" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eZBufferMode>("Z-buffer mode", &m_renderOptions.zBuffer, 4, zm, zmStr, NULL, false, false, true);
}

void ZobComponentSkybox::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (m_type != ZobEntity::type_editor && cb)
	{
		cb(0, 0, m_name);
	}
	ReLoadVariables();
	if (m_sprite == NULL && m_texturePath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			DirectZob::LogError("Error loading sprite %s texture at %s", m_texturePath.GetName().c_str(), m_texturePath.GetFullPath().c_str());
			m_texturePath.Reset();
		}
	}
}

void ZobComponentSkybox::ReloadMaterial(zobId id)
{
	ZobComponent* zb = ZobEntity::GetEntity<ZobComponent>(id);
	if (zb)
	{
		DirectZob::LogInfo("pouet");
	}
}
void ZobComponentSkybox::SetVisible(bool v)
{ 
	m_sprite->SetVisible(v); 
}

void ZobComponentSkybox::SetForEditor()
{
	m_renderOptions.zBuffer = Triangle::RenderOptions::noZFar;
	m_renderOptions.lightMode = Triangle::RenderOptions::eLightMode_none;
}

void ZobComponentSkybox::Set(ZobFilePath zfp) 
{ 
	m_texturePath = zfp;
	m_texturePath.SetFileType(ZobFilePath::eFileType_texture);
	Init(NULL);
}

void ZobComponentSkybox::PreUpdate(float dt)
{
	//m_material->
}

void ZobComponentSkybox::PostUpdate()
{
	/*if (m_sprite)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		m_sprite->QueueForDrawing(m_zobObject, mm, rs, c, e, ro);
	}*/
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (m_sprite && c)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		ZobMatrix4x4 rotScale = ZobMatrix4x4();
		rotScale.Identity();
		ZobVector3 l = c->GetLeft();
		ZobVector3 u = c->GetUp();
		ZobVector3 f = c->GetForward();
		rotScale.FromVectors(l, u, f);
		ZobVector3 s = mm->GetScale();
		rotScale.AddScale(s);
		rotScale.AddTranslation(mm->GetTranslation());
		const Triangle::RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		//m_sprite->Update(mm, rs, c, e, ro);
		m_sprite->Update(&rotScale, &rotScale, c, e, ro);
	}
}

void ZobComponentSkybox::QueueForDrawing(const Camera* camera, Engine* engine)
{
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (m_sprite && c)
	{
		const ZobMatrix4x4* mm = m_zobObject->GetModelMatrix();
		const ZobMatrix4x4* rs = m_zobObject->GetRotationMatrix();
		ZobMatrix4x4 rotScale = ZobMatrix4x4();
		rotScale.Identity();
		ZobVector3 l = c->GetLeft();
		ZobVector3 u = c->GetUp();
		ZobVector3 f = c->GetForward();
		rotScale.FromVectors(l, u, f);
		rotScale.AddScale(mm->GetScale());
		rotScale.AddTranslation(mm->GetTranslation());
		Triangle::RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		//m_sprite->QueueForDrawing(m_zobObject, mm, rs, c, e, ro);
		m_sprite->QueueForDrawing(m_zobObject, &rotScale, &rotScale, c, e, ro);
	}
}

void ZobComponentSkybox::EditorUpdate()
{
	if (!m_sprite)
	{
		if (m_texturePath.IsDefined())
		{
			LoadMeshInternal();
		}
	}
	else
	{
		if (m_sprite->GetName() != m_texturePath.GetName())
		{
			LoadMeshInternal();
		}
	}
}

bool ZobComponentSkybox::LoadMeshInternal()
{
	std::string s = std::string(m_texturePath.GetName());
	throw "todo";
	//m_sprite = new ZobSprite(s);
	if (m_sprite)
	{
		ZobColor color = &ZobColor::White;
 		m_material = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texturePath.GetName(), &m_ambientColor, &m_diffuseColor, &m_specularColor, 0, 1, &m_texturePath);
		m_sprite->Setmaterial(m_material);
	}
	return m_sprite != NULL;
}