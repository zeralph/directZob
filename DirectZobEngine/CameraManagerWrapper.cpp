#ifdef _WINDLL
#include "CameraManagerWrapper.h"
#include "DirectZob.h"
using namespace directZobCLI;

CameraManagerWrapper::CameraManagerWrapper():ManagedObject(directZob::DirectZob::GetInstance()->GetCameraManager(), false)
{

}

array<System::String^>^ CameraManagerWrapper::GetCameraList()
{
	const std::vector<std::string> data = m_Instance->GetCameraList();
	int l = (int)data.size();
	array<System::String ^>^ arr = gcnew array<System::String ^>(l);
	for (int i = 0; i < l; i++)
	{
		arr[i] = gcnew System::String(data.at(i).c_str());
	}
	return arr;
}

System::String^ CameraManagerWrapper::GetCurrentCamera()
{
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		return gcnew System::String(c->GetName().c_str());
	}
	return nullptr;
}

ManagedVector3^ CameraManagerWrapper::GetCurrentCameraPosition()
{
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetTransform());
		return v;
	}
	return gcnew directZobCLI::ManagedVector3();
}

ManagedVector3^ CameraManagerWrapper::GetCurrentCameraTarget()
{
	/*
	Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetTarget());
		return v;
	}
	*/
	return gcnew directZobCLI::ManagedVector3();
}

void CameraManagerWrapper::RotateAroundAxis(float x, float y)
{
	directZob::DirectZob::GetInstance()->Lock();
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		directZob::Vector3 v = directZob::Vector3(0, 1, 0);
		directZob::Vector3 p = directZob::Vector3(0, 0, 0);
		directZob::Vector3 l = directZob::Vector3(0, 1, 0);
		c->GetTargetVector(&p);
		v = c->GetUp();
		c->RotateAroundPointAxis(&p, &v, NULL, x);
		v = c->GetLeft();
		c->RotateAroundPointAxis(&p, &v, &l, y);
	}
	directZob::DirectZob::GetInstance()->Unlock();
}

void CameraManagerWrapper::Move(float x, float y)
{
	directZob::DirectZob::GetInstance()->Lock();
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		c->Move(x, y, true);
	}
	directZob::DirectZob::GetInstance()->Unlock();
}

void CameraManagerWrapper::Zoom(float z)
{
	directZob::DirectZob::GetInstance()->Lock();
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		c->Zoom(z);
	}
	directZob::DirectZob::GetInstance()->Unlock();
}

void CameraManagerWrapper::SetCurrentCameraPosition(ManagedVector3^ p)
{
	directZob::DirectZob::GetInstance()->Lock();
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		directZob::Vector3 v = p->ToVector3();
		c->SetTranslation(v.x, v.y, v.z);
	}
	directZob::DirectZob::GetInstance()->Unlock();
}


void CameraManagerWrapper::CreateEditorCamera()
{
	m_Instance->CreateEditorCamera();
}

void CameraManagerWrapper::CreateCamera()
{
	m_Instance->CreateCamera(NULL);
}

void CameraManagerWrapper::SetCurrentCamera(System::String^ name)
{
	directZob::DirectZob::GetInstance()->Lock();
	std::string stdName;
	MarshalString(name, stdName);
	m_Instance->SetNextCamera(stdName);
	directZob::DirectZob::GetInstance()->Unlock();
}

void CameraManagerWrapper::SetLookAt( ManagedVector3^ target)
{
	directZob::DirectZob::GetInstance()->Lock();
	directZob::Camera* c = m_Instance->GetCurrentCamera();
	if (c)
	{
		directZob::Vector3 t = target->ToVector3();
		c->SetTarget(&t);
	}
	directZob::DirectZob::GetInstance()->Unlock();
}

#endif //_WINDLL