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
            comboPhysicType.SelectedIndex = 0;
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
            }
        }

        private void UpdateValues()
        {
            string s = (string)comboPhysicType.SelectedItem;
            m_currentZobObjectWrapper.SetPhysicComponentType(s);
            s = (string)comboBoxPhysicShape.SelectedItem;
            m_currentZobObjectWrapper.SetPhysicComponentShapeType(s);
            m_forceUpdateOnNextFrame = 2;
            PhysicsGroupBox.Enabled = false;
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            UpdateValues();
        }
    }
}
