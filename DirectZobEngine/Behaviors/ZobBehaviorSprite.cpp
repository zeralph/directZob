#include "ZobBehaviorSprite.h"
#include "../DirectZob.h"

ZobBehaviorSprite::~ZobBehaviorSprite()
{

}

ZobBehaviorSprite::ZobBehaviorSprite(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_type = eBehavior_sprite;
	m_texturePath.Init();
	m_sprite = NULL;
	m_renderOptions.zBuffered = true;
	//m_renderOptions.lightMode = Triangle::RenderOptions::eLightMode_none;
	//m_renderOptions.cullMode = Triangle::RenderOptions::eCullMode_CounterClockwiseFace;
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
		ZobVector3 color = ZobVector3(1, 1, 1);
		std::string f = m_texturePath.GetFullPath();
		const ZobMaterial * mat = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texturePath.name, &color, &color, &color, 0, 1, f);
		m_sprite->Setmaterial(mat);
	}
	return m_sprite != NULL;
}