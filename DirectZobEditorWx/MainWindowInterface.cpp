#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/wfstream.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/dc.h>
#include <wx/display.h>
#include <wx/fileconf.h>
#include <thread>
#include "MainWindowInterface.h"
#include "MainWindow.h"
#include "ZobEditorManager.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/ZobObjectManager.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../DirectZobEngine/Managers/CameraManager.h"
#include "../DirectZobEngine/Managers/LightManager.h"
#include "../DirectZobEngine/Types.h"
#include "Inspector.h"


MainWindowInterface* MainWindowInterface::m_singleton = NULL;

MainWindowInterface::MainWindowInterface(DirectZob* dz, ZobEditorManager* dze) : MainWindow(NULL, -1, "DirectZob editor", wxDefaultPosition, wxSize(958, 648), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
{
    wxString configDir = wxStandardPaths::Get().GetConfigDir();
    //m_config.;
    m_directZob = dz;
    m_editor = dze;
    m_singleton = this;
    m_currentZobObjectInspector = new Inspector(m_panelInspector);
    m_EngineInspector = new Inspector(m_panelEngine);
    m_EngineInspector->Set(DirectZob::GetInstance()->GetEngine()->GetVariablesExposer());
    m_SceneInspector = new Inspector(m_panelScene);
    m_SceneInspector->Set(DirectZob::GetInstance()->GetLightManager()->GetVariablesExposer());

#if WINDOWS
    m_renderPanel->SetDoubleBuffered(true);
#endif
    m_renderedAFrame = false;
    m_timer.SetOwner(this);
    m_timer.Start(16);
    this->Bind(wxEVT_TIMER, &MainWindowInterface::OnTimer, this);
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainWindowInterface::OnIdle));
    m_renderPanel->Bind(wxEVT_PAINT, &MainWindowInterface::OnPaint, this);
    wxDisplay display(wxDisplay::GetFromWindow(this));
    wxRect screen = display.GetGeometry();
    this->SetSize(50, 50, screen.width-100, screen.height-100);
    m_run = true;
    std::thread t = std::thread(&MainWindowInterface::MyThread, this);
    t.detach();
}

void MainWindowInterface::MyThread()
{
    while (m_run)
    {
        if (!m_renderedAFrame)
        {
            m_editor->UpdateEngine();
            RenderAFrame();
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
            m_renderedAFrame = true;
        }
    }
    m_directZob->Unload();
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
    }
}

void MainWindowInterface::OnTimer(wxTimerEvent& event)
{
    m_editor->UpdateInterface();
}

void MainWindowInterface::OnPaint(wxPaintEvent& event)
{
    DrawFrame();
    m_renderedAFrame = false;
}

void MainWindowInterface::OnIdle(wxIdleEvent& evt)
{
    if (m_renderedAFrame)
    {
        //m_editor->Update();
        m_renderPanel->Refresh();
        m_renderPanel->Update();
    }
}

void MainWindowInterface::DrawFrame()
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

MainWindowInterface::~MainWindowInterface()
{
    m_run = false;
    delete m_EngineInspector;
    delete m_currentZobObjectInspector;
}

void MainWindowInterface::SetCurrentZobVariables(ZobObject* z)
{
    m_singleton->m_currentZobObjectInspector->Set(z);
    //m_singleton->m_notebookInspector->SetSelection(eInspectorTabs::eInpector_engine);
    m_singleton->m_notebookInspector->SetSelection(eInspectorTabs::eInpector_object);
    //m_singleton->m_notebookInspector->Refresh();
}

void MainWindowInterface::SetCurrentZobVariables(ZobVariablesExposer* zvars)
{
    m_singleton->m_currentZobObjectInspector->Set(zvars);
    m_singleton->m_notebookInspector->ChangeSelection(eInspectorTabs::eInpector_object);
    m_singleton->m_notebookInspector->Refresh();
}

void MainWindowInterface::OnNewScene()
{
    m_singleton->m_currentZobObjectInspector->Clear();
    //m_singleton->m_notebookInspector->ChangeSelection(eInspectorTabs::eInpector_engine);
    m_singleton->m_notebookInspector->ChangeSelection(eInspectorTabs::eInpector_object);
    m_singleton->m_notebookInspector->Refresh();
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
    const std::vector<Camera*>* cameras = DirectZob::GetInstance()->GetCameraManager()->GetCameras();
    Camera* curCam = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
    int i = 0;
    int j = 0;
    std::vector<Camera*>::const_iterator iter;
    for (iter = cameras->begin(); iter != cameras->end(); iter++)
    {
        m_singleton->m_cameraSelector->Append((*iter)->GetName());
        if ((*iter)->GetName() == "EditorCamera")
        {
            j = i;
        }
        i++;
    }
    m_singleton->m_cameraSelector->SetSelection(j);
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
//        RefreshObjectTree();
    }
}

void MainWindowInterface::UpdateTreeView()
{
    m_singleton->RefreshObjectTree();
}

void MainWindowInterface::RefreshObjectTree()
{
    wxEventBlocker(m_singleton->m_treeNode);
    ZobObject* root = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
    m_singleton->m_treeNode->DeleteAllItems();
    wxTreeItemId rootNode = m_singleton->m_treeNode->AddRoot(root->GetName());
    zobTreeItemData* data = new zobTreeItemData();
    data->zobIdStr = root->ZobIdToString();
    m_singleton->m_treeNode->SetItemData(rootNode, data);
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
        zobId id = (*iter)->GetIdValue();
        ZobEntity::ZobType zt  =ZobEntity::GetType(id);
        if (zt!= ZobEntity::type_editor || ZobEditorManager::sShowEditorObjects)
        {
            wxTreeItemId newNode = m_treeNode->AppendItem(node, (*iter)->GetName());
            zobTreeItemData* data = new zobTreeItemData();
            data->zobIdStr = (*iter)->ZobIdToString();
            m_singleton->m_treeNode->SetItemData(newNode, data);
            BuildObjectTree(*iter, newNode);
        }
    }
 }

void MainWindowInterface::AddLog(std::string& s)
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
//   m_singleton->m_log->AppendText(s);
   //m_singleton->m_log->AppendText("\n");
}

void MainWindowInterface::UpdateControls()
{
    m_singleton->m_currentZobObjectInspector->UpdateControls();
    m_singleton->m_EngineInspector->UpdateControls();
}

void MainWindowInterface::OnMouseClick(wxMouseEvent& event)
{
    m_editor->OnMouseClick(event);
}

void MainWindowInterface::OnKeyDown(wxKeyEvent& event)
{
    bool handled = false;
    if (wxGetKeyState(WXK_CONTROL))
    {
        //Object additioon
        if(wxGetKeyState(WXK_SHIFT))
        {
            wxCommandEvent e;
            switch (event.GetKeyCode())
            {
            case 'M':
                OnMenuAddMesh(e);
                handled = true;
                break;
            case 'O':
                OnMenuAddObject(e);
                handled = true;
                break;
            case 'S':
                OnMenuAddSprite(e);
                handled = true;
                break;
            case 'A':
                OnMenuAddAsset(e);
                handled = true;
                break;
            case 'L':
                OnMenuAddLight(e);
                handled = true;
                break;
            case 'C':
                OnMenuAddCamera(e);
                handled = true;
                break;
            }
        }
    }
    else
    {
        switch (event.GetKeyCode())
        {
        case 'Z':
            m_editor->ZoomToSelected();
            handled = true;
            break;
        }
    }
    if (!handled)
    {
        event.Skip();
    }
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
    if(event.m_wheelRotation != 0)
    {
        float f = (float)event.m_wheelDelta / 40.0f * event.m_wheelRotation / abs(event.m_wheelRotation);
        DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->Zoom(f);
    }
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
    if(event.ButtonUp(wxMouseButton::wxMOUSE_BTN_RIGHT))
    {
        m_renderPanel->PopupMenu(m_treeMenu, wxDefaultPosition);
    }
}

void MainWindowInterface::OnMotion(wxMouseEvent& event)
{
    m_editor->SetLastMouseEvent(event);
    //RefreshObjectTree();
}

void MainWindowInterface::OnNew(wxCommandEvent& event) 
{ 
    m_editor->Unload();
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
    m_editor->Unload();
    m_directZob->Unload();
    m_directZob->LoadScene(p, f, MainWindowInterface::OnSceneLoaded, NULL);
}

void MainWindowInterface::OnSaveAs(wxCommandEvent& event) 
{ 
    wxFileDialog openFileDialog(this, _("DirectZob Scene"), "", "", "dzs files (*.dzs)|*.dzs", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    std::string p = std::string(openFileDialog.GetDirectory().mb_str());
    p.append("\\");
    std::string f = std::string(openFileDialog.GetFilename().mb_str());
    m_directZob->SaveScene(p, f);
}

void MainWindowInterface::OnSave(wxCommandEvent& event) 
{ 
    if (m_directZob->CanFastSave())
    {
        m_directZob->SaveScene();
    }
}

void MainWindowInterface::OnExit(wxCommandEvent& event) 
{ 
    m_run = false;
}

void MainWindowInterface::OnGizmosGrid(wxCommandEvent& event)
{
    bool b = DirectZob::GetInstance()->GetEngine()->ShowGrid();
    DirectZob::GetInstance()->GetEngine()->ShowGrid(!b);
}
void MainWindowInterface::OnGizmosText(wxCommandEvent& event)
{
    bool b = DirectZob::GetInstance()->GetEngine()->ShowText();
    DirectZob::GetInstance()->GetEngine()->ShowText(!b);
}
void MainWindowInterface::OnGizmosPhysics(wxCommandEvent& event)
{
    bool b = DirectZob::GetInstance()->GetEngine()->DrawPhysyicsGizmos();
    DirectZob::GetInstance()->GetEngine()->DrawPhysyicsGizmos(!b);
}
void MainWindowInterface::OnGizmosObjects(wxCommandEvent& event)
{
    bool b = DirectZob::GetInstance()->GetEngine()->DrawZobObjectGizmos();
    DirectZob::GetInstance()->GetEngine()->DrawZobObjectGizmos(!b);
}

void MainWindowInterface::OnTreeRightClick(wxTreeEvent& event)
{
    wxEventBlocker blocker(m_singleton->m_treeNode);
    wxTreeItemId t = event.GetItem();
    if (m_treeNode->GetSelection() != t)
    {
        m_treeNode->Unselect();
        m_treeNode->SelectItem(t);
    }
    m_treeNode->PopupMenu(m_treeMenu, wxDefaultPosition);// event.GetPosition());
}

void MainWindowInterface::OnMenuAddObject(wxCommandEvent& event)
{   
    //m_newObjectDialog
    m_editor->AddObject();
    RefreshObjectTree();
}
void MainWindowInterface::OnTreeMenuRemove(wxCommandEvent& event)
{
    /*		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		zobId zid = ZobEntity::ZobIdFromString(id);
		ZobObject* p = ZobEntity::GetEntity<ZobObject>(zid);
		if (m_selectedObject == p)
		{
			SelectObject(p->GetParent());
		}
		DirectZob::GetInstance()->GetZobObjectManager()->RemoveZobObject(p);
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);*/
}
void MainWindowInterface::OnTreeMenuAddComponent(wxCommandEvent& event)
{

}
void MainWindowInterface::OnTreeMenuZoom(wxCommandEvent& event)
{
    m_editor->ZoomToSelected();
}
void MainWindowInterface::OnTreeMenuDuplicate(wxCommandEvent& event)
{
    m_editor->Duplicate();
    RefreshObjectTree();
}
void MainWindowInterface::OnTreeMenuShowHide(wxCommandEvent& event)
{
    m_editor->ShowHide();
}
void MainWindowInterface::OnMenuAddMesh(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Mesh"), "", "", "gltf files (*.gltf)|*.gltf|obj files (*.obj)|*.obj|fbx files (*.fbx)|*.fbx", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    std::string p = std::string(openFileDialog.GetDirectory().mb_str());
    std::string f = std::string(openFileDialog.GetFilename().mb_str());
    m_editor->LoadMesh(p, f);
    RefreshObjectTree();
}
void MainWindowInterface::OnMenuAddSprite(wxCommandEvent& event)
{

}
void MainWindowInterface::OnMenuAddAsset(wxCommandEvent& event)
{

}
void MainWindowInterface::OnMenuAddLight(wxCommandEvent& event)
{
    m_editor->CreateLight();
    RefreshObjectTree();
}
void MainWindowInterface::OnMenuAddCamera(wxCommandEvent& event)
{
    m_editor->CreateCamera();
    RefreshObjectTree();
}