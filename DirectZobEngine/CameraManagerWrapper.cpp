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
		int l = data.size();
		array<System::String ^>^ arr = gcnew array<System::String ^>(l);
		for (int i = 0; i < l; i++)
		{
			arr[i] = gcnew System::String(data.at(i).c_str());
		}
		return arr;
	}

	System::String^ CameraManagerWrapper::GetCurrentCamera()
	{
		return gcnew System::String(m_Instance->GetCurrentCamera()->GetName().c_str());
	}

	ManagedVector3^ CameraManagerWrapper::GetCurrentCameraPosition()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetPosition());
		return v;
	}

	ManagedVector3^ CameraManagerWrapper::GetCurrentCameraTarget()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetCurrentCamera()->GetTarget());
		return v;
	}

	void CameraManagerWrapper::RotateAroundAxis(float x, float y)
	{
		m_Instance->GetCurrentCamera()->RotateAroundAxis(x, y);
	}

	void CameraManagerWrapper::Move(float x, float y)
	{
		m_Instance->GetCurrentCamera()->Move(x, y);
	}

	void CameraManagerWrapper::Zoom(float z)
	{
		m_Instance->GetCurrentCamera()->Zoom(z);
	}

	void CameraManagerWrapper::SetCurrentCameraPosition(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->GetCurrentCamera()->SetPosition(&v);
	}
}
#endif //_WINDLL