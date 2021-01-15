using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class PhysicControl : UserControl
    {
        private Form1 m_mainForm;
        private CLI.ZobObjectWrapper m_currentZobObjectWrapper = null;
        private int m_forceUpdateOnNextFrame = 0;
        public PhysicControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
            this.Visible = false;
        }
        public void BindEvents()
        {
            ZobObjectListControl z = m_mainForm.GetZobObjectListControl();
            z.OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnSceneUpdated += new Form1.OnSceneUpdateHandler(OnSceneUpdated);
            m_mainForm.GetEngineWindow().OnBeginFrame += new EventHandler(OnBeginFrame);
        }

        private void OnBeginFrame(object s, EventArgs e)
        {
            if (m_forceUpdateOnNextFrame > 0)
            {
                m_forceUpdateOnNextFrame--;
                if (m_forceUpdateOnNextFrame == 0)
                {
                    SetValues();
                    PhysicsGroupBox.Enabled = true;
                }
            }
        }
        private void Physics_Enter(object sender, EventArgs e)
        {

        }
        private void OnSceneUpdated(object sender, Form1.SceneUpdateEventArg e)
        {
        }
        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            m_currentZobObjectWrapper = e.newZobObject;
            if (e.newZobObject != null)
            {

                this.Visible = true;
                SetValues();
            }
            else
            {
                this.Visible = false;
            }

        }

        private void SetValues()
        {
            if (m_currentZobObjectWrapper != null)
            {
                string type = "";
                string shapeType = "";
                m_currentZobObjectWrapper.GetPhysicComponentInfo(ref type, ref shapeType);
                comboPhysicType.SelectedItem = type;
                comboBoxPhysicShape.SelectedItem = shapeType;

                float radius = 0;
                float height = 0;
                float hx = 0;
                float hy = 0;
                float hz = 0;
                string s = "";
                m_currentZobObjectWrapper.GetPhysicComponentShapeInfo(ref radius, ref height, ref hx, ref hy, ref hz, ref s);
                textRadius.Text = radius.ToString();
                textBHeight.Text = height.ToString();
                textHalExtendsX.Text = hx.ToString();
                textHalExtendsY.Text = hy.ToString();
                textHalExtendsZ.Text = hz.ToString();
                float b = 0;
                float m = 0;
                float f = 0;
                float rr = 0;
                m_currentZobObjectWrapper.GetPhysicComponentColliderInfo(ref b, ref f, ref m, ref rr);
                textBounciness.Text = b.ToString();
                textFrictionCoeff.Text = f.ToString();
                textMassDensity.Text = m.ToString();
                textRollingResistance.Text = rr.ToString();
                bool bScaleWithObject = false;
                m_currentZobObjectWrapper.GetPhysicComponentScaleWithObject(ref bScaleWithObject);
                checkBoxScaleWithObject.Checked = bScaleWithObject;
            }
        }

        private void UpdateValues()
        {
            string type = (string)comboPhysicType.SelectedItem;
            string shapeType = (string)comboBoxPhysicShape.SelectedItem;
            m_currentZobObjectWrapper.SetPhysicComponentInfo(type, shapeType);
            textHalExtendsX.Text = textHalExtendsX.Text.Replace('.', ',');
            textHalExtendsY.Text = textHalExtendsY.Text.Replace('.', ',');
            textHalExtendsZ.Text = textHalExtendsZ.Text.Replace('.', ',');
            textRadius.Text = textRadius.Text.Replace('.', ',');
            textBHeight.Text = textBHeight.Text.Replace('.', ',');
            textBounciness.Text = textBounciness.Text.Replace('.', ',');
            textFrictionCoeff.Text = textFrictionCoeff.Text.Replace('.', ',');
            textMassDensity.Text = textMassDensity.Text.Replace('.', ',');
            textRollingResistance.Text = textRollingResistance.Text.Replace('.', ',');
            float radius = 0;
            float height = 0;
            float hx = 0;
            float hy = 0;
            float hz = 0;
            string s = "";
            m_currentZobObjectWrapper.GetPhysicComponentShapeInfo(ref radius, ref height, ref hx, ref hy, ref hz, ref s);
            float.TryParse(textHalExtendsX.Text, out hx);         
            float.TryParse(textHalExtendsY.Text, out hy);           
            float.TryParse(textHalExtendsZ.Text, out hz);          
            float.TryParse(textRadius.Text, out radius);       
            float.TryParse(textBHeight.Text, out height);
            m_currentZobObjectWrapper.SetPhysicComponentShapeInfo(radius, height, hx, hy, hz, s);
            float b = 0;
            float m = 0;
            float f = 0;
            float rr = 0;
            m_currentZobObjectWrapper.GetPhysicComponentColliderInfo(ref b, ref f, ref m, ref rr);
            float.TryParse(textBounciness.Text, out b);
            float.TryParse(textFrictionCoeff.Text, out f);
            float.TryParse(textMassDensity.Text, out m);
            float.TryParse(textRollingResistance.Text, out rr);
            b = Math.Max(Math.Min(b, 1), 0);
            m_currentZobObjectWrapper.SetPhysicComponentColliderInfo(b, f, m, rr);
            m_forceUpdateOnNextFrame = 2;
            PhysicsGroupBox.Enabled = false;
            m_currentZobObjectWrapper.SetPhysicComponentScaleWithObject(checkBoxScaleWithObject.Checked);
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            UpdateValues();
        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void checkBoxScaleWithObject_CheckedChanged(object sender, EventArgs e)
        {
            
        }

        private void comboPhysicType_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
