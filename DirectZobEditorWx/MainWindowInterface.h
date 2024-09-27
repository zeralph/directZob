#include <wx/rawbmp.h>
#include "MainWindow.h"
#include "../DirectZobEngine/Events.h"

class ZobObject;
class ZobEditorManager;
class DirectZob;
class zobTreeItemData : public wxTreeItemData
{
public:
    std::string id;
};
class MainWindowInterface : public MainWindow
{
public:
                                MainWindowInterface(DirectZob * dz, ZobEditorManager* dze);
                                ~MainWindowInterface();
    void                        MyOnPaint(wxPaintEvent& event);
    static void                 AddLog(const Events::event& e);
    static void                 OnSceneLoaded();
    static void                 RefreshCamerasList();
    void                        OnMouseWheel(wxMouseEvent& event) override;
    void                        OnLeftDown(wxMouseEvent& event) override;
    void                        OnMiddleDown(wxMouseEvent& event) override;
    void                        OnMotion(wxMouseEvent& event) override;
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
//private:

    wxBitmap m_bitmapBuffer;
    DirectZob* m_directZob;
    ZobEditorManager* m_editor;

private:
    void                        BuildObjectTree(ZobObject* z, wxTreeItemId& node);
    static MainWindowInterface* m_singleton;
    wxFont* m_logFont;
};