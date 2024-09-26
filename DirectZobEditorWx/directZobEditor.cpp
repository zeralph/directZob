

#include <wx/wx.h>
#include <wx/mstream.h>
#include <thread> 
#include <mutex>
#include <wx/rawbmp.h>
#include "MainWindowInterface.h"
#include "ZobEditorManager.h"
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/CameraManager.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override;
    void UpdateEngine();
    void onIdle(wxIdleEvent& evt);

private:
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

bool MyApp::OnInit()
{
    m_directZob = new DirectZob();
    m_directZob->Init(0, 640, 480, true);
    m_editor = new ZobEditorManager();
    m_mainWindow = new MainWindowInterface(m_directZob, m_editor);
    wxCommandEvent e;
    m_mainWindow->OnNew(e);
    m_mainWindow->Show(true);
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
    return true;
}


void MyApp::onIdle(wxIdleEvent& evt)
{
    if (1)
    {
        int w, h;
        m_mainWindow->m_renderPanel->GetClientSize(&w, &h);
        //w = 640;
        //h = 480;
        m_directZob->Resize(w, h);
        if (m_directZob->GetEngine()->Started())
        {
            m_editor->Update();
            m_directZob->RunAFrame();
            m_directZob->EditorUpdate();
        }
        const uint* buffer = m_directZob->GetBufferData();
        //m_mainWindow->m_renderPanel->paintNow();
        wxClientDC dc(m_mainWindow->m_renderPanel);
        int width = m_directZob->GetBufferWidth();
        int height = m_directZob->GetBufferHeight();
        uint* engineData = m_directZob->GetEditorBufferDataNoConst();


        // Now transfer the pixel data into a wxBitmap
        wxBitmap b(width, height, 24);
        wxNativePixelData data(b);
        if (!data)
        {
            // ... raw access to bitmap data unavailable, do something else ...
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
        m_mainWindow->m_bitmapBuffer = b;
        dc.DrawBitmap(m_mainWindow->m_bitmapBuffer, 0, 0);
        //m_mainWindow->m_renderPanel->Refresh();
        m_mainWindow->m_renderPanel->Update();
        evt.RequestMore(); // render continuously, not only once on idle
    }
}

void MyApp::UpdateEngine()
{
    //m_directZob->RunAFrame();
    //m_directZob->EditorUpdate();
    //const uint* buffer = m_directZob->GetBufferData();
    //m_mainWindow->m_renderPanel->OnPaint()
}