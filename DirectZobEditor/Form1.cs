using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace DirectZobEditor
{
    public partial class Form1 : Form
    {
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

        public static Form1 m_mainForm = null;
        public static Form1 GetMainForm() { return m_mainForm; }
        public event EventHandler OnNewScene;
        public event EventHandler OnSceneLoaded;
        public event EventHandler OnSceneSaved;

        public bool m_canBeSafelyClosed = false;

        public delegate void OnSceneUpdateHandler(object s, SceneUpdateEventArg e);
        public event OnSceneUpdateHandler OnSceneUpdated;

        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.MeshManagerWrapper m_meshManagerWrapper;
        private CLI.LightManagerWrapper m_lightManagerWrapper;

        private CameraControl m_camControl;
        private ZobObjectListControl m_zobObjectList;
        private EngineWindow m_engineWindow;
        private EngineControl m_engineControl;
        private ZobObjectControl m_zobObjectControl;
        private SceneControl m_sceneControl;
        private ZobLightControl m_lightControl;
        private ZobCameraControl m_cameraControl;
        private ZobMeshControl m_meshControl;
        private PhysicControl m_physicsControl;
        private ZobSpriteControl m_spriteControl;
        private bool m_ctrlPressed = false;

        private string m_path = "C:\\";
        private string m_file;

        private string[] m_events;
        private eplayMode m_playMode = eplayMode.ePlayMode_stop;
        public Form1()
        {
            m_mainForm = this;
            InitializeComponent();
            this.KeyPreview = true;
            m_directZobWrapper = new CLI.DirectZobWrapper();
            m_directZobWrapper.Init(1024, 768);

            m_meshManagerWrapper = new CLI.MeshManagerWrapper();
            m_lightManagerWrapper = new CLI.LightManagerWrapper();
            Application.ApplicationExit += new EventHandler(this.OnApplicationExit);
            this.Load += new EventHandler(this.Onloaded);
            //--
            m_engineWindow = new EngineWindow(this, m_directZobWrapper);
            m_engineControl = new EngineControl(this, m_engineWindow.GetEngineWrapper());
            m_camControl = new CameraControl(this);
            m_zobObjectList = new ZobObjectListControl(this);
            m_sceneControl = new SceneControl(this, m_lightManagerWrapper);
            m_lightControl = new ZobLightControl(this);
            m_cameraControl = new ZobCameraControl(this);
            m_meshControl = new ZobMeshControl();
            m_spriteControl = new ZobSpriteControl(this);
            m_physicsControl = new PhysicControl(this);
            m_zobObjectControl = new ZobObjectControl(this, m_lightControl, m_cameraControl, m_meshControl);
            m_engineWindow.BindEvents();
            m_engineControl.BindEvents();
            m_camControl.BindEvents();
            m_zobObjectList.BindEvents();
            m_sceneControl.BindEvents();
            m_lightControl.BindEvents();
            m_cameraControl.BindEvents();
            m_meshControl.BindEvents();
            m_spriteControl.BindEvents();
            m_physicsControl.BindEvents();
            m_zobObjectControl.BindEvents();
            ObjectControlsFlowLayout.Controls.Add(m_camControl);
            EngineRendererPanel.Controls.Add(m_engineWindow);
            ZobObjectListPanel.Controls.Add(m_zobObjectList);
            ObjectControlsFlowLayout.Controls.Add(m_zobObjectControl);
            ObjectControlsFlowLayout.Controls.Add(m_physicsControl);
            ObjectControlsFlowLayout.Controls.Add(m_meshControl);
            ObjectControlsFlowLayout.Controls.Add(m_cameraControl);
            ObjectControlsFlowLayout.Controls.Add(m_lightControl);
            ObjectControlsFlowLayout.Controls.Add(m_spriteControl);
            EngineControlsFlowLayout.Controls.Add(m_engineControl);
            SceneControlsFlowLayout.Controls.Add(m_sceneControl);
            this.propertiesPanel.MinimumSize = new Size(300, 500);
            m_engineWindow.OnEngineStopped += new EventHandler(OnEngineClosed);
            this.WindowState = FormWindowState.Maximized;
            m_directZobWrapper.NewScene();
            EventHandler handler = OnNewScene;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
        }

        public string Getpath()
        {
            return m_path;
        }

        public void UpdateAfterEngine()
        {
            m_camControl.UpdateControl();
            m_zobObjectList.UpdateControl();

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
        }




        private void UpdateEventsLog()
        {
            int l = m_events.Count();
            for (int i = 0; i < l; i++)
            {
                Event json = JsonConvert.DeserializeObject<Event>(m_events[i]);
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

        public CameraControl GetCameraControl()
        {
            return m_camControl;
        }
        public EngineWindow GetEngineWindow()
        {
            return m_engineWindow;
        }

        public ZobObjectControl GetZobObjectControl()
        {
            return m_zobObjectControl;
        }

        public ZobObjectListControl GetZobObjectListControl()
        {
            return m_zobObjectList;
        }

        public CLI.DirectZobWrapper GetDirectZobWrapper()
        {
            return m_directZobWrapper;
        }

        public CLI.MeshManagerWrapper GetMeshManagerWrapper()
        {
            return m_meshManagerWrapper;
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
            m_directZobWrapper.NewScene();
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
                    m_directZobWrapper.NewScene();
                    EventHandler handler = OnNewScene;
                    if (null != handler)
                    {
                        handler(this, EventArgs.Empty);
                    }
                    m_directZobWrapper.LoadScene(m_path, m_file);
                    handler = OnSceneLoaded;
                    if (null != handler)
                    {
                        handler(this, EventArgs.Empty);
                    }
                    Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                    ev.type = Form1.SceneUpdateType.loadScene;
                    PropagateSceneUpdateEvent(ev);
                    this.Text = "DirectZob " + m_path + m_file;
                }
            }
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
        }

        private void LoadTextureToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void LoadMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
//                openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "all meshes (*.obj;*.fbx)|*.obj;*.fbx|obj files (*.obj)|*.obj|fbx files (*.fbx)|*.fbx";
                openFileDialog.FilterIndex = 1;
  //              openFileDialog.RestoreDirectory = true;
                //openFileDialog.AutoUpgradeEnabled = false;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetDirectoryName(openFileDialog.FileName) + "\\";
                    string file = openFileDialog.SafeFileName;
                    string name = file;
                    name.Replace(' ', '_');
                    //m_meshManagerWrapper.LoadMesh(name, path, file);
                    CLI.ZobObjectWrapper root = m_zobObjectList.GetWrapper().GetRootObject();
                    CLI.ZobObjectWrapper z = m_zobObjectList.GetWrapper().AddZobObject(root);
                    //z.SetMesh(name);
                    z.LoadMesh(file, file, path);
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
            Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(!m_canBeSafelyClosed)
            {
                m_directZobWrapper.Unload();
                m_engineWindow.StopEngine();
                e.Cancel = true;
            }
            else
            {
                //goodbye !
            }
        }
        private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            
        }

        private void OnEngineClosed(object sender, EventArgs e)
        {
            m_canBeSafelyClosed = true;
            Application.Exit();
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
            m_lightManagerWrapper.CreateLight(1);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void pointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_lightManagerWrapper.CreateLight(0);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void directionalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_lightManagerWrapper.CreateLight(2);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createLight;
            PropagateSceneUpdateEvent(ev);
        }

        private void createSpriteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CLI.ZobObjectWrapper root = m_zobObjectList.GetWrapper().GetRootObject();
            CLI.ZobObjectWrapper z = m_zobObjectList.GetWrapper().AddZobSprite(root);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createSprite;
            ev.zobObject = z;
            PropagateSceneUpdateEvent(ev);
        }

        private void createZobObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CLI.ZobObjectWrapper root = m_zobObjectList.GetWrapper().GetRootObject();
            CLI.ZobObjectWrapper z = m_zobObjectList.GetWrapper().AddZobObject(root);
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.objectAdded;
            ev.zobObject = z;
            PropagateSceneUpdateEvent(ev);
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            if (m_playMode != eplayMode.ePlayMode_play)
            {
                //save objects position
                if (m_playMode == eplayMode.ePlayMode_stop)
                {
                    m_zobObjectList.SaveTransforms();
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
                    m_zobObjectList.RestoreTransforms();
                }
                m_playMode = eplayMode.ePlayMode_stop;
                Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
                ev.type = Form1.SceneUpdateType.stopPhysics;
                PropagateSceneUpdateEvent(ev);
            }
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void btnGrid_Click(object sender, EventArgs e)
        {
            m_engineWindow.GetEngineWrapper().ShowGrid(btnGrid.Checked);
        }

        private void btnWireframe_Click(object sender, EventArgs e)
        {
            m_engineWindow.GetEngineWrapper().WireFrame(btnWireframe.Checked);
        }

        private void btnNormals_Click(object sender, EventArgs e)
        {
            m_engineWindow.GetEngineWrapper().ShowNormals(btnNormals.Checked);
        }

        private void btnGizmos_Click(object sender, EventArgs e)
        {
            m_engineWindow.GetEngineWrapper().DrawGizmos(btnGizmos.Checked);
        }

        private void btnTranslateLocal_Click(object sender, EventArgs e)
        {
            m_engineWindow.SetModificator(EngineWindow.objectModificator.translate_local);
            btnRotateLocal.Checked = false;
            btnScale.Checked = false;
            btnTranslateWorld.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnRotateLocal_Click(object sender, EventArgs e)
        {
            m_engineWindow.SetModificator(EngineWindow.objectModificator.rotate_local);
            btnTranslateLocal.Checked = false;
            btnTranslateWorld.Checked = false;
            btnScale.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnScale_Click(object sender, EventArgs e)
        {
            m_engineWindow.SetModificator(EngineWindow.objectModificator.scale);
            btnTranslateLocal.Checked = false;
            btnRotateLocal.Checked = false;
            btnTranslateWorld.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnTranslateWorld_Click(object sender, EventArgs e)
        {
            m_engineWindow.SetModificator(EngineWindow.objectModificator.translate_world);
            btnRotateLocal.Checked = false;
            btnScale.Checked = false;
            btnTranslateLocal.Checked = false;
            btnRotateWorld.Checked = false;
        }

        private void btnRotateWorld_Click(object sender, EventArgs e)
        {
            m_engineWindow.SetModificator(EngineWindow.objectModificator.rotate_world);
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
            m_camControl.GetWrapper().CreateCamera("Fixed");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void fPSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_camControl.GetWrapper().CreateCamera("FPS");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalFreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_camControl.GetWrapper().CreateCamera("Orbital");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }

        private void orbitalToParentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_camControl.GetWrapper().CreateCamera("Orbital");
            Form1.SceneUpdateEventArg ev = new Form1.SceneUpdateEventArg();
            ev.type = Form1.SceneUpdateType.createCamera;
            PropagateSceneUpdateEvent(ev);
        }
        #endregion
    }

    public class Event
    {
        public string data { get; set; }
        public int type { get; set; }
    };
}
