using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;
using CLI;

namespace DirectZobEditor
{
    public class ObjectSelectionEventArg : EventArgs
    {
        public CLI.ZobObjectWrapper previousZobObject;
        public CLI.ZobObjectWrapper newZobObject;
    }
    public class ObjectModificationEventArg : EventArgs
    {
        public ManagedVector3 t;
        public ManagedVector3 r;
        public ManagedVector3 s;
    }
    public partial class ZobObjectListControl : UserControl
    {
        
        public delegate void OnObjectSelectedHandler(object s, ObjectSelectionEventArg e);
        public event OnObjectSelectedHandler OnObjectSelected;
        private Form1 m_mainForm = null;
        private CLI.ZobObjectWrapper m_currentSelectedZobObject = null;

        public ZobObjectListControl(Form1 f)
        {
            InitializeComponent();
            //m_zobObjectManagerWrapper = new CLI.ZobObjectManagerWrapper();
            //UserControl c = m_zobObjectManagerWrapper.GetUserControl();
            //splitContainer1.Panel2.Controls.Add(c);
            m_mainForm = f;
            this.Dock = DockStyle.Fill;
        }
        public void BindEvents()
        {
        }

        public void RestoreTransforms()
        {
            //m_zobObjectManagerWrapper.RestoreTransforms();
        }

        public void SaveTransforms()
        {
            //m_zobObjectManagerWrapper.SaveTransforms();
        }

        private void OnZobObjectSelectionChange(CLI.ZobObjectWrapper newZobObject)
        {
            if(newZobObject == null)
            { 
                return;
            }
            OnObjectSelectedHandler handler = OnObjectSelected;
            if (null != handler)
            {
                ObjectSelectionEventArg ev = new ObjectSelectionEventArg();
                if(m_currentSelectedZobObject != null)
                {
                    //m_currentSelectedZobObject.GetRenderOptions().EnableColorization(false);
                    //m_currentSelectedZobObject.GetRenderOptions().SetTransparency(false);
                }
                ev.previousZobObject = m_currentSelectedZobObject;
                m_currentSelectedZobObject = newZobObject;
                if (m_currentSelectedZobObject != null)
                {
                    //m_currentSelectedZobObject.GetRenderOptions().SetColorization(255, 0, 255);
                    //m_currentSelectedZobObject.GetRenderOptions().EnableColorization(true);
                    //m_currentSelectedZobObject.GetRenderOptions().SetTransparency(true);
                }
                ev.newZobObject = newZobObject;
                handler(this, ev);
            }
            //m_zobObjectManagerWrapper.Refresh();
        }

        private void AddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
            ZobTreeNode n = (ZobTreeNode)ZobObjectTree.SelectedNode;
            if (n != null)
            {
                ZobObjectWrapper z = m_zobObjectManagerWrapper.AddZobObject(n.zobOjectWrapper);
                //ZobObjectTree.Nodes.Clear();
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.objectAdded;
                ev.zobObject = z;
                m_mainForm.PropagateSceneUpdateEvent(ev);
                UpdateTree();
            }
            */
        }

        private void RemoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
            ZobTreeNode n = (ZobTreeNode)ZobObjectTree.SelectedNode;
            if(n!=null)
            {
                m_zobObjectManagerWrapper.RemoveZobObject(n.zobOjectWrapper);
                m_currentSelectedZobObject = null;
                //ZobObjectTree.Nodes.Clear();
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.objectRemoved;
                ev.zobObject = n.zobOjectWrapper;
                m_mainForm.PropagateSceneUpdateEvent(ev);
                UpdateTree();
            }
            */
        }
 
        private void btnAddZobObject_Click(object sender, EventArgs e)
        {
            LoadZobObject();
        }

        public void LoadZobObject()
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                //openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "xml files (*.xml)|*.xml|zob object (*.dzo)|*.dzo";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(openFileDialog.FileName) + "\\";
                    string file = openFileDialog.SafeFileName;
                    m_mainForm.GetDirectZobWrapper().LoadZobObject(path, file);
                }
            }
        }

        private void saveAsFactoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                //openFileDialog.InitialDirectory = m_path;
                saveFileDialog.Filter = "xml files (*.xml)|*.xml|zob object (*.dzo)|*.dzo";
                saveFileDialog.FilterIndex = 2;
                saveFileDialog.RestoreDirectory = true;
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(saveFileDialog.FileName) + "\\";
                    string file = saveFileDialog.FileName;
                    m_currentSelectedZobObject.SaveToFactoryFile(file);
                }
            }
        }

        private void duplicateToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void AddBehaviorToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            ToolStripMenuItem ts = (ToolStripMenuItem)sender;
            ts.DropDownItems.Clear();
            List<String> lb = ZobBehaviorWrapper.GetBehaviors();
            for(int i=0; i<lb.Count; i++)
            {
                ts.DropDownItems.Add(lb[i]);
                ts.DropDownItems[i].Click += AddBehaviorToolStripMenuItem_Click;
            }
        }

        private void AddBehaviorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_currentSelectedZobObject != null)
            {
                ToolStripMenuItem ts = (ToolStripMenuItem)sender;
                ZobBehaviorWrapper.CreateBehavior(m_currentSelectedZobObject, ts.Text);
            }
        }
    }
}
