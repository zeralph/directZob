using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CLI;

namespace DirectZobEditor
{
    public partial class ZobMeshControl : UserControl
    {
        Form1 m_mainForm = null;
        CLI.ZobObjectWrapper m_zobObject = null;
        public ZobMeshControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
        }
        public void BindEvents()
        {
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
        }
        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            if (e.newZobObject != null && e.newZobObject.HasMesh())
            {
                m_zobObject = e.newZobObject;
                this.Visible = true;
            }
            else
            {
                this.Visible = false;
            }
            UpdateValues();
            RefreshInterface();
        }

        private ManagedRenderOptions GetObjectRenderOptions()
        {
            if (m_zobObject != null)
            {
                ManagedRenderOptions o = m_zobObject.GetRenderOptions();
                if (o != null)
                {
                    return o;
                }
            }
            return null;
        }

        private void UpdateValues()
        {
            int? lightMode = GetObjectRenderOptions()?.GetLightMode();
            comboLighting.SelectedIndex = (int)lightMode.GetValueOrDefault();
            bool? zBuffer = GetObjectRenderOptions()?.GetZBuffer();
            zbuffered.Checked = (bool)zBuffer.GetValueOrDefault();
            bool? transparency = GetObjectRenderOptions()?.GetTransparency();
            transparent.Checked = (bool)transparency.GetValueOrDefault();

        }
        private void valueChanged(object sender, EventArgs e)
        {
            SetValues();
        }
        private void SetValues()
        {
        }

        private void RefreshInterface()
        {
        }

        private void comboLighting_SelectedIndexChanged(object sender, EventArgs e)
        {
            int i = comboLighting.SelectedIndex;
            GetObjectRenderOptions()?.SetLightMode(i);
        }

        private void zbuffered_CheckedChanged(object sender, EventArgs e)
        {
            GetObjectRenderOptions()?.SetZBuffer(zbuffered.Checked);
        }

        private void transparent_CheckedChanged(object sender, EventArgs e)
        {
            GetObjectRenderOptions()?.SetTransparency(transparent.Checked);
        }
    }
}
