#include "ZobObject.h"
#include "DirectZob.h"
#include "Mesh.h"
#include "Sprite.h"
#include "ZobPhysic/ZobPhysicComponent.h"
#include "SceneLoader.h"
#include "../Misc/ZobXmlHelper.h"
#include "../../dependencies/optick/include/optick.h"

static int sObjectNumber = 0;
ZobObject::ZobObject(ZobType t, ZobSubType s, const std::string& name, ZobObject* parent /*= NULL*/, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(t,s)
{
	DirectZob::LogInfo("ZobObject %s creation", name.c_str());
	DirectZob::AddIndent();
	std::string guid = ZobGuidToString();
	m_varExposer = new ZobVariablesExposer(guid);
	sObjectNumber++;
	m_behaviors.clear();
	m_factoryFile = factoryFile?factoryFile->c_str():"";
	m_physicComponent = new ZobPhysicComponent(this);
	if (name.length() == 0)
	{
		std::string n = "newObject_";
		n.append(std::to_string(sObjectNumber));
		m_name = n;
	}
	else
	{
		m_name = name;
	}
	DirectZob::LogInfo("Adding new ZobObject %s", m_name.c_str());
	if (!parent && m_name != "root")
	{
		m_parent = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
		//DirectZob::LogError("Cannot add %s because it has no parent", name.c_str());
	}
	else
	{
		m_parent = parent;
	}
	m_markedForDeletion = false;
	m_mesh = NULL;
	m_children.clear();
	m_newParent = m_parent;
	SetParentInternal();
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	m_physicComponent->GetLocalPositionAddress()->x = 0;
	m_physicComponent->GetLocalPositionAddress()->y = 0;
	m_physicComponent->GetLocalPositionAddress()->z = 0;
	m_physicComponent->GetLocalRotationAddress()->x = 0;
	m_physicComponent->GetLocalRotationAddress()->y = 0;
	m_physicComponent->GetLocalRotationAddress()->z = 0;
	m_physicComponent->GetLocalScaleAddress()->x = 1;
	m_physicComponent->GetLocalScaleAddress()->y = 1;
	m_physicComponent->GetLocalScaleAddress()->z = 1;
	InitVariablesExposer();
	m_physicComponent->Init();
	Update(0);
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(std::string id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(id)
{
	std::string guid = ZobGuidToString();
	m_varExposer = new ZobVariablesExposer(guid);
	m_behaviors.clear();
	sObjectNumber++;
	m_factoryFile = factoryFile ? factoryFile->c_str() : "";
	const char* name;
	float x, y, z;
	TiXmlElement* f;
	name = node->Attribute(XML_ATTR_NAME);
	assert(name);
	m_name = name;
	DirectZob::LogInfo("ZobObject %s creation", name);
	DirectZob::AddIndent();
	DirectZob::LogInfo("Adding new ZobObject %s", m_name.c_str());
	if (!parent && m_name != "root")
	{
		m_parent = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
		//DirectZob::LogError("Cannot add %s because it has no parent", name.c_str());
	}
	else
	{
		m_parent = parent;
	}
	m_markedForDeletion = false;
	m_mesh = NULL;
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	//mesh loading section
	TiXmlElement* meshNode = node->FirstChildElement(XML_ATTR_TYPE_MESH);
	if (meshNode)
	{
		std::string meshName = meshNode->Attribute(XML_ATTR_PATH);
		std::string meshPath = meshNode->Attribute(XML_ATTR_PATH) ? meshNode->Attribute(XML_ATTR_PATH) : std::string("");
		std::string meshFile = meshNode->Attribute(XML_ATTR_FILE);
		if (meshName.length() > 0 && meshFile.length() > 0)
		{
			LoadMesh(meshName, meshFile, meshPath);
		}
		f = meshNode->FirstChildElement(XML_ELEMENT_RENDER_OPTIONS);
		if (f)
		{
			TiXmlElement* n = f->FirstChildElement(XML_ELEMENT_RENDER_OPTIONS_LIGHTING);
			if (n)
			{
				std::string l = n->GetText();
				if (l == XML_ELEMENT_RENDER_OPTIONS_LIGHTING_NONE)
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_none;
				}
				else if (l == XML_ELEMENT_RENDER_OPTIONS_LIGHTING_FLAT)
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_flat;
				}
				else if (l == XML_ELEMENT_RENDER_OPTIONS_LIGHTING_FLAT_PHONG)
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_flatPhong;
				}
				else if (l == XML_ELEMENT_RENDER_OPTIONS_LIGHTING_GOURAUD)
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_gouraud;
				}
				else if (l == XML_ELEMENT_RENDER_OPTIONS_LIGHTING_PHONG)
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
				}
			}
			n = f->FirstChildElement(XML_ELEMENT_RENDER_OPTIONS_ZBUFFER);
			if (n)
			{
				std::string l = n->GetText();
				if (l == XML_VALUE_TRUE)
				{
					m_renderOptions.zBuffered = true;
				}
				else
				{
					m_renderOptions.zBuffered = false;
				}
			}
			n = f->FirstChildElement(XML_ELEMENT_RENDER_OPTIONS_TRANSPARENCY);
			if (n)
			{
				std::string l = n->GetText();
				if (l == XML_VALUE_TRUE)
				{
					m_renderOptions.bTransparency = true;
				}
				else
				{
					m_renderOptions.bTransparency = false;
				}
			}
			n = f->FirstChildElement(XML_ELEMENT_RENDER_OPTIONS_CULLMODE);
			if (n)
			{
				std::string l = n->GetText();
				if (l == XML_ELEMENT_RENDER_OPTIONS_CULLMODE_CLOCKWISE)
				{
					m_renderOptions.cullMode = eCullMode_ClockwiseFace;
				}
				else if (l == XML_ELEMENT_RENDER_OPTIONS_CULLMODE_COUNTERCLOCKWISE)
				{
					m_renderOptions.cullMode = eCullMode_CounterClockwiseFace;
				}
				else
				{
					m_renderOptions.cullMode = eCullMode_None;
				}
			}
		}
	}
	m_physicComponent = new ZobPhysicComponent(this);
	//parenting
	m_children.clear();
	m_newParent = m_parent;
	SetParentInternal();
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	InitVariablesExposer();
	m_varExposer->ReadNode(node);
	//Behavior
	TiXmlElement* behaviorsNode = node->FirstChildElement("Behaviors");
	if(behaviorsNode)
	{
		for (TiXmlElement* e = behaviorsNode->FirstChildElement("Behavior"); e != NULL; e = e->NextSiblingElement("Behavior"))
		{
			ZobBehavior* b = ZobBehaviorFactory::CreateBehavior(this, e);
		}
	}
	
	//Init
	m_physicComponent->Init();
	Update(0);
	DirectZob::RemoveIndent();
}

void ZobObject::InitVariablesExposer()
{
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_zobId, "GUID", GetIdAddress(), true, false);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_ZobVector3, "Position", GetPhysicComponentNoConst()->GetLocalPositionAddress(), false, true);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_ZobVector3, "Rotation", GetPhysicComponentNoConst()->GetLocalRotationAddress(), false, true);
	m_varExposer->WrapVariable(ZobVariablesExposer::eWrapperType_ZobVector3, "Scale", GetPhysicComponentNoConst()->GetLocalScaleAddress(), false, true);

}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();

	DirectZob::GetInstance()->GetZobObjectManager()->AddIdToDeleted(GetIdValue());
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		delete m_behaviors[i];
	}
	delete m_physicComponent;
	if (m_parent != NULL)
	{
		m_parent->RemoveChildReference(this);
	}
	m_parent = NULL;
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		m_children[i] = NULL;
		delete z;
	}
	delete m_varExposer;
	m_children.clear();
	DirectZob::RemoveIndent();
}

void ZobObject::LoadMesh(std::string name, std::string file, std::string path)
{
/*
	if (!path.length())	//no path, only filename
	{
		file = path;
		path = std::string(SceneLoader::GetResourcePath());
	}
	else
	{
		file = path.substr(i+1, path.size() - i);
		path = path.substr(0, i + 1);
	}
*/
	Mesh* m = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(name, path, file);
	m_mesh = m;
}

void ZobObject::SetMesh(std::string name)
{
	Mesh* m = DirectZob::GetInstance()->GetMeshManager()->GetMesh(name);
	m_mesh = m;
}

const std::string ZobObject::GetMeshName() const
{
	if (m_mesh)
	{
		return m_mesh->GetName();
	}
	return "";
}

const std::string ZobObject::GetMeshPath() const
{
	if (m_mesh)
	{
		return m_mesh->GetPath();
	}
	return "";
}

const std::string ZobObject::GetMeshFileName() const
{
	if (m_mesh)
	{
		return m_mesh->GetFileName();
	}
	return "";
}

void ZobObject::UpdateMesh(const Camera* camera, Core::Engine* engine)
{
	OPTICK_EVENT();
	if (m_mesh)
	{
		m_mesh->Update(&m_modelMatrix, &m_rotationScaleMatrix, camera, engine, GetIdValue(), &m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdateMesh(camera, engine);
	}
}

void ZobObject::Init()
{
	m_varExposer->Load();
	m_physicComponent->Init();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->Init();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Init();
	}
}

void ZobObject::EditorUpdate()
{
	m_physicComponent->EditorUpdate();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->EditorUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->EditorUpdate();
	}
}

void ZobObject::PreUpdate()
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->PreUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->PreUpdate();
	}
	SetParentInternal();
	DeleteInternal();
}

void ZobObject::UpdateBehavior(float dt)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->Update(dt);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdateBehavior(dt);
	}
}

void ZobObject::SetParentInternal()
{
	if (m_newParent != NULL)
	{
		if (!HasChild(m_newParent))
		{
			DirectZob::LogInfo("Reparented %s \tfrom %s \tto %s", GetName().c_str(), m_parent->GetName().c_str(), m_newParent->GetName().c_str());
			ZobVector3 pos = GetWorldPosition();
			ZobVector3 rot = GetWorldRotation();
			ZobVector3 sca = GetScale();
			if (GetParent()->RemoveChildReference(this))
			{
				if (m_newParent->AddChildReference(this))
				{
					m_parent = m_newParent;
					//remove inherited scale factor
					//ZobVector3 ps = parent->GetScale();
					//s.x *= ps.x;
					//s.y *= ps.y;
					//s.z *= ps.z;
					SetWorldPosition(pos.x, pos.y, pos.z);
					SetWorldRotation(rot.x, rot.y, rot.z);
					//SetScale(sca.x, sca.y, sca.z);
				}
			}
		}
		else
		{
			DirectZob::LogWarning("Trying to reparent an object with one of its descendants !");
		}
		m_newParent = NULL;
	}
}

void ZobObject::DeleteInternal()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if (z->IsMarkedForDeletion())
		{
			RemoveChildReference(z);
			delete z;
		}
		else
		{
			z->PreUpdate();
		}
	}
}

//void ZobObject::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void ZobObject::Update(float dt)
{
	OPTICK_EVENT();
	Transform parentTransform;
	ZobVector3 parentScale;
	if (m_parent)
	{
		const ZobPhysicComponent* zpc = m_parent->GetPhysicComponent();
		parentTransform = zpc->GetWorldTransform();
		parentScale = zpc->GetWorldScale();
	}
	else
	{
		parentTransform = Transform::identity();
		parentScale = ZobVector3(1, 1, 1);
	}
	ZobVector3 scale = parentScale * m_physicComponent->GetLocalScale();
	Transform newTransform = m_physicComponent->GetLocalTransform();
	Vector3 p = parentTransform.getPosition();
	//p.x *= scale.x;
	//p.y *= scale.y;
	//p.z *= scale.z;
	//parentTransform.setPosition(p);
	//newTransform = parentTransform * newTransform;
	//m_physicComponent->SetWorldTransform(newTransform);
	m_physicComponent->Update();
	m_physicComponent->SetWorldScale(scale.x, scale.y, scale.z);
	const ZobMatrix4x4* parentMatrix = m_parent?m_parent->GetModelMatrix():&ZobMatrix4x4::IdentityMatrix;
	const ZobMatrix4x4* parentRSMatrix = m_parent?m_parent->GetRotationScaleMatrix():&ZobMatrix4x4::IdentityMatrix;
	ZobVector3 t = GetWorldPosition();
	ZobVector3 wpos = GetWorldRotation();	
	m_modelMatrix.Identity();
	m_rotationScaleMatrix.Identity();
	m_rotationScaleMatrix.SetRotation(GetWorldRotation());
	m_rotationScaleMatrix.SetScale(scale);
	m_modelMatrix.SetPosition(&t);
	m_modelMatrix.SetRotation(&wpos);
	m_modelMatrix.SetScale(scale);
	m_left = ZobVector3(1, 0, 0);
	m_forward = ZobVector3(0, 0, 1);
	m_up = ZobVector3(0, 1, 0);
	m_rotationScaleMatrix.Mul(&m_left);
	m_rotationScaleMatrix.Mul(&m_forward);
	m_rotationScaleMatrix.Mul(&m_up);
	m_left.Normalize();
	m_forward.Normalize();
	m_up.Normalize();
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Update(dt);
	}
}

void ZobObject::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	OPTICK_EVENT();
	if(GetType() == ZOBGUID::type_editor)
	{
//		return;
	}
	if (m_mesh)
	{
		m_mesh->QueueForDrawing(this, m_modelMatrix, m_rotationScaleMatrix, camera, engine, GetIdValue(), &m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->QueueForDrawing(camera, engine);
	}
	if(engine->DrawGizmos() && engine->DrawZobObjectGizmos())
	{
		DrawGizmos(camera, engine);
	}
}

ZobVector3 ZobObject::GetScale() const 
{ 
	return m_physicComponent->GetWorldScale(); 
}

void ZobObject::SetScale(float x, float y, float z)
{ 
	ZobVector3 s = m_parent->GetScale();
	x /= s.x;
	y /= s.y;
	z /= s.z;
	m_physicComponent->SetLocalScale(x, y, z);
}

void ZobObject::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	uint c;
	ZobVector3 x = m_left;
	ZobVector3 y = m_up;
	ZobVector3 z = m_forward;
	ZobVector3 t = GetWorldPosition();
	x = x + t;
	y = y + t;
	z = z + t;
	c = 0x00FF0000;
	engine->QueueLine(camera, &t, &x, c, true, false);
	c = 0x0000FF00;
	engine->QueueLine(camera, &t, &y, c, true, false);
	c = 0x000000FF;
	engine->QueueLine(camera, &t, &z, c, true, false);
	ZobVector3 p = GetWorldPosition();
	ZobVector3 r = GetWorldRotation();
	m_physicComponent->DrawGizmos(camera, &p, &r);
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->DrawGizmos(camera, &p, &r);
	}
}

int ZobObject::GetChildPosition(const ZobObject* z)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) == z)
		{
			return i;
		}
	}
	return -1;
}

bool ZobObject::RemoveChildReference(const ZobObject* z)
{
	int i = GetChildPosition(z);
	if (i >= 0)
	{
		std::swap(m_children.at(i), m_children.at(m_children.size() - 1));
		m_children.pop_back();
		return true;
	}
	return false;
}

bool ZobObject::AddChildReference(ZobObject* z)
{
	m_children.push_back(z);
	return true;
}

ZobObject* ZobObject::GetChild(const std::string& name)
{
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return (*iter);
		}
	}
	return NULL;
}

ZobObject* ZobObject::GetChild(const int i)
{
	if (i >= 0 && i < m_children.size())
	{
		return m_children.at(i);
	}
	return NULL;
}

const void ZobObject::GetFullNodeName(std::string& fullname) const
{
	std::string s = GetName();
	std::string s2 = "";
	const ZobObject* z = this;
	while (z->GetParent())
	{
		z = z->GetParent();
		s2 = z->GetName();
		s2.append("/");
		s2.append(s);
		s = s2;
	}
	fullname = s;
}

void ZobObject::SetParent(ZobObject* p)
{
	m_newParent = p;
	SetParentInternal();
}

bool ZobObject::HasChild(const ZobObject* o)
{
	if (o == this)
	{
		return true;
	}
	else
	{
		bool bRet = false;
		for (int i = 0; i < m_children.size(); i++)
		{
			bRet |= m_children.at(i)->HasChild(o);
		}
		return bRet;
	}
}

void ZobObject::SetLightingMode(RenderOptions::eLightMode l)
{
	m_renderOptions.lightMode = l;
}

void ZobObject::SaveToFactoryFile(std::string& file)
{
	TiXmlDocument doc(XML_ELEMENT_ZOBOBJECT);
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	SaveRecusrive(&doc, this);
	//doc.LinkEndChild(&n);
	if (!doc.SaveFile(file.c_str()))
	{
		DirectZob::LogError("Error saving zobObject : ", doc.ErrorDesc());
	}
}

void ZobObject::SaveRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	TiXmlNode* newNode = z->SaveUnderNode(node);
	for (int i = 0; i < z->GetNbChildren(); i++)
	{
		SaveRecusrive(newNode, z->GetChild(i));
	}
	node->InsertEndChild(*newNode);
}

TiXmlNode* ZobObject::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement* objectNode = new TiXmlElement(XML_ELEMENT_ZOBOBJECT);
	objectNode->SetAttribute(XML_ATTR_NAME, GetName().c_str());
	std::string guid = ZobGuidToString();
	objectNode->SetAttribute(XML_ATTR_GUID, guid.c_str());
	std::string meshName = GetMeshName();
	std::string meshFileName = GetMeshFileName();
	std::string meshPath = GetMeshPath();

	m_varExposer->SaveUnderNode(objectNode);

	if (meshName.length() > 0 && meshFileName.length() > 0)
	{
		TiXmlElement meshNode = TiXmlElement(XML_ATTR_TYPE_MESH);
		meshNode.SetAttribute(XML_ATTR_NAME, meshName.c_str());
		meshNode.SetAttribute(XML_ATTR_FILE, meshFileName.c_str());
		meshNode.SetAttribute(XML_ATTR_PATH, meshPath.c_str());
		objectNode->SetAttribute(XML_ATTR_TYPE, XML_ATTR_TYPE_MESH);
		TiXmlElement renderOptions = TiXmlElement(XML_ELEMENT_RENDER_OPTIONS);
		TiXmlElement lighting = TiXmlElement(XML_ELEMENT_RENDER_OPTIONS_LIGHTING);
		TiXmlText t(XML_ELEMENT_RENDER_OPTIONS_LIGHTING_NONE);
		if (m_renderOptions.lightMode == RenderOptions::eLightMode_flat)
		{
			t = XML_ELEMENT_RENDER_OPTIONS_LIGHTING_FLAT;
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_flatPhong)
		{
			t = XML_ELEMENT_RENDER_OPTIONS_LIGHTING_FLAT_PHONG;
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_gouraud)
		{
			t = XML_ELEMENT_RENDER_OPTIONS_LIGHTING_GOURAUD;
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_phong)
		{
			t = XML_ELEMENT_RENDER_OPTIONS_LIGHTING_PHONG;
		}
		lighting.InsertEndChild(t);
		renderOptions.InsertEndChild(lighting);
		TiXmlElement zBuffer = TiXmlElement(XML_ELEMENT_RENDER_OPTIONS_ZBUFFER);
		if (m_renderOptions.zBuffered)
		{
			t = XML_VALUE_TRUE;
		}
		else
		{
			t = XML_VALUE_FALSE;
		}
		zBuffer.InsertEndChild(t);
		renderOptions.InsertEndChild(zBuffer);
		TiXmlElement transparency = TiXmlElement(XML_ELEMENT_RENDER_OPTIONS_TRANSPARENCY);
		if (m_renderOptions.bTransparency)
		{
			t = XML_VALUE_TRUE;
		}
		else
		{
			t = XML_VALUE_FALSE;
		}
		transparency.InsertEndChild(t);
		renderOptions.InsertEndChild(transparency);

		TiXmlElement cullMode = TiXmlElement(XML_ELEMENT_RENDER_OPTIONS_CULLMODE);
		t = XML_ELEMENT_RENDER_OPTIONS_CULLMODE_CLOCKWISE;
		if (m_renderOptions.cullMode == eCullMode_CounterClockwiseFace)
		{
			t = XML_ELEMENT_RENDER_OPTIONS_CULLMODE_COUNTERCLOCKWISE;
		}
		cullMode.InsertEndChild(t);
		renderOptions.InsertEndChild(cullMode);
		meshNode.InsertEndChild(renderOptions);
		objectNode->InsertEndChild(meshNode);
	}
	TiXmlElement behaviors = TiXmlElement(XML_ELEMENT_BEHAVIORS);
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->SaveUnderNode(&behaviors);
	}
	objectNode->InsertEndChild(behaviors);
	return (TiXmlNode*)objectNode;
}

void ZobObject::CreateSprite()
{
	Sprite* s = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_mesh = s;
}
/*
void ZobObject::SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw)
{
	m_physicComponent->SetQuaternion(left, up, fw);
}

void ZobObject::SetQuaternion(float x, float y, float z, float w)
{
	m_physicComponent->SetQuaternion(x, y, z, w);
}
*/
void ZobObject::LookAt(const ZobVector3* target, bool addToCurrentRotation)
{
	//m_physicComponent->LookAt(target);
	Transform t = Transform(m_physicComponent->GetWorldTransform());
	ZobVector3 v = ZobVector3(t.getPosition().x, t.getPosition().y, t.getPosition().z);
	v.x = target->x - v.x;
	v.y = target->y - v.y;
	v.z = target->z - v.z;
	v.Normalize();
	if (v.y > 0.99f)
	{
		int g = 0;
		g++;
	}
	//TODO if v.y > 0.99 ...
	ZobVector3 forward = v;
	ZobVector3 left = ZobVector3::Cross(&forward, &ZobVector3::Vector3Y);
	left.Normalize();
	left.Mul(-1.0f);
	ZobVector3 up = ZobVector3::Cross(&forward, &left);
	up.Normalize();
	LookAt(&forward, &left, &up, addToCurrentRotation);
}

void ZobObject::LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up, bool addToCurrentRotation)
{
	//m_physicComponent->LookAt(forward, left, up);
	Quaternion q;
	q.w = sqrt(fmaxf(0, 1 + left->x + up->y + forward->z)) / 2.0f;
	q.x = sqrt(fmaxf(0, 1 + left->x - up->y - forward->z)) / 2.0f;
	q.y = sqrt(fmaxf(0, 1 - left->x + up->y - forward->z)) / 2.0f;
	q.z = sqrt(fmaxf(0, 1 - left->x - up->y + forward->z)) / 2.0f;

	q.x = copysignf(q.x, up->z - forward->y);
	q.y = copysignf(q.y, forward->x - left->z);
	q.z = copysignf(q.z, left->y - up->x);

	//q.normalize();
	if (q.isValid())
	{
		if (false && addToCurrentRotation)
		{
			Quaternion q2 = m_physicComponent->GetLocalTransform().getOrientation();
			q = q2 * q;
			q.normalize();

		}
		m_physicComponent->SetLocalOrientation(q);
	}
}

void ZobObject::SetWorldRotation(float x, float y, float z)
{
	//m_physicComponent->SetOrientation(x, y, z);
	float dy = DEG_TO_RAD(y);
	float dz = DEG_TO_RAD(z);
	float dx = DEG_TO_RAD(x);
	Quaternion q;
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(dx, dy, dz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	m_physicComponent->SetLocalOrientation(q);
}

void ZobObject::SetLocalRotation(float x, float y, float z)
{
	//m_physicComponent->SetOrientation(x, y, z);
	float dy = DEG_TO_RAD(y);
	float dz = DEG_TO_RAD(z);
	float dx = DEG_TO_RAD(x);
	Quaternion q;
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(dx, dy, dz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	Transform parentTransform = Transform::identity();
	if (m_parent)
	{
		parentTransform = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetLocalTransform();
	newTransform.setOrientation(q);
	//newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
}

void ZobObject::SetLocalPosition(float x, float y, float z)
{
	Vector3 position = Vector3(x, y, z);
	Transform t = GetPhysicComponent()->GetLocalTransform();
	t.setPosition(position);
	m_physicComponent->SetLocalTransform(t);
}

void ZobObject::SetWorldPosition(float x, float y, float z)
{
	/*
	Transform parentTransform = Transform::identity();
	if (m_parent)
	{
		parentTransform = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetWorldTransform();
	newTransform.setPosition(position);
	newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
	*/
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetWorldTransform();
	newTransform.setPosition(position);
	m_physicComponent->SetWorldTransform(newTransform);
}

ZobVector3 ZobObject::GetLocalRotation() const
{
	Quaternion q= m_physicComponent->GetLocalTransform().getOrientation();
	ZobVector3 v = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	v.x = RAD_TO_DEG(v.x);
	v.y = RAD_TO_DEG(v.y);
	v.z = RAD_TO_DEG(v.z);
	return v;
}

ZobVector3 ZobObject::GetLocalPosition() const
{
	Vector3 p = m_physicComponent->GetLocalTransform().getPosition();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

ZobVector3 ZobObject::GetWorldRotation() const
{
	Quaternion q = m_physicComponent->GetWorldTransform().getOrientation();
	ZobVector3 v = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	v.x = RAD_TO_DEG(v.x);
	v.y = RAD_TO_DEG(v.y);
	v.z = RAD_TO_DEG(v.z);
	return v;
}

ZobVector3 ZobObject::GetWorldPosition() const
{
	Vector3 p = m_physicComponent->GetWorldTransform().getPosition();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

void ZobObject::RegenerateZobIds()
{
	ZobGuidRegenerate();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->ZobGuidRegenerate();
	}
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RegenerateZobIds();
	}
}

void ZobObject::SaveTransform()
{
	m_physicComponent->SaveTransform();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->SaveTransform();
	}
}

void ZobObject::RestoreTransform()
{
	m_physicComponent->RestoreTransform();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RestoreTransform();
	}
}

void ZobObject::ResetPhysic()
{
	m_physicComponent->ResetPhysic();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->ResetPhysic();
	}
}