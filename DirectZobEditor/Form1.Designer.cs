﻿namespace DirectZobEditor
{
    partial class Form1
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

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadSceneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSceneAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSceneToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.assetsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadTextureToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createLightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.spotToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pointToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.directionalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createCameraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createSpriteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createZobObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.setupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.engineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sceneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.FirstPanelSplitter = new System.Windows.Forms.SplitContainer();
            this.ZobObjectListPanel = new System.Windows.Forms.Panel();
            this.SecondPanelSplitter = new System.Windows.Forms.SplitContainer();
            this.ThirdPanelSPlitter = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btnPlay = new System.Windows.Forms.ToolStripButton();
            this.btnPause = new System.Windows.Forms.ToolStripButton();
            this.btnStop = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnGrid = new System.Windows.Forms.ToolStripButton();
            this.btnWireframe = new System.Windows.Forms.ToolStripButton();
            this.btnGizmos = new System.Windows.Forms.ToolStripButton();
            this.btnNormals = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnTranslate = new System.Windows.Forms.ToolStripButton();
            this.btnRotate = new System.Windows.Forms.ToolStripButton();
            this.btnScale = new System.Windows.Forms.ToolStripButton();
            this.EngineRendererPanel = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.textLog = new System.Windows.Forms.RichTextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.propertiesPanel = new System.Windows.Forms.Panel();
            this.EngineControlsFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FirstPanelSplitter)).BeginInit();
            this.FirstPanelSplitter.Panel1.SuspendLayout();
            this.FirstPanelSplitter.Panel2.SuspendLayout();
            this.FirstPanelSplitter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SecondPanelSplitter)).BeginInit();
            this.SecondPanelSplitter.Panel1.SuspendLayout();
            this.SecondPanelSplitter.Panel2.SuspendLayout();
            this.SecondPanelSplitter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ThirdPanelSPlitter)).BeginInit();
            this.ThirdPanelSPlitter.Panel1.SuspendLayout();
            this.ThirdPanelSPlitter.Panel2.SuspendLayout();
            this.ThirdPanelSPlitter.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.propertiesPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 774);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1278, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.assetsToolStripMenuItem,
            this.setupToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1278, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuStrip1_ItemClicked);
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.loadSceneToolStripMenuItem,
            this.saveSceneAsToolStripMenuItem,
            this.saveSceneToolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.fileToolStripMenuItem_Click);
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.newToolStripMenuItem.Text = "New scene";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // loadSceneToolStripMenuItem
            // 
            this.loadSceneToolStripMenuItem.Name = "loadSceneToolStripMenuItem";
            this.loadSceneToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.loadSceneToolStripMenuItem.Text = "Load scene ...";
            this.loadSceneToolStripMenuItem.Click += new System.EventHandler(this.loadSceneToolStripMenuItem_Click);
            // 
            // saveSceneAsToolStripMenuItem
            // 
            this.saveSceneAsToolStripMenuItem.Name = "saveSceneAsToolStripMenuItem";
            this.saveSceneAsToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.saveSceneAsToolStripMenuItem.Text = "Save scene as ...";
            this.saveSceneAsToolStripMenuItem.Click += new System.EventHandler(this.saveSceneAsToolStripMenuItem_Click);
            // 
            // saveSceneToolStripMenuItem1
            // 
            this.saveSceneToolStripMenuItem1.Name = "saveSceneToolStripMenuItem1";
            this.saveSceneToolStripMenuItem1.Size = new System.Drawing.Size(157, 22);
            this.saveSceneToolStripMenuItem1.Text = "Save scene";
            this.saveSceneToolStripMenuItem1.Click += new System.EventHandler(this.saveSceneToolStripMenuItem1_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.ExitToolStripMenuItem_Click);
            // 
            // assetsToolStripMenuItem
            // 
            this.assetsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadTextureToolStripMenuItem,
            this.loadMeshToolStripMenuItem,
            this.createLightToolStripMenuItem,
            this.createCameraToolStripMenuItem,
            this.createSpriteToolStripMenuItem,
            this.createZobObjectToolStripMenuItem});
            this.assetsToolStripMenuItem.Name = "assetsToolStripMenuItem";
            this.assetsToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.assetsToolStripMenuItem.Text = "Assets";
            // 
            // loadTextureToolStripMenuItem
            // 
            this.loadTextureToolStripMenuItem.Name = "loadTextureToolStripMenuItem";
            this.loadTextureToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.loadTextureToolStripMenuItem.Text = "Load Texture ...";
            this.loadTextureToolStripMenuItem.Click += new System.EventHandler(this.LoadTextureToolStripMenuItem_Click);
            // 
            // loadMeshToolStripMenuItem
            // 
            this.loadMeshToolStripMenuItem.Name = "loadMeshToolStripMenuItem";
            this.loadMeshToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.loadMeshToolStripMenuItem.Text = "Load Mesh ...";
            this.loadMeshToolStripMenuItem.Click += new System.EventHandler(this.LoadMeshToolStripMenuItem_Click);
            // 
            // createLightToolStripMenuItem
            // 
            this.createLightToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.spotToolStripMenuItem,
            this.pointToolStripMenuItem,
            this.directionalToolStripMenuItem});
            this.createLightToolStripMenuItem.Name = "createLightToolStripMenuItem";
            this.createLightToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.createLightToolStripMenuItem.Text = "Create Light ";
            // 
            // spotToolStripMenuItem
            // 
            this.spotToolStripMenuItem.Name = "spotToolStripMenuItem";
            this.spotToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.spotToolStripMenuItem.Text = "Spot";
            this.spotToolStripMenuItem.Click += new System.EventHandler(this.spotToolStripMenuItem_Click);
            // 
            // pointToolStripMenuItem
            // 
            this.pointToolStripMenuItem.Name = "pointToolStripMenuItem";
            this.pointToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.pointToolStripMenuItem.Text = "Point";
            this.pointToolStripMenuItem.Click += new System.EventHandler(this.pointToolStripMenuItem_Click);
            // 
            // directionalToolStripMenuItem
            // 
            this.directionalToolStripMenuItem.Name = "directionalToolStripMenuItem";
            this.directionalToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.directionalToolStripMenuItem.Text = "Directional";
            this.directionalToolStripMenuItem.Click += new System.EventHandler(this.directionalToolStripMenuItem_Click);
            // 
            // createCameraToolStripMenuItem
            // 
            this.createCameraToolStripMenuItem.Name = "createCameraToolStripMenuItem";
            this.createCameraToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.createCameraToolStripMenuItem.Text = "Create Camera";
            this.createCameraToolStripMenuItem.Click += new System.EventHandler(this.createCameraToolStripMenuItem_Click);
            // 
            // createSpriteToolStripMenuItem
            // 
            this.createSpriteToolStripMenuItem.Name = "createSpriteToolStripMenuItem";
            this.createSpriteToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.createSpriteToolStripMenuItem.Text = "Create Sprite";
            this.createSpriteToolStripMenuItem.Click += new System.EventHandler(this.createSpriteToolStripMenuItem_Click);
            // 
            // createZobObjectToolStripMenuItem
            // 
            this.createZobObjectToolStripMenuItem.Name = "createZobObjectToolStripMenuItem";
            this.createZobObjectToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.createZobObjectToolStripMenuItem.Text = "Create ZobObject";
            this.createZobObjectToolStripMenuItem.Click += new System.EventHandler(this.createZobObjectToolStripMenuItem_Click);
            // 
            // setupToolStripMenuItem
            // 
            this.setupToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.engineToolStripMenuItem,
            this.sceneToolStripMenuItem});
            this.setupToolStripMenuItem.Name = "setupToolStripMenuItem";
            this.setupToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
            this.setupToolStripMenuItem.Text = "Setup";
            // 
            // engineToolStripMenuItem
            // 
            this.engineToolStripMenuItem.Name = "engineToolStripMenuItem";
            this.engineToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.engineToolStripMenuItem.Text = "Engine ...";
            this.engineToolStripMenuItem.Click += new System.EventHandler(this.engineToolStripMenuItem_Click);
            // 
            // sceneToolStripMenuItem
            // 
            this.sceneToolStripMenuItem.Name = "sceneToolStripMenuItem";
            this.sceneToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.sceneToolStripMenuItem.Text = "Scene ...";
            this.sceneToolStripMenuItem.Click += new System.EventHandler(this.sceneToolStripMenuItem_Click);
            // 
            // FirstPanelSplitter
            // 
            this.FirstPanelSplitter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FirstPanelSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.FirstPanelSplitter.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.FirstPanelSplitter.Location = new System.Drawing.Point(0, 24);
            this.FirstPanelSplitter.Name = "FirstPanelSplitter";
            // 
            // FirstPanelSplitter.Panel1
            // 
            this.FirstPanelSplitter.Panel1.Controls.Add(this.ZobObjectListPanel);
            this.FirstPanelSplitter.Panel1MinSize = 200;
            // 
            // FirstPanelSplitter.Panel2
            // 
            this.FirstPanelSplitter.Panel2.Controls.Add(this.SecondPanelSplitter);
            this.FirstPanelSplitter.Size = new System.Drawing.Size(1278, 750);
            this.FirstPanelSplitter.SplitterDistance = 200;
            this.FirstPanelSplitter.TabIndex = 3;
            // 
            // ZobObjectListPanel
            // 
            this.ZobObjectListPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ZobObjectListPanel.Location = new System.Drawing.Point(0, 0);
            this.ZobObjectListPanel.Name = "ZobObjectListPanel";
            this.ZobObjectListPanel.Size = new System.Drawing.Size(198, 748);
            this.ZobObjectListPanel.TabIndex = 0;
            this.ZobObjectListPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.ZobObjectListPanel_Paint);
            // 
            // SecondPanelSplitter
            // 
            this.SecondPanelSplitter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.SecondPanelSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SecondPanelSplitter.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.SecondPanelSplitter.Location = new System.Drawing.Point(0, 0);
            this.SecondPanelSplitter.Name = "SecondPanelSplitter";
            // 
            // SecondPanelSplitter.Panel1
            // 
            this.SecondPanelSplitter.Panel1.Controls.Add(this.ThirdPanelSPlitter);
            // 
            // SecondPanelSplitter.Panel2
            // 
            this.SecondPanelSplitter.Panel2.AutoScroll = true;
            this.SecondPanelSplitter.Panel2.Controls.Add(this.propertiesPanel);
            this.SecondPanelSplitter.Panel2MinSize = 350;
            this.SecondPanelSplitter.Size = new System.Drawing.Size(1074, 750);
            this.SecondPanelSplitter.SplitterDistance = 700;
            this.SecondPanelSplitter.TabIndex = 0;
            // 
            // ThirdPanelSPlitter
            // 
            this.ThirdPanelSPlitter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ThirdPanelSPlitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ThirdPanelSPlitter.Location = new System.Drawing.Point(0, 0);
            this.ThirdPanelSPlitter.Name = "ThirdPanelSPlitter";
            this.ThirdPanelSPlitter.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // ThirdPanelSPlitter.Panel1
            // 
            this.ThirdPanelSPlitter.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // ThirdPanelSPlitter.Panel2
            // 
            this.ThirdPanelSPlitter.Panel2.Controls.Add(this.tabControl1);
            this.ThirdPanelSPlitter.Size = new System.Drawing.Size(700, 750);
            this.ThirdPanelSPlitter.SplitterDistance = 600;
            this.ThirdPanelSPlitter.TabIndex = 0;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.toolStrip1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.EngineRendererPanel, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(698, 598);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnPlay,
            this.btnPause,
            this.btnStop,
            this.toolStripSeparator1,
            this.btnGrid,
            this.btnWireframe,
            this.btnGizmos,
            this.btnNormals,
            this.toolStripSeparator2,
            this.btnTranslate,
            this.btnRotate,
            this.btnScale});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(698, 25);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btnPlay
            // 
            this.btnPlay.CheckOnClick = true;
            this.btnPlay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPlay.Image = ((System.Drawing.Image)(resources.GetObject("btnPlay.Image")));
            this.btnPlay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(23, 22);
            this.btnPlay.Text = "toolStripButton1";
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // btnPause
            // 
            this.btnPause.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPause.Image = ((System.Drawing.Image)(resources.GetObject("btnPause.Image")));
            this.btnPause.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(23, 22);
            this.btnPause.Text = "toolStripButton2";
            this.btnPause.Click += new System.EventHandler(this.btnPause_Click);
            // 
            // btnStop
            // 
            this.btnStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnStop.Image = ((System.Drawing.Image)(resources.GetObject("btnStop.Image")));
            this.btnStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(23, 22);
            this.btnStop.Text = "toolStripButton3";
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnGrid
            // 
            this.btnGrid.Checked = true;
            this.btnGrid.CheckOnClick = true;
            this.btnGrid.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnGrid.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnGrid.Name = "btnGrid";
            this.btnGrid.Size = new System.Drawing.Size(23, 22);
            this.btnGrid.Text = "Gr";
            this.btnGrid.Click += new System.EventHandler(this.btnGrid_Click);
            // 
            // btnWireframe
            // 
            this.btnWireframe.CheckOnClick = true;
            this.btnWireframe.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnWireframe.Image = ((System.Drawing.Image)(resources.GetObject("btnWireframe.Image")));
            this.btnWireframe.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnWireframe.Name = "btnWireframe";
            this.btnWireframe.Size = new System.Drawing.Size(26, 22);
            this.btnWireframe.Text = "Wf";
            this.btnWireframe.ToolTipText = "Wireframe";
            this.btnWireframe.Click += new System.EventHandler(this.btnWireframe_Click);
            // 
            // btnGizmos
            // 
            this.btnGizmos.Checked = true;
            this.btnGizmos.CheckOnClick = true;
            this.btnGizmos.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnGizmos.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnGizmos.Image = ((System.Drawing.Image)(resources.GetObject("btnGizmos.Image")));
            this.btnGizmos.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnGizmos.Name = "btnGizmos";
            this.btnGizmos.Size = new System.Drawing.Size(23, 22);
            this.btnGizmos.Text = "Gi";
            this.btnGizmos.Click += new System.EventHandler(this.btnGizmos_Click);
            // 
            // btnNormals
            // 
            this.btnNormals.CheckOnClick = true;
            this.btnNormals.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnNormals.Image = ((System.Drawing.Image)(resources.GetObject("btnNormals.Image")));
            this.btnNormals.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNormals.Name = "btnNormals";
            this.btnNormals.Size = new System.Drawing.Size(25, 22);
            this.btnNormals.Text = "Ns";
            this.btnNormals.Click += new System.EventHandler(this.btnNormals_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // btnTranslate
            // 
            this.btnTranslate.Checked = true;
            this.btnTranslate.CheckOnClick = true;
            this.btnTranslate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnTranslate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnTranslate.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnTranslate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnTranslate.Name = "btnTranslate";
            this.btnTranslate.Size = new System.Drawing.Size(23, 22);
            this.btnTranslate.Text = "T";
            this.btnTranslate.Click += new System.EventHandler(this.btnTranslate_Click);
            // 
            // btnRotate
            // 
            this.btnRotate.CheckOnClick = true;
            this.btnRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnRotate.Image = ((System.Drawing.Image)(resources.GetObject("btnRotate.Image")));
            this.btnRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRotate.Name = "btnRotate";
            this.btnRotate.Size = new System.Drawing.Size(23, 22);
            this.btnRotate.Text = "R";
            this.btnRotate.Click += new System.EventHandler(this.btnRotate_Click);
            // 
            // btnScale
            // 
            this.btnScale.CheckOnClick = true;
            this.btnScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnScale.Image = ((System.Drawing.Image)(resources.GetObject("btnScale.Image")));
            this.btnScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnScale.Name = "btnScale";
            this.btnScale.Size = new System.Drawing.Size(23, 22);
            this.btnScale.Text = "S";
            this.btnScale.Click += new System.EventHandler(this.btnScale_Click);
            // 
            // EngineRendererPanel
            // 
            this.EngineRendererPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRendererPanel.Location = new System.Drawing.Point(3, 28);
            this.EngineRendererPanel.Name = "EngineRendererPanel";
            this.EngineRendererPanel.Size = new System.Drawing.Size(692, 598);
            this.EngineRendererPanel.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(698, 144);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.textLog);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(690, 118);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // textLog
            // 
            this.textLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textLog.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textLog.Location = new System.Drawing.Point(3, 3);
            this.textLog.Name = "textLog";
            this.textLog.Size = new System.Drawing.Size(684, 112);
            this.textLog.TabIndex = 0;
            this.textLog.Text = "";
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(690, 118);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // propertiesPanel
            // 
            this.propertiesPanel.Controls.Add(this.EngineControlsFlowLayout);
            this.propertiesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertiesPanel.Location = new System.Drawing.Point(0, 0);
            this.propertiesPanel.MinimumSize = new System.Drawing.Size(300, 0);
            this.propertiesPanel.Name = "propertiesPanel";
            this.propertiesPanel.Size = new System.Drawing.Size(368, 748);
            this.propertiesPanel.TabIndex = 0;
            // 
            // EngineControlsFlowLayout
            // 
            this.EngineControlsFlowLayout.AutoScroll = true;
            this.EngineControlsFlowLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineControlsFlowLayout.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.EngineControlsFlowLayout.Location = new System.Drawing.Point(0, 0);
            this.EngineControlsFlowLayout.MinimumSize = new System.Drawing.Size(300, 0);
            this.EngineControlsFlowLayout.Name = "EngineControlsFlowLayout";
            this.EngineControlsFlowLayout.Size = new System.Drawing.Size(368, 748);
            this.EngineControlsFlowLayout.TabIndex = 0;
            this.EngineControlsFlowLayout.WrapContents = false;
            this.EngineControlsFlowLayout.Resize += new System.EventHandler(this.EngineControlsFlowLayout_Resize);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1278, 796);
            this.Controls.Add(this.FirstPanelSplitter);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "DirectZob";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.FirstPanelSplitter.Panel1.ResumeLayout(false);
            this.FirstPanelSplitter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.FirstPanelSplitter)).EndInit();
            this.FirstPanelSplitter.ResumeLayout(false);
            this.SecondPanelSplitter.Panel1.ResumeLayout(false);
            this.SecondPanelSplitter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.SecondPanelSplitter)).EndInit();
            this.SecondPanelSplitter.ResumeLayout(false);
            this.ThirdPanelSPlitter.Panel1.ResumeLayout(false);
            this.ThirdPanelSPlitter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ThirdPanelSPlitter)).EndInit();
            this.ThirdPanelSPlitter.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.propertiesPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.SplitContainer FirstPanelSplitter;
        private System.Windows.Forms.SplitContainer SecondPanelSplitter;
        private System.Windows.Forms.SplitContainer ThirdPanelSPlitter;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.RichTextBox textLog;
        private System.Windows.Forms.Panel propertiesPanel;
        private System.Windows.Forms.Panel ZobObjectListPanel;
        private System.Windows.Forms.Panel EngineRendererPanel;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadSceneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSceneAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSceneToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem assetsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadTextureToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadMeshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createLightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createCameraToolStripMenuItem;
        private System.Windows.Forms.FlowLayoutPanel EngineControlsFlowLayout;
        private System.Windows.Forms.ToolStripMenuItem spotToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pointToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem directionalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createSpriteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createZobObjectToolStripMenuItem;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btnPlay;
        private System.Windows.Forms.ToolStripButton btnPause;
        private System.Windows.Forms.ToolStripButton btnStop;
        private System.Windows.Forms.ToolStripMenuItem setupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem engineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sceneToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton btnGrid;
        private System.Windows.Forms.ToolStripButton btnWireframe;
        private System.Windows.Forms.ToolStripButton btnGizmos;
        private System.Windows.Forms.ToolStripButton btnNormals;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton btnTranslate;
        private System.Windows.Forms.ToolStripButton btnRotate;
        private System.Windows.Forms.ToolStripButton btnScale;
    }
}

