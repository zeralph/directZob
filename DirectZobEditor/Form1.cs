using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace DirectZobEditor
{
    public partial class Form1 : Form
    {


        public event EventHandler OnNewScene;
        public event EventHandler OnSceneLoaded;

        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.CameraManagerWrapper m_camerManagerWrapper;
        private CLI.MeshManagerWrapper m_meshManagerWrapper;
        //public UpdateLogWindow UpdateLogWindowDelegate;
        
        private CameraControl m_camControl;
        private ZobObjectListControl m_zobObjectList;
        private EngineWindow m_engineWindow;
        private EngineControl m_engineControl;
        private ZobObjectControl m_zobObjectControl;
        private bool m_ctrlPressed = false;

        private string m_path;
        private string m_file;

        private string[] m_events;

        public Form1()
        {
            InitializeComponent();
            this.KeyPreview = true;
            m_directZobWrapper = new CLI.DirectZobWrapper();
            m_directZobWrapper.Init();

            m_meshManagerWrapper = new CLI.MeshManagerWrapper();


            m_path = @"C:\_GIT\directZob\resources\";
            m_file = "scene1.xml";
            //path = @"D:\_PERSO\directZob\directZob\resources\";
            m_directZobWrapper.LoadScene(m_path, m_file);


            //UpdateLogWindowDelegate = new UpdateLogWindow(UpdateLogWindowMethod);

            propertiesPanel.Width = 600;
            m_camControl = new CameraControl();
            propertiesPanel.Controls.Add(m_camControl);
            m_zobObjectList = new ZobObjectListControl(this);
            ZobObjectListPanel.Controls.Add(m_zobObjectList);
            m_engineWindow = new EngineWindow(this, m_directZobWrapper);
            EngineRendererPanel.Controls.Add(m_engineWindow);
            m_engineControl = new EngineControl(m_engineWindow.GetEngineWrapper());
            propertiesPanel.Controls.Add(m_engineControl);
            m_engineControl.Location = new Point(0, m_camControl.Height + 10);
            m_zobObjectControl = new ZobObjectControl(this);
            propertiesPanel.Controls.Add(m_zobObjectControl);
            m_zobObjectControl.Location = new Point(0, m_engineControl.Location.Y + m_engineControl.Height + 10);

            this.WindowState = FormWindowState.Maximized;

        }

        public void UpdateAfterEngine()
        {
            m_camControl.UpdateControl();
            m_zobObjectList.UpdateControl();

            m_events = m_directZobWrapper.GetEventsAndClear();
            UpdateEventsLog();
            //textLog.Invoke(UpdateLogWindowDelegate);
            //EngineRender.Invoke(UpdateEngineWindowDelegate);
        }




        private void UpdateEventsLog()
        {
            int l = m_events.Count();
            for (int i = 0; i < l; i++)
            {
                Event json = JsonConvert.DeserializeObject<Event>(m_events[i]);
                if (json.type == 0)
                {
                    textLog.AppendText(json.data);
                    textLog.AppendText("\n");
                }
            }
        }

        public bool IsCtrlPressed()
        {
            return m_ctrlPressed;
        }

        public CameraControl GetCameraControl()
        {
            return m_camControl;
        }
        
        public ZobObjectControl GetZobObjectControl()
        {
            return m_zobObjectControl;
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
            EventHandler handler = OnNewScene;
            if (null != handler)
            {
                handler(this, EventArgs.Empty);
            }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveSceneToolStripMenuItem1.Available = m_directZobWrapper.CanFastSave();
        }

        private void loadSceneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "xml files (*.xml)|*.xml";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    m_path = openFileDialog.InitialDirectory;
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
                }
            }
        }

        private void saveSceneAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //m_directZobWrapper.SaveScene(path, file);
        }

        private void saveSceneToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.SaveScene();
        }

        private void LoadTextureToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void LoadMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "obj files (*.obj)|*.obj";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    m_path = openFileDialog.InitialDirectory;
                    m_file = openFileDialog.SafeFileName;
                    //m_meshManagerWrapper.L
                }
            }
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_directZobWrapper.NewScene();
            //m_directZobWrapper = null;
            Application.Exit();
        }
    }

    public class Event
    {
        public string data { get; set; }
        public int type { get; set; }
    };
}
