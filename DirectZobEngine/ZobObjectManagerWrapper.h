#pragma once
#include "ManagedObject.h"
#include "ZobObjectManager.h"
#include "ZobObjectInterface.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public ref class ZobObjectManagerWrapper : public ManagedObject<ZobObjectManager>
	{
	public:

		ZobObjectManagerWrapper();

		System::String^ GetZobObjectList();
		ZobObjectInterface^ GetZobObject(System::String^ name);
		void RemoveZobObject(System::String^ name);
		void AddZobObject(System::String^ parent);

	private:

	};
}