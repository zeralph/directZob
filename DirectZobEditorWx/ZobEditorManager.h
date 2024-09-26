#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/CameraManager.h"



class ZobEditorManager
{

public:
	ZobEditorManager(); 
	~ZobEditorManager();

	void Update();
	void UpdateLog();
	void OnNewScene();
	void DrawGrid();
	void Scale(float s);

private:
	ZobObject* m_editorRootNode;
	ZobObject* m_gizmosNode;
	ZobObject* m_translateX;
	ZobComponentMesh* m_componentTranslateX;
	ZobObject* m_translateY;
	ZobComponentMesh* m_componentTranslateY;
	ZobObject* m_translateZ;
	ZobComponentMesh* m_componentTranslateZ;
	ZobObject* m_rotateX;
	ZobComponentMesh* m_componentRotateX;
	ZobObject* m_rotateY;
	ZobComponentMesh* m_componentRotateY;
	ZobObject* m_rotateZ;
	ZobComponentMesh* m_componentRotateZ;
	static char* gArrow;
	static long gArrowLen;
	static char* gCircle;
	static long gCircleLen;
};