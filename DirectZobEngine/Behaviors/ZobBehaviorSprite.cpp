#include "ZobBehaviorSprite.h"
#include "../DirectZob.h"
#include "ZobBehaviorFactory.h"
ZobBehaviorSprite::~ZobBehaviorSprite()
{

}

ZobBehaviorSprite::ZobBehaviorSprite(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_type = eBehavior_sprite;
	m_texturePath.Init();
	m_sprite = NULL;
	m_material = NULL;
	m_zobObject = zobObject;
	m_ambientColor = &ZobColor::White;
	m_diffuseColor = &ZobColor::White;
	m_specularColor = &ZobColor::White;
	m_varExposer->WrapVariable<ZobFilePath>("File", &m_texturePath, &ZobBehaviorSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("Ambient color", &m_ambientColor, &ZobBehaviorSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("diffuse color", &m_diffuseColor, &ZobBehaviorSprite::ReloadMaterial, false, true);
	//m_varExposer->WrapVariable<ZobColor>("specular color", &m_specularColor, &ZobBehaviorSprite::ReloadMaterial, false, true);

	Triangle::RenderOptions::eCullMode cm[3] = { Triangle::RenderOptions::eCullMode_None, Triangle::RenderOptions::eCullMode_ClockwiseFace, Triangle::RenderOptions::eCullMode_CounterClockwiseFace };
	const char* cmStr[3] = { "None", "Clockwise", "Counter clockwise" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eCullMode>("Cull mode", &m_renderOptions.cullMode, 3, cm, cmStr, NULL, false, true);

	Triangle::RenderOptions::eLightMode lm[5] = { Triangle::RenderOptions::eLightMode_none, Triangle::RenderOptions::eLightMode_flat, Triangle::RenderOptions::eLightMode_gouraud, Triangle::RenderOptions::eLightMode_phong , Triangle::RenderOptions::eLightMode_flatPhong };
	const char* lmStr[5] = { "None", "Flat", "Gouraud", "Phong", "Flat phong" };
	m_varExposer->WrapEnum<Triangle::RenderOptions::eLightMode>("Lighting", &m_renderOptions.lightMode, 5, lm, lmStr, NULL, false, true);

	m_varExposer->WrapVariable<bool>("ZBuffered", &m_renderOptions.zBuffered, NULL, false, true);
}

void ZobBehaviorSprite::Init()
{
	ReLoadVariables();
	if (m_sprite == NULL && m_texturePath.IsDefined())
	{
		if (!LoadMeshInternal())
		{
			DirectZob::LogError("Mesh loading error %s %s %s", m_texturePath.file.c_str(), m_texturePath.path.c_str(), m_texturePath.name.c_str());
			m_texturePath.Init();
		}
	}
}

void ZobBehaviorSprite::ReloadMaterial(zobId id)
{
	const ZobBehavior* zb = ZobBehaviorFactory::GetBehaviorFromZobId(id);
	ZobObjectManager* zm = DirectZob::GetInstance()->GetZobObjectManager();
	if (zm)
	{
		ZobObject* z = zm->GetZobObjectFromlId(id);
		if (z)
		{
			DirectZob::LogInfo("pouet");
		}
	}
}

void ZobBehaviorSprite::Set(ZobFilePath zfp) 
{ 
	m_texturePath.file = zfp.file;
	m_texturePath.name = zfp.name;
	m_texturePath.path = zfp.path;
	m_texturePath.bAbsolute = zfp.bAbsolute;
	LoadMeshInternal();
}

void ZobBehaviorSprite::PreUpdate(float dt)
{
	//m_material->
}

void ZobBehaviorSprite::UpdateAfterObject(float dt)
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

void ZobBehaviorSprite::PostUpdate()
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
		rotScale.AddScale(mm->GetScale());
		rotScale.AddTranslation(mm->GetTranslation());
		const Triangle::RenderOptions* ro = &this->m_renderOptions;
		const Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		//m_sprite->Update(mm, rs, c, e, ro);
		m_sprite->Update(&rotScale, &rotScale, c, e, ro);
	}
}

void ZobBehaviorSprite::QueueForDrawing(const Camera* camera, Engine* engine)
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

void ZobBehaviorSprite::EditorUpdate()
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
		if (m_sprite->GetName() != m_texturePath.name)
		{
			LoadMeshInternal();
		}
	}
}

bool ZobBehaviorSprite::LoadMeshInternal()
{
	std::string s = std::string(m_texturePath.name);
	m_sprite = new ZobSprite(s);
	if (m_sprite)
	{
		ZobColor color = &ZobColor::White;
		std::string f = m_texturePath.GetFullPath();
		m_material = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texturePath.name, &m_ambientColor, &m_diffuseColor, &m_specularColor, 0, 1, f);
		m_sprite->Setmaterial(m_material);
	}
	return m_sprite != NULL;
}