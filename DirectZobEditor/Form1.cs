using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Resources;
using System.Threading;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace DirectZobEditor
{
    public partial class Form1 : Form
    {
        private string workspace = "D://_Git//directZob//resources//";
        enum eplayMode
        {
            ePlayMode_play=0,
            ePlayMode_pause,
            ePlayMode_stop,
        }
        #region eventArgs
        public enum SceneUpdateType
        {
            unknown = 0,
            loadScene,
            createCamera,
            createLight,
            createSprite,
            stopPhysics,
            updateAfterEngine,
            objectAdded,
            objectRemoved,
            objectMoved,
            objectSelected,
            objectDeselected,
        }
        public class SceneUpdateEventArg : EventArgs
        {
            public SceneUpdateType type = SceneUpdateType.unknown;
            public CLI.ZobObjectWrapper zobObject = null;
        }
        #endregion
        private bool m_running = false;
        public static Form1 m_mainForm = null;
        public static Form1 GetMainForm() { return m_mainForm; }
        public event EventHandler OnNewScene;
        public event EventHandler OnSceneLoadedEventHandler;
        public event EventHandler OnSceneSaved;

        public bool m_canBeSafelyClosed = false;

        public delegate void OnSceneUpdateHandler(object s, SceneUpdateEventArg e);
        public event OnSceneUpdateHandler OnSceneUpdated;

        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.ZobObjectManagerWrapper m_zobObjectManagerWrapper;

        private Thread m_engineThread;
        //private ZobObjectListControl m_zobObjectList;
        private EngineWindow m_engineWindow;
        private bool m_snap = false;
        private bool m_ctrlPressed = false;

        private string m_path = "C:\\";
        private string m_file;

        private string[] m_events;
        private eplayMode m_playMode = eplayMode.ePlayMode_stop;

        public CLI.engineCallback onSceneLoadedCallback;
        public delegate void OnSceneLoaded();
        public OnSceneLoaded OnSceneLoadedDelegate;

        public Form1()
        {
            Font f = new Font("Microsoft Tai Le", 8);
            this.Font = f;
            m_mainForm = this;
            m_running = true;
            InitializeComponent();
            m_engineWindow = new EngineWindow(this);
            PictureBox p = m_engineWindow.GetEngineRenderwindow();
            System.Resources.ResourceManager resources = new ResourceManager("DirectZobEditor.Properties.Resources", typeof(Form1).Assembly);
            m_directZobWrapper = new CLI.DirectZobWrapper(this.ZobObjectListPanel, this.propertiesPanel, this.globalPropertiesPanel, m_engineWindow.GetEngineRenderwindow(), resources);
            m_directZobWrapper.Init(p.Width, p.Height);
            CLI.ZobObjectManagerWrapper.OnObjectSelectedEvent += new CLI.ZobObjectManagerWrapper.OnObjectSelected(OnObjectSelected);
            m_zobObjectManagerWrapper = m_directZobWrapper.GetZobObjectManagerWrapper();
            Application.ApplicationExit += new EventHandler(this.OnApplicationExit);
            this.Load += new EventHandler(this.Onloaded);
            onSceneLoadedCallback = new CLI.engineCallback(onSceneLoadedCallbackMethod);
            OnSceneLoadedDelegate = new OnSceneLoaded(OnSceneLoadedMethod);
            //CLI.DirectZobWrapperEvents.OnNewSceneEvent += DirectZobWrapperEvents_OnNewSceneEvent;
            OnSceneSaved += Form1_OnSceneSaved;
            OnNewScene += Form1_OnNewScene;
            this.propertiesPanel.MinimumSize = new Size(300, 500);

            this.WindowState = FormWindowState.Maximized;
            EngineRendererPanel.Controls.Add(m_engineWindow);
            m_directZobWrapper.NewScene(workspace);
            EventHandler handler = OnNewScene;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
            bBoxToolStripMenuItem.Checked = false;
            cameraToolStripMenuItem.Checked = false;
            textToolStripMenuItem.Checked = false;
            physicsToolStripMenuItem.Checked = false;
            m_engineThread = new Thread(RunEngineThread);
            m_engineThread.Start();
            SetSnap("None");

        }

        private void Form1_OnNewScene(object sender, EventArgs e)
        {
            toolStripStatusScanePath.Text = GetDirectZobWrapper().GetResourcePath();
        }

        private void Form1_OnSceneSaved(object sender, EventArgs e)
        {
            toolStripStatusScanePath.Text = GetDirectZobWrapper().GetResourcePath();
        }

        private void OnObjectSelected(CLI.ZobObjectWrapper z)
        {
            if(z != null)
            {

            }
            //throw new NotImplementedException();
        }

        public string Getpath()
        {
            return m_path;
        }

        public bool Snap()
        {
            return m_snap;
        }
        public void UpdateAfterEngine()
        {
            this.Invoke(new Action(()=> 
            {
                UpdateCameraList();
                //           m_zobObjectList.UpdateControl();

                m_events = m_directZobWrapper.GetEventsAndClear();
                UpdateEventsLog();
                //textLog.Invoke(UpdateLogWindowDelegate);
                //EngineRender.Invoke(UpdateEngineWindowDelegate);
                if (m_directZobWrapper.IsPhysicPlaying())
                {
                    Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                    ev.type = Form1.SceneUpdateType.updateAfterEngine;
                    PropagateSceneUpdateEvent(ev);
                }
            }));
        }

        public CLI.EngineWrapper GetEngineWrapper()
        {
            return m_directZobWrapper.GetEngineWrapper();
        }

        private void UpdateCameraList()
        {
            string s = m_directZobWrapper.GetCurrentCameraName();
            string[] c = m_directZobWrapper.GetCameraList();
            if (toolStripComboBoxCurrentCamera.Items.Count != c.Count())
            {
                toolStripComboBoxCurrentCamera.Items.Clear();
                for (int i = 0; i < c.Count(); i++)
                {
                    toolStripComboBoxCurrentCamera.Items.Add(c[i]);
                    if (c[i] == s)
                    {
                        toolStripComboBoxCurrentCamera.SelectedIndex = i;
                    }
                }
            }
        }

        private void UpdateEventsLog()
        {
            int l = m_events.Count();
            for (int i = 0; i < l; i++)
            {
                LogEvent json = JsonConvert.DeserializeObject<LogEvent>(m_events[i]);
                if (json != null)
                {
                    if (json.type == 0)
                    {
                        AppendText(textLog, json.data + "\n", Color.Blue);
                    }
                    else if (json.type == 1)
                    {
                        AppendText(textLog, json.data + "\n", Color.Orange);
                    }
                    else if (json.type == 2)
                    {
                        AppendText(textLog, json.data + "\n", Color.Red);
                        MessageBox.Show(json.data, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
            if (l > 0)
            {
                textLog.ScrollToCaret();
            }
        }

        private void AppendText(RichTextBox box, string text, Color color)
        {
            box.SelectionStart = box.TextLength;
            box.SelectionLength = 0;

            box.SelectionColor = color;
            box.AppendText(text);
            box.SelectionColor = box.ForeColor;
        }
      

        public bool IsCtrlPressed()
        {
            return m_ctrlPressed;
        }

        public EngineWindow GetEngineWindow()
        {
            return m_engineWindow;
        }

        public CLI.DirectZobWrapper GetDirectZobWrapper()
        {
            return m_directZobWrapper;
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.ControlKey)
                m_ctrlPressed = true;
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ControlKey)
                m_ctrlPressed = false;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.NewScene(workspace);
            m_directZobWrapper.CreateEditorCamera();
            m_path = "";
            m_file = "";
            EventHandler handler = OnNewScene;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
            this.Text = "DirectZob " + m_path + m_file;
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveSceneToolStripMenuItem1.Enabled = m_directZobWrapper.CanFastSave();
        }

        private void loadSceneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                //openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "xml files (*.xml)|*.xml|zob scene (*.dzs)|*.dzs";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    m_path = Path.GetDirectoryName(openFileDialog.FileName) +"\\";
                    m_file = openFileDialog.SafeFileName;
                    m_directZobWrapper.NewScene(m_path);
                    EventHandler handler = OnNewScene;
                    if (null != handler)
                    {
                        handler(this, EventArgs.Empty);
                    }
                    m_directZobWrapper.LoadScene(m_path, m_file, onSceneLoadedCallback);
                    this.Text = "DirectZob " + m_path + m_file;
                }
            }
        }

        private void onSceneLoadedCallbackMethod()
        {
            this.Invoke(OnSceneLoadedDelegate);
            toolStripStatusScanePath.Text = GetDirectZobWrapper().GetResourcePath();
        }

        private void OnSceneLoadedMethod()
        {
            m_zobObjectManagerWrapper.AddEditorGizmos();
            m_zobObjectManagerWrapper.Refresh();
            m_directZobWrapper.CreateEditorCamera();
            EventHandler handler = OnSceneLoadedEventHandler;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.loadScene;
            PropagateSceneUpdateEvent(ev);
        }

        private void saveSceneAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = m_path;
                saveFileDialog.Filter = "xml files (*.xml)|*.xml|zob scene (*.dzs)|*.dzs";
                saveFileDialog.FilterIndex = 2;
                saveFileDialog.RestoreDirectory = true;
                //openFileDialog.AutoUpgradeEnabled = false;
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(saveFileDialog.FileName) + "\\";
                    string file = Path.GetFileName(saveFileDialog.FileName);
                    if(File.Exists(saveFileDialog.FileName) && (path!=m_path || file != m_file))
                    {
                        DialogResult r = MessageBox.Show("Warning", "File already exists. Overwrite ?", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
                        if(r == DialogResult.No)
                        {
                            return;
                        }
                    }
                    m_path = path;
                    m_file = file;
                    m_directZobWrapper.SaveScene(path, file);
                    EventHandler handler = OnSceneSaved;
                    if (null != handler)
                    {
                        handler(this, EventArgs.Empty);
                    }
                    this.Text = "DirectZob " + m_path + m_file;
                }
            }
        }
        private void saveSceneToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.SaveScene();
            EventHandler handler = OnSceneSaved;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
            toolStripStatusScanePath.Text = GetDirectZobWrapper().GetResourcePath();
        }

        private void LoadTextureToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void createSpriteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "images (*.png;*.jpg;*.jpeg:*.tga)|*.png;*.jpg;*.jpeg:*.tga";
                openFileDialog.FilterIndex = 1;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(openFileDialog.FileName) + "\\";
                    string file = openFileDialog.SafeFileName;
                    string name = file;
                    name.Replace(' ', '_');
                    CLI.ZobObjectWrapper root = m_zobObjectManagerWrapper.GetRootObject();
                    CLI.ZobObjectWrapper z = null;
                    z = m_zobObjectManagerWrapper.CreateZobObject();
                    z.SetName(name);
                    string workspace = m_directZobWrapper.GetResourcePath();
                    //z.SetMesh(name);
                    if (!string.IsNullOrEmpty(workspace))
                    {
                        //Uri r = new Uri(workspace);
                        //r = r.MakeRelativeUri(new Uri(path));
                        //path = r.ToString();
                    }
                    z.LoadSprite(file, file, path);
                    m_zobObjectManagerWrapper.ReScan();
                    OnSceneUpdateHandler handler = OnSceneUpdated;
                    if (null != handler)
                    {
                        SceneUpdateEventArg ev = new SceneUpdateEventArg();
                        ev.type = SceneUpdateType.objectAdded;
                        ev.zobObject = z;
                        handler(this, ev);
                    }
                }
            }
        }

        private void LoadMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "all meshes (*.obj;*.fbx)|*.obj;*.fbx|obj files (*.obj)|*.obj|fbx files (*.fbx)|*.fbx";
                openFileDialog.FilterIndex = 1;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(openFileDialog.FileName) + "\\";
                    string file = openFileDialog.SafeFileName;
                    string name = file;
                    name.Replace(' ', '_');
                    CLI.ZobObjectWrapper root = m_zobObjectManagerWrapper.GetRootObject();
                    CLI.ZobObjectWrapper z = null;
                    z = m_zobObjectManagerWrapper.CreateZobObject();
                    z.SetName(name);
                    string workspace = m_directZobWrapper.GetResourcePath();
                    //z.SetMesh(name);
                    if (!string.IsNullOrEmpty(workspace))
                    {
                        //Uri r = new Uri(workspace);
                        //r = r.MakeRelativeUri(new Uri(path));
                        //path = r.ToString();
                    }
                    z.LoadMesh(file, file, path);
                    m_zobObjectManagerWrapper.ReScan();
                    OnSceneUpdateHandler handler = OnSceneUpdated;
                    if (null != handler)
                    {
                        SceneUpdateEventArg ev = new SceneUpdateEventArg();
                        ev.type = SceneUpdateType.objectAdded;
                        ev.zobObject = z;
                        handler(this, ev);
                    }
                }
            }
        }

        private void OnApplicationExit(object sender, EventArgs e)
        {
            m_directZobWrapper.Unload();
        }

        private void Onloaded(object sender, EventArgs e)
        {

        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_running = false;
            //e.Cancel = true; 
            //Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_running = false;
            e.Cancel = true;
        }
        private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            
        }

        public void PropagateSceneUpdateEvent(SceneUpdateEventArg e)
        {
            OnSceneUpdateHandler handler = OnSceneUpdated;
            if (null != handler)
            {
                handler(this, e);
            }
        }

        private void ZobObjectListPanel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void EngineControlsFlowLayout_Resize(object sender, EventArgs e)
        {
            FlowLayoutPanel fl = (FlowLayoutPanel)sender;
            if(fl != null )
            {
                foreach (Control ctrl in fl.Controls)
                {
                    ctrl.Width = fl.Width - 15;
                }
            }
        }

        private void spotToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateLight(1);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void pointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateLight(0);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void directionalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateLight(2);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void createZobObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //throw new Exception("deprecated");
            m_zobObjectManagerWrapper.CreateZobObject();
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            if (m_playMode != eplayMode.ePlayMode_play)
            {
                //save objects position
                if (m_playMode == eplayMode.ePlayMode_stop)
                {
 //                   m_zobObjectList.SaveTransforms();
                }
                m_playMode = eplayMode.ePlayMode_play;
                m_directZobWrapper.StartPhysic();
            }
        }

        private void btnPause_Click(object sender, EventArgs e)
        {
            if (m_playMode == eplayMode.ePlayMode_play)
            {
                m_directZobWrapper.StopPhysic(false);
                m_playMode = eplayMode.ePlayMode_pause;
            }
            else if (m_playMode == eplayMode.ePlayMode_pause)
            {
                m_directZobWrapper.StartPhysic();
                m_playMode = eplayMode.ePlayMode_play;
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (m_playMode != eplayMode.ePlayMode_stop)
            {
                m_directZobWrapper.StopPhysic(true);
                if (m_playMode != eplayMode.ePlayMode_stop)
                {
 //                   m_zobObjectList.RestoreTransforms();
                }
                m_playMode = eplayMode.ePlayMode_stop;
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.stopPhysics;
                PropagateSceneUpdateEvent(ev);
            }
        }
        private void RunEngineThread()
        {
            while (m_running)
            {
                m_directZobWrapper.RunAFrame();
                UpdateAfterEngine();
            }
            m_mainForm.Invoke(new Action(() =>
            {
                m_directZobWrapper.Unload();
                m_directZobWrapper = null;
                Environment.Exit(0);
            }));
        }
        public void InformEngineStatus(string s)
        {
            toolStripStatusEngineState.Text = s;
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void btnGrid_Click(object sender, EventArgs e)
        {
            GetEngineWrapper().ShowGrid(btnGrid.Checked);
        }

        private void btnWireframe_Click(object sender, EventArgs e)
        {
            GetEngineWrapper().WireFrame(btnWireframe.Checked);
        }

        private void btnNormals_Click(object sender, EventArgs e)
        {
            GetEngineWrapper().ShowNormals(btnNormals.Checked);
        }

        private void btnGizmos_Click(object sender, EventArgs e)
        {
            GetEngineWrapper().DrawGizmos(btnGizmos.Checked);
        }

        private void btnTranslateLocal_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().SetObjectModificator((CLI.ZobObjectsEditor.eGizmoModificatorType)1, (CLI.ZobObjectsEditor.eGizmoModificatorSpace)1);
            btnRotateLocal.Checked = false;
            btnScale.Checked = false;
            btnTranslateWorld.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnRotateLocal_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().SetObjectModificator((CLI.ZobObjectsEditor.eGizmoModificatorType)2, (CLI.ZobObjectsEditor.eGizmoModificatorSpace)1);
            btnTranslateLocal.Checked = false;
            btnTranslateWorld.Checked = false;
            btnScale.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnScale_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().SetObjectModificator((CLI.ZobObjectsEditor.eGizmoModificatorType)3, (CLI.ZobObjectsEditor.eGizmoModificatorSpace)1);
            btnTranslateLocal.Checked = false;
            btnRotateLocal.Checked = false;
            btnTranslateWorld.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnTranslateWorld_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().SetObjectModificator((CLI.ZobObjectsEditor.eGizmoModificatorType)1, (CLI.ZobObjectsEditor.eGizmoModificatorSpace)2);
            btnRotateLocal.Checked = false;
            btnScale.Checked = false;
            btnTranslateLocal.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnRotateWorld_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().SetObjectModificator((CLI.ZobObjectsEditor.eGizmoModificatorType)2, (CLI.ZobObjectsEditor.eGizmoModificatorSpace)2);
            btnRotateLocal.Checked = false;
            btnScale.Checked = false;
            btnTranslateLocal.Checked = false;
            btnTranslateWorld.Checked = false;
        }

        #region camera_menu
        private void createCameraToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void fixedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("Fixed");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void fPSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("FPS");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalFreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("Orbital");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalToParentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("Orbital");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }
        #endregion

        private void bBoxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bBoxToolStripMenuItem.Checked = !bBoxToolStripMenuItem.Checked;
            m_directZobWrapper.GetEngineWrapper().ShowBBoxes(bBoxToolStripMenuItem.Checked);
        }

        private void cameraToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cameraToolStripMenuItem.Checked = !cameraToolStripMenuItem.Checked;
            m_directZobWrapper.GetEngineWrapper().DrawCameraGizmos(cameraToolStripMenuItem.Checked);
        }

        private void physicsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            physicsToolStripMenuItem.Checked = !physicsToolStripMenuItem.Checked;
            m_directZobWrapper.GetEngineWrapper().DrawPhysicsGizmos(physicsToolStripMenuItem.Checked);
        }

        private void textToolStripMenuItem_Click(object sender, EventArgs e)
        {
            textToolStripMenuItem.Checked = !textToolStripMenuItem.Checked;
            m_directZobWrapper.GetEngineWrapper().ShowText(textToolStripMenuItem.Checked);
        }

        private void toolStripComboBoxCurrentCamera_SelectedIndexChanged(object sender, EventArgs e)
        {
            string s = (string)toolStripComboBoxCurrentCamera.SelectedItem;
            if (!string.IsNullOrEmpty(s))
            {
                m_directZobWrapper.SetCurrentCamera(s);
            }
        }

        private void regenerateZobIdsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //make an alert
            m_directZobWrapper.RegenerateZobIds();
        }

        private void GigaPaint(object o)
        {
            Color backColor = (Color)System.Drawing.ColorTranslator.FromHtml("#2D2D30");
            Color foreColor = (Color)System.Drawing.ColorTranslator.FromHtml("#F0F0F0");
            Control c = o as Control;
            if (c != null)
            {
                c.BackColor = backColor;
                c.ForeColor = foreColor;
                for (int i = 0; i < c.Controls.Count; i++)
                {
                    object o2 = (object)c.Controls[i];
                    GigaPaint(o2);
                }
            }
            MenuStrip ms = o as MenuStrip;
            if (ms != null)
            {
                ms.BackColor = backColor;
                ms.ForeColor = foreColor;
                for (int i = 0; i < ms.Items.Count; i++)
                {
                    object o2 = (object)ms.Items[i];
                    GigaPaint(o2);
                }
            }
            ToolStrip ts = o as ToolStrip;
            if (ts != null)
            {
                ts.BackColor = backColor;
                ts.ForeColor = foreColor;
                for (int i = 0; i < ts.Items.Count; i++)
                {
                    object o2 = (object)ts.Items[i];
                    GigaPaint(o2);
                }
            }
            ToolStripMenuItem tsm = o as ToolStripMenuItem;
            if (tsm != null)
            {
                tsm.BackColor = backColor;
                tsm.ForeColor = foreColor;
                for (int i = 0; i < tsm.DropDownItems.Count; i++)
                {
                    object o2 = (object)tsm.DropDownItems[i];
                    GigaPaint(o2);
                }
            }
            TabControl tc = o as TabControl;
            if (tc != null)
            {
                tc.BackColor = backColor;
                tc.ForeColor = foreColor;
                for (int i = 0; i < tc.TabPages.Count; i++)
                {
                    object o2 = (object)tc.TabPages[i];
                    GigaPaint(o2);
                }
            }
        }

        private void EngineRendererPanel_Resize(object sender, EventArgs e)
        {
            if (m_engineWindow != null)
            {
                PictureBox p = m_engineWindow.GetEngineRenderwindow();
                if(p != null)
                {
                    float r = (float)p.Width / (float)p.Height;
                    toolStripStatusLabel1.Text = "Size : " + p.Width + "x" + p.Height + " | Ratio : "+r;
                    m_directZobWrapper.GetEngineWrapper().Resize(p.Width, p.Height);
                }
            }
        }

        private void SetSnap(string s)
        {
            float f = 0.0f;
            switch (s)
            {
                case "0.1":
                    {
                        f = 0.1f;
                        break;
                    }
                case "0.5":
                    {
                        f = 0.5f;
                        break;
                    }
                case "1":
                    {
                        f = 1.0f;
                        break;
                    }
                default:
                case "None":
                    {
                        f = 0.0f;
                        break;
                    }
            }
            toolStripSnap.Text = "Snap (" + s + ")";
            m_directZobWrapper.GetEngineWrapper().SetSnap(f);
        }

        private void toolStripSnap_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string s =  e.ClickedItem.Text;
            SetSnap(s);
        }

        private void assetsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void materialsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MaterialsForm.MaterialsForm f = new MaterialsForm.MaterialsForm(m_directZobWrapper);
            f.Show();
        }
    }

    public class LogEvent
    {
        public string data { get; set; }
        public int type { get; set; }
    };
}
