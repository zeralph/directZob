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
        private directZobCLI.ZobObjectWrapper m_currentZobObjectWrapper = null;
        private directZobCLI.MeshManagerWrapper m_meshManagerWrapper = null;

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
            m_meshManagerWrapper = new directZobCLI.MeshManagerWrapper();
            m_mainForm.OnNewScene += new EventHandler(OnNewScene);
            ClearValues();
            ZobObjectListControl z = m_mainForm.GetZobObjectListControl();
            z.OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnSceneUpdated += new EventHandler(OnSceneUpdated);
        }

        private void OnSceneUpdated(object sender, EventArgs e)
        {
            SetValues();
        }

        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            directZobCLI.ZobObjectWrapper oldObject = e.previousZobObject;
            m_currentZobObjectWrapper = e.newZobObject;
            if (m_currentZobObjectWrapper != null)
            {

                SetValues();
                this.Visible = true;
            }
            else
            {
                this.Visible = false;
                ClearValues();
            }
            
        }
        private void SetValues()
        {
            if (m_currentZobObjectWrapper != null)
            {
                zobName.Text = m_currentZobObjectWrapper.GetName();
                zobPosX.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetTransform().x);
                zobPosY.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetTransform().y);
                zobPosZ.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetTransform().z);
                zobRotX.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetRotation().x);
                zobRotY.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetRotation().y);
                zobRotZ.Text = String.Format("{0:0.000}", m_currentZobObjectWrapper.GetRotation().z);
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
                directZobCLI.ManagedVector3 p = m_currentZobObjectWrapper.GetTransform();
                directZobCLI.ManagedVector3 r = m_currentZobObjectWrapper.GetRotation();
                directZobCLI.ManagedVector3 s = m_currentZobObjectWrapper.GetScale();

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

                m_currentZobObjectWrapper.SetTransform(p);
                m_currentZobObjectWrapper.SetRotation(r);
                m_currentZobObjectWrapper.SetScale(s);
                SetValues();
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
    }
}
