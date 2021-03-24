using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace DirectZobEditor
{
    public partial class ZobObjectControl : UserControl
    {
        private Form1 m_mainForm;
        private CLI.ZobObjectWrapper m_currentZobObjectWrapper = null;
        private CLI.MeshManagerWrapper m_meshManagerWrapper = null;

        ZobLightControl m_lightControl = null;
        ZobCameraControl m_cameraControl = null;
        ZobMeshControl m_meshControl = null;
        public ZobObjectControl(Form1 form, ZobLightControl l, ZobCameraControl c, ZobMeshControl m)
        {
            InitializeComponent();
            m_mainForm = form;
            m_lightControl = l;
            m_cameraControl = c;
            m_meshControl = m;
            m_lightControl.Visible = false;
            m_meshControl.Visible = false;
            m_cameraControl.Visible = false;
            this.Visible = false;
            m_meshManagerWrapper = new CLI.MeshManagerWrapper();
            ClearValues();
        }

        public void BindEvents()
        {
            m_mainForm.OnNewScene += new EventHandler(OnNewScene);
            ZobObjectListControl z = m_mainForm.GetZobObjectListControl();
            z.OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnSceneUpdated += new Form1.OnSceneUpdateHandler(OnSceneUpdated);
            m_mainForm.GetEngineWindow().OnObjectMoved+= new EngineWindow.OnObjectMovedHandler(OnObjectMoved);
            m_mainForm.GetEngineWindow().OnObjectRotated+= new EngineWindow.OnObjectRotatedHandler(OnObjectRotated);
            m_mainForm.GetEngineWindow().OnObjectScaled+= new EngineWindow.OnObjectScaledHandler(OnObjectScaled);
        }
        private void OnSceneUpdated(object sender, EventArgs e)
        {
            SetValues();
        }

        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            CLI.ZobObjectWrapper oldObject = e.previousZobObject;
            m_currentZobObjectWrapper = e.newZobObject;
            if (m_currentZobObjectWrapper != null)
            {

                SetValues();
                this.Visible = true;
                UserControl behaviorControl = m_currentZobObjectWrapper.FillBehaviorControl();
                if (behaviorControl != null)
                {
                    this.behaviorBox.Controls.Add(behaviorControl);
                    this.Height += behaviorControl.Height; 
                }
            }
            else
            {
                this.Visible = false;
                ClearValues();
            }
            
        }

        private void OnObjectMoved(object s, ObjectModificationEventArg e)
        {
            zobPosX.Text = String.Format("{0:0.000}",e.t.x);
            zobPosY.Text = String.Format("{0:0.000}", e.t.y);
            zobPosZ.Text = String.Format("{0:0.000}", e.t.z);
        }
        private void OnObjectRotated(object s, ObjectModificationEventArg e)
        {
            zobRotX.Text = String.Format("{0:0.000}", e.r.x);
            zobRotY.Text = String.Format("{0:0.000}", e.r.y);
            zobRotZ.Text = String.Format("{0:0.000}", e.r.z);
        }
        private void OnObjectScaled(object s, ObjectModificationEventArg e)
        {
            zobScaleX.Text = String.Format("{0:0.000}", e.s.x);
            zobScaleY.Text = String.Format("{0:0.000}", e.s.y);
            zobScaleZ.Text = String.Format("{0:0.000}", e.s.z);
        }
        private void SetValues()
        {
            if (m_currentZobObjectWrapper != null && m_currentZobObjectWrapper.IsValid())
            {
                zobId.Text = m_currentZobObjectWrapper.GetId().ToString();
                zobName.Text = m_currentZobObjectWrapper.GetName();
                CLI.ManagedVector3 p = m_currentZobObjectWrapper.GetWorldPosition();
                CLI.ManagedVector3 r = m_currentZobObjectWrapper.GetWorldRotation();
                zobPosX.Text = String.Format("{0:0.000}", p.x);
                zobPosY.Text = String.Format("{0:0.000}", p.y);
                zobPosZ.Text = String.Format("{0:0.000}", p.z);
                zobRotX.Text = String.Format("{0:0.000}", r.x);
                zobRotY.Text = String.Format("{0:0.000}", r.y);
                zobRotZ.Text = String.Format("{0:0.000}", r.z);
                zobScaleX.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetScale().x);
                zobScaleY.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetScale().y);
                zobScaleZ.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetScale().z);
            }
            else
            {
                ClearValues();
            }
        }
        private void ClearValues()
        {
            zobName.Text = "";
            zobPosX.Text = "";
            zobPosY.Text = "";
            zobPosZ.Text = "";
            zobRotX.Text = "";
            zobRotY.Text = "";
            zobRotZ.Text = "";
            zobScaleX.Text = "";
            zobScaleY.Text = "";
            zobScaleZ.Text = "";
        }

        private void UpdateValues()
        {
            if (m_currentZobObjectWrapper != null && m_currentZobObjectWrapper.IsValid())
            {
                CLI.ManagedVector3 p = m_currentZobObjectWrapper.GetWorldPosition();
                CLI.ManagedVector3 r = m_currentZobObjectWrapper.GetWorldRotation();
                CLI.ManagedVector3 s = m_currentZobObjectWrapper.GetScale();

                float px = p.x;
                float py = p.y;
                float pz = p.z;
                float rx = r.x;
                float ry = r.y;
                float rz = r.z;
                float sx = s.x;
                float sy = s.y;
                float sz = s.z;
                float f;
                zobPosX.Text = zobPosX.Text.Replace('.', ',');
                zobPosY.Text = zobPosY.Text.Replace('.', ',');
                zobPosZ.Text = zobPosZ.Text.Replace('.', ',');

                zobPosX.Text = zobPosX.Text.Replace('.', ',');
                zobPosY.Text = zobPosY.Text.Replace('.', ',');
                zobPosZ.Text = zobPosZ.Text.Replace('.', ',');

                zobRotX.Text = zobRotX.Text.Replace('.', ',');
                zobRotY.Text = zobRotY.Text.Replace('.', ',');
                zobRotZ.Text = zobRotZ.Text.Replace('.', ',');

                zobScaleX.Text = zobScaleX.Text.Replace('.', ',');
                zobScaleY.Text = zobScaleY.Text.Replace('.', ',');
                zobScaleZ.Text = zobScaleZ.Text.Replace('.', ',');

                if (float.TryParse(zobPosX.Text, out f))
                {
                    p.x = f;
                }
                if (float.TryParse(zobPosY.Text, out f))
                {
                    p.y = f;
                }
                if (float.TryParse(zobPosZ.Text, out f))
                {
                    p.z = f;
                }
                if (float.TryParse(zobRotX.Text, out f))
                {
                    r.x = f;
                }
                if (float.TryParse(zobRotY.Text, out f))
                {
                    r.y = f;
                }
                if (float.TryParse(zobRotZ.Text, out f))
                {
                    r.z = f;
                }
                if (float.TryParse(zobScaleX.Text, out f))
                {
                    s.x = f;
                }
                if (float.TryParse(zobScaleY.Text, out f))
                {
                    s.y = f;
                }
                if (float.TryParse(zobScaleZ.Text, out f))
                {
                    s.z = f;
                }

                m_currentZobObjectWrapper.SetWorldTransform(p);
                m_currentZobObjectWrapper.SetWorldRotation(r);
                m_currentZobObjectWrapper.SetScale(s);
                //SetValues();
            }
        }

        private void UpdateFromTextBoxes(object sender, EventArgs e)
        {
            if(checkBoxLinkScale.Checked)
            {
                if (sender == zobScaleX)
                {
                    zobScaleY.Text = zobScaleX.Text;
                    zobScaleZ.Text = zobScaleX.Text;
                }
                if (sender == zobScaleY)
                {
                    zobScaleX.Text = zobScaleY.Text;
                    zobScaleZ.Text = zobScaleY.Text;
                }
                if (sender == zobScaleZ)
                {
                    zobScaleY.Text = zobScaleZ.Text;
                    zobScaleX.Text = zobScaleZ.Text;
                }
            }
            UpdateValues();
        }

        private void ZobScaleZ_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == (char)Keys.Return)
            {
                e.Handled = true;
                if (checkBoxLinkScale.Checked)
                {
                    if (sender == zobScaleX)
                    {
                        zobScaleY.Text = zobScaleX.Text;
                        zobScaleZ.Text = zobScaleX.Text;
                    }
                    if (sender == zobScaleY)
                    {
                        zobScaleX.Text = zobScaleY.Text;
                        zobScaleZ.Text = zobScaleY.Text;
                    }
                    if (sender == zobScaleZ)
                    {
                        zobScaleY.Text = zobScaleZ.Text;
                        zobScaleX.Text = zobScaleZ.Text;
                    }
                }
                UpdateValues();
            }
        }

        public class JSONMesh
        {
            public string name { get; set; }
        }

        private void OnNewScene(object s, EventArgs e)
        {
            m_currentZobObjectWrapper = null;
            UpdateValues();
        }

        private void valueChanged(object sender, EventArgs e)
        {
            UpdateValues();
        }

        private void zobName_Leave(object sender, EventArgs e)
        {
            if (zobName.Text.Length > 0)
            {
                m_currentZobObjectWrapper.SetName(zobName.Text);
            }
        }
    }
}
