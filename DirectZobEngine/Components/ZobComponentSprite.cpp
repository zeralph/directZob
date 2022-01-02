#include "ZobComponentSprite.h"
#include "../DirectZob.h"
#include "ZobComponentFactory.h"
#include "../Sprite.h"

ZobComponentSprite::~ZobComponentSprite()
{

}

ZobComponentSprite::ZobComponentSprite(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	m_type = eComponent_sprite;
	m_texturePath.Reset();
	m_sprite = NULL;
	m_material = NULL;
	m_zobObject = zobObject;
	m_ambientColor = &ZobColor::White;
	m_diffuseColor = &ZobColor::White;
	m_specularColor = &ZobColor::White;
	m_texturePath.SetFileType(ZobFilePath::eFileType_texture);
	m_varExposer->WrapVariable<ZobFilePath>("File", &m_texturePath, &ZobComponentSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("Ambient color", &m_ambientColor, &ZobComponentSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("diffuse color", &m_diffuseColor, &ZobComponentSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("specular color", &m_specularColor, &ZobComponentSprite::ReloadMaterial, false, true);

	Triangle::RenderOptions::eCullMode cm[3] = { Triangle::RenderOptions::eCullMode_None, Triangle::RenderOptions::eCullMode_ClockwiseFace, Triangle::RenderOptions::eCullMode_CounterClockwiseFace };
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, NULL, false, true);

	Triangle::RenderOptions::eLightMode lm[5] = { Triangle::RenderOptions::eLightMode_none, Triangle::RenderOptions::eLightMode_flat, Triangle::RenderOptions::eLightMode_gouraud, Triangle::RenderOptions::eLightMode_phong , Triangle::RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, NULL, false, true);

	m_varExposer->WrapVariable<bool>("ZBuffered", &m_renderOptions.zBuffered, NULL, false, true);
}

void ZobComponentSprite::Init()
{
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

void ZobComponentSprite::ReloadMaterial(zobId id)
{
	ZobComponent* zb = ZobEntity::GetEntity<ZobComponent>(id);
	if (zb)
	{
		DirectZob::LogInfo("pouet");
	}
}
void ZobComponentSprite::SetVisible(bool v)
{ 
	m_sprite->SetVisible(v); 
}

void ZobComponentSprite::SetForEditor()
{
	m_renderOptions.zBuffered = false;
	m_renderOptions.lightMode = Triangle::RenderOptions::eLightMode_none;
}

void ZobComponentSprite::Set(ZobFilePath zfp) 
{ 
	m_texturePath = zfp;
	m_texturePath.SetFileType(ZobFilePath::eFileType_texture);
	Init();
}

void ZobComponentSprite::PreUpdate(float dt)
{
	//m_material->
}

void ZobComponentSprite::UpdateAfterObject(float dt)
{
	/*
	const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		ZobVector3 t = m_modelMatrix.GetTranslation();
		ZobVector3 l = camera->GetLeft();
		ZobVector3 u = camera->GetUp();
		ZobVector3 f = camera->GetForward();
		ZobMatrix4x4 rotScale = ZobMatrix4x4::identity();
		m_rotationScaleMatrix.FromVectors(l, u, f);
		m_modelMatrix.FromVectors(l, u, f);
		m_rotationScaleMatrix.SetScale(GetScale());
		m_modelMatrix.AddTranslation(t);
		m_modelMatrix.SetScale(GetScale());

		m_left = ZobVector3(1, 0, 0);
		m_forward = ZobVector3(0, 0, 1);
		m_up = ZobVector3(0, 1, 0);


	}
	*/
}

void ZobComponentSprite::PostUpdate()
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

void ZobComponentSprite::QueueForDrawing(const Camera* camera, Engine* engine)
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

void ZobComponentSprite::EditorUpdate()
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

bool ZobComponentSprite::LoadMeshInternal()
{
	std::string s = std::string(m_texturePath.GetName());
	m_sprite = new ZobSprite(s);
	if (m_sprite)
	{
		ZobColor color = &ZobColor::White;
 		m_material = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texturePath.GetName(), &m_ambientColor, &m_diffuseColor, &m_specularColor, 0, 1, m_texturePath);
		m_sprite->Setmaterial(m_material);
	}
	return m_sprite != NULL;
}