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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.btnAddZobObject = new System.Windows.Forms.Button();
            this.BtnForceRefresh = new System.Windows.Forms.Button();
            this.ZobObjectTree = new System.Windows.Forms.TreeView();
            this.TreeNodeRightClick = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ZoomToStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsFactoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.TreeNodeRightClick.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.splitContainer1);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 900);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Objects";
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
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.ZobObjectTree);
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
            // ZobObjectTree
            // 
            this.ZobObjectTree.AllowDrop = true;
            this.ZobObjectTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ZobObjectTree.Location = new System.Drawing.Point(0, 0);
            this.ZobObjectTree.Name = "ZobObjectTree";
            this.ZobObjectTree.Size = new System.Drawing.Size(194, 802);
            this.ZobObjectTree.TabIndex = 0;
            this.ZobObjectTree.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.ZobObjectTree_ItemDrag);
            this.ZobObjectTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.ZobObjectTree_AfterSelect);
            this.ZobObjectTree.DragDrop += new System.Windows.Forms.DragEventHandler(this.ZobObjectTree_DragDrop);
            this.ZobObjectTree.DragEnter += new System.Windows.Forms.DragEventHandler(this.ZobObjectTree_DragEnter);
            this.ZobObjectTree.DragOver += new System.Windows.Forms.DragEventHandler(this.ZobObjectTree_DragOver);
            this.ZobObjectTree.DragLeave += new System.EventHandler(this.ZobObjectTree_DragLeave);
            this.ZobObjectTree.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ZobObjectTree_MouseClick);
            // 
            // TreeNodeRightClick
            // 
            this.TreeNodeRightClick.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.removeToolStripMenuItem,
            this.ZoomToStripMenuItem,
            this.saveAsFactoryToolStripMenuItem,
            this.editToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.TreeNodeRightClick.Name = "contextMenuStrip1";
            this.TreeNodeRightClick.Size = new System.Drawing.Size(181, 158);
            this.TreeNodeRightClick.Closed += new System.Windows.Forms.ToolStripDropDownClosedEventHandler(this.TreeNodeRightClick_Closed);
            this.TreeNodeRightClick.Opening += new System.ComponentModel.CancelEventHandler(this.TreeNodeRightClick_Opening);
            this.TreeNodeRightClick.Opened += new System.EventHandler(this.TreeNodeRightClick_Opened);
            // 
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
            // ZoomToStripMenuItem
            // 
            this.ZoomToStripMenuItem.Name = "ZoomToStripMenuItem";
            this.ZoomToStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.ZoomToStripMenuItem.Text = "Zoom to";
            this.ZoomToStripMenuItem.Click += new System.EventHandler(this.ZoomToStripMenuItem_Click);
            // 
            // saveAsFactoryToolStripMenuItem
            // 
            this.saveAsFactoryToolStripMenuItem.Name = "saveAsFactoryToolStripMenuItem";
            this.saveAsFactoryToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
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
            // ZobObjectListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobObjectListControl";
            this.Size = new System.Drawing.Size(200, 900);
            this.groupBox1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.TreeNodeRightClick.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TreeView ZobObjectTree;
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
    }
}
