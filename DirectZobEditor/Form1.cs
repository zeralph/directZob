﻿using System;
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
        private CLI.DirectZobWrapper m_directZobWrapper;
        private CLI.CameraManagerWrapper m_camerManagerWrapper;
        //public UpdateLogWindow UpdateLogWindowDelegate;
        
        private CameraControl m_camControl;
        private ZobObjectList m_zobObjectList;
        private EngineWindow m_engineWindow;
        private bool m_ctrlPressed = false;

        private string[] m_events;

        public Form1()
        {
            InitializeComponent();
            this.KeyPreview = true;
            m_directZobWrapper = new CLI.DirectZobWrapper();
            m_directZobWrapper.Init();
            
         
            string path = @"C:\_GIT\directZob\resources\";
            string file = "scene1.xml";
            //path = @"C:\_GIT\directZob\resources\";
            m_directZobWrapper.LoadScene(path, file);


            //UpdateLogWindowDelegate = new UpdateLogWindow(UpdateLogWindowMethod);

            m_camControl = new CameraControl();
            propertiesPanel.Controls.Add(m_camControl);
            m_zobObjectList = new ZobObjectList();
            ZobObjectListPanel.Controls.Add(m_zobObjectList);
            m_engineWindow = new EngineWindow(this, m_directZobWrapper);
            EngineRendererPanel.Controls.Add(m_engineWindow);

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
    }

    public class Event
    {
        public string data { get; set; }
        public int type { get; set; }
    };
}
