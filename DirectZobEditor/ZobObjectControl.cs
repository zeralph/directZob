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
        private CLI.ZobObjectWrapper m_zobObjectWrapper = null;
        private CLI.MeshManagerWrapper m_meshManagerWrapper = null;
        public ZobObjectControl(Form1 form)
        {
            InitializeComponent();
            m_mainForm = form;
            m_meshManagerWrapper = new CLI.MeshManagerWrapper();
            m_mainForm.OnNewScene += new EventHandler(OnNewScene);
            ClearValues();
            RefreshMeshList();
        }

        public void SetZobObjectWrapper(CLI.ZobObjectWrapper z)
        {
            m_zobObjectWrapper = z;
            if(z != null)
            {
                SetValues();
                RefreshMeshList();
            }
            else
            {
                ClearValues();
            }
        }

        private void SetValues()
        {
            zobName.Text = m_zobObjectWrapper.GetName();
            zobPosX.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetTransform().x);
            zobPosY.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetTransform().y);
            zobPosZ.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetTransform().z);
            zobRotX.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetRotation().x);
            zobRotY.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetRotation().y);
            zobRotZ.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetRotation().z);
            zobScaleX.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetScale().x);
            zobScaleY.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetScale().y);
            zobScaleZ.Text = String.Format("{0:0.00}", m_zobObjectWrapper.GetScale().z);
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
            if (m_zobObjectWrapper != null)
            {
                CLI.ManagedVector3 p = m_zobObjectWrapper.GetTransform();
                CLI.ManagedVector3 r = m_zobObjectWrapper.GetRotation();
                CLI.ManagedVector3 s = m_zobObjectWrapper.GetScale();

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

                m_zobObjectWrapper.SetTransform(p);
                m_zobObjectWrapper.SetRotation(r);
                m_zobObjectWrapper.SetScale(s);
                SetValues();
            }
        }

        private void RefreshMeshList()
        {
            meshList.Items.Clear();
            string s = m_meshManagerWrapper.GetMeshList();
            List<JSONMesh> z = JsonConvert.DeserializeObject<List<JSONMesh>>(s);
            meshList.Items.Add("");
            for (int i = 0; i < z.Count; i++)
            {
                meshList.Items.Add(z[i].name);
            }
            if(m_zobObjectWrapper != null)
            {
                s = m_zobObjectWrapper.GetMeshName();
                meshList.SelectedItem = s;
            }
        }

        private void UpdateFromTextBoxes(object sender, EventArgs e)
        {
            UpdateValues();
        }

        private void ZobScaleZ_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == (char)Keys.Return)
            {
                e.Handled = true;
                //e.SuppressKeyPress = true;
                UpdateValues();
            }
        }

        public class JSONMesh
        {
            public string name { get; set; }
        }

        private void MeshList_SelectedValueChanged(object sender, EventArgs e)
        {
            if(m_zobObjectWrapper != null)
            {
                m_zobObjectWrapper.SetMesh(meshList.SelectedItem.ToString());
            }
        }

        private void OnNewScene(object s, EventArgs e)
        {
            m_zobObjectWrapper = null;
            UpdateValues();
        }
    }
}
