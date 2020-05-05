#ifdef _WINDLL
#include "CameraManagerWrapper.h"
namespace CLI
{
	CameraManagerWrapper::CameraManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetCameraManager(), false)
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
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			return gcnew System::String(c->GetName().c_str());
		}
		return nullptr;
	}

	ManagedVector3^ CameraManagerWrapper::GetCurrentCameraPosition()
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetPosition());
			return v;
		}
		return gcnew CLI::ManagedVector3();
	}

	ManagedVector3^ CameraManagerWrapper::GetCurrentCameraTarget()
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetTarget());
			return v;
		}
		return gcnew CLI::ManagedVector3();
	}

	void CameraManagerWrapper::RotateAroundAxis(float x, float y)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			c->RotateAroundAxis(x, y);
		}
	}

	void CameraManagerWrapper::Move(float x, float y)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			c->Move(x, y);
		}
	}

	void CameraManagerWrapper::Zoom(float z)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			c->Zoom(z);
		}
	}

	void CameraManagerWrapper::SetCurrentCameraPosition(ManagedVector3^ p)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			Vector3 v = p->ToVector3();
			c->SetPosition(&v);
		}
	}

	void CameraManagerWrapper::CreateCamera()
	{
		m_Instance->CreateCamera();
	}

	void CameraManagerWrapper::SetCurrentCamera(System::String^ name)
	{
		std::string stdName;
		MarshalString(name, stdName);
		m_Instance->SetCurrentCamera(stdName);
	}

	void CameraManagerWrapper::SetLookAt(ManagedVector3^ position, ManagedVector3^ target, ManagedVector3^ up)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			Vector3 p = position->ToVector3();
			Vector3 t = target->ToVector3();
			Vector3 u = up->ToVector3();
			c->SetLookAt(&p, &t, &u);
		}
	}
}
#endif //_WINDLL