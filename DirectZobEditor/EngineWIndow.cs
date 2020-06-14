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
        public CLI.onFrameEndCallback onFrameEndCallback;
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
        bool m_exitEngineThread = false;
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
            bTX.Visible = false;
            bTY.Visible = false;
            bTZ.Visible = false;
            bCenter.Visible = false;
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnNewScene += new EventHandler(OnSceneChanged);
        }

        public void RemoveTranslationGizmos()
        {
            bTY.Visible = false;
            bTX.Visible = false;
            bTZ.Visible = false;
        }

        public void OnSceneChanged(object s, EventArgs e)
        {
            m_selectedObject = null;
        }

        public void SetTranslationGizmos(ZobObjectWrapper z)
        {
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
			if (m_engineThread != null)
			{		
				m_exitEngineThread = true;
				m_engineThread.Join();
				m_engineThread = null;
			}
        }

        private void GenerateRenderWindow()
        {
			End();
            m_engineThread = null;
            m_width = m_engineWrapper.GetBufferWidth();
            m_height = m_engineWrapper.GetBufferHeight();
            EngineRender.Width = m_width;
            EngineRender.Height = m_height;
            m_EngineGraphics = EngineRender.CreateGraphics();
            m_engineThread = new Thread(RunEngineThread);
            m_engineThread.IsBackground = true;
            UpdateEngineWindowDelegate = new UpdateEngineWindow(UpdateEngineWindowMethod);
            onFrameEndCallback = new onFrameEndCallback(onFrameEndCallbackMethod);
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

        private void onFrameEndCallbackMethod()
        {
            EngineRender.Invoke(UpdateEngineWindowDelegate);
            //UpdateEngineWindowMethod();
        }

        private void UpdateEngineWindowMethod()
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
            SetTranslationGizmos(m_selectedObject);
            m_mainForm.UpdateAfterEngine();
        }

        private void RunEngineThread()
        {
            m_directZobWrapper.Run(onFrameEndCallback);
            /*
            while (!m_exitEngineThread)
            {
				try
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
				catch(Exception e)
				{

				}
            }
            */
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
            m_lastMouseX = Cursor.Position.X;
            m_lastMouseY = Cursor.Position.Y;
        }

        private void EngineRender_MouseHover(object sender, EventArgs e)
        {

        }

        private void EngineRender_MouseClick(object sender, MouseEventArgs e)
        {
            //m_lastMouseX = e.X;
            //m_lastMouseY = e.Y;
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
                MoveAlongAxis(m_selectedObject.GetLeft());
            }
        }
        private void bTY_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_selectedObject != null && e.Button == MouseButtons.Left)
            {
                MoveAlongAxis(m_selectedObject.GetUp());
            }
        }
        private void bTZ_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_selectedObject != null && e.Button == MouseButtons.Left)
            {
                MoveAlongAxis(m_selectedObject.GetForward());
            }
        }
        private void MoveAlongAxis(ManagedVector3 axis)
        {
            float speed = 10.0f;
            ManagedVector3 pX = new ManagedVector3(axis);
            ManagedVector3 p0 = m_selectedObject.GetTransform();
            pX.Add(p0);
            m_engineWrapper.GetProjectedCoords(p0);
            m_engineWrapper.GetProjectedCoords(pX);
            p0 = ToScreenCoords(p0);
            pX = ToScreenCoords(pX);
            Vector2 v1 = new Vector2(m_lastMouseX - Cursor.Position.X, m_lastMouseY - Cursor.Position.Y);
            if (v1.X != 0 || v1.Y != 0)
            {
                Vector2 v2 = new Vector2(pX.x - p0.x, pX.y - p0.y);
                v1 = Vector2.Normalize(v1);
                v2 = Vector2.Normalize(v2);
                float d = -Vector2.Dot(v1, v2) / speed;
                Vector3 u1 = new Vector3(v1, 0);
                Vector3 u2 = new Vector3(v2, 0);
                pX = new ManagedVector3(axis);
                pX.x *= d;
                pX.y *= d;
                pX.z *= d;
                p0 = m_selectedObject.GetTransform();
                p0.Add(pX);
                m_selectedObject.SetTransform(p0);
                m_mainForm.PropagateSceneUpdateEvent(EventArgs.Empty);
            }
            m_lastMouseX = Cursor.Position.X;
            m_lastMouseY = Cursor.Position.Y;
        }
    }
}
