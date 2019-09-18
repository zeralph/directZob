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
        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.EngineWrapper m_engineWrapper;
        private Thread m_engineThread;
        private Form1 m_mainForm;
        public delegate void UpdateEngineWindow();
        public delegate void UpdateLogWindow();
        public UpdateEngineWindow UpdateEngineWindowDelegate;
        
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
            m_directZobWrapper.RunAFrame();
            IntPtr p = m_engineWrapper.GetBufferData();
            m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);
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
                m_directZobWrapper.RunAFrame();
                EngineRender.Invoke(UpdateEngineWindowDelegate);

                //Application.DoEvents();
            }

            //Refresh();
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
                    m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)-dx, (float)dy);
                }
                else
                {
                    m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx*2.0f, (float)-dy*2.0f);
                }
            }
            else if(e.Button == MouseButtons.Middle)
            {
                m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx * 2.0f, (float)-dy * 2.0f);
            }
            else
            {
                CLI.ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetWrapper().GetObjectAtCoords(e.X, e.Y);
                if(z!=null && z.IsValid())
                {
                    string fullName = z.GetFullName();
                    m_mainForm.GetZobObjectControl().SetZobObjectWrapper(z);
                    //m_mainForm.GetZobObjectListControl().GetWrapper().
                }
            }
            m_lastMouseX = e.X;
            m_lastMouseY = e.Y;
        }

        private void EngineRender_MouseDown(object sender, MouseEventArgs e)
        {
            m_lastMouseX = e.X;
            m_lastMouseY = e.Y;
        }

        private void EngineRender_MouseHover(object sender, EventArgs e)
        {

        }
    }
}
