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
    public partial class ZobLightControl : UserControl
    {
        Form1 m_mainForm = null;
        ZobLightWrapper m_light = null;
        public ZobLightControl(Form1 f)
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
            if(e.newZobObject != null)
            {
                m_light = e.newZobObject as ZobLightWrapper;
                if(m_light != null)
                {
                    this.Visible = true;
                }
                else
                {
                    this.Visible = false;
                }
            }
            else
            {
                this.Visible = false;
            }
            UpdateValues();
            RefreshInterface();
        }

        private void buttonColor_Click(object sender, EventArgs e)
        {
            lightColorDialog.Color = buttonColor.BackColor;
            if (lightColorDialog.ShowDialog() == DialogResult.OK)
            {
                buttonColor.BackColor = lightColorDialog.Color;
                m_light.SetColor(ToVector(lightColorDialog.Color));
            }
        }

        private void SetValues()
        {
            float f;
            lightDistance.Text = lightDistance.Text.Replace('.', ',');
            if (float.TryParse(lightDistance.Text, out f))
            {
                m_light.SetDistance(f);
            }
            lightIntensity.Text = lightIntensity.Text.Replace('.', ',');
            if (float.TryParse(lightIntensity.Text, out f))
            {
                m_light.SetIntensity(f);
            }
            lightAngle.Text = lightAngle.Text.Replace('.', ',');
            if (float.TryParse(lightAngle.Text, out f))
            {
                m_light.SetSpotAngle(f);
            }
        }

        private void UpdateValues()
        {
            if(m_light != null)
            {
                ManagedVector3 v = m_light.GetColor();
                Color c = ToColor(v);
                buttonColor.BackColor = c;
                comboBoxLightType.SelectedIndex = m_light.GetType();
                lightDistance.Text = m_light.GetDistance().ToString();
                lightIntensity.Text = m_light.GetIntensity().ToString();
                lightAngle.Text = m_light.GetSpotAngle().ToString();
            }
        }

        private Color ToColor(CLI.ManagedVector3 v)
        {
            return Color.FromArgb((int)(v.x * 255), (int)(v.y * 255), (int)(v.z * 255));
        }

        private CLI.ManagedVector3 ToVector(Color c)
        {
            CLI.ManagedVector3 v = new ManagedVector3();
            v.x = (float)c.R / 255.0f;
            v.y = (float)c.G / 255.0f;
            v.z = (float)c.B / 255.0f;
            return v;
        }

        private void comboBoxLightType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_light != null)
            {
                int i = ((ComboBox)sender).SelectedIndex;
                m_light.SetType(i);
                RefreshInterface();
            }
        }

        private void valueChanged(object sender, EventArgs e)
        {
            SetValues();
        }

        private void RefreshInterface()
        {
            int i = comboBoxLightType.SelectedIndex;
            if (i == 0) //Point
            {
                lightAngle.Enabled = false;
                lightDistance.Enabled = true;
                lightIntensity.Enabled = true;
            }
            else if (i == 1) //spot
            {
                lightAngle.Enabled = true;
                lightDistance.Enabled = true;
                lightIntensity.Enabled = true;
            }
            else if (i == 2) //dir
            {
                lightAngle.Enabled = false;
                lightDistance.Enabled = false;
                lightIntensity.Enabled = true;
            }
        }
    }
}
