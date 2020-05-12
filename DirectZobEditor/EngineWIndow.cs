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
using CLI;
using System.Runtime.CompilerServices;

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
            this.Dock = DockStyle.Fill;
            m_mainForm = f;
            m_directZobWrapper = directZobWrapper;
            m_engineWrapper = new CLI.EngineWrapper();
            GenerateRenderWindow();
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
        }

        public void RemoveTranslationGizmos()
        {
            bTY.Visible = false;
            bTX.Visible = false;
            bTZ.Visible = false;
        }

        public void SetTranslationGizmos(ZobObjectWrapper z)
        {
            if (z != null && z.IsValid())
            {       
                ManagedVector3 p0 = z.GetTransform();
                ManagedVector3 pX = z.GetLeft();
                ManagedVector3 pY = z.GetUp();
                ManagedVector3 pZ = z.GetForward();
                float d = m_engineWrapper.GetDistanceToCamera(p0) / 10.0f;
                pX.Mul(d);
                pY.Mul(d);
                pZ.Mul(d);
                pX.Add(p0);
                pY.Add(p0);
                pZ.Add(p0);
                m_engineWrapper.GetProjectedCoords(p0);
                m_engineWrapper.GetProjectedCoords(pX);
                m_engineWrapper.GetProjectedCoords(pY);
                m_engineWrapper.GetProjectedCoords(pZ);
                p0 = ToScreenCoords(p0);
                pX = ToScreenCoords(pX);
                pY = ToScreenCoords(pY);
                pZ = ToScreenCoords(pZ);
                Point pp0 = new Point((int)p0.x, (int)p0.y);
                Point ppX = new Point((int)pX.x, (int)pX.y);
                Point ppY = new Point((int)pY.x, (int)pY.y);
                Point ppZ = new Point((int)pZ.x, (int)pZ.y);
                pp0.X += EngineRender.Location.X;
                pp0.Y += EngineRender.Location.Y;
                ppX.X += EngineRender.Location.X;
                ppX.Y += EngineRender.Location.Y;
                ppY.X += EngineRender.Location.X;
                ppY.Y += EngineRender.Location.Y;
                ppZ.X += EngineRender.Location.X;
                ppZ.Y += EngineRender.Location.Y;
                bCenter.Location = pp0;
                bTX.Location = ppX;
                bTY.Location = ppY;
                bTZ.Location = ppZ;
                bTY.Visible = true;
                bTX.Visible = true;
                bTZ.Visible = true;
            }
        }

        public ManagedVector3 ToScreenCoords(ManagedVector3 v)
        {
            ManagedVector3 vout = new ManagedVector3();
            vout.x = (v.x / v.z + 1) * EngineRender.Width / 2;
            vout.y = (v.y / v.z + 1) * EngineRender.Height / 2;
            vout.z = v.z;
            return vout;
        }

        public void ResizeRenderWindow()
        {
            GenerateRenderWindow();
        }

        public CLI.EngineWrapper GetEngineWrapper()
        {
            return m_engineWrapper;
        }

        public void End()
        {
            if (m_engineThread.IsAlive)
            {
                m_engineThread.Abort();
            }
            m_exiting = true;
        }

        private void GenerateRenderWindow()
        {
            if (m_engineThread != null)
            {
                if (m_engineThread.IsAlive)
                {
                    m_engineThread.Abort();
                }
            }
            m_engineThread = null;
            m_width = m_engineWrapper.GetBufferWidth();
            m_height = m_engineWrapper.GetBufferHeight();
            EngineRender.Width = m_width;
            EngineRender.Height = m_height;
            m_EngineGraphics = EngineRender.CreateGraphics();
            m_engineThread = new Thread(RunEngineThread);
            m_engineThread.IsBackground = true;
            UpdateEngineWindowDelegate = new UpdateEngineWindow(UpdateEngineWindowMethod);
            EngineRender.SizeMode = PictureBoxSizeMode.Zoom;
            m_engineThread.Start();
            if (OnBeginFrame != null)
            {
                OnBeginFrame(this, EventArgs.Empty);
            }
            IntPtr p = m_engineWrapper.GetBufferData();
            m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);
            if (OnEndFrame != null)
            {
                OnEndFrame(this, EventArgs.Empty);
            }
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


                int w = EngineRender.Width;
                int h = EngineRender.Height;
                float hFactor = (float)m_engineBitmap.Height / (float)EngineRenderPanel.Height;
                float wFactor = (float)m_engineBitmap.Width / (float)EngineRenderPanel.Width;
                if (hFactor > wFactor)
                {
                    h = EngineRenderPanel.Height;
                    w = (int)((float)m_engineBitmap.Width / (float)m_engineBitmap.Height * h);
                }
                else
                {
                    w = EngineRenderPanel.Width;
                    h = (int)((float)m_engineBitmap.Height / (float)m_engineBitmap.Width * w);
                }
                EngineRender.Width = w;
                EngineRender.Height = h;
                m_EngineGraphics.DrawImage(m_engineBitmap, 0, 0, w, h);
                int x = (EngineRenderPanel.Width - w) / 2;
                int y = (EngineRenderPanel.Height - h) / 2;
                EngineRender.Location = new Point(x, y);

            }
            SetTranslationGizmos(m_selectedObject);
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
                try
                {
                    m_directZobWrapper.RunAFrame();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
                EngineRender.Invoke(UpdateEngineWindowDelegate);
                if (OnEndFrame != null)
                {
                    OnEndFrame(this, EventArgs.Empty);
                }
            }
        }

        private void UpdateCamera()
        {

        }

        private void EngineRender_MouseWheel(object sender, MouseEventArgs e)
        {
            m_mainForm.GetCameraControl().GetWrapper().Zoom(e.Delta / 50.0f);
        }

        private void EngineRender_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (m_mainForm.IsCtrlPressed())
                {
                    int dx = m_lastMouseX - e.X;
                    int dy = m_lastMouseY - e.Y;
                    m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)-dx, (float)dy);
                    //m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx*2.0f, (float)-dy*2.0f);
                    m_lastMouseX = e.X;
                    m_lastMouseY = e.Y;
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                int dx = m_lastMouseX - e.X;
                int dy = m_lastMouseY - e.Y;
                m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx * 2.0f, (float)-dy * 2.0f);
                m_lastMouseX = e.X;
                m_lastMouseY = e.Y;
            }
        }

        private void EngineRender_MouseHover(object sender, EventArgs e)
        {

        }

        private void EngineRender_MouseClick(object sender, MouseEventArgs e)
        {
            m_lastMouseX = e.X;
            m_lastMouseY = e.Y;
            if (e.Button == MouseButtons.Left && !m_mainForm.IsCtrlPressed())
            {
                //                m_selectedObject = m_mainForm.GetZobObjectListControl().SelectObjectAtCoords(e.X, e.Y, CLI.ZobObjectManagerWrapper.eObjectTypes.eObjectTypes_scene);
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

        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            CLI.ZobObjectWrapper oldObject = e.previousZobObject;
            m_selectedObject = e.newZobObject;
            if (m_selectedObject != null)
            {
                SetTranslationGizmos(m_selectedObject);
            }
            else
            {
                RemoveTranslationGizmos();
            }
        }

        private void bTX_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_selectedObject != null && e.Button == MouseButtons.Left)
            {
                ManagedVector3 pX = m_selectedObject.GetLeft();
                ManagedVector3 p0 = m_selectedObject.GetTransform();
                float d = m_engineWrapper.GetDistanceToCamera(p0) / 10.0f;
                pX.Mul(d);
                pX.Add(p0);
                m_engineWrapper.GetProjectedCoords(p0);
                m_engineWrapper.GetProjectedCoords(pX);
                p0 = ToScreenCoords(p0);
                pX = ToScreenCoords(pX);
                float x = (m_lastMouseX - e.X);
                float y = (m_lastMouseY - e.Y);
                pX = m_selectedObject.GetLeft();
                pX.x = (x / 100.0f) ;
                //pX.y = pX.y + y / 100.0f;
                p0 = m_selectedObject.GetTransform();
                p0.Add(pX);
                m_selectedObject.SetTransform(p0); 
                m_lastMouseX = e.X;
                m_lastMouseY = e.Y;
            }
            //e.Handled = true;
        }

        private void bTX_Click(object sender, EventArgs e)
        {
            //e.ToString(); ;
            MouseEventArgs m = (MouseEventArgs)e;
            m_lastMouseX = m.X;
            m_lastMouseY = m.Y;
        }
    }
}
