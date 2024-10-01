

#include <wx/wx.h>
#include <thread> 
#include <mutex>
#include "MainWindowInterface.h"
#include "ZobEditorManager.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/CameraManager.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override;
    int OnExit() override;
    void UpdateEngine();

private:
    void RenderAFrame();
    DirectZob* m_directZob;
    std::thread m_engineThread;
    MainWindowInterface* m_mainWindow;
    ZobEditorManager* m_editor;
};

wxIMPLEMENT_APP(MyApp);

enum
{
    ID_Hello = 1
};

int MyApp::OnExit()
{
    m_directZob->Unload();
    delete m_directZob;
    delete m_editor;
    return 0;
}

bool MyApp::OnInit()
{
    m_directZob = new DirectZob();
    m_directZob->Init(0, 640, 480, true);
    m_editor = new ZobEditorManager();
    m_mainWindow = new MainWindowInterface(m_directZob, m_editor);
    wxCommandEvent e;
    m_mainWindow->OnNew(e);
    m_mainWindow->Show(true);
    return true;
}

void MyApp::UpdateEngine()
{
    //m_directZob->RunAFrame();
    //m_directZob->EditorUpdate();
    //const uint* buffer = m_directZob->GetBufferData();
    //m_mainWindow->m_renderPanel->OnPaint()
}