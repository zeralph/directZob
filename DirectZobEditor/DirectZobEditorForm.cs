using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Resources;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Interop;
using DirectZobEditor.Packer;
using Newtonsoft.Json;

namespace DirectZobEditor
{
    public partial class DirectZobEditorForm : Form
    {
        private string workspace = "";
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
        public static DirectZobEditorForm m_mainForm = null;
        public static DirectZobEditorForm GetMainForm() { return m_mainForm; }
        public event EventHandler OnNewScene;
        public event EventHandler OnSceneLoadedEventHandler;
        public event EventHandler OnSceneSaved;

        public bool m_canBeSafelyClosed = false;

        public delegate void OnSceneUpdateHandler(object s, SceneUpdateEventArg e);
        public event OnSceneUpdateHandler OnSceneUpdated;

        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.ZobObjectManagerWrapper m_zobObjectManagerWrapper;

        private ZobEntityViewerForm m_entityForm;
        private MaterialsForm.ZobMaterialsForm m_materialsForm;
        private ZobPackerForm m_packerForm;
        private ZobSceneLoadForm m_loadingForm;

        private Thread m_engineThread;
        //private ZobObjectListControl m_zobObjectList;
        private EngineWindow m_engineWindow;
        private bool m_snap = false;
        private bool m_ctrlPressed = false;

        private string m_path = "C:\\";
        private string m_file;

        private string[] m_events;
        private eplayMode m_playMode = eplayMode.ePlayMode_stop;
        private eplayMode m_nextPlayMode = eplayMode.ePlayMode_stop;

        public CLI.engineCallback onSceneLoadedCallback;
        public CLI.sceneLoadingCallback onSceneLoadingCallback;
        public delegate void OnSceneLoaded();
        public OnSceneLoaded OnSceneLoadedDelegate;

        private IntPtr m_engineWindowHandle;
        public DirectZobEditorForm()
        {
            Font f = new Font("Microsoft Tai Le", 8);
            this.Font = f;
            m_mainForm = this;
            m_running = true;
            InitializeComponent();
            m_engineWindow = new EngineWindow(this);
            PictureBox p = m_engineWindow.GetEngineRenderwindow();
            System.Resources.ResourceManager resources = new ResourceManager("DirectZobEditor.Properties.Resources", typeof(DirectZobEditorForm).Assembly);
            m_directZobWrapper = new CLI.DirectZobWrapper(this.ZobObjectListPanel, this.propertiesPanel, this.globalPropertiesPanel, m_engineWindow.GetEngineRenderwindow(), resources);
            m_directZobWrapper.Init(p.Width, p.Height);
            CLI.ZobObjectManagerWrapper.OnObjectSelectedEvent += new CLI.ZobObjectManagerWrapper.OnObjectSelected(OnObjectSelected);
            m_zobObjectManagerWrapper = m_directZobWrapper.GetZobObjectManagerWrapper();
            Application.ApplicationExit += new EventHandler(this.OnApplicationExit);
            this.Load += new EventHandler(this.Onloaded);
            onSceneLoadedCallback = new CLI.engineCallback(onSceneLoadedCallbackMethod);
            OnSceneLoadedDelegate = new OnSceneLoaded(OnSceneLoadedMethod);
            onSceneLoadingCallback = new CLI.sceneLoadingCallback(OnSceneLoadingMethod);

            SetSnap("None");
            InformEngineStatus("STOPPED");
            m_engineWindowHandle = this.Handle;// m_engineWindow.Handle;
            LoadEditorSettings();

            OnSceneSaved += Form1_OnSceneSaved;
            OnNewScene += Form1_OnNewScene;
            this.propertiesPanel.MinimumSize = new Size(300, 500);
            m_directZobWrapper.NewScene("");
            p.MouseEnter += P_MouseEnter;
            p.Click += P_Click;
            this.WindowState = FormWindowState.Maximized;
            EngineRendererPanel.Controls.Add(m_engineWindow);
            EngineRendererPanel.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            EngineRendererPanel.AutoSize = true;
            EventHandler handler = OnNewScene;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
            m_engineThread = new Thread(RunEngineThread);
            m_engineThread.Start();



            

            
        }

        private void P_Click(object sender, EventArgs e)
        {
            //m_engineWindow.Focus();
        }

        private void P_MouseEnter(object sender, EventArgs e)
        {
            //m_engineWindow.Focus();
        }
        private void Form1_OnNewScene(object sender, EventArgs e)
        {
            toolStripStatusScanePath.Text = "| Workspace : " + GetDirectZobWrapper().GetResourcePath();
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
                    DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
                    ev.type = DirectZobEditorForm.SceneUpdateType.updateAfterEngine;
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

        public void logInfo(string text)
        {
            AppendText(textLog, text + "\n", Color.Blue);
            textLog.ScrollToCaret();
        }
        public void logWarning(string text)
        {
            AppendText(textLog, text + "\n", Color.Orange);
            textLog.ScrollToCaret();
        }
        public void logError(string text)
        {
            AppendText(textLog, text + "\n", Color.Red);
            textLog.ScrollToCaret();
            MessageBox.Show(text, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                string workspace = m_directZobWrapper.GetResourcePath();
                if (!String.IsNullOrEmpty(workspace) && Directory.Exists(workspace))
                {
                    openFileDialog.InitialDirectory = workspace.Replace("/", "\\"); ;
                }
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
                    ShowLoadingForm();
                    m_directZobWrapper.LoadScene(m_path, m_file, onSceneLoadedCallback, null);//, onSceneLoadingCallback);
                    this.Text = "DirectZob " + m_path + m_file;
                }
            }
        }

        private void onSceneLoadedCallbackMethod()
        {
            this.Invoke(OnSceneLoadedDelegate);
            toolStripStatusScanePath.Text = GetDirectZobWrapper().GetResourcePath();
            CloseLoadingForm();
        }

        private void OnSceneLoadingMethod(int nbObj, int curObj, String s)
        {
            m_loadingForm.Invoke(new Action(() =>
            {
                m_loadingForm.SetText(nbObj, curObj, s);
            }));       
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
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.loadScene;
            PropagateSceneUpdateEvent(ev);
        }

        private void saveSceneAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = m_path;
                saveFileDialog.Filter = "xml files (*.xml)|*.xml|zob scene (*.dzs)|*.dzs";
                saveFileDialog.FilterIndex = 2;
                string workspace = m_directZobWrapper.GetResourcePath();
                if (!String.IsNullOrEmpty(workspace) && Directory.Exists(workspace))
                {
                    saveFileDialog.InitialDirectory = workspace.Replace("/", "\\");
                }
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
            SaveEditorSettings();
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
            SaveEditorSettings();
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
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void pointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateLight(0);
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void directionalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateLight(2);
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void createZobObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //throw new Exception("deprecated");
            m_zobObjectManagerWrapper.CreateZobObject();
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            m_nextPlayMode = eplayMode.ePlayMode_play;
        }

        private void btnPause_Click(object sender, EventArgs e)
        {
            m_nextPlayMode = eplayMode.ePlayMode_pause;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            m_nextPlayMode = eplayMode.ePlayMode_stop;
        }

        private void UpdatePlayStatus()
        {
            if(m_nextPlayMode != m_playMode)
            {
                if(m_nextPlayMode == eplayMode.ePlayMode_pause)
                {
                    if (m_playMode == eplayMode.ePlayMode_play)
                    {
                        m_directZobWrapper.StopPhysic(false);
                        m_playMode = eplayMode.ePlayMode_pause;
                        InformEngineStatus("PAUSED");
                    }
                    else if (m_playMode == eplayMode.ePlayMode_pause)
                    {
                        m_directZobWrapper.StartPhysic();
                        m_playMode = eplayMode.ePlayMode_play;
                        InformEngineStatus("PLAYING");
                    }
                }
                else if(m_nextPlayMode == eplayMode.ePlayMode_stop)
                {
                    m_directZobWrapper.StopPhysic(true);
                    m_zobObjectManagerWrapper.RestoreTransforms();
                    DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
                    ev.type = DirectZobEditorForm.SceneUpdateType.stopPhysics;
                    PropagateSceneUpdateEvent(ev);
                    InformEngineStatus("STOPPED");
                }
                else if(m_nextPlayMode == eplayMode.ePlayMode_play)
                {
                    m_zobObjectManagerWrapper.SaveTransforms();
                    m_directZobWrapper.StartPhysic();
                    InformEngineStatus("PLAYING");
                }
                m_playMode= m_nextPlayMode;
            }
        }

        private void RunEngineThread()
        {
            while (m_running)
            {
                m_directZobWrapper.RunAFrame();
                UpdateAfterEngine();
                UpdatePlayStatus();
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
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void fPSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("FPS");
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalFreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("Orbital");
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalToParentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.CreateCamera("Orbital");
            DirectZobEditorForm.SceneUpdateEventArg ev = new DirectZobEditorForm.SceneUpdateEventArg();
            ev.type = DirectZobEditorForm.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }
        #endregion

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

        private void SaveEditorSettings()
        {
            CLI.EditorSettings s = new CLI.EditorSettings();
            m_directZobWrapper.SaveEditorSettings(s);
            JsonSerializer serializer = new JsonSerializer();
            using (StreamWriter file = File.CreateText(@"../settings.json"))
            {
                serializer.Formatting = Newtonsoft.Json.Formatting.Indented;
                serializer.Serialize(file, s);
            }
        }
        private void LoadEditorSettings()
        {
            string f = @"../settings.json";
            if (File.Exists(f))
            {
                using (TextReader tr = new StreamReader(f))
                {
                    using (JsonReader jr = new JsonTextReader(tr))
                    {
                        JsonSerializer serializer = new JsonSerializer();
                        CLI.EditorSettings s = serializer.Deserialize<CLI.EditorSettings>(jr);
                        m_directZobWrapper.LoadEditorSettings(s);
                    }
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
                    Rectangle rect = p.DisplayRectangle;
                    int x = p.Width;
                    int y = p.Height;
                    float r = (float)p.Width / (float)p.Height;
                    toolStripStatusLabel1.Text = "Size : " + x + "x" + y + " | Ratio : "+r;
                    m_directZobWrapper.GetEngineWrapper().Resize(x, y);
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
            if(m_materialsForm == null)
            {
                m_materialsForm = new MaterialsForm.ZobMaterialsForm(this, m_directZobWrapper);
                m_materialsForm.StartPosition = FormStartPosition.CenterParent;
                Point p = new Point();
                p.X = this.Location.X + this.Width / 2 - m_materialsForm.Width / 2;
                p.Y = this.Location.Y + this.Height / 2 - m_materialsForm.Height / 2;
                m_materialsForm.Show();
                m_materialsForm.Location = p;
            }
        }

        private void ShowLoadingForm()
        {
            if (m_loadingForm == null)
            {
                m_loadingForm = new ZobSceneLoadForm(this, m_directZobWrapper);
                m_loadingForm.StartPosition = FormStartPosition.CenterParent;
                Point p = new Point();
                p.X = this.Location.X + this.Width / 2 - m_loadingForm.Width / 2;
                p.Y = this.Location.Y + this.Height / 2 - m_loadingForm.Height / 2;
                m_loadingForm.Show();
                m_loadingForm.Location = p;
            }
        }

        private void entitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_entityForm == null)
            {
                m_entityForm = new ZobEntityViewerForm(this, m_directZobWrapper);
                m_entityForm.StartPosition = FormStartPosition.CenterParent;
                Point p = new Point();
                p.X = this.Location.X + this.Width / 2 - m_entityForm.Width / 2;
                p.Y = this.Location.Y + this.Height / 2 - m_entityForm.Height / 2;
                m_entityForm.Show();
                m_entityForm.Location = p;
            }
        }

        public void UnregisterZobMaterialsViewer()
        {
            m_materialsForm = null;
        }
        public void UnregisterZobEntitySelector()
        {
            m_entityForm = null;
        }
        public void CloseLoadingForm()
        {
            m_loadingForm.AskForClose();
            m_loadingForm = null;
        }

        private void toolStripComboBox1_Click(object sender, EventArgs e)
        {

        }

        private void packLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_packerForm == null)
            {
                m_packerForm = new ZobPackerForm(this, m_directZobWrapper);
                m_packerForm.StartPosition = FormStartPosition.CenterParent;
                Point p = new Point();
                p.X = this.Location.X + this.Width / 2 - m_packerForm.Width / 2;
                p.Y = this.Location.Y + this.Height / 2 - m_packerForm.Height / 2;
                m_packerForm.Show();
                m_packerForm.Location = p;
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.GetEngineWrapper().DrawPhysicsGizmos(true);
            m_directZobWrapper.GetEngineWrapper().DrawGizmos(true);
        }
    }

    public class LogEvent
    {
        public string data { get; set; }
        public int type { get; set; }
    };
}
