#ifdef _WINDLL
#include "CameraManagerWrapper.h"
#include "DirectZob.h"
namespace CLI
{
	CameraManagerWrapper::CameraManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetCameraManager(), false)
	{

	}

	cli::array<System::String^>^ CameraManagerWrapper::GetCameraList()
	{
		const std::vector<std::string> data = m_Instance->GetCameraList();
		int l = (int)data.size();
		cli::array<System::String ^>^ arr = gcnew cli::array<System::String ^>(l);
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
		/*
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetTarget());
			return v;
		}
		*/
		return gcnew CLI::ManagedVector3();
	}

	void CameraManagerWrapper::RotateAroundAxis(float x, float y)
	{
		//x = 0;
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 v = ZobVector3(0, 1, 0);
			ZobVector3 p = ZobVector3(0, 0, 0);
			ZobVector3 l = ZobVector3(0, 1, 0);
			ZobVector3 u = ZobVector3(1, 0, 0);
			c->GetTargetVector(&p);
			v = c->GetUp();
			v = l;
			c->RotateAroundPointAxis(&p, &v, NULL, x, false);
			v = c->GetLeft();
			v.y = 0.0f;
			v.Normalize();
			c->RotateAroundPointAxis(&p, &v, NULL, y, false);

		}
		DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::Move(float x, float y)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			c->Move(x, y, true);
		}
		DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::Zoom(float z)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			c->Zoom(z);
		}
		DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::From2DToWorld(ManagedVector3^ p)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 v = p->ToVector3();
			c->From2DToWorld(&v);
			p->FromVector3(v);
		}
		DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::SetCurrentCameraPosition(ManagedVector3^ p)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 v = p->ToVector3();
			c->SetPosition(v.x, v.y, v.z);
		}
		DirectZob::GetInstance()->Unlock();
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
		DirectZob::GetInstance()->Lock();
		std::string stdName;
		MarshalString(name, stdName);
		m_Instance->SetNextCamera(stdName);
		DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::SetLookAt( ManagedVector3^ target)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 t = target->ToVector3();
			c->SetTarget(&t);
		}
		DirectZob::GetInstance()->Unlock();
	}
}
#endif //_WINDLL