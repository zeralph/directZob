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
        public PhysicControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
            this.Visible = false;
            ZobObjectListControl z = m_mainForm.GetZobObjectListControl();
            z.OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
            m_mainForm.OnSceneUpdated += new EventHandler(OnSceneUpdated);
        }

        private void Physics_Enter(object sender, EventArgs e)
        {
            comboPhysicType.SelectedIndex = 0;
        }
        private void OnSceneUpdated(object sender, EventArgs e)
        {
        }
        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            CLI.ZobObjectWrapper oldObject = e.previousZobObject;
            m_currentZobObjectWrapper = e.newZobObject;
            if (e.newZobObject != null)
            {

                this.Visible = true;
            }
            else
            {
                this.Visible = false;
            }

        }

        private void comboPhysicType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(m_currentZobObjectWrapper != null)
            {
                m_currentZobObjectWrapper.SetPhysicComponent(1);
            }
        }
    }
}
