﻿using System;
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
        public CLI.engineCallback onFrameStartCallback;
        public CLI.engineCallback onFrameEndCallback;
        public CLI.engineCallback onSceneUpdatedCallback;
        public CLI.engineCallback onQueuingCallback;
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
        private ManagedVector3 m_startScaleValue;
        private ManagedVector3 m_startLeft;
        private ManagedVector3 m_startUp;
        private ManagedVector3 m_startForward;
        private ManagedVector3 m_lastK = new ManagedVector3();
        private bool m_objectReadyForModification = false;

        private ManagedVector3 m_pickObjectPosition = null;
        private ZobObjectWrapper m_newObjectPicked = null;

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
            
            m_engineThread.Start();
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
                ManagedVector3 p0 = z.GetTransform().Copy();
                ManagedVector3 pX = z.GetLeft().Copy();
                ManagedVector3 pY = z.GetUp().Copy();
                ManagedVector3 pZ = z.GetForward().Copy();
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
                p0 = z.GetTransform().Copy();
                pX = z.GetLeft().Copy();
                pY = z.GetUp().Copy();
                pZ = z.GetForward().Copy();
                pX.Add(p0);
                pY.Add(p0);
                pZ.Add(p0);
                switch (m_objectModificator)
                {
                    case objectModificator.translate:
                        bTX.Text = "Tx";
                        bTY.Text = "Ty";
                        bTZ.Text = "Tz";
                        m_engineWrapper.DrawLine(p0, pX, 0xFF0000);
                        m_engineWrapper.DrawLine(p0, pY, 0x00FF00);
                        m_engineWrapper.DrawLine(p0, pZ, 0x0000FF);
                        break;
                    case objectModificator.rotate:
                        bTX.Text = "Rx";
                        bTY.Text = "Ry";
                        bTZ.Text = "Rz";
                        m_engineWrapper.DrawCircle(p0, z.GetLeft(), 1.0f, 0xFF0000);
                        m_engineWrapper.DrawCircle(p0, z.GetUp(), 1.0f, 0x00FF00);
                        m_engineWrapper.DrawCircle(p0, z.GetForward(), 1.0f, 0x0000FF);
                        break;
                    case objectModificator.scale:
                        bTX.Text = "Sx";
                        bTY.Text = "Sy";
                        bTZ.Text = "Sz";
                        m_engineWrapper.DrawLine(p0, pX, 0xFF0000);
                        m_engineWrapper.DrawLine(p0, pY, 0x00FF00);
                        m_engineWrapper.DrawLine(p0, pZ, 0x0000FF);
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
            onFrameStartCallback = new engineCallback(onFrameStartCallbackMethod);
            onFrameEndCallback = new engineCallback(onFrameEndCallbackMethod);
            onSceneUpdatedCallback = new engineCallback(onEngineSceneUpdated);
            onQueuingCallback = new engineCallback(onQueuingCallbackMethod);
            EngineRender.SizeMode = PictureBoxSizeMode.Zoom;
            GenerateEngineBitmap();
            IntPtr hwnd = EngineRender.Handle;
            m_EngineGraphics = Graphics.FromHwnd(hwnd);
            m_EngineGraphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            UpdateGraphicsParameters();
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

        private void onQueuingCallbackMethod()
        {
            GetEngineWrapper().QueueObjectsToRender();
        }
        private void onEngineSceneUpdated()
        {
            if(m_pickObjectPosition != null)
            {
                CLI.ZobObjectWrapper z = GetEngineWrapper().GetObjectAt2DCoords(m_pickObjectPosition);
                m_newObjectPicked = z;
                m_pickObjectPosition = null;
            }
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
            if(m_newObjectPicked != null)
            {
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.objectSelected;
                ev.zobObject = m_newObjectPicked;
                m_mainForm.PropagateSceneUpdateEvent(ev);
                m_newObjectPicked = null;

            }
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
            m_directZobWrapper.Run(onFrameStartCallback, onFrameEndCallback, onSceneUpdatedCallback, onQueuingCallback);
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
                    ManagedVector3 p = m_mainForm.GetCameraControl().GetWrapper().GetCurrentCameraPosition().Copy();
                    float y = p.y;
                    ManagedVector3 t = m_mainForm.GetCameraControl().GetWrapper().GetCurrentCameraTarget().Copy();
                    p.x = t.x - p.x;
                    p.y = t.y - p.y;
                    p.z = t.z - p.z;
                    float d = p.sqrtLength();
                    float z = Math.Abs(p.z) / p.z;
                    p.z = 0;
                    p.Normalize();
                    float a = p.Dot(new ManagedVector3(1, 0, 0));
                    a = (float)Math.Acos((double)a) * z;
                    a += dx/200.0f;
                    p.x = 1.0f * (float)Math.Cos(a) * d;
                    p.z = z * (float)Math.Sin(a) * d;
                    p.y = y;
                    //m_mainForm.GetCameraControl().GetWrapper().SetCurrentCameraPosition(p);
                    m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)dx / 5.0f, (float)dy / 5.0f);
                    m_mainForm.GetCameraControl().GetWrapper().SetLookAt(new ManagedVector3(0, 0, 0));
                    //m_mainForm.GetCameraControl().GetWrapper().RotateAroundAxis((float)dx/5.0f, (float)dy/5.0f);
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
            m_pickObjectPosition = v.Copy();
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
                    m_startTranslation = z.GetTransform().Copy();
                    m_startRotation = z.GetRotation().Copy();
                    m_startLeft = z.GetLeft().Copy();
                    m_startUp = z.GetUp().Copy();
                    m_startForward = z.GetForward().Copy();
                    m_startScaleValue = z.GetScale().Copy();
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
                //return;
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
                switch (a)
                {
                    case axis.X:
                        RotateObjectAroundX(z, v);
                        break;
                    case axis.Y:
                        RotateObjectAroundY(z, v);
                        //pn =z.GetForward().Copy();
                        break;
                    case axis.Z:
                        RotateObjectAroundZ(z, v);
                        //pn = z.GetLeft().Copy();
                        break;
                }
            }
        }

        private void RotateObjectAroundX(ZobObjectWrapper z, ManagedVector3 mousePositsion)
        {
            ManagedVector3 pn = z.GetUp().Copy();
            ManagedVector3 p0 = m_startTranslation.Copy();
            m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(mousePositsion, p0, pn);
            ManagedVector3 left = new ManagedVector3((mousePositsion.x - p0.x), (mousePositsion.y - p0.y), (mousePositsion.z - p0.z));
            left.Normalize();
            ManagedVector3 up = z.GetUp().Copy(); 
            ManagedVector3 forward = left.Cross(up);
            z.LookAt(forward, left, up);

            ManagedVector3 p1 = p0.Copy();
            p1.Add(left);
            p1.Mul(2.0f);
            ManagedVector3 p2 = p0.Copy();
            p2.Add(forward);
            p2.Mul(2.0f);
            ManagedVector3 p3 = p0.Copy();
            p3.Add(up);
            p3.Mul(2.0f);
            m_engineWrapper.DrawTriangle(p0, p1, p2, 0xFFFF00);
            OnZobObjectRotated(z);
        }

        private void RotateObjectAroundY(ZobObjectWrapper z, ManagedVector3 mousePositsion)
        {
            ManagedVector3 pn = z.GetForward().Copy();
            ManagedVector3 p0 = m_startTranslation.Copy();
            m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(mousePositsion, p0, pn);
            ManagedVector3 up = new ManagedVector3((mousePositsion.x - p0.x), (mousePositsion.y - p0.y), (mousePositsion.z - p0.z));
            up.Normalize();
            ManagedVector3 forward = z.GetForward().Copy();
            ManagedVector3 left = up.Cross(forward);         
            z.LookAt(forward, left, up);
            ManagedVector3 p1 = p0.Copy();
            p1.Add(left);
            p1.Mul(2.0f);
            ManagedVector3 p2 = p0.Copy();
            p2.Add(forward);
            p2.Mul(2.0f);
            ManagedVector3 p3 = p0.Copy();
            p3.Add(up);
            p3.Mul(2.0f);
            m_engineWrapper.DrawTriangle(p0, p1, p2, 0xFFFF00);
            OnZobObjectRotated(z);
        }
        private void RotateObjectAroundZ(ZobObjectWrapper z, ManagedVector3 mousePositsion)
        {
            ManagedVector3 pn = z.GetLeft().Copy();
            ManagedVector3 p0 = m_startTranslation.Copy();

            m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(mousePositsion, p0, pn);
            ManagedVector3 forward= new ManagedVector3((mousePositsion.x - p0.x), (mousePositsion.y - p0.y), (mousePositsion.z - p0.z));
            forward.Normalize();
            ManagedVector3 left = z.GetLeft().Copy();
            ManagedVector3 up = forward.Cross(left);
            z.LookAt(forward, left, up);

            ManagedVector3 p1 = p0.Copy();
            p1.Add(left);
            p1.Mul(2.0f);
            ManagedVector3 p2 = p0.Copy();
            p2.Add(forward);
            p2.Mul(2.0f);
            ManagedVector3 p3 = p0.Copy();
            p3.Add(up);
            p3.Mul(2.0f);
            m_engineWrapper.DrawTriangle(p0, p1, p2, 0xFFFF00);

            OnZobObjectRotated(z);
        }
        private void ScaleObject(MouseEventArgs e, axis a)
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
                ManagedVector3 p0 = z.GetTransform().Copy();
                ManagedVector3 pn = c.GetForward().Copy();
                m_mainForm.GetCameraControl().GetWrapper().From2DToWorldOnPlane(v, p0, pn);
                if (a != axis.none)
                {
                    ManagedVector3 k = new ManagedVector3((v.x - p0.x), (v.y - p0.y), (v.z - p0.z));
                    float d = k.sqrtLength();
                    k.Normalize();
                    d = Math.Max(0.001f, d);
                    v = new ManagedVector3();
                    float l = 1.0f;
                    switch (a)
                    {
                        case axis.X:
                            float t = k.Dot(z.GetLeft());
                            if (t != 0.0f)
                            {
                                d = d  * Math.Abs(t) / t;
                                l = m_mainForm.IsCtrlPressed() ? d : 1.0f;
                                v.x = m_startScaleValue.x * d;
                                v.y = m_startScaleValue.y * l;
                                v.z = m_startScaleValue.z * l;
                                z.SetScale(v);
                                ManagedVector3 v2 = p0.Copy();
                                v2.x *= v.x;
                                m_engineWrapper.GetProjectedCoords(v2);
                                //bTX.
                                OnZobObjectScaled(z);
                            }
                            break;
                        case axis.Y:
                            if (k.Dot(z.GetUp()) > 0.0f)
                            {
                                v.x = m_startScaleValue.x * l;
                                v.y = m_startScaleValue.y * d;
                                v.z = m_startScaleValue.z * l;
                                z.SetScale(v);
                                OnZobObjectScaled(z);
                            }
                            break;
                        case axis.Z:
                            if (k.Dot(z.GetForward()) > 0.0f)
                            {
                                v.x = m_startScaleValue.x * l;
                                v.y = m_startScaleValue.y * l;
                                v.z = m_startScaleValue.z * d;
                                z.SetScale(v);
                                OnZobObjectScaled(z);
                            }
                            break;
                    }
                }
            }
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
            OnObjectScaledHandler handler = OnObjectScaled;
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

        private void bTX_MouseHover(object sender, EventArgs e)
        {
            ZobCameraWrapper c = m_mainForm.GetCameraControl().GetWrapper().GetCurrentCamera();
            ZobObjectWrapper z = m_mainForm.GetZobObjectListControl().GetSelectedZobObject();
            if (c != null && z != null)// && e.Button == MouseButtons.Left)
            { 
                switch (m_objectModificator)
                {
                    case objectModificator.rotate:
                        ManagedVector3 p0 = z.GetTransform().Copy();
                        ManagedVector3 p1 = p0.Copy();
                        p1.Add(z.GetLeft());
                        p1.Mul(2.0f);
                        ManagedVector3 p2 = p0.Copy();
                        p2.Add(z.GetForward());
                        p2.Mul(2.0f);
                        m_engineWrapper.DrawTriangle(p0, p1, p2, 0xFFFF00);
                        break;
                    case objectModificator.scale:
                        break;
                    case objectModificator.translate:
                        break;
                    default:
                        break;
                }
            }
        }

        private void bTX_MouseDown(object sender, MouseEventArgs e)
        {

        }
    }
}
