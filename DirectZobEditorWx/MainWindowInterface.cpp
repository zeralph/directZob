#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/wfstream.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/dc.h>
#include <wx/display.h>
#include <wx/fileconf.h>
#include "MainWindowInterface.h"
#include "MainWindow.h"
#include "ZobEditorManager.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/ZobObjectManager.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../DirectZobEngine/Managers/CameraManager.h"
#include "../DirectZobEngine/Types.h"
#include "Inspector.h"

MainWindowInterface* MainWindowInterface::m_singleton = NULL;

zobTreeItemData::zobTreeItemData() 
{
    zobIdStr = "";
}
zobTreeItemData::~zobTreeItemData() 
{
    zobIdStr = "";
}

MainWindowInterface::MainWindowInterface(DirectZob* dz, ZobEditorManager* dze) : MainWindow(NULL, -1, "DirectZob editor", wxDefaultPosition, wxSize(958, 648), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
{
    //wxFileConfig* conf = new wxFileConfig()
    wxString configDir = wxStandardPaths::Get().GetConfigDir();
    m_directZob = dz;
    m_editor = dze;
    m_singleton = this;
//    m_logFont = new wxFont(8, wxSCRIPT, wxNORMAL, wxNORMAL);
//    m_singleton->m_log->SetFont(*m_logFont);
    m_currentZobObjectInspector = new Inspector(m_panelInspector);
 //   m_renderPanel->SetDoubleBuffered(true);
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainWindowInterface::OnIdle));
    m_renderPanel->Bind(wxEVT_PAINT, &MainWindowInterface::OnPaint, this);
    m_timer.SetOwner(this);
    m_timer.Start(16);
    this->Bind(wxEVT_TIMER, &MainWindowInterface::OnTimer, this);
    wxDisplay display(wxDisplay::GetFromWindow(this));
    wxRect screen = display.GetGeometry();
    this->SetSize(50, 50, screen.width-100, screen.height-100);
    //this->ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL)  
}

MainWindowInterface::~MainWindowInterface()
{
    m_timer.Stop();
    delete m_currentZobObjectInspector;
}

void MainWindowInterface::SetCurrentZobVariables(ZobVariablesExposer* zvars)
{
    m_singleton->m_currentZobObjectInspector->Set(zvars);
}

void MainWindowInterface::OnNewScene()
{
    m_singleton->m_currentZobObjectInspector->Clear();
}

void MainWindowInterface::OnPlay(wxCommandEvent& event)
{
    m_Pause->SetValue(0);
    m_Stop->SetValue(0);
    m_Play->SetValue(1);
    m_directZob->StartPhysic();
}

void MainWindowInterface::OnPause(wxCommandEvent& event)
{
    m_Pause->SetValue(1);
    m_Stop->SetValue(0);
    m_Play->SetValue(0);
    m_directZob->StopPhysic(0);
}

void MainWindowInterface::OnInspectorUpdate(wxCommandEvent& event)
{
    
}

void MainWindowInterface::OnStop(wxCommandEvent& event)
{
    m_Pause->SetValue(0);
    m_Stop->SetValue(1);
    m_Play->SetValue(0);
    m_directZob->StopPhysic(1);
}

void MainWindowInterface::OnMaximize(wxMaximizeEvent& event)
{
    //m_splitter3->SetSashSize(-250);
    //m_panelInspector->SetSize(250, -1);
    //wxSize toto = m_panelInspector->GetSize();
}

void MainWindowInterface::OnSceneLoaded()
{
    m_singleton->m_editor->OnNewScene();
    m_singleton->RefreshObjectTree();
}

void MainWindowInterface::OnCameraSelected(wxCommandEvent& event) 
{ 
    //RefreshCamerasList();
    std::string cam = std::string(m_singleton->m_cameraSelector->GetStringSelection().mb_str());
    DirectZob::GetInstance()->GetCameraManager()->SetNextCamera(cam);
}

void MainWindowInterface::RefreshCamerasList()
{
    m_singleton->m_cameraSelector->Clear();
    m_singleton->m_cameraSelector->Append("AA");
    const std::vector<Camera*>* cameras = DirectZob::GetInstance()->GetCameraManager()->GetCameras();
    std::vector<Camera*>::const_iterator iter = cameras->begin();
    for (iter; iter != cameras->end(); iter++)
    {
        m_singleton->m_cameraSelector->Append((*iter)->GetName());
    }
}

void MainWindowInterface::OnTreeSelChanged(wxTreeEvent& event)
{
    zobTreeItemData* data = static_cast<zobTreeItemData*>(m_treeNode->GetItemData(event.GetItem()));
    if (data)
    {
        zobId id = ZobEntity::ZobIdFromString(data->zobIdStr);
        ZobObject* z = ZobEntity::GetEntity<ZobObject>(id);
        DirectZob::LogInfo("Selected %s", z->GetName().c_str());
        m_editor->SetSelectedObject(z);
        RefreshObjectTree();
    }
}

void MainWindowInterface::UpdateTreeView()
{
    m_singleton->RefreshObjectTree();
}

void MainWindowInterface::RefreshObjectTree()
{
    ZobObject* root = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
    m_singleton->m_treeNode->DeleteAllItems();
    wxTreeItemId rootNode = m_singleton->m_treeNode->AddRoot(root->GetName());
    m_singleton->BuildObjectTree(root, rootNode);
    m_singleton->m_treeNode->ExpandAll();
    ZobObject* z = m_editor->GetSelectedObject();
    if (z)
    {
        std::string id = z->ZobIdToString();
        wxTreeItemId item = SelectZobObjectInTree(m_singleton->m_treeNode->GetRootItem(), id);
        if (item.IsOk())
        {
            wxEventBlocker blocker(m_singleton->m_treeNode);
            m_singleton->m_treeNode->Unselect();
            m_singleton->m_treeNode->SelectItem(item);
        }
    }
}

void MainWindowInterface::OnStartDrag(wxTreeEvent& event)
{
    event.Allow();
    wxTreeItemId item = event.GetItem();
    if (item.IsOk())
    {
        zobTreeItemData* data = static_cast<zobTreeItemData*>(m_treeNode->GetItemData(item));
        m_dragSource = data ? data->zobIdStr : "";
    }
}

void MainWindowInterface::OnEndDrag(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    if (item.IsOk())
    {
        zobTreeItemData* data = static_cast<zobTreeItemData*>(m_treeNode->GetItemData(item));
        std::string dragTo = data ? data->zobIdStr : "";
        if (!dragTo.empty() && !m_dragSource.empty())
        {
            ZobObject* source = ZobEntity::GetEntity<ZobObject>(ZobEntity::ZobIdFromString(m_dragSource));
            ZobObject* newParent = ZobEntity::GetEntity<ZobObject>(ZobEntity::ZobIdFromString(dragTo));
            DirectZob::GetInstance()->GetZobObjectManager()->Reparent(source, newParent);
            DirectZob::LogInfo("Re-parented '%s' under '%s'", source->GetName().c_str(), newParent->GetName().c_str());
            RefreshObjectTree();
        }
    }
}

wxTreeItemId MainWindowInterface::SelectZobObjectInTree(wxTreeItemId root, std::string& zobIdStr)
{
    wxTreeItemId item = root;
    wxTreeItemId child;
    wxTreeItemIdValue cookie;
    while (item.IsOk())
    {
        zobTreeItemData* data = static_cast<zobTreeItemData*>(m_treeNode->GetItemData(item));
        if (data && data->zobIdStr == zobIdStr)
        {
            return item;
        }
        child = m_treeNode->GetFirstChild(item, cookie);
        if (child.IsOk()) child = SelectZobObjectInTree( child, zobIdStr);
        if (child.IsOk()) return child;
        item = m_treeNode->GetNextSibling(item);
    } // while(item.IsOk())
    return item;
}

void MainWindowInterface::BuildObjectTree(ZobObject* z, wxTreeItemId node)
{  
    for (std::vector<ZobObject*>::const_iterator iter = z->GetChildren()->begin(); iter != z->GetChildren()->end(); iter++ )
    {
        wxTreeItemId newNode = m_treeNode->AppendItem(node, (*iter)->GetName());
        zobTreeItemData* data = new zobTreeItemData();
        data->zobIdStr = (*iter)->ZobIdToString();
        m_singleton->m_treeNode->SetItemData(newNode, data);
        BuildObjectTree(*iter, newNode);
    }
 }

void MainWindowInterface::AddLog(const Events::event& e)
{
    /*
    switch (e.t)
    {
    case Events::LogError:
        m_singleton->m_log->BeginTextColour(wxColour(255, 0, 0));
        break;
    case Events::LogWarning:
        m_singleton->m_log->BeginTextColour(wxColour(128, 128, 0));
        break;
    default:
        m_singleton->m_log->BeginTextColour(wxColour(0, 0, 200));
        break;
    } 
    m_singleton->m_log->WriteText(e.m);
    m_singleton->m_log->Newline();
    m_singleton->m_log->ScrollIntoView(m_singleton->m_log->GetCaretPosition(), WXK_PAGEDOWN);
    */
   m_singleton->m_log->AppendText(e.m);
   m_singleton->m_log->AppendText("\n");
}

void MainWindowInterface::UpdateControls()
{
    m_singleton->m_currentZobObjectInspector->UpdateControls();
}

void MainWindowInterface::OnMouseClick(wxMouseEvent& event)
{
    m_editor->OnMouseClick(event);
}

void MainWindowInterface::OnModificatorClick(wxCommandEvent& event)
{
    wxToggleButton* b = (wxToggleButton*)event.GetEventObject();
    std::string s = std::string(b->GetLabel().c_str());
    if (s == "WT")
    {
        m_WT->SetValue(true);
        m_WR->SetValue(false);
        m_LT->SetValue(false);
        m_LR->SetValue(false);
        m_editor->SetObjectModificator(ZobEditorManager::eGizmo_translate, ZobEditorManager::space_world);
    }
    else if (s == "LT")
    {
        m_WT->SetValue(false);
        m_WR->SetValue(false);
        m_LT->SetValue(true);
        m_LR->SetValue(false);
        m_editor->SetObjectModificator(ZobEditorManager::eGizmo_translate, ZobEditorManager::space_local);
    }
    else if (s == "WR")
    {
        m_WT->SetValue(false);
        m_WR->SetValue(true);
        m_LT->SetValue(false);
        m_LR->SetValue(false);
        m_editor->SetObjectModificator(ZobEditorManager::eGizmo_rotate, ZobEditorManager::space_world);
    }
    else if (s == "LR")
    {
        m_WT->SetValue(false);
        m_WR->SetValue(false);
        m_LT->SetValue(false);
        m_LR->SetValue(true);
        m_editor->SetObjectModificator(ZobEditorManager::eGizmo_rotate, ZobEditorManager::space_local);
    }
}

void MainWindowInterface::OnMouseWheel(wxMouseEvent& event)
{ 
    float f = (float)event.m_wheelDelta / 40.0f * event.m_wheelRotation / abs(event.m_wheelRotation);
    DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->Zoom(f);
}

void MainWindowInterface::OnMouseDown(wxMouseEvent& event)
{
    m_editor->OnMouseDown(event);
    //m_mouseDx = event.GetX();
    //m_mouseDy = event.GetY();
    //DirectZob::LogError("-> %f, %f", m_mouseDx, m_mouseDy);
}

void MainWindowInterface::OnMouseUp(wxMouseEvent& event)
{
    m_editor->OnMouseUp(event);
    //m_mouseDx = event.GetX();
    //m_mouseDy = event.GetY();
    //DirectZob::LogError("-> %f, %f", m_mouseDx, m_mouseDy);
}

void MainWindowInterface::OnMotion(wxMouseEvent& event)
{
    m_editor->SetLastMouseEvent(event);
    //RefreshObjectTree();
}

void MainWindowInterface::OnNew(wxCommandEvent& event) 
{ 
    m_directZob->Unload();
//    m_singleton->m_log->Clear();
    m_directZob->NewScene("");
    //m_singleton->m_editor->OnNewScene();
    OnSceneLoaded();
}

void MainWindowInterface::OnOpen(wxCommandEvent& event) 
{ 
    wxFileDialog openFileDialog(this, _("DirectZob Scene"), "", "", "dzs files (*.dzs)|*.dzs", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    std::string p = std::string(openFileDialog.GetDirectory().mb_str());
    std::string f = std::string(openFileDialog.GetFilename().mb_str());
    m_directZob->LoadScene(p, f, MainWindowInterface::OnSceneLoaded, NULL);
}

void MainWindowInterface::OnSaveAs(wxCommandEvent& event) 
{ 
    event.Skip(); 
}

void MainWindowInterface::OnSave(wxCommandEvent& event) 
{ 
    event.Skip(); 
}

void MainWindowInterface::OnExit(wxCommandEvent& event) 
{ 
    
}

void MainWindowInterface::OnTimer(wxTimerEvent& event)
{
    RenderAFrame();
}

void MainWindowInterface::OnPaint(wxPaintEvent& event)
{
#ifdef LINUX
	wxPaintDC  dc(m_renderPanel);
#elif WINDOWS
	wxBufferedPaintDC  dc(m_renderPanel);
#elif MACOS
	wxPaintDC  dc(m_renderPanel);
#endif

    if (m_bitmapBuffer.IsOk())
    {
        dc.DrawBitmap(m_bitmapBuffer, 0, 0);
    }
}

void MainWindowInterface::OnIdle(wxIdleEvent& evt)
{
    if (0)
    {
        RenderAFrame();
        m_renderPanel->Refresh();
        m_renderPanel->Update();
    }
}


void MainWindowInterface::RenderAFrame()
{
    int w, h;
    m_renderPanel->GetClientSize(&w, &h);
    m_directZob->Resize(w, h);
    if (m_directZob->GetEngine()->Started())
    {

        m_directZob->RunAFrame();
        m_directZob->EditorUpdate();
        m_editor->Update();
    }
    const uint* buffer = m_directZob->GetBufferData();
    int width = m_directZob->GetBufferWidth();
    int height = m_directZob->GetBufferHeight();
    uint* engineData = m_directZob->GetEditorBufferDataNoConst();
    // Now transfer the pixel data into a wxBitmap
    wxBitmap b(width, height, 24);
    wxNativePixelData data(b);
    if (!data)
    {
        return;
    }
    wxNativePixelData::Iterator p(data);
    int curPixelDataLoc = 0;
    for (int y = 0; y < height; ++y)
    {
        wxNativePixelData::Iterator rowStart = p;
        for (int x = 0; x < width; ++x, ++p)
        {
            uint c = engineData[curPixelDataLoc];
            if (c != 0)
            {
                int tt = 0;
                tt++;
            }
            uint r = (c & 0xFF0000) >> 16;
            uint g = (c & 0x00FF00) >> 8;
            uint b = (c & 0x0000FF) >> 0;
            p.Red() = r;// 
            p.Green() = g;// c & 0x00FF00 >> 8;
            p.Blue() = b;// c & 0x0000FF;
            curPixelDataLoc++;
        }
        p = rowStart;
        p.OffsetY(data, 1);
    }
    m_bitmapBuffer = b;
    m_renderPanel->Refresh();
    m_renderPanel->Update();
}