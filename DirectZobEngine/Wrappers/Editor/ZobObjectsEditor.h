#ifdef _WINDLL
#pragma once
#include "../ManagedObject.h"
#include "../../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../../DirectZob.h"
#include <string>
#include <vector>

#define EDITOR_OBJECT "Editor_object"
#define EDITOR_ARROW_X "Editor_arrow_x"
#define EDITOR_ARROW_Y "Editor_arrow_y"
#define EDITOR_ARROW_Z "Editor_arrow_z"

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
		void Select(ZobObject* z);
		void UnSelect();
	private:
		zobId m_gizmosGuid;
		ZobObject* m_editorRoot;
		ZobObject* m_translateX;
		ZobObject* m_translateY;
		ZobObject* m_translateZ;
		ZobBehaviorMesh* m_behaviorTranslateX;
		ZobBehaviorMesh* m_behaviorTranslateY;
		ZobBehaviorMesh* m_behaviorTranslateZ;
		ZobObject* m_currentSelectedGizmo;
		static string gArrow;
	};
}
#endif //_WINDLL

