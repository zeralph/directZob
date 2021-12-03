#ifdef _WINDLL
#include "CameraManagerWrapper.h"
#include "../DirectZob.h"
namespace CLI
{
	CameraManagerWrapper::CameraManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetCameraManager(), false)
	{
		m_nextRotation.x = 0;
		m_nextRotation.y = 0;
		m_nextRotation.z = 0;
		m_nextMove.x = 0;
		m_nextMove.y = 0;
		m_nextMove.z = 0;
		m_nextZoom = 0;
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

	System::String^ CameraManagerWrapper::GetCurrentCameraName()
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
			ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetWorldPosition());
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

	void CameraManagerWrapper::Update(float dt)
	{
	}

	void CameraManagerWrapper::Rotate(float x, float y, float z)
	{
		m_nextRotation.x = +x;
		m_nextRotation.y = +y;
		m_nextRotation.z = +z;
	}

	void CameraManagerWrapper::Move(float x, float z, float y)
	{
		m_nextMove.x = +x;
		m_nextMove.y = +y;
		m_nextMove.z = +z;
	}

	void CameraManagerWrapper::Zoom(float z)
	{
		m_nextZoom = z;
	}

	CameraManagerWrapper::Ray^ CameraManagerWrapper::From2DToWorld(float x, float y)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			DirectZobType::Ray r = c->From2DToWorld(x, y);
			return gcnew CameraManagerWrapper::Ray(&r.p, &r.n);
		}
		return nullptr;
	}

	bool CameraManagerWrapper::From2DToWorldOnPlane(float x, float y, ManagedVector3^ p0, ManagedVector3^ pn, ManagedVector3^ ret)
	{
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 cp0 = p0->ToVector3();
			ZobVector3 cpn = pn->ToVector3();
			ZobVector3 cret = ZobVector3(0,0,0);
			if (c->From2DToWorldOnPlane(x, y, &cp0, &cpn, &cret))
			{
				ret->x = cret.x;
				ret->y = cret.y;
				ret->z = cret.z;
				return true;
			}
		}
		return false;
	}

	void CameraManagerWrapper::SetCurrentCameraPosition(ManagedVector3^ p)
	{
		DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 v = p->ToVector3();
			c->SetWorldPosition(v.x, v.y, v.z);
		}
		DirectZob::GetInstance()->Unlock();
	}


	void CameraManagerWrapper::CreateEditorCamera()
	{
		m_Instance->CreateEditorCamera();
	}

	void CameraManagerWrapper::CreateCamera(System::String^ sType)
	{
		Camera::eCameraType type = Camera::eCamera_base;
		std::string sTypeC;
		MarshalString(sType, sTypeC);
		if (sTypeC == "Fixed")
		{
			type = Camera::eCamera_base;
		}
		else if (sTypeC == "Follow")
		{
			type = Camera::eCamera_follow;
		}
		else if (sTypeC == "FPS")
		{
			type = Camera::eCamera_fps;
		}
		else if (sTypeC == "Orbital")
		{
			type = Camera::eCamera_orbital;
		}
		m_Instance->CreateCamera(type, NULL);
	}

	void CameraManagerWrapper::SetCurrentCamera(System::String^ name)
	{
		//DirectZob::GetInstance()->Lock();
		std::string stdName;
		MarshalString(name, stdName);
		m_Instance->SetNextCamera(stdName);
		//DirectZob::GetInstance()->Unlock();
	}

	void CameraManagerWrapper::SetLookAt( ManagedVector3^ target)
	{
		//DirectZob::GetInstance()->Lock();
		Camera* c = m_Instance->GetCurrentCamera();
		if (c)
		{
			ZobVector3 t = target->ToVector3();
			c->SetTarget(&t);
		}
		//DirectZob::GetInstance()->Unlock();
	}
}
#endif //_WINDLL