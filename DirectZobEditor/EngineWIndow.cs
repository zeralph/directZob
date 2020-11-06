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
using System.Numerics;

namespace DirectZobEditor
{
    public partial class EngineWindow : UserControl
    {
        public enum objectModificator
        {
            none=0,
            translate,
            rotate,
            scale,
        };
        public enum axis
        {
            none=0,
            X,
            Y,
            Z,
        };
        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.EngineWrapper m_engineWrapper;
        private Thread m_engineThread;
        private Form1 m_mainForm;
        //engine rendering
        public CLI.onFrameStartCallback onFrameStartCallback;
        public CLI.onFrameEndCallback onFrameEndCallback;
        public event EventHandler OnBeginFrame;
        public event EventHandler OnEndFrame;
        public delegate void BeforeUpdateEngine();
        public delegate void AfterUpdateEngine();
        public delegate void UpdateLogWindow();
        public BeforeUpdateEngine BeforeUpdateEngineWindowDelegate;
        public AfterUpdateEngine AfterUpdateEngineWindowDelegate;
        //object move, rotate and scale
        public delegate void OnObjectMovedHandler(object s, ObjectModificationEventArg e);
        public event OnObjectMovedHandler OnObjectMoved;
        public delegate void OnObjectRotatedHandler(object s, ObjectModificationEventArg e);
        public event OnObjectRotatedHandler OnObjectRotated;
        public delegate void OnObjectScaledHandler(object s, ObjectModificationEventArg e);
        public event OnObjectScaledHandler OnObjectScaled;
        Graphics m_EngineGraphics = null;
        Bitmap m_engineBitmap = null;
        int m_width;
        int m_height;
        int m_lastMouseX = -1;
        int m_lastMouseY = -1;
        private bool m_engineRendering = false;
        private objectModificator m_objectModificator = objectModificator.translate;

        private ManagedVector3 m_startTranslation;
        private ManagedVector3 m_startRotation;
        private ManagedVector3 m_startLeft;
        private ManagedVector3 m_startUp;
        private ManagedVector3 m_startForward;
        private ManagedVector3 m_lastK = new ManagedVector3();
        private bool m_objectReadyForModification = false;

        public EngineWindow(Form1 f, CLI.DirectZobWrapper directZobWrapper)
        {
            InitializeComponent();
            this.Dock = DockStyle.Fill;
            m_mainForm = f;
            m_directZobWrapper = directZobWrapper;
            m_engineWrapper = new CLI.EngineWrapper();
            GenerateRenderWindow();
            bTX.Visible = false;
            bTY.Visible = false;
            bTZ.Visible = false;
            bCenter.Visible = false;
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnNewScene += new EventHandler(OnSceneChanged);
            EngineRender.HandleDestroyed += new EventHandler(OnClose);
        }

        private void OnClose(object sender, EventArgs e)
        {
            m_directZobWrapper.Stop();
            m_EngineGraphics.Dispose();
        }

        public void SetModificator(objectModificator m)
        {
            m_objectModificator = m;
        }
        public void RemoveTranslationGizmos()
        {
            bTY.Visible = false;
            bTX.Visible = false;
            bTZ.Visible = false;
        }

        public void OnSceneChanged(object s, EventArgs e)
        {
        }

        public void UpdateModificationGizmos()
        {
            ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetSelectedZobObject();
            if (z != null && z.IsValid())
            {
                int btnSize = 20 / 2;
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
                pp0.X += EngineRender.Location.X - btnSize;
                pp0.Y += EngineRender.Location.Y - btnSize;
                ppX.X += EngineRender.Location.X - btnSize;
                ppX.Y += EngineRender.Location.Y - btnSize;
                ppY.X += EngineRender.Location.X - btnSize;
                ppY.Y += EngineRender.Location.Y - btnSize;
                ppZ.X += EngineRender.Location.X - btnSize;
                ppZ.Y += EngineRender.Location.Y - btnSize;
                bCenter.Location = pp0;
                bTX.Location = ppX;
                bTY.Location = ppY;
                bTZ.Location = ppZ;
                bCenter.Visible = true;
                bTY.Visible = true;
                bTX.Visible = true;
                bTZ.Visible = true;
                p0 = z.GetTransform();
                pX = z.GetLeft();
                pY = z.GetUp();
                pZ = z.GetForward();
                switch (m_objectModificator)
                {
                    case objectModificator.translate:
                        bTX.Text = "Tx";
                        bTY.Text = "Ty";
                        bTZ.Text = "Tz";
                        break;
                    case objectModificator.rotate:
                        bTX.Text = "Rx";
                        bTY.Text = "Ry";
                        bTZ.Text = "Rz";                    
                        m_engineWrapper.DrawLine(p0, pX, 0xFF0000);
                        m_engineWrapper.DrawLine(p0, pY, 0x00FF00);
                        m_engineWrapper.DrawLine(p0, pZ, 0x0000FF);
                        break;
                    case objectModificator.scale:
                        bTX.Text = "Sx";
                        bTY.Text = "Sy";
                        bTZ.Text = "Sz";
                        break;
                    default:
                        break;
                }
            }
            else
            {
                bCenter.Visible = false;
                bTY.Visible = false;
                bTX.Visible = false;
                bTZ.Visible = false;
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
            m_width = m_engineWrapper.GetBufferWidth();
            m_height = m_engineWrapper.GetBufferHeight();
            EngineRender.Width = m_width;
            EngineRender.Height = m_height;
            if(m_EngineGraphics != null)
            {
                m_EngineGraphics = null;
            }
            m_EngineGraphics = EngineRender.CreateGraphics();
            lock (m_engineBitmap)
            {
                GenerateEngineBitmap();          
            }
            UpdateGraphicsParameters();
        }

        public CLI.EngineWrapper GetEngineWrapper()
        {
            return m_engineWrapper;
        }
        private void GenerateEngineBitmap()
        {
            if(m_engineBitmap != null)
            {
                m_engineBitmap = null;
            }
            IntPtr p = m_engineWrapper.GetBufferData();
            m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);
        }

        private void GenerateRenderWindow()
        {
            m_engineThread = null;
            m_width = m_engineWrapper.GetBufferWidth();
            m_height = m_engineWrapper.GetBufferHeight();
            EngineRender.Width = m_width;
            EngineRender.Height = m_height;
            m_EngineGraphics = EngineRender.CreateGraphics();
            m_engineThread = new Thread(RunEngineThread);
            //m_engineThread.IsBackground = true;
            BeforeUpdateEngineWindowDelegate = new BeforeUpdateEngine(BeforeUpdateEngineWindowMethod);
            AfterUpdateEngineWindowDelegate = new AfterUpdateEngine(AfterUpdateEngineWindowMethod);
            onFrameStartCallback = new onFrameStartCallback(onFrameStartCallbackMethod);
            onFrameEndCallback = new onFrameEndCallback(onFrameEndCallbackMethod);
            EngineRender.SizeMode = PictureBoxSizeMode.Zoom;
            GenerateEngineBitmap();
            IntPtr hwnd = EngineRender.Handle;
            m_EngineGraphics = Graphics.FromHwnd(hwnd);
            m_EngineGraphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            UpdateGraphicsParameters();
            m_engineThread.Start();
            if (OnBeginFrame != null)
            {
                OnBeginFrame(this, EventArgs.Empty);
            }
            if (OnEndFrame != null)
            {
                OnEndFrame(this, EventArgs.Empty);
            }
        }

        #region engineSynchronisationDelegates
        private void onFrameStartCallbackMethod()
        {
            try
            {
                EngineRender.Invoke(BeforeUpdateEngineWindowDelegate);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
        private void BeforeUpdateEngineWindowMethod()
        {
            m_engineRendering = true;
        }
        private void onFrameEndCallbackMethod()
        {
            try
            {
                EngineRender.Invoke(AfterUpdateEngineWindowDelegate);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
        private void AfterUpdateEngineWindowMethod()
        {
            lock (EngineRender)
            {
                IntPtr p = m_engineWrapper.GetBufferData();
                Rectangle rect = new Rectangle(0, 0, m_engineBitmap.Width, m_engineBitmap.Height);
                System.Drawing.Imaging.BitmapData bmpData =
                    m_engineBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly,
                    m_engineBitmap.PixelFormat);
                bmpData.Scan0 = p;
                m_engineBitmap.UnlockBits(bmpData);
                UpdateGraphicsParameters();
                m_mainForm.UpdateAfterEngine();
                UpdateModificationGizmos();
            }
            m_engineRendering = false;
        }
        #endregion
        private void UpdateGraphicsParameters()
        {
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
            if (EngineRender.Width != w || EngineRender.Height != h)
            {
                EngineRender.Width = w;
                EngineRender.Height = h;
                int x = (EngineRenderPanel.Width - w) / 2;
                int y = (EngineRenderPanel.Height - h) / 2;
                EngineRender.Location = new Point(x, y);
            }
            m_EngineGraphics.DrawImage(m_engineBitmap, 0, 0, w, h);
        }
        private void RunEngineThread()
        {
            m_directZobWrapper.Run(onFrameStartCallback, onFrameEndCallback);
        }

        private void UpdateCamera()
        {

        }

        private void EngineRender_MouseWheel(object sender, MouseEventArgs e)
        {
            m_mainForm.GetCameraControl().GetWrapper().Zoom(-e.Delta / 50.0f);
        }

        private void EngineRender_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_engineRendering)
            {
                int dx = m_lastMouseX - Cursor.Position.X;
                int dy = m_lastMouseY - Cursor.Position.Y;
                if (e.Button == MouseButtons.Left && m_mainForm.IsCtrlPressed())
                {
                    m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)dx, (float)dy);
                }
                else if (e.Button == MouseButtons.Middle)
                {
                    m_mainForm.GetCameraControl().GetWrapper().Move((float)-dx * 2.0f, (float)dy * 2.0f);
                }
            }
            m_lastMouseX = Cursor.Position.X;
            m_lastMouseY = Cursor.Position.Y;
        }

        private void EngineRender_MouseHover(object sender, EventArgs e)
        {

        }
        private void EngineRender_MouseClick(object sender, MouseEventArgs e)
        {
            if (m_engineRendering)
            {
                //return;
            }
            float x = (float)e.X;
            float y = (float)e.Y;
            x /= EngineRender.Width;
            y /= EngineRender.Height;
            x = x * 2.0f - 1.0f;
            y = y * 2.0f - 1.0f;
            ManagedVector3 v = new ManagedVector3();
            v.x = x;
            v.y = y;
            v.z = 0.0f;
            CLI.ZobObjectWrapper z = GetEngineWrapper().GetObjectAt2DCoords(v);
            if(z != null)
            {
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.objectSelected;
                ev.zobObject = z;
                m_mainForm.PropagateSceneUpdateEvent(ev);
            }
            else
            {
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.objectDeselected;
                ev.zobObject = null;
                m_mainForm.PropagateSceneUpdateEvent(ev);
            }
        }

        private void TRSButton_Click(object sender, EventArgs e)
        {
            ToolStripButton b = (ToolStripButton)sender;
            //if(b.Name == )
        }

        private void EngineRender_MouseDown(object sender, MouseEventArgs e)
        {

        }

        private void EngineRender_MouseUp(object sender, MouseEventArgs e)
        {
            m_objectReadyForModification = false;
        }

        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            /*
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
            */
        }
        
        private void ModifyObject(MouseEventArgs e, axis a)
        {
            if(!m_objectReadyForModification && e.Button == MouseButtons.Left)
            {
                ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetSelectedZobObject();
                if (z != null)
                {
                    m_startTranslation = z.GetTransform();
                    m_startRotation = z.GetRotation();
                    m_startLeft = z.GetLeft();
                    m_startUp = z.GetUp();
                    m_startForward = z.GetForward();
                    m_objectReadyForModification = true;
                }
            }
            else if(m_objectReadyForModification && e.Button != MouseButtons.Left)
            {
                m_objectReadyForModification = false;
            }
            if (m_objectReadyForModification)
            {
                switch (m_objectModificator)
                {
                    case objectModificator.rotate:
                        RotateObject(e, a);
                        break;
                    case objectModificator.scale:
                        ScaleObject(e, a);
                        break;
                    case objectModificator.translate:
                        TranslateObject(e, a);
                        break;
                    default:
                        break;
                }
            }
        }
        private void RotateObject(MouseEventArgs e, axis a)
        {
            if (m_engineRendering)
            {
                return;
            }
            ZobCameraWrapper c = m_mainForm.GetCameraControl().GetWrapper().GetCurrentCamera();
            ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetSelectedZobObject();
            if (c != null && z != null && e.Button == MouseButtons.Left && a != axis.none)
            {
                Point p = bTX.PointToScreen(new Point(e.X, e.Y));
                p = EngineRender.PointToClient(p);
                float x = (float)p.X;
                float y = (float)p.Y;
                x /= EngineRender.Width;
                y /= EngineRender.Height;
                x = x * 2.0f - 1.0f;
                y = y * 2.0f - 1.0f;
                ManagedVector3 v = new ManagedVector3();
                v.x = x;
                v.y = y;
                v.z = 0.0f;
                ManagedVector3 pn = z.GetUp();
                switch (a)
                {
                    case axis.X:
                        pn = z.GetUp();
                        break;
                    case axis.Y:
                        pn = z.GetForward();
                        break;
                    case axis.Z:
                        pn = z.GetLeft();
                        break;
                }

                ManagedVector3 p0 = m_startTranslation;
                ManagedVector3 or = m_startRotation;
                m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(v, p0, pn);
                if (a != axis.none)
                {
                    ManagedVector3 forward = new ManagedVector3((v.x - p0.x), (v.y - p0.y), (v.z - p0.z));
                    ManagedVector3 k3 = forward;
                    forward.Normalize();
                    ManagedVector3 up = pn; //z.GetUp(); //new ManagedVector3(0, 1, 0);
                    ManagedVector3 left = up.Cross(forward);
                    z.LookAt(forward, left, up);
                    m_engineWrapper.DrawLine(p0, left, 0xFF00FF);
                    m_engineWrapper.DrawLine(p0, up, 0xFF00FF);
                    m_engineWrapper.DrawLine(p0, forward, 0xFF00FF);
                    OnZobObjectRotated(z);
                }  
            }
        }
        private void ScaleObject(MouseEventArgs e, axis a)
        {
        }

        private void TranslateObject(MouseEventArgs e, axis a)
        {
            if (m_engineRendering)
            {
                return;
            }
            ZobCameraWrapper c = m_mainForm.GetCameraControl().GetWrapper().GetCurrentCamera();
            ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetSelectedZobObject();
            if (c != null && z != null && e.Button == MouseButtons.Left)
            {
                Point p = bTX.PointToScreen(new Point(e.X, e.Y));
                p = EngineRender.PointToClient(p);
                float x = (float)p.X;
                float y = (float)p.Y;
                x /= EngineRender.Width;
                y /= EngineRender.Height;
                x = x * 2.0f - 1.0f;
                y = y * 2.0f - 1.0f;
                ManagedVector3 v = new ManagedVector3();
                v.x = x;
                v.y = y;
                v.z = 0.0f;
                ManagedVector3 p0 = z.GetTransform();
                ManagedVector3 pn = c.GetForward();
                m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(v, p0, pn);
                if (a != axis.none)
                {
                    ManagedVector3 k = new ManagedVector3((v.x - p0.x), (v.y - p0.y), (v.z - p0.z));
                    ManagedVector3 k2 = new ManagedVector3(k);
                    k2.Normalize();
                    v = c.GetForward();
                    switch (a)
                    {
                        case axis.X:
                            v = z.GetLeft();
                            break;
                        case axis.Y:
                            v = z.GetUp();
                            break;
                        case axis.Z:
                            v = z.GetForward();
                            break;
                    }
                    k.x = k.x * v.x;
                    k.y = k.y * v.y;
                    k.z = k.z * v.z;
                    float f = k.sqrtLength();
                    float sign = v.Dot(k2);
                    v.Mul(f * sign);
                    v.Add(p0);
                }
                z.SetTransform(v);
                OnZobObjectMoved(z);
            }
        }

        private void bTX_MouseMove(object sender, MouseEventArgs e)
        {
            ModifyObject(e, axis.X);
        }
        private void bTY_MouseMove(object sender, MouseEventArgs e)
        {
            ModifyObject(e, axis.Y);
        }
        private void bTZ_MouseMove(object sender, MouseEventArgs e)
        {
            ModifyObject(e, axis.Z);
        }
        private void bCenter_MouseMove(object sender, MouseEventArgs e)
        {
            ModifyObject(e, axis.none);
        }
        #region object handlers for move, scale and rotate
        private void OnZobObjectMoved(CLI.ZobObjectWrapper zobObject)
        {
            OnObjectMovedHandler handler = OnObjectMoved;
            if (null != handler)
            {
                ObjectModificationEventArg ev = new ObjectModificationEventArg();
                ev.t = zobObject.GetTransform();
                handler(this, ev);
            }
        }
        private void OnZobObjectScaled(CLI.ZobObjectWrapper zobObject)
        {
            OnObjectMovedHandler handler = OnObjectMoved;
            if (null != handler)
            {
                ObjectModificationEventArg ev = new ObjectModificationEventArg();
                ev.s = zobObject.GetScale();
                handler(this, ev);
            }
        }
        private void OnZobObjectRotated(CLI.ZobObjectWrapper zobObject)
        {
            OnObjectRotatedHandler handler = OnObjectRotated;
            if (null != handler)
            {
                ObjectModificationEventArg ev = new ObjectModificationEventArg();
                ev.r = zobObject.GetRotation();
                handler(this, ev);
            }
        }
        #endregion
    }
}
