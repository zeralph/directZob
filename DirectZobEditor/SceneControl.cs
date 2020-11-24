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
    public partial class SceneControl : UserControl
    {
        Form1 m_mainForm;
        LightManagerWrapper m_LightManagerWrapper;
        public SceneControl(Form1 f, LightManagerWrapper l)
        {
            m_mainForm = f;
            m_LightManagerWrapper = l;
            InitializeComponent();
            clearColorDialog.AnyColor = true;
            m_mainForm.OnNewScene += new EventHandler(OnSceneChanged);
            m_mainForm.OnSceneLoaded += new EventHandler(OnSceneChanged);
            trackBarAmbientIntensity.Maximum = 100;
            trackBarAmbientIntensity.Minimum = 0;
            trackBarFogDistance.Minimum = 0;
            trackBarFogDistance.Maximum = 5000;
            trackBarFogIntensity.Minimum = 0;
            trackBarFogIntensity.Maximum = 100;
            UpdateValues();
        }

        public void OnSceneChanged(object s, EventArgs e)
        {
            UpdateValues();
        }

        private void UpdateValues()
        {
            buttonAmbientColor.BackColor = ToColor(m_LightManagerWrapper.GetAmbientColor());
            buttonFogColor.BackColor = ToColor(m_LightManagerWrapper.GetFogColor());
            buttonClearColor.BackColor = ToColor(m_LightManagerWrapper.GetClearColor());
            trackBarAmbientIntensity.Value = (int)(m_LightManagerWrapper.GetAmbientIntensity() * 100);
            trackBarFogIntensity.Value = (int)(m_LightManagerWrapper.GetFogIntensity());
            trackBarFogDistance.Value = (int)(m_LightManagerWrapper.GetFogDistance());
            comboFogType.SelectedIndex = (int)(m_LightManagerWrapper.GetFogType());
        }

        private void buttonClearColor_Click(object sender, EventArgs e)
        {
            clearColorDialog.Color = buttonClearColor.BackColor;
            if ( clearColorDialog.ShowDialog() == DialogResult.OK )
            {
                buttonClearColor.BackColor = clearColorDialog.Color;
                m_LightManagerWrapper.SetClearColor(ToVector(clearColorDialog.Color));
            }
        }

        private void buttonFogColor_Click(object sender, EventArgs e)
        {
            fogColorDialog.Color = buttonFogColor.BackColor;
            if (fogColorDialog.ShowDialog() == DialogResult.OK)
            {
                buttonFogColor.BackColor = fogColorDialog.Color;
                m_LightManagerWrapper.SetFogColor(ToVector(fogColorDialog.Color));
            }
        }

        private void buttonAmbientColor_Click(object sender, EventArgs e)
        {
            ambientColorDialog.Color = buttonAmbientColor.BackColor;
            if (ambientColorDialog.ShowDialog() == DialogResult.OK)
            {
                buttonAmbientColor.BackColor = ambientColorDialog.Color;
                m_LightManagerWrapper.SetAmbientColor(ToVector(ambientColorDialog.Color));
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

        private void trackBarAmbientIntensity_Scroll(object sender, EventArgs e)
        {
            TrackBar t = (TrackBar)sender;
            float f = (float)t.Value / 100.0f;
            textAmbientIntensity.Text = f.ToString();
            m_LightManagerWrapper.SetAmbientIntensity(f);
        }

        private void trackBarFogIntensity_Scroll(object sender, EventArgs e)
        {
            TrackBar t = (TrackBar)sender;
            float f = t.Value;
            textFogIntensity.Text = f.ToString();
            m_LightManagerWrapper.SetFogIntensity(f);
        }

        private void trackBarFogDistance_Scroll(object sender, EventArgs e)
        {
            TrackBar t = (TrackBar)sender;
            float f = t.Value;
            textFogDistance.Text = f.ToString();
            m_LightManagerWrapper.SetFogDistance(f);
        }

        private void comboFogType_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox b = (ComboBox)sender;
            m_LightManagerWrapper.SetFogType(b.SelectedIndex);
        }
    }
}
