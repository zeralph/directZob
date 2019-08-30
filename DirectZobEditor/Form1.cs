using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace DirectZobEditor
{
    public partial class Form1 : Form
    {
        private CLI.InterfaceWrapper m_directZobInterface;
        private Thread m_engineThread;
        public delegate void UpdateEngineWindow();
        public UpdateEngineWindow UpdateEngineWindowDelegate;
        Graphics m_EngineGraphics = null;
        Bitmap m_engineBitmap = null;
        int m_width;
        int m_height;
        public Form1()
        {
            InitializeComponent();
            m_directZobInterface = new CLI.InterfaceWrapper();
            m_width = m_directZobInterface.GetBufferWidth();
            m_height = m_directZobInterface.GetBufferHeight();

            EngineWIndow.Width = m_width;
            EngineWIndow.Height = m_height;
            int i = m_directZobInterface.Test();
            Console.WriteLine("TEST : " + i);
            m_EngineGraphics = EngineWIndow.CreateGraphics();
            m_engineThread = new Thread(RunEngineThread);

            UpdateEngineWindowDelegate = new UpdateEngineWindow(UpdateEngineWindowMethod);
            m_engineThread.Start();
            m_directZobInterface.RunAFrame();
            IntPtr p = m_directZobInterface.GetBufferData();
            m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);
        }

        public void UpdateEngineWindowMethod()
        {
            //if(m_directZobInterface.RunAFrame() >= 0)
            {
                IntPtr p = m_directZobInterface.GetBufferData();
                Rectangle rect = new Rectangle(0, 0, m_engineBitmap.Width, m_engineBitmap.Height);
                System.Drawing.Imaging.BitmapData bmpData =
                    m_engineBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite,
                    m_engineBitmap.PixelFormat);
                bmpData.Scan0 = p;
                m_engineBitmap.UnlockBits(bmpData);

                //m_engineBitmap = new System.Drawing.Bitmap(m_width, m_height, 4 * m_width, System.Drawing.Imaging.PixelFormat.Format32bppRgb, p);

                m_EngineGraphics.DrawImage(m_engineBitmap, 0, 0);
            }
        }

        private void RunEngineThread()
        {
            while(true)
            {
                m_directZobInterface.RunAFrame();
                EngineWIndow.Invoke(UpdateEngineWindowDelegate);
                //Application.DoEvents();
            }
            
            //Refresh();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_engineThread.Abort();
        }
    }
}
