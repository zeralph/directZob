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
                string s = m_currentZobObjectWrapper.GetPhysicComponentType();
                comboPhysicType.SelectedItem = s;
                s = m_currentZobObjectWrapper.GetPhysicComponentShapeType();
                comboBoxPhysicShape.SelectedItem = s;
                textRadius.Text = m_currentZobObjectWrapper.GetPhysicComponentShapeRadius().ToString();
                textBHeight.Text = m_currentZobObjectWrapper.GetPhysicComponentShapeHeight().ToString();
                CLI.ManagedVector3 v = m_currentZobObjectWrapper.GetPhysicComponentShapeHalfExtends();
                textHalExtendsX.Text = v.x.ToString();
                textHalExtendsY.Text = v.y.ToString();
                textHalExtendsZ.Text = v.z.ToString();
            }
        }

        private void UpdateValues()
        {
            string s = (string)comboPhysicType.SelectedItem;
            m_currentZobObjectWrapper.SetPhysicComponentType(s);
            s = (string)comboBoxPhysicShape.SelectedItem;
            m_currentZobObjectWrapper.SetPhysicComponentShapeType(s);

            textHalExtendsX.Text = textHalExtendsX.Text.Replace('.', ',');
            textHalExtendsY.Text = textHalExtendsY.Text.Replace('.', ',');
            textHalExtendsZ.Text = textHalExtendsZ.Text.Replace('.', ',');
            textRadius.Text = textRadius.Text.Replace('.', ',');
            textBHeight.Text = textBHeight.Text.Replace('.', ',');
            float f;
            CLI.ManagedVector3 v = m_currentZobObjectWrapper.GetPhysicComponentShapeHalfExtends();
            float radius = m_currentZobObjectWrapper.GetPhysicComponentShapeRadius();
            float height = m_currentZobObjectWrapper.GetPhysicComponentShapeHeight();
            if (float.TryParse(textHalExtendsX.Text, out f))
            {
                v.x = f;
            }
            if (float.TryParse(textHalExtendsY.Text, out f))
            {
                v.y = f;
            }
            if (float.TryParse(textHalExtendsZ.Text, out f))
            {
                v.z = f;
            }
            if (float.TryParse(textRadius.Text, out f))
            {
                radius = f;
            }
            if (float.TryParse(textBHeight.Text, out f))
            {
                height = f;
            }
            m_currentZobObjectWrapper.SetPhysicComponentShapeRadius(radius);
            m_currentZobObjectWrapper.SetPhysicComponentShapeHeight(height);
            m_currentZobObjectWrapper.SetPhysicComponentShapeHalfExtends(v.x, v.y, v.z);
            m_forceUpdateOnNextFrame = 2;
            PhysicsGroupBox.Enabled = false;
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            UpdateValues();
        }
    }
}
