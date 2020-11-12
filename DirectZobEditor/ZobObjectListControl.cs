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
using System.IO;
using CLI;
using System.CodeDom;

namespace DirectZobEditor
{
    public partial class ZobObjectListControl : UserControl
    {
        
        public delegate void OnObjectSelectedHandler(object s, ObjectSelectionEventArg e);
        public event OnObjectSelectedHandler OnObjectSelected;
        private CLI.ZobObjectManagerWrapper m_zobObjectManagerWrapper;
        private Form1 m_mainForm = null;
        private CLI.ZobObjectWrapper m_currentSelectedZobObject = null;
        public ZobObjectListControl(Form1 f)
        {
            InitializeComponent();
            m_zobObjectManagerWrapper = new CLI.ZobObjectManagerWrapper();
            m_mainForm = f;
            UpdateTree();
            m_mainForm.OnNewScene += new EventHandler(OnSceneChanged);
            m_mainForm.OnSceneLoaded += new EventHandler(OnSceneChanged);
            m_mainForm.OnSceneUpdated += new Form1.OnSceneUpdateHandler(OnSceneUpdated);
            this.Dock = DockStyle.Fill;
        }

        public ZobObjectWrapper GetSelectedZobObject()
        {
            return m_currentSelectedZobObject;
        }

        public void RestoreTransforms()
        {
            m_zobObjectManagerWrapper.RestoreTransforms();
        }

        public void SaveTransforms()
        {
            m_zobObjectManagerWrapper.SaveTransforms();
        }

        public CLI.ZobObjectManagerWrapper GetWrapper()
        {
            return m_zobObjectManagerWrapper;
        }
        public void UpdateControl()
        {

        }
        public CLI.ZobObjectWrapper GetObjectAtCoords(int x, int y, CLI.ZobObjectManagerWrapper.eObjectTypes t)
        {
            return m_zobObjectManagerWrapper.GetObjectAtCoords(x, y, t);
        }

        public CLI.ZobObjectWrapper SelectObjectAtCoords(int x, int y, CLI.ZobObjectManagerWrapper.eObjectTypes t)
        {
            CLI.ZobObjectWrapper z = GetObjectAtCoords(x, y, t);
            if(z!=null && z.IsValid())
            {
                string s = z.GetFullNodeName();
                string[] s2 = s.Split('/');
                TreeNode n = ZobObjectTree.Nodes[0];
                for (int i=1; i<s2.Length; i++)
                {
                    n = n.Nodes[s2[i]];
                }
                ZobObjectTree.SelectedNode = n;
            }
            OnZobObjectSelectionChange(z);
            return z;
        }

        private void UpdateTree()
        {
            ZobObjectWrapper test = m_zobObjectManagerWrapper.GetRootObject();
            test = null;
            ZobTreeNode rootNode;
            if (ZobObjectTree.Nodes.Count > 0)
            {
                rootNode = (ZobTreeNode)ZobObjectTree.Nodes[0];
            }
            else
            {
                rootNode = new ZobTreeNode(m_zobObjectManagerWrapper.GetRootObject(), TreeNodeRightClick);
                ZobObjectTree.Nodes.Add(rootNode);
            }
            rootNode.zobOjectWrapper = m_zobObjectManagerWrapper.GetRootObject();
            MergeNode(rootNode);
        }

        private void MergeNode(ZobTreeNode parent)
        {
            ZobObjectWrapper z = parent.zobOjectWrapper;
            List<ZobTreeNode> l = parent.Nodes.Cast<ZobTreeNode>().ToList();
            List<ZobObjectWrapper> lz = z.GetChildren();
            for(int i=0; i<lz.Count(); i++)
            {
                ZobTreeNode n2 = new ZobTreeNode(lz[i], TreeNodeRightClick);
                int idx = l.IndexOf(n2);
                if (idx>=0)
                {
                    l.Remove(n2);
                    n2 = (ZobTreeNode)parent.Nodes[i];
                }
                else
                {
                    parent.Nodes.Add(n2);
                    Console.WriteLine("add " + n2.Name + " under " + parent);
                }
                MergeNode(n2);
            }
            for( int i=0; i<l.Count; i++)
            {
                parent.Nodes.Remove(l[i]);
            }
            //for(int i=0; i<z.)
        }
        private void ZobObjectTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            CLI.ZobObjectWrapper i = null;
            if (ZobObjectTree.SelectedNode != null && ZobObjectTree.SelectedNode.Name != "root")
            {
                ZobTreeNode n = (ZobTreeNode)ZobObjectTree.SelectedNode;
                OnZobObjectSelectionChange(n.zobOjectWrapper);
            }
            OnZobObjectSelectionChange(i);
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
                    m_currentSelectedZobObject.GetRenderOptions().SetTransparency(false);
                }
                ev.previousZobObject = m_currentSelectedZobObject;
                m_currentSelectedZobObject = newZobObject;
                if (m_currentSelectedZobObject != null)
                {
                    //m_currentSelectedZobObject.GetRenderOptions().SetColorization(255, 0, 255);
                    //m_currentSelectedZobObject.GetRenderOptions().EnableColorization(true);
                    m_currentSelectedZobObject.GetRenderOptions().SetTransparency(true);
                }
                ev.newZobObject = newZobObject;
                handler(this, ev);
            }
            UpdateTree();
        }

        private void ZobObjectTree_MouseClick(object sender, MouseEventArgs e)
        {
            //TreeNode n = ZobObjectTree.SelectedNode;
            TreeNode n = ZobObjectTree.GetNodeAt(e.X, e.Y);
            ZobObjectTree.SelectedNode = n;
            if (e.Button == MouseButtons.Right && n != null && n.ContextMenuStrip != null)
            {
                n.ContextMenuStrip.Show();
            }
        }

        private void AddToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
        }

        private void RemoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
        }

        private void OnSceneUpdated(object s, Form1.SceneUpdateEventArg e)
        {
            UpdateTree();
            if(e.type == Form1.SceneUpdateType.objectSelected)
            {
                OnZobObjectSelectionChange(e.zobObject);
            }
            else if (e.type == Form1.SceneUpdateType.objectDeselected)
            {
                OnZobObjectSelectionChange(null);
            }
        }
        private void OnSceneChanged(object s, EventArgs e)
        {
            ZobObjectWrapper root = m_zobObjectManagerWrapper.GetRootObject();
            ZobTreeNode rootNode = new ZobTreeNode(root, null);
            ZobObjectTree.Nodes.Add(rootNode);
            m_currentSelectedZobObject = null;
            ZobObjectTree.Nodes.Clear();
            string p = Application.StartupPath;
            CLI.CameraManagerWrapper cm = m_mainForm.GetCameraControl().GetWrapper();
            //create an editor camera
            cm.CreateEditorCamera();
        }

        private void BtnForceRefresh_Click(object sender, EventArgs e)
        {
            //ZobObjectTree.Nodes.Clear();
            UpdateTree();
        }

        private void TreeNodeRightClick_Opening(object sender, CancelEventArgs e)
        {
            TreeNode n = ZobObjectTree.SelectedNode;
            if (n != null)
            {
                ContextMenuStrip c = (ContextMenuStrip)sender;
                if ( n.Name == "root")
                {
                    c.Items[0].Enabled = true;
                    c.Items[1].Enabled = false;
                    c.Items[2].Enabled = false;
                }
                else
                {
                    c.Items[0].Enabled = true;
                    c.Items[1].Enabled = true;
                    c.Items[2].Enabled = true;
                }
            }
        }

        private void TreeNodeRightClick_Opened(object sender, EventArgs e)
        {
        }

        private void TreeNodeRightClick_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            //ZobObjectTree.Nodes.Clear();
            //UpdateTree();
        }

        private void ZoomToStripMenuItem_Click(object sender, EventArgs e)
        {
            ZobTreeNode n = (ZobTreeNode)ZobObjectTree.SelectedNode;
            if (n != null)
            {
                CLI.ZobObjectWrapper z = n.zobOjectWrapper;
                if(z != null)
                {
                    CLI.ManagedVector3 target = z.GetTransform();
                    //TODO : camer wrapper et setlookat
                    m_mainForm.GetCameraControl().GetWrapper().SetLookAt(target);
                }
            }
        }
    }

    public class ZobObjectTree
    {
        public string name { get; set; }
        public ZobObjectTree[] children { get; set; }
    };

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
    public class ZobTreeNode : TreeNode
    {
        private ZobObjectWrapper _zobOjectWrapper;
        public ZobObjectWrapper zobOjectWrapper
        {
            get { return _zobOjectWrapper; }
            set { _zobOjectWrapper = value; }
        }
        public ZobTreeNode(ZobObjectWrapper zobObject, ContextMenuStrip menu):base()
        {
            this.Name = zobObject.GetName();
            this.Text = this.Name;
            this._zobOjectWrapper = zobObject;
            this.ContextMenuStrip = menu;
        }

        public static bool operator ==(ZobTreeNode z1, ZobTreeNode z2) 
        {
            if ((Object)z2 == null)
            {
                return false;
            }
            return z1.Name == z2.Name; 
        }
        public static bool operator !=(ZobTreeNode z1, ZobTreeNode z2) 
        { 
            if((Object)z2 == null)
            {
                return true;
            }
            return z1.Name != z2.Name; ; 
        }

        public override bool Equals(Object o)
        {
            if(o == null)
            { 
                return false;
            }
            ZobTreeNode n = (ZobTreeNode)o;
            return (ZobTreeNode)this == (ZobTreeNode)o;
        }
    }
}
