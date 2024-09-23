using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class ZobEntityViewerForm : Form
    {
        private DirectZobInterface.DirectZobWrapper m_directZobWrapper;
        private DirectZobEditorForm m_mainForm;
        public ZobEntityViewerForm(DirectZobEditorForm f, DirectZobInterface.DirectZobWrapper directZobWrapper)
        {
            m_directZobWrapper = directZobWrapper;
            m_mainForm = f;
            InitializeComponent();
            List<DirectZobInterface.ZobEntityInfo> l = m_directZobWrapper.GetZobObjectManagerWrapper().GetEntitiesInfo();
            listView1.Items.Clear();
			ImageList imageList = new ImageList();
			System.Resources.ResourceManager resources = new ResourceManager("DirectZobEditor.Properties.Resources", typeof(DirectZobEditorForm).Assembly);
			Image  im = (Image )resources.GetObject("_object");
			if (im != null)
			{
				Bitmap  bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("light1");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("camera2");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("mesh3");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("sprite");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("hud");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("component");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("cameraController");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("unknown");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			im = (Image)resources.GetObject("physic");
			if (im != null)
			{
				Bitmap bmp = new Bitmap(im);
				imageList.Images.Add(bmp);
			}
			listView1.LargeImageList = imageList;
			listView1.SmallImageList = imageList;
			for (int i=0; i<l.Count; i++)
            {
                if (!l[i]._isEditor)
                {
                    ListViewItem it = new ListViewItem(l[i]._name);
					
					it.ImageIndex = (int)l[i]._imgType;
					//it.SubItems.Add(l[i]._name);
					it.SubItems.Add(l[i]._type);
                    it.SubItems.Add(l[i]._zid.ToString());
                    listView1.Items.Add(it);
					
                }
            }
        }

        private void ZobEntitySelector_Load(object sender, EventArgs e)
        {

        }

        private void ZobEntitySelector_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_mainForm.UnregisterZobEntitySelector();
;       }
    }
}
