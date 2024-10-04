#include <wx/rawbmp.h>
#include <wx/time.h>
#include <wx/timer.h>
#include <wx/graphics.h>
#include <wx/fileconf.h>
#include "MainWindow.h"
#include "../DirectZobEngine/Events.h"

class ZobObject;
class ZobEditorManager;
class DirectZob;
class Inspector;
class ZobVariablesExposer;
class zobTreeItemData : public wxTreeItemData
{
public:
    zobTreeItemData();
    ~zobTreeItemData();
public:
    std::string zobIdStr;
};
class MainWindowInterface : public MainWindow
{
public:
                                MainWindowInterface(DirectZob * dz, ZobEditorManager* dze);
                                ~MainWindowInterface() override;
    static MainWindowInterface* GetInstance() { return m_singleton; }
    static void                 OnNewScene();
    static void                 AddLog(std::string& e);
    static void                 OnSceneLoaded();
    static void                 RefreshCamerasList();
    static void                 UpdateTreeView();
    static void                 SetCurrentZobVariables(ZobVariablesExposer* zvars);
    static void                 SetCurrentZobVariables(ZobObject* z);
    static void                 UpdateControls();
    static void                 OnInspectorUpdate(wxCommandEvent& event);
    void                        OnModificatorClick(wxCommandEvent& event) override;
    void                        OnMouseWheel(wxMouseEvent& event) override;
    void                        OnMouseDown(wxMouseEvent& event) override;
    void                        OnMouseUp(wxMouseEvent& event) override;
    void                        OnMotion(wxMouseEvent& event) override;
    void                        OnMouseClick(wxMouseEvent& event) override;
    void                        OnNew(wxCommandEvent& event) override;
    void                        OnOpen(wxCommandEvent& event) override;
    void                        OnSaveAs(wxCommandEvent& event) override;
    void                        OnSave(wxCommandEvent& event) override;
    void                        OnExit(wxCommandEvent& event) override;
    void                        OnTreeSelChanged(wxTreeEvent& event) override;
    void                        OnCameraSelected(wxCommandEvent& event) override;
    void                        OnMaximize(wxMaximizeEvent& event) override;
    void                        OnPlay(wxCommandEvent& event) override;
    void                        OnPause(wxCommandEvent& event) override;
    void                        OnStop(wxCommandEvent& event) override;
    void                        OnStartDrag(wxTreeEvent& event) override ;
    void                        OnEndDrag(wxTreeEvent& event) override;
    void                        OnPaint(wxPaintEvent& event);
    void                        OnTimer(wxTimerEvent& event);
    void                        OnIdle(wxIdleEvent& evt);
    void                        OnGizmosGrid(wxCommandEvent& event) override;
    void                        OnGizmosText(wxCommandEvent& event) override;
    void                        OnGizmosPhysics(wxCommandEvent& event) override;
    void                        OnGizmosObjects(wxCommandEvent& event) override;
    
//private:
    
    DirectZob* m_directZob;
    ZobEditorManager* m_editor;

private:
    enum eInspectorTabs
    {
        eInpector_object = 0,
        eInpector_engine = 1,
    };
    void                        MyThread();
    wxTreeItemId                SelectZobObjectInTree(wxTreeItemId root, std::string& zobIdStr);
    void                        RefreshObjectTree();
    void                        RenderAFrame();
    void                        DrawFrame();
    void                        BuildObjectTree(ZobObject* z, wxTreeItemId node);
    static                      MainWindowInterface* m_singleton;
    std::string                 m_dragSource;
 //   wxFont*                     m_logFont;
    Inspector*                  m_currentZobObjectInspector;
    Inspector*                  m_EngineInspector;
    Inspector*                  m_SceneInspector;
    wxBitmap                    m_bitmapBuffer;
    wxFileConfig                m_config;
    wxTimer                     m_timer;
    bool                        m_run;
    volatile bool               m_renderedAFrame;
};