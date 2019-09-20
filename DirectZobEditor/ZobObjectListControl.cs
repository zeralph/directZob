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
        }

        public CLI.ZobObjectManagerWrapper GetWrapper()
        {
            return m_zobObjectManagerWrapper;
        }
        public void UpdateControl()
        {

        }

        public bool SelectObjectAtCoords(int x, int y)
        {
            CLI.ZobObjectWrapper z = m_zobObjectManagerWrapper.GetObjectAtCoords(x, y);
            bool bRet = false;
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
                bRet = true;
            }
            OnZobObjectSelectionChange(z);
            return bRet;
        }

        private void UpdateTree()
        {
            string s = m_zobObjectManagerWrapper.GetZobObjectList();
            ZobObjectTree z = JsonConvert.DeserializeObject<ZobObjectTree>(s);
            //ZobObjectTree.Nodes.Clear();
            AddNode(z, null);
            ZobObjectTree.ExpandAll();
        }

        private string GetFullNodeName(TreeNode n)
        {
            if (n != null)
            {
                string s = "";
                while (n.Name != "root")
                {
                    s = n.Name + "/" + s;
                    n = n.Parent;
                }
                if (s.Length > 0)
                {
                    s = s.Substring(0, s.Length - 1);
                }
                return s;
            }
            return "";
        }

        private void AddNode(ZobObjectTree z, TreeNode p)
        {
            if (z != null)
            {
                TreeNode n = null;
                if (p != null)
                {
                    if (!p.Nodes.ContainsKey(z.name))
                    {
                        n = p.Nodes.Add(z.name, z.name);
                        n.ContextMenuStrip = TreeNodeRightClick;
                    }
                    else
                    {
                        n = p.Nodes[z.name];
                    }
                }
                else
                {
                    if (!ZobObjectTree.Nodes.ContainsKey(z.name))
                    {
                        n = ZobObjectTree.Nodes.Add(z.name, z.name);
                        n.ContextMenuStrip = TreeNodeRightClick;
                    }
                    else
                    {
                        n = ZobObjectTree.Nodes[z.name];
                    }
                }
                if(z.children != null)
                {
                    for (int i = 0; i < z.children.Length; i++)
                    {
                        AddNode(z.children[i], n);
                    }
                }
            }
        }

        private void ZobObjectTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            CLI.ZobObjectWrapper i = null;
            if (ZobObjectTree.SelectedNode != null && ZobObjectTree.SelectedNode.Name != "root")
            {
                TreeNode n = ZobObjectTree.SelectedNode;
                string s = GetFullNodeName(n);
                i = m_zobObjectManagerWrapper.GetZobObject(s);
            }
            OnZobObjectSelectionChange(i);
        }

        private void OnZobObjectSelectionChange(CLI.ZobObjectWrapper newZobObject)
        {
            OnObjectSelectedHandler handler = OnObjectSelected;
            if (null != handler)
            {
                ObjectSelectionEventArg ev = new ObjectSelectionEventArg();
                ev.previousZobObject = m_currentSelectedZobObject;
                m_currentSelectedZobObject = newZobObject;
                ev.newZobObject = newZobObject;
                handler(this, ev);
            }
            CLI.ZobObjectWrapper gizmos = m_zobObjectManagerWrapper.GetEditorGizmos();
            m_zobObjectManagerWrapper.Reparent(gizmos, newZobObject);
            //ZobObjectTree.Nodes.Clear();
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
            TreeNode n = ZobObjectTree.SelectedNode;
            if (n != null)
            {
                m_zobObjectManagerWrapper.AddZobObject(GetFullNodeName(n));
                ZobObjectTree.Nodes.Clear();
                UpdateTree();
            }
        }

        private void RemoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode n = ZobObjectTree.SelectedNode;
            if(n!=null)
            {
                m_zobObjectManagerWrapper.RemoveZobObject(GetFullNodeName(n));
                ZobObjectTree.Nodes.Clear();
                UpdateTree();
            }
        }

        private void OnSceneChanged(object s, EventArgs e)
        {
            ZobObjectTree.Nodes.Clear();
            string p = Application.StartupPath;
            p = Path.Combine(p, "..\\..\\..\\resources\\editor\\");
            m_zobObjectManagerWrapper.CreateEditorGizmos(p);
            UpdateTree();
        }

        private void BtnForceRefresh_Click(object sender, EventArgs e)
        {
            ZobObjectTree.Nodes.Clear();
            UpdateTree();
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
}
