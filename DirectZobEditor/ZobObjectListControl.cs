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
            if (lz != null)
            {
                for (int i = 0; i < lz.Count(); i++)
                {
                    ZobTreeNode n2 = new ZobTreeNode(lz[i], TreeNodeRightClick);
                    int idx = l.IndexOf(n2);
                    if (idx >= 0)
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
            UpdateTree();
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

        private void ZobObjectTree_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop((TreeNode)e.Item, DragDropEffects.Move);
        }

        private void ZobObjectTree_DragDrop(object sender, DragEventArgs e)
        {
            // Retrieve the client coordinates of the drop location.
            Point targetPoint = ZobObjectTree.PointToClient(new Point(e.X, e.Y));

            // Retrieve the node at the drop location.
            ZobTreeNode targetNode = (ZobTreeNode)ZobObjectTree.GetNodeAt(targetPoint);

            // Retrieve the node that was dragged.
            ZobTreeNode draggedNode = (ZobTreeNode)e.Data.GetData(typeof(ZobTreeNode));

            ZobObjectWrapper toMoveObj = draggedNode.zobOjectWrapper;
            ZobObjectWrapper targetObj = targetNode.zobOjectWrapper;

            m_zobObjectManagerWrapper.Reparent(toMoveObj, targetObj);
            /*
            // Confirm that the node at the drop location is not 
            // the dragged node or a descendant of the dragged node.
            if (!draggedNode.Equals(targetNode) && !ContainsNode(draggedNode, targetNode))
            {
                if (e.Effect == DragDropEffects.Move)
                {
                    draggedNode.Remove();
                    targetNode.Nodes.Add(draggedNode);
                }
                targetNode.Expand();
            }
            */
            UpdateTree();
            UpdateTree();
            //ZobTreeNode nz = FindZobTreeNode(targetObj, (ZobTreeNode)ZobObjectTree.Nodes[0]);
            //ExpandTo(nz);
            targetNode.Expand();
        }
/*
        private ZobTreeNode FindZobTreeNode(ZobObjectWrapper z, ZobTreeNode startNode)
        {
            if (startNode.zobOjectWrapper.GetName() == z.GetName())
            {
                return startNode;
            }
            else
            {
                for (int i = 0; i < startNode.Nodes.Count; i++)
                {
                    return FindZobTreeNode(z, (ZobTreeNode)startNode.Nodes[i]);
                }
            }
            return (ZobTreeNode)null;
        }

        private void ExpandTo(ZobTreeNode n)
        {
            if (n != null && n.Parent != null)
            {
                Console.WriteLine("expand " + n.Parent);
                n.Parent.Expand();
                ExpandTo((ZobTreeNode)n.Parent);
            }
        }
*/
        private bool ContainsNode(TreeNode node1, TreeNode node2)
        {
            if (node2.Parent == null) return false;
            if (node2.Parent.Equals(node1)) return true;
            return ContainsNode(node1, node2.Parent);
        }
        private void ZobObjectTree_DragLeave(object sender, EventArgs e)
        {
            //ZobTreeNode s = (ZobTreeNode)sender;
            UpdateTree();
        }

        private void ZobObjectTree_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = e.AllowedEffect;
            //ZobTreeNode s = (ZobTreeNode)sender;
        }

        private void ZobObjectTree_DragOver(object sender, DragEventArgs e)
        {
            // Retrieve the client coordinates of the mouse position.
            Point targetPoint = ZobObjectTree.PointToClient(new Point(e.X, e.Y));

            // Select the node at the mouse position.
            ZobObjectTree.SelectedNode = ZobObjectTree.GetNodeAt(targetPoint);
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
            if (zobObject != null && zobObject.IsFromFactoryFile())
            {
                this.ForeColor = Color.Blue;
            }
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
        public override int GetHashCode()
        {
            using (System.Security.Cryptography.MD5 md5 = MD5.Create())
            {
                return (int)new BigInteger(md5.ComputeHash(Encoding.Default.GetBytes(this.Name)));
            }
        }
        public override string ToString()
        {
            return "ZobTreeNode_"+this.Name;
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
