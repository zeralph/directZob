#ifdef _WINDLL
#pragma once
#include "../ManagedObject.h"
#include "../../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../../DirectZob.h"
#include <string>
#include <vector>

using namespace System;
using namespace System::Collections::Generic;

namespace CLI
{
	public class ZobObjectsEditor
	{
	public:
		ZobObjectsEditor();
		~ZobObjectsEditor();
		void AddEditorGizmos();
		void SetParent(ZobObject* z);

	private:
		zobId m_gizmosGuid;
		ZobObject* m_translateX;
		ZobObject* m_translateY;
		ZobObject* m_translateZ;
		static string gArrow;
	};
}
#endif //_WINDLL

