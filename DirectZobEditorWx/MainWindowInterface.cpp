#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include "MainWindowInterface.h"
#include "MainWindow.h"
#include "ZobEditorManager.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/ZobObjectManager.h"
#include "../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../DirectZobEngine/Managers/CameraManager.h"
#include "../DirectZobEngine/Types.h"


MainWindowInterface* MainWindowInterface::m_singleton = NULL;

MainWindowInterface::MainWindowInterface(DirectZob* dz, ZobEditorManager* dze) : MainWindow(NULL, -1, "DirectZob editor", wxDefaultPosition, wxSize(958, 648), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
{
    m_directZob = dz;
    m_editor = dze;
    m_singleton = this;
}

void MainWindowInterface::MyOnPaint(wxPaintEvent& event)
{
    /*wxPaintDC dc(m_renderPanel);
    if (m_bitmapBuffer.IsOk())
    {
        dc.DrawBitmap(m_bitmapBuffer, 0, 0);
    }
    */
}

void MainWindowInterface::OnSceneLoaded()
{
    m_singleton->m_editor->OnNewScene();
    ZobObject* root = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
    m_singleton->m_treeNode->DeleteAllItems();
    wxTreeItemId node = m_singleton->m_treeNode->AddRoot(root->GetName());
    m_singleton->BuildObjectTree(root, node);
    m_singleton->m_treeNode->Expand(node);
}

void MainWindowInterface::OnCameraSelected(wxCommandEvent& event) 
{ 
    //RefreshCamerasList();
    std::string cam = m_singleton->m_cameraSelector->GetStringSelection();
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
        zobId id = ZobEntity::ZobIdFromString(data->id);
        ZobObject* z = ZobEntity::GetEntity<ZobObject>(id);
        DirectZob::LogInfo("Selected %s", z->GetName().c_str());
        m_editor->SetSelectedObject(z);
    }
}

void MainWindowInterface::BuildObjectTree(ZobObject* z, wxTreeItemId& node)
{  
    for (std::vector<ZobObject*>::const_iterator iter = z->GetChildren()->begin(); iter != z->GetChildren()->end(); iter++ )
    {
        wxTreeItemId newNode = m_treeNode->AppendItem(node, (*iter)->GetName());
        zobTreeItemData* data = new zobTreeItemData();
        data->id = (*iter)->ZobIdToString();
        m_singleton->m_treeNode->SetItemData(newNode, data);
        BuildObjectTree(*iter, newNode);
    }
 }

void MainWindowInterface::AddLog(const Events::event& e)
{
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
    //wxFont font(8, wxTELETYPE, wxNORMAL, wxNORMAL);
    //m_singleton->m_log->SetFont(font);
    m_singleton->m_log->WriteText(e.m);
    m_singleton->m_log->Newline();
    m_singleton->m_log->ScrollIntoView(m_singleton->m_log->GetCaretPosition(), WXK_PAGEDOWN);
}

void MainWindowInterface::OnMouseWheel(wxMouseEvent& event)
{ 
    float f = (float)event.m_wheelDelta / 40.0f * event.m_wheelRotation / abs(event.m_wheelRotation);
    //DirectZob::LogError("-> %i", event.m_wheelRotation);
    DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->Zoom(f);
}

void MainWindowInterface::OnLeftDown(wxMouseEvent& event)
{
    m_editor->ResetMousePos(event);
    //m_mouseDx = event.GetX();
    //m_mouseDy = event.GetY();
    //DirectZob::LogError("-> %f, %f", m_mouseDx, m_mouseDy);
}

void MainWindowInterface::OnMiddleDown(wxMouseEvent& event)
{
    m_editor->ResetMousePos(event);
    //m_mouseDx = event.GetX();
    //m_mouseDy = event.GetY();
    //DirectZob::LogError("-> %f, %f", m_mouseDx, m_mouseDy);
}

void MainWindowInterface::OnMotion(wxMouseEvent& event)
{
    m_editor->SetLastMouseEvent(event);
}

void MainWindowInterface::OnNew(wxCommandEvent& event) 
{ 
    m_directZob->Unload();
    m_singleton->m_log->Clear();
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
    std::string p = openFileDialog.GetDirectory();
    std::string f = openFileDialog.GetFilename();
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
    event.Skip(); 
}