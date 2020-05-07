#include "SceneLoader.h"
#include "DirectZob.h"

std::string SceneLoader::m_path = "";
std::string SceneLoader::m_file = "";
char tmpBuffer[256];

void SceneLoader::LoadMesh(TiXmlElement* node)
{
	std::string name = node->Attribute("name");
	std::string file = node->Attribute("file");
	DirectZob::GetInstance()->GetMeshManager()->LoadMesh(name, m_path, file);
}

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent)
{
	float x, y, z;
	std::string name;
	std::string mesh;
	TiXmlElement* f;
	std::string type;
	ZobObject* zob = NULL;
	Vector3 position, rotation, scale, orientation = Vector3();	 
	name = node->Attribute("name");
	type = node->Attribute("type")? node->Attribute("type"):"mesh";
	f = node->FirstChildElement("Position");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	position = Vector3(x, y, z);
	f = node->FirstChildElement("Rotation");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		rotation = Vector3(x, y, z);
	}
	f = node->FirstChildElement("Scale");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		scale = Vector3(x, y, z);
	}
	if (type == "mesh")
	{
		f = node->FirstChildElement("Mesh");
		std::string meshName = f ? f->GetText() : "";
		MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
		Mesh* m = meshManager->GetMesh(meshName);
		if (parent == nullptr)
		{
			ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
			parent = zobObjectManager->GetRootObject();
		}
		zob = new ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobOject, name, m, parent);
		zob->SetTranslation(position.x, position.y, position.z);
		zob->SetRotation(rotation.x, rotation.y, rotation.z);
		zob->SetScale(scale.x, scale.y, scale.z);
	}
	else if (type == "camera")
	{
		if (parent == NULL)
		{
			ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
			parent = zobObjectManager->GetRootObject();
		}
		f = node->FirstChildElement("Fov");
		float fov = f ? atof(f->GetText()) : 45.0f;
		zob = DirectZob::GetInstance()->GetCameraManager()->CreateCamera(name, fov, parent);
	}
	else if (type == "pointlight")
	{
		Vector3 color = Vector3(1.0f, 0.0f, 1.0f);
		f = node->FirstChildElement("Color");
		if (f)
		{
			x = atof(f->Attribute("r"));
			y = atof(f->Attribute("g"));
			z = atof(f->Attribute("b"));
			color = Vector3(x/255.0f, y/255.0f, z/255.0f);
		}
		f = node->FirstChildElement("Intensity");
		float intensity = f ? atof(f->GetText()) : 1.0f;
		f = node->FirstChildElement("FallOffDistance");
		float falloff = f ? atof(f->GetText()) : 1.0f;
		if (parent == NULL)
		{
			ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
			parent = zobObjectManager->GetRootObject();
		}
		zob = DirectZob::GetInstance()->GetLightManager()->CreatePointLight(name, position, color, intensity, falloff, parent);
	}
	else
	{
		DirectZob::LogError("Error creating ZObjects %s", name.c_str());
	}
	for (TiXmlElement* e = node->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
	{
		LoadZobObject(e, zob);
	}
}

void SceneLoader::SaveGlobals(TiXmlElement* node)
{
	LightManager* lm = DirectZob::GetInstance()->GetLightManager();
	Vector3 ambient = lm->GetAmbientColor();
	ambient = Vector2Color(&ambient);
	TiXmlText t("");
	TiXmlElement e = TiXmlElement("AmbientColor");
	e.SetAttribute("r", ambient.x);
	e.SetAttribute("g", ambient.y);
	e.SetAttribute("b", ambient.z);
	node->InsertEndChild(e);
	Vector3 fogColor = lm->GetFogColor();
	fogColor = Vector2Color(&fogColor);
	e = TiXmlElement("FogColor");
	e.SetAttribute("r", fogColor.x);
	e.SetAttribute("g", fogColor.y);
	e.SetAttribute("b", fogColor.z);
	node->InsertEndChild(e);
	Vector3 clearColor = lm->GetClearColor();
	clearColor = Vector2Color(&clearColor);
	e = TiXmlElement("ClearColor");
	e.SetAttribute("r", clearColor.x);
	e.SetAttribute("g", clearColor.y);
	e.SetAttribute("b", clearColor.z);
	node->InsertEndChild(e);
	e = TiXmlElement("FogDensity");
	_snprintf_s(tmpBuffer, 256, "%.2f", lm->GetFogDensity());
	t.SetValue(tmpBuffer);
	e.InsertEndChild(t);
	node->InsertEndChild(e);
	e = TiXmlElement("FogDistance");
	_snprintf_s(tmpBuffer, 256, "%.2f", lm->GetFogDistance());
	t.SetValue(tmpBuffer);
	e.InsertEndChild(t);
	node->InsertEndChild(e);
	e = TiXmlElement("FogType");
	t.SetValue("");
	switch (lm->GetFogType())
	{
		case FogType_Exp:
			t.SetValue("exp");
			break;
		case FogType_Exp2:
			t.SetValue("exp2");
			break;
		case FogType_Linear:
			t.SetValue("linear");
			break;
		case FogType_NoFog:
		default:
			break;
	}
	e.InsertEndChild(t);
	node->InsertEndChild(e);
}

void SceneLoader::LoadGlobals(TiXmlElement* node)
{
	if (node)
	{
		LightManager* lm = DirectZob::GetInstance()->GetLightManager();
		TiXmlElement* e; 
		Vector3 ambient = lm->GetAmbientColor();
		e = node->FirstChildElement("AmbientColor");
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			ambient = Vector3(x, y, z);
		}
		Vector3 fog = lm->GetFogColor();
		e = node->FirstChildElement("FogColor");
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			fog = Vector3(x, y, z );
		}
		e = node->FirstChildElement("ClearColor");
		Vector3 clear = lm->GetClearColor();
		if (e)
		{
			float x = atof(e->Attribute("r"));
			float y = atof(e->Attribute("g"));
			float z = atof(e->Attribute("b"));
			clear = Vector3(x, y, z);
		}
		e = node->FirstChildElement("FogDensity");
		float fogDensity = lm->GetFogDensity();
		if (e)
		{
			fogDensity = atof(e->GetText());
		}
		e = node->FirstChildElement("FogDistance");
		float FogDistance = lm->GetFogDistance();
		if (e)
		{
			FogDistance = atof(e->GetText());
		}
		e = node->FirstChildElement("FogType");
		FogType fogType = lm->GetFogType();
		if (e)
		{
			std::string type = std::string(e->GetText());
			if (type == "linear")
			{
				fogType = FogType::FogType_Linear;
			}
			else if (type == "exp")
			{
				fogType = FogType::FogType_Exp;
			}
			else if (type == "exp2")
			{
				fogType = FogType::FogType_Exp2;
			}
			else 
			{
				fogType = FogType::FogType_NoFog;
			}
		}
		fog /= 255.0f;
		ambient /= 255.0f;
		clear /= 255.0f;
		lm->Setup(&fog, &ambient, &clear, FogDistance, fogDensity, fogType);
		/*
		int x = 320;
		int y = 240;
		DirectZob::GetInstance()->GetEngine()->Resize(x, y);
		*/
	}
}

void SceneLoader::UnloadScene()
{
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	LightManager* lightManager = DirectZob::GetInstance()->GetLightManager();
	Engine* engine = DirectZob::GetInstance()->GetEngine();
	engine->Stop();
	SLEEP(1000);	//ugly but ...
	zobObjectManager->UnloadAll();
	meshManager->UnloadAll();
	materialManager->UnloadAll();
	lightManager->UnloadAll();
}

void SceneLoader::NewScene()
{
	UnloadScene();
	m_path = "";
	m_file = "";
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	DirectZob::GetInstance()->GetEngine()->Start();
}

void SceneLoader::LoadScene(std::string& path, std::string& file)
{
	DirectZob::LogInfo("Load scene %s%s", path.c_str(), file.c_str());
	UnloadScene();
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	m_path = path;
	m_file = file;
	float x, y, z, fov, znear, zfar;
	std::string name, texture, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = path + file;
	doc.ClearError();
	doc.LoadFile(fullPath.c_str());
	if (doc.Error())
	{
		DirectZob::LogError("Error loading %s : %s", fullPath.c_str(), doc.ErrorDesc());
		m_path = "";
		m_file = "";
	}
	else
	{
		DirectZob::LogInfo("loading objects");
		TiXmlElement* root = doc.FirstChildElement("root");
		/*TiXmlElement* textures = root->FirstChildElement("Textures");
		for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			LoadTexture(e, MaterialManager);
		}*/
		TiXmlElement* meshes = root->FirstChildElement("Meshes");
		for (TiXmlElement* e = meshes->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			LoadMesh(e);
		}
		TiXmlElement* scene = root->FirstChildElement("Scene");
		LoadGlobals(scene->FirstChildElement("Globals"));
		for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
		{
			LoadZobObject(e, NULL);
		}
	}
	DirectZob::LogInfo("Scene loaded");
	DirectZob::GetInstance()->GetEngine()->Start();
}

void SceneLoader::SaveScene()
{
	if (CanFastSave())
	{
		SaveScene(m_path, m_file);
	}
}

void SceneLoader::SaveScene(std::string &path, std::string &file)
{
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	std::string fullPath;
	m_file = file;
	m_path = path;
	fullPath = path + file;
	TiXmlDocument doc("Scene");
	TiXmlElement * root = new TiXmlElement("root");
	doc.LinkEndChild(root);
	TiXmlElement meshes = TiXmlElement("Meshes");
	int n = meshManager->GetNbMeshes();
	for (int i = 0; i < n; i++)
	{
		TiXmlElement e = TiXmlElement("Mesh");
		e.SetAttribute("name", meshManager->GetMesh(i)->GetName().c_str());
		e.SetAttribute("file", meshManager->GetMesh(i)->GetFile().c_str());
		meshes.InsertEndChild(e);
	}
	root->InsertEndChild(meshes);
	TiXmlElement globals = TiXmlElement("Globals");
	SaveGlobals(&globals);
	root->InsertEndChild(globals);
	TiXmlElement scene = TiXmlElement("Scene");
	ZobObject* rootObj = zobObjectManager->GetRootObject();
	for (int i = 0; i < rootObj->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(&scene, rootObj->GetChild(i));
	}
	root->InsertEndChild(scene);
	if (!doc.SaveFile(fullPath.c_str()))
	{
		DirectZob::LogError("Error saving scene : ", doc.ErrorDesc());
	}
}

void SceneLoader::SaveZobObjectRecusrive(TiXmlElement* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	TiXmlElement o = TiXmlElement("ZobObject");
	TiXmlElement p = TiXmlElement("Position");
	TiXmlElement r = TiXmlElement("Rotation");
	TiXmlElement s = TiXmlElement("Scale");
	o.SetAttribute("name", z->GetName().c_str());
	std::string meshName = z->GetMeshName();
	p.SetDoubleAttribute("x", z->GetTransform().x);
	p.SetDoubleAttribute("y", z->GetTransform().y);
	p.SetDoubleAttribute("z", z->GetTransform().z);
	r.SetDoubleAttribute("x", z->GetRotation().x);
	r.SetDoubleAttribute("y", z->GetRotation().y);
	r.SetDoubleAttribute("z", z->GetRotation().z);
	s.SetDoubleAttribute("x", z->GetScale().x);
	s.SetDoubleAttribute("y", z->GetScale().y);
	s.SetDoubleAttribute("z", z->GetScale().z);
	o.InsertEndChild(p);
	o.InsertEndChild(r);
	o.InsertEndChild(s);
	if (meshName.length() > 0)
	{
		TiXmlElement m = TiXmlElement("Mesh");
		TiXmlText t = TiXmlText(meshName.c_str());
		m.InsertEndChild(t);
		o.InsertEndChild(m);
		o.SetAttribute("type", "mesh");
	}
	else if (z->GetSubType() == ZOBGUID::subtype_zobCamera)
	{

	}
	else if (z->GetSubType() == ZOBGUID::subtype_zobLight)
	{
		Light* l = (Light*)z;
		if (l)
		{
			Vector3 c = l->GetColor();
			int r = (int)(c.x * 255.0f);
			int g = (int)(c.y * 255.0f);
			int b = (int)(c.z * 255.0f);
			TiXmlElement color = TiXmlElement("Color");
			s.SetAttribute("r", r);
			s.SetAttribute("g", g);
			s.SetAttribute("b", b);
			TiXmlElement intensity = TiXmlElement("Intensity");
			_snprintf_s(tmpBuffer, 256, "%.2f", l->GetIntensity());
			intensity.SetValue(tmpBuffer);
			o.InsertEndChild(intensity);
			TiXmlElement fallOff = TiXmlElement("FallOffDistance");
			_snprintf_s(tmpBuffer, 256, "%.2f", l->GetFallOffDistance());
			intensity.SetValue(tmpBuffer);
			o.InsertEndChild(fallOff);
		}
	}
	for (int i = 0; i < z->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(&o, z->GetChild(i));
	}
	node->InsertEndChild(o);
}
