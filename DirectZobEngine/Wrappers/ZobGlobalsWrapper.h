#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../ZobObjects/ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"
#include "ZobUserControls/ZobUserControls.h"
#include "../Behaviors/ZobBehavior.h"
#include "../ZobObjects/ZobObject.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace CLI {

	public ref class ZobGlobalsWrapper
	{
	public:
		ZobGlobalsWrapper(Panel^ parentPanel);
		~ZobGlobalsWrapper() override;
	private:
		void					CreateGlobalView();
		ZobGroupBox^			FillGlobalsControl();
		ZobGroupBox^			m_globalsPanel;
		ZobPropertiesContainer^ m_container;
		Panel^					m_parentPanel;
	protected:

	};
}
#endif //_WINDLL 