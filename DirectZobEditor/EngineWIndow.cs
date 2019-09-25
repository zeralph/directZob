using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class EngineWindow : UserControl
    {
        public event EventHandler OnBeginFrame;
        public event EventHandler OnEndFrame;
        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.EngineWrapper m_engineWrapper;
        private Thread m_engineThread;
        private Form1 m_mainForm;
        public delegate void UpdateEngineWindow();
        public delegate void UpdateLogWindow();
        public UpdateEngineWindow UpdateEngineWindowDelegate;
        private CLI.ZobObjectWrapper m_selectedObject = null;
        private string m_currentMouseAction = "";
        Graphics m_EngineGraphics = null;
        Bitmap m_engineBitmap = null;
        int m_width;
        int m_height;
        bool m_exiting = false;
        int m_lastMouseX = -1;
        int m_lastMouseY = -1;

        public EngineWindow(Form1 f, CLI.DirectZobWrapper directZobWrapper)
        {
            InitializeComponent();
            m_mainForm = f;
            m_directZobWrapper = directZobWrapper;
            m_engineWrapper = new CLI.EngineWrapper();
            m_width = m_engineWrapper.GetBufferWidth();
            m_height = m_engineWrapper.GetBufferHeight();
            EngineRender.Width = m_width;
            EngineRender.Height = m_height;
            m_EngineGraphics = EngineRender.CreateGraphics();
            m_engineThread = new Thread(RunEngineThread);
            m_engineThread.IsBackground = true;
            UpdateEngineWindowDelegate = new UpdateEngineWindow(UpdateEngineWindowMethod);

            m_engineThread.Start();
            if (OnBeginFrame != null)
            {
                OnBeginFrame(this, EventArgs.Empty);
            }
            m_directZobWrapper.RunAFrame();
            IntPtr p = m_engineWrapper.GetBufferData();
            m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);
            if (OnEndFrame != null)
            {
                OnEndFrame(this, EventArgs.Empty);
            }
        }

        public CLI.EngineWrapper GetEngineWrapper()
        {
            return m_engineWrapper;
        }
        private void UpdateEngineWindowMethod()
        {
            //if(m_directZobWrapper.RunAFrame() >= 0)
            {
                IntPtr p = m_engineWrapper.GetBufferData();
                Rectangle rect = new Rectangle(0, 0, m_engineBitmap.Width, m_engineBitmap.Height);
                System.Drawing.Imaging.BitmapData bmpData =
                    m_engineBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly,
                    m_engineBitmap.PixelFormat);
                bmpData.Scan0 = p;
                m_engineBitmap.UnlockBits(bmpData);
                IntPtr hwnd = EngineRender.Handle;
                m_EngineGraphics = Graphics.FromHwnd(hwnd);
                m_EngineGraphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                m_EngineGraphics.DrawImage(m_engineBitmap, 0, 0, EngineRender.Width, EngineRender.Height);
            }
            m_mainForm.UpdateAfterEngine();
        }

        private void RunEngineThread()
        {
            while (!m_exiting)
            {
                if (OnBeginFrame != null)
                {
                    OnBeginFrame(this, EventArgs.Empty);
                }
                m_directZobWrapper.RunAFrame();
                EngineRender.Invoke(UpdateEngineWindowDelegate);
                if (OnEndFrame != null)
                {
                    OnEndFrame(this, EventArgs.Empty);
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_exiting = true;
            Thread.Sleep(100);
            //m_engineThread.Abort();
            //Application.DoEvents();
        }

        private void UpdateCamera()
        {

        }

        private void EngineRender_MouseWheel(object sender, MouseEventArgs e)
        {
            m_mainForm.GetCameraControl().GetWrapper().Zoom(e.Delta/50.0f);
        }

        private void EngineRender_MouseMove(object sender, MouseEventArgs e)
        {
            int dx = m_lastMouseX - e.X;
            int dy = m_lastMouseY - e.Y;
            if (e.Button == MouseButtons.Left)
            {
                if (!m_mainForm.IsCtrlPressed())
                {
                    //MOVE OBJECT HERE
                    if (m_selectedObject != null && m_currentMouseAction.Length != 0)
                    {
                        CLI.ManagedVector3 p = m_selectedObject.GetTransform();
                        switch (m_currentMouseAction)
                        {
                            case "transformY":
                                p.y += dy;
                                break;
                            default:
                                break;
                        }
                        m_selectedObject.SetTransform(p);
                    }
                }
                else
                {
                    m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)-dx, (float)dy);
                    //m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx*2.0f, (float)-dy*2.0f);
                }
            }
            else if(e.Button == MouseButtons.Middle)
            {
                m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx * 2.0f, (float)-dy * 2.0f);
            }
            else
            {

            }
            m_lastMouseX = e.X;
            m_lastMouseY = e.Y;
        }

        private void EngineRender_MouseHover(object sender, EventArgs e)
        {

        }

        private void EngineRender_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && !m_mainForm.IsCtrlPressed())
            {
                m_selectedObject = m_mainForm.GetZobObjectListControl().SelectObjectAtCoords(e.X, e.Y, CLI.ZobObjectManagerWrapper.eObjectTypes.eObjectTypes_scene);
            }
        }

        private void TRSButton_Click(object sender, EventArgs e)
        {
            ToolStripButton b = (ToolStripButton)sender;
            //if(b.Name == )
        }

        private void EngineRender_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && !m_mainForm.IsCtrlPressed())
            {
                if (m_selectedObject == null)
                {
                    m_selectedObject = m_mainForm.GetZobObjectListControl().SelectObjectAtCoords(e.X, e.Y, CLI.ZobObjectManagerWrapper.eObjectTypes.eObjectTypes_scene);
                }
                if (m_selectedObject != null && m_currentMouseAction.Length == 0)
                {
                    CLI.ZobObjectWrapper zAction = m_mainForm.GetZobObjectListControl().GetObjectAtCoords(e.X, e.Y, CLI.ZobObjectManagerWrapper.eObjectTypes.eObjectTypes_editor);
                    if (zAction != null)
                    {
                        m_currentMouseAction = zAction.GetName();
                    }
                }
            }
        }

        private void EngineRender_MouseUp(object sender, MouseEventArgs e)
        {
            m_currentMouseAction = "";
        }
    }
}
