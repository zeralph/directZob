namespace DirectZobEditor
{
    partial class ZobObjectListControl
    {
        /// <summary> 
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur de composants

        /// <summary> 
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas 
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.btnAddZobObject = new System.Windows.Forms.Button();
            this.BtnForceRefresh = new System.Windows.Forms.Button();
            this.TreeNodeRightClick = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.duplicateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ZoomToStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsFactoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aDDBEHAVIURToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.physicBoxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.physicSphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.TreeNodeRightClick.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 16);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.btnAddZobObject);
            this.splitContainer1.Panel1.Controls.Add(this.BtnForceRefresh);
            this.splitContainer1.Size = new System.Drawing.Size(194, 881);
            this.splitContainer1.SplitterDistance = 75;
            this.splitContainer1.TabIndex = 2;
            // 
            // btnAddZobObject
            // 
            this.btnAddZobObject.Location = new System.Drawing.Point(3, 26);
            this.btnAddZobObject.Name = "btnAddZobObject";
            this.btnAddZobObject.Size = new System.Drawing.Size(188, 23);
            this.btnAddZobObject.TabIndex = 2;
            this.btnAddZobObject.Text = "Add Zob object";
            this.btnAddZobObject.UseVisualStyleBackColor = true;
            this.btnAddZobObject.Click += new System.EventHandler(this.btnAddZobObject_Click);
            // 
            // BtnForceRefresh
            // 
            this.BtnForceRefresh.Location = new System.Drawing.Point(3, 3);
            this.BtnForceRefresh.Name = "BtnForceRefresh";
            this.BtnForceRefresh.Size = new System.Drawing.Size(188, 23);
            this.BtnForceRefresh.TabIndex = 1;
            this.BtnForceRefresh.Text = "Refresh";
            this.BtnForceRefresh.UseVisualStyleBackColor = true;
            this.BtnForceRefresh.Click += new System.EventHandler(this.BtnForceRefresh_Click);
            // 
            // TreeNodeRightClick
            // 
            this.TreeNodeRightClick.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.removeToolStripMenuItem,
            this.duplicateToolStripMenuItem,
            this.ZoomToStripMenuItem,
            this.saveAsFactoryToolStripMenuItem,
            this.editToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.aDDBEHAVIURToolStripMenuItem});
            this.TreeNodeRightClick.Name = "contextMenuStrip1";
            this.TreeNodeRightClick.Size = new System.Drawing.Size(165, 180);            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.addToolStripMenuItem.Text = "Add ...";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.AddToolStripMenuItem_Click);
            // 
            // removeToolStripMenuItem
            // 
            this.removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            this.removeToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.removeToolStripMenuItem.Text = "Remove";
            this.removeToolStripMenuItem.Click += new System.EventHandler(this.RemoveToolStripMenuItem_Click);
            // 
            // duplicateToolStripMenuItem
            // 
            this.duplicateToolStripMenuItem.Name = "duplicateToolStripMenuItem";
            this.duplicateToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.duplicateToolStripMenuItem.Text = "Duplicate";
            this.duplicateToolStripMenuItem.Click += new System.EventHandler(this.duplicateToolStripMenuItem_Click);
            // 
            // ZoomToStripMenuItem
            // 
            this.ZoomToStripMenuItem.Name = "ZoomToStripMenuItem";
            this.ZoomToStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.ZoomToStripMenuItem.Text = "Zoom to";
            // 
            // saveAsFactoryToolStripMenuItem
            // 
            this.saveAsFactoryToolStripMenuItem.Name = "saveAsFactoryToolStripMenuItem";
            this.saveAsFactoryToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.saveAsFactoryToolStripMenuItem.Text = "Save as factory ...";
            this.saveAsFactoryToolStripMenuItem.Click += new System.EventHandler(this.saveAsFactoryToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // aDDBEHAVIURToolStripMenuItem
            // 
            this.aDDBEHAVIURToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.physicBoxToolStripMenuItem,
            this.physicSphereToolStripMenuItem});
            this.aDDBEHAVIURToolStripMenuItem.Name = "aDDBEHAVIURToolStripMenuItem";
            this.aDDBEHAVIURToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.aDDBEHAVIURToolStripMenuItem.Text = "Add behavior";
            this.aDDBEHAVIURToolStripMenuItem.DropDownOpening += new System.EventHandler(this.AddBehaviorToolStripMenuItem_DropDownOpening);
            // 
            // physicBoxToolStripMenuItem
            // 
            this.physicBoxToolStripMenuItem.Name = "physicBoxToolStripMenuItem";
            this.physicBoxToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.physicBoxToolStripMenuItem.Text = "Physic box";
            // 
            // physicSphereToolStripMenuItem
            // 
            this.physicSphereToolStripMenuItem.Name = "physicSphereToolStripMenuItem";
            this.physicSphereToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.physicSphereToolStripMenuItem.Text = "Physic sphere";
        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip TreeNodeRightClick;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeToolStripMenuItem;
        private System.Windows.Forms.Button BtnForceRefresh;
        private System.Windows.Forms.ToolStripMenuItem ZoomToStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button btnAddZobObject;
        private System.Windows.Forms.ToolStripMenuItem saveAsFactoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem duplicateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aDDBEHAVIURToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem physicBoxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem physicSphereToolStripMenuItem;
    }
}
