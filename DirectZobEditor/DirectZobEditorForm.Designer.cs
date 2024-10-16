﻿namespace DirectZobEditor
{
    partial class DirectZobEditorForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DirectZobEditorForm));
            this.bottomStatusBar = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusEngineState = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusScanePath = new System.Windows.Forms.ToolStripStatusLabel();
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
            this.LoadSpriteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createLightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.spotToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pointToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.directionalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createCameraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fixedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fPSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.orbitalFreeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.orbitalToParentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createZobObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.materialsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.entitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.setupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.regenerateZobIdsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.publishToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.packLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.FirstPanelSplitter = new System.Windows.Forms.SplitContainer();
            this.ZobObjectListPanel = new System.Windows.Forms.Panel();
            this.ViewAndLogsAndObjectControl = new System.Windows.Forms.SplitContainer();
            this.ViewAndLogs = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.buttonBar = new System.Windows.Forms.ToolStrip();
            this.btnPlay = new System.Windows.Forms.ToolStripButton();
            this.btnStop = new System.Windows.Forms.ToolStripButton();
            this.btnPause = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnRotateLocal = new System.Windows.Forms.ToolStripButton();
            this.btnTranslateLocal = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnTranslateWorld = new System.Windows.Forms.ToolStripButton();
            this.btnRotateWorld = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnScale = new System.Windows.Forms.ToolStripButton();
            this.toolStripSnap = new System.Windows.Forms.ToolStripDropDownButton();
            this.noneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripComboBoxCurrentCamera = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.btnWireframe = new System.Windows.Forms.ToolStripButton();
            this.btnGizmos = new System.Windows.Forms.ToolStripButton();
            this.btnGrid = new System.Windows.Forms.ToolStripButton();
            this.btnNormals = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.EngineRendererPanel = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.textLog = new System.Windows.Forms.RichTextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabsControl = new System.Windows.Forms.TabControl();
            this.tabObject = new System.Windows.Forms.TabPage();
            this.propertiesPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tabScene = new System.Windows.Forms.TabPage();
            this.globalPropertiesPanel = new System.Windows.Forms.Panel();
            this.SceneControlsFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.tabEngine = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.EngineControlsFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.loadAssetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bottomStatusBar.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FirstPanelSplitter)).BeginInit();
            this.FirstPanelSplitter.Panel1.SuspendLayout();
            this.FirstPanelSplitter.Panel2.SuspendLayout();
            this.FirstPanelSplitter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ViewAndLogsAndObjectControl)).BeginInit();
            this.ViewAndLogsAndObjectControl.Panel1.SuspendLayout();
            this.ViewAndLogsAndObjectControl.Panel2.SuspendLayout();
            this.ViewAndLogsAndObjectControl.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ViewAndLogs)).BeginInit();
            this.ViewAndLogs.Panel1.SuspendLayout();
            this.ViewAndLogs.Panel2.SuspendLayout();
            this.ViewAndLogs.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.buttonBar.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabsControl.SuspendLayout();
            this.tabObject.SuspendLayout();
            this.tabScene.SuspendLayout();
            this.globalPropertiesPanel.SuspendLayout();
            this.tabEngine.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // bottomStatusBar
            // 
            this.bottomStatusBar.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.bottomStatusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusEngineState,
            this.toolStripStatusLabel1,
            this.toolStripStatusScanePath});
            this.bottomStatusBar.Location = new System.Drawing.Point(0, 530);
            this.bottomStatusBar.Name = "bottomStatusBar";
            this.bottomStatusBar.Size = new System.Drawing.Size(1442, 22);
            this.bottomStatusBar.TabIndex = 1;
            this.bottomStatusBar.Text = "statusStrip1";
            // 
            // toolStripStatusEngineState
            // 
            this.toolStripStatusEngineState.Name = "toolStripStatusEngineState";
            this.toolStripStatusEngineState.Size = new System.Drawing.Size(36, 17);
            this.toolStripStatusEngineState.Text = "NULL";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.BorderStyle = System.Windows.Forms.Border3DStyle.RaisedInner;
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // toolStripStatusScanePath
            // 
            this.toolStripStatusScanePath.Name = "toolStripStatusScanePath";
            this.toolStripStatusScanePath.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusScanePath.Text = "toolStripStatusLabel2";
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.assetsToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.setupToolStripMenuItem,
            this.publishToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(3, 1, 0, 1);
            this.menuStrip1.Size = new System.Drawing.Size(1442, 24);
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
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 22);
            this.fileToolStripMenuItem.Text = "File";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.fileToolStripMenuItem_Click);
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
            this.newToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.newToolStripMenuItem.Text = "New scene";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // loadSceneToolStripMenuItem
            // 
            this.loadSceneToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.load;
            this.loadSceneToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.loadSceneToolStripMenuItem.Name = "loadSceneToolStripMenuItem";
            this.loadSceneToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.loadSceneToolStripMenuItem.Text = "Load scene ...";
            this.loadSceneToolStripMenuItem.Click += new System.EventHandler(this.loadSceneToolStripMenuItem_Click);
            // 
            // saveSceneAsToolStripMenuItem
            // 
            this.saveSceneAsToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.save2;
            this.saveSceneAsToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.saveSceneAsToolStripMenuItem.Name = "saveSceneAsToolStripMenuItem";
            this.saveSceneAsToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.saveSceneAsToolStripMenuItem.Text = "Save scene as ...";
            this.saveSceneAsToolStripMenuItem.Click += new System.EventHandler(this.saveSceneAsToolStripMenuItem_Click);
            // 
            // saveSceneToolStripMenuItem1
            // 
            this.saveSceneToolStripMenuItem1.Image = global::DirectZobEditor.Properties.Resources.save2;
            this.saveSceneToolStripMenuItem1.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.saveSceneToolStripMenuItem1.Name = "saveSceneToolStripMenuItem1";
            this.saveSceneToolStripMenuItem1.Size = new System.Drawing.Size(157, 22);
            this.saveSceneToolStripMenuItem1.Text = "Save scene";
            this.saveSceneToolStripMenuItem1.Click += new System.EventHandler(this.saveSceneToolStripMenuItem1_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.exit;
            this.exitToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
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
            this.LoadSpriteToolStripMenuItem,
            this.createLightToolStripMenuItem,
            this.createCameraToolStripMenuItem,
            this.createZobObjectToolStripMenuItem,
            this.loadAssetToolStripMenuItem});
            this.assetsToolStripMenuItem.Name = "assetsToolStripMenuItem";
            this.assetsToolStripMenuItem.Size = new System.Drawing.Size(52, 22);
            this.assetsToolStripMenuItem.Text = "Assets";
            this.assetsToolStripMenuItem.Click += new System.EventHandler(this.assetsToolStripMenuItem_Click);
            // 
            // loadTextureToolStripMenuItem
            // 
            this.loadTextureToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.texture;
            this.loadTextureToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.loadTextureToolStripMenuItem.Name = "loadTextureToolStripMenuItem";
            this.loadTextureToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.loadTextureToolStripMenuItem.Text = "Load Texture ...";
            this.loadTextureToolStripMenuItem.Click += new System.EventHandler(this.LoadTextureToolStripMenuItem_Click);
            // 
            // loadMeshToolStripMenuItem
            // 
            this.loadMeshToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.mesh3;
            this.loadMeshToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.loadMeshToolStripMenuItem.Name = "loadMeshToolStripMenuItem";
            this.loadMeshToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.loadMeshToolStripMenuItem.Text = "Load Mesh ...";
            this.loadMeshToolStripMenuItem.Click += new System.EventHandler(this.LoadMeshToolStripMenuItem_Click);
            // 
            // LoadSpriteToolStripMenuItem
            // 
            this.LoadSpriteToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.sprite;
            this.LoadSpriteToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.LoadSpriteToolStripMenuItem.Name = "LoadSpriteToolStripMenuItem";
            this.LoadSpriteToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.LoadSpriteToolStripMenuItem.Text = "Load Sprite ...";
            this.LoadSpriteToolStripMenuItem.Click += new System.EventHandler(this.createSpriteToolStripMenuItem_Click);
            // 
            // createLightToolStripMenuItem
            // 
            this.createLightToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.spotToolStripMenuItem,
            this.pointToolStripMenuItem,
            this.directionalToolStripMenuItem});
            this.createLightToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.light1;
            this.createLightToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.createLightToolStripMenuItem.Name = "createLightToolStripMenuItem";
            this.createLightToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
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
            this.createCameraToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fixedToolStripMenuItem,
            this.fPSToolStripMenuItem,
            this.orbitalFreeToolStripMenuItem,
            this.orbitalToParentToolStripMenuItem});
            this.createCameraToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.camera2;
            this.createCameraToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.createCameraToolStripMenuItem.Name = "createCameraToolStripMenuItem";
            this.createCameraToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.createCameraToolStripMenuItem.Text = "Create Camera";
            this.createCameraToolStripMenuItem.Click += new System.EventHandler(this.createCameraToolStripMenuItem_Click);
            // 
            // fixedToolStripMenuItem
            // 
            this.fixedToolStripMenuItem.Name = "fixedToolStripMenuItem";
            this.fixedToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.fixedToolStripMenuItem.Text = "Fixed";
            this.fixedToolStripMenuItem.Click += new System.EventHandler(this.fixedToolStripMenuItem_Click);
            // 
            // fPSToolStripMenuItem
            // 
            this.fPSToolStripMenuItem.Name = "fPSToolStripMenuItem";
            this.fPSToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.fPSToolStripMenuItem.Text = "FPS";
            this.fPSToolStripMenuItem.Click += new System.EventHandler(this.fPSToolStripMenuItem_Click);
            // 
            // orbitalFreeToolStripMenuItem
            // 
            this.orbitalFreeToolStripMenuItem.Name = "orbitalFreeToolStripMenuItem";
            this.orbitalFreeToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.orbitalFreeToolStripMenuItem.Text = "Orbital free";
            this.orbitalFreeToolStripMenuItem.Click += new System.EventHandler(this.orbitalFreeToolStripMenuItem_Click);
            // 
            // orbitalToParentToolStripMenuItem
            // 
            this.orbitalToParentToolStripMenuItem.Name = "orbitalToParentToolStripMenuItem";
            this.orbitalToParentToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.orbitalToParentToolStripMenuItem.Text = "Orbital to parent";
            this.orbitalToParentToolStripMenuItem.Click += new System.EventHandler(this.orbitalToParentToolStripMenuItem_Click);
            // 
            // createZobObjectToolStripMenuItem
            // 
            this.createZobObjectToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources._object;
            this.createZobObjectToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.createZobObjectToolStripMenuItem.Name = "createZobObjectToolStripMenuItem";
            this.createZobObjectToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.createZobObjectToolStripMenuItem.Text = "Create ZobObject";
            this.createZobObjectToolStripMenuItem.Click += new System.EventHandler(this.createZobObjectToolStripMenuItem_Click);
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.materialsToolStripMenuItem,
            this.entitiesToolStripMenuItem});
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(63, 22);
            this.windowToolStripMenuItem.Text = "Window";
            // 
            // materialsToolStripMenuItem
            // 
            this.materialsToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources.material;
            this.materialsToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.materialsToolStripMenuItem.Name = "materialsToolStripMenuItem";
            this.materialsToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.materialsToolStripMenuItem.Text = "Materials ...";
            this.materialsToolStripMenuItem.Click += new System.EventHandler(this.materialsToolStripMenuItem_Click);
            // 
            // entitiesToolStripMenuItem
            // 
            this.entitiesToolStripMenuItem.Image = global::DirectZobEditor.Properties.Resources._object;
            this.entitiesToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.entitiesToolStripMenuItem.Name = "entitiesToolStripMenuItem";
            this.entitiesToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.entitiesToolStripMenuItem.Text = "Entities ...";
            this.entitiesToolStripMenuItem.Click += new System.EventHandler(this.entitiesToolStripMenuItem_Click);
            // 
            // setupToolStripMenuItem
            // 
            this.setupToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.regenerateZobIdsToolStripMenuItem});
            this.setupToolStripMenuItem.Name = "setupToolStripMenuItem";
            this.setupToolStripMenuItem.Size = new System.Drawing.Size(49, 22);
            this.setupToolStripMenuItem.Text = "Setup";
            // 
            // regenerateZobIdsToolStripMenuItem
            // 
            this.regenerateZobIdsToolStripMenuItem.Name = "regenerateZobIdsToolStripMenuItem";
            this.regenerateZobIdsToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.regenerateZobIdsToolStripMenuItem.Text = "Regenerate ZobIds";
            this.regenerateZobIdsToolStripMenuItem.Click += new System.EventHandler(this.regenerateZobIdsToolStripMenuItem_Click);
            // 
            // publishToolStripMenuItem
            // 
            this.publishToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.packLevelToolStripMenuItem});
            this.publishToolStripMenuItem.Name = "publishToolStripMenuItem";
            this.publishToolStripMenuItem.Size = new System.Drawing.Size(58, 22);
            this.publishToolStripMenuItem.Text = "Publish";
            // 
            // packLevelToolStripMenuItem
            // 
            this.packLevelToolStripMenuItem.Name = "packLevelToolStripMenuItem";
            this.packLevelToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.packLevelToolStripMenuItem.Text = "Pack level";
            this.packLevelToolStripMenuItem.Click += new System.EventHandler(this.packLevelToolStripMenuItem_Click);
            // 
            // FirstPanelSplitter
            // 
            this.FirstPanelSplitter.BackColor = System.Drawing.SystemColors.Control;
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
            this.FirstPanelSplitter.Panel2.Controls.Add(this.ViewAndLogsAndObjectControl);
            this.FirstPanelSplitter.Size = new System.Drawing.Size(1442, 506);
            this.FirstPanelSplitter.SplitterDistance = 200;
            this.FirstPanelSplitter.TabIndex = 3;
            // 
            // ZobObjectListPanel
            // 
            this.ZobObjectListPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.ZobObjectListPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ZobObjectListPanel.Location = new System.Drawing.Point(0, 0);
            this.ZobObjectListPanel.Name = "ZobObjectListPanel";
            this.ZobObjectListPanel.Size = new System.Drawing.Size(198, 504);
            this.ZobObjectListPanel.TabIndex = 0;
            this.ZobObjectListPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.ZobObjectListPanel_Paint);
            // 
            // ViewAndLogsAndObjectControl
            // 
            this.ViewAndLogsAndObjectControl.BackColor = System.Drawing.Color.White;
            this.ViewAndLogsAndObjectControl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ViewAndLogsAndObjectControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewAndLogsAndObjectControl.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.ViewAndLogsAndObjectControl.Location = new System.Drawing.Point(0, 0);
            this.ViewAndLogsAndObjectControl.Name = "ViewAndLogsAndObjectControl";
            // 
            // ViewAndLogsAndObjectControl.Panel1
            // 
            this.ViewAndLogsAndObjectControl.Panel1.Controls.Add(this.ViewAndLogs);
            // 
            // ViewAndLogsAndObjectControl.Panel2
            // 
            this.ViewAndLogsAndObjectControl.Panel2.AutoScroll = true;
            this.ViewAndLogsAndObjectControl.Panel2.Controls.Add(this.tabsControl);
            this.ViewAndLogsAndObjectControl.Panel2MinSize = 100;
            this.ViewAndLogsAndObjectControl.Size = new System.Drawing.Size(1238, 506);
            this.ViewAndLogsAndObjectControl.SplitterDistance = 824;
            this.ViewAndLogsAndObjectControl.TabIndex = 0;
            // 
            // ViewAndLogs
            // 
            this.ViewAndLogs.BackColor = System.Drawing.Color.White;
            this.ViewAndLogs.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ViewAndLogs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewAndLogs.Location = new System.Drawing.Point(0, 0);
            this.ViewAndLogs.Name = "ViewAndLogs";
            this.ViewAndLogs.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // ViewAndLogs.Panel1
            // 
            this.ViewAndLogs.Panel1.Controls.Add(this.tableLayoutPanel1);
            this.ViewAndLogs.Panel1MinSize = 0;
            // 
            // ViewAndLogs.Panel2
            // 
            this.ViewAndLogs.Panel2.Controls.Add(this.tabControl1);
            this.ViewAndLogs.Panel2MinSize = 0;
            this.ViewAndLogs.Size = new System.Drawing.Size(824, 506);
            this.ViewAndLogs.SplitterDistance = 399;
            this.ViewAndLogs.TabIndex = 0;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.buttonBar, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.EngineRendererPanel, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(822, 397);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // buttonBar
            // 
            this.buttonBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.buttonBar.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.buttonBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnPlay,
            this.btnStop,
            this.btnPause,
            this.toolStripSeparator1,
            this.btnRotateLocal,
            this.btnTranslateLocal,
            this.toolStripSeparator2,
            this.btnTranslateWorld,
            this.btnRotateWorld,
            this.toolStripSeparator3,
            this.btnScale,
            this.toolStripSnap,
            this.toolStripSeparator4,
            this.toolStripLabel1,
            this.toolStripComboBoxCurrentCamera,
            this.toolStripSeparator5,
            this.btnWireframe,
            this.btnGizmos,
            this.btnGrid,
            this.btnNormals,
            this.toolStripButton1,
            this.toolStripButton2,
            this.toolStripButton3});
            this.buttonBar.Location = new System.Drawing.Point(0, 0);
            this.buttonBar.Name = "buttonBar";
            this.buttonBar.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.buttonBar.Size = new System.Drawing.Size(822, 25);
            this.buttonBar.TabIndex = 1;
            this.buttonBar.Text = "toolStrip1";
            // 
            // btnPlay
            // 
            this.btnPlay.AutoSize = false;
            this.btnPlay.CheckOnClick = true;
            this.btnPlay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPlay.Image = global::DirectZobEditor.Properties.Resources.play;
            this.btnPlay.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnPlay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(23, 22);
            this.btnPlay.Text = "toolStripButton1";
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // btnStop
            // 
            this.btnStop.AutoSize = false;
            this.btnStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnStop.Image = global::DirectZobEditor.Properties.Resources.stop;
            this.btnStop.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(23, 22);
            this.btnStop.Text = "toolStripButton3";
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnPause
            // 
            this.btnPause.AutoSize = false;
            this.btnPause.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPause.Image = global::DirectZobEditor.Properties.Resources.pause;
            this.btnPause.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnPause.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(23, 22);
            this.btnPause.Text = "toolStripButton2";
            this.btnPause.Click += new System.EventHandler(this.btnPause_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnRotateLocal
            // 
            this.btnRotateLocal.AutoSize = false;
            this.btnRotateLocal.CheckOnClick = true;
            this.btnRotateLocal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnRotateLocal.Image = global::DirectZobEditor.Properties.Resources.rotate_L;
            this.btnRotateLocal.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnRotateLocal.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRotateLocal.Name = "btnRotateLocal";
            this.btnRotateLocal.Size = new System.Drawing.Size(23, 22);
            this.btnRotateLocal.Text = "RL";
            this.btnRotateLocal.ToolTipText = "Rotate in object\'s space";
            this.btnRotateLocal.Click += new System.EventHandler(this.btnRotateLocal_Click);
            // 
            // btnTranslateLocal
            // 
            this.btnTranslateLocal.AutoSize = false;
            this.btnTranslateLocal.Checked = true;
            this.btnTranslateLocal.CheckOnClick = true;
            this.btnTranslateLocal.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnTranslateLocal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnTranslateLocal.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnTranslateLocal.Image = global::DirectZobEditor.Properties.Resources.move_W;
            this.btnTranslateLocal.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnTranslateLocal.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnTranslateLocal.Name = "btnTranslateLocal";
            this.btnTranslateLocal.Size = new System.Drawing.Size(23, 22);
            this.btnTranslateLocal.ToolTipText = "Translate in object\'s space";
            this.btnTranslateLocal.Click += new System.EventHandler(this.btnTranslateLocal_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // btnTranslateWorld
            // 
            this.btnTranslateWorld.AutoSize = false;
            this.btnTranslateWorld.CheckOnClick = true;
            this.btnTranslateWorld.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnTranslateWorld.Image = global::DirectZobEditor.Properties.Resources.rotate_L;
            this.btnTranslateWorld.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnTranslateWorld.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnTranslateWorld.Name = "btnTranslateWorld";
            this.btnTranslateWorld.Size = new System.Drawing.Size(23, 22);
            this.btnTranslateWorld.Text = "TW";
            this.btnTranslateWorld.ToolTipText = "Rotate in world space";
            this.btnTranslateWorld.Click += new System.EventHandler(this.btnTranslateWorld_Click);
            // 
            // btnRotateWorld
            // 
            this.btnRotateWorld.AutoSize = false;
            this.btnRotateWorld.CheckOnClick = true;
            this.btnRotateWorld.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnRotateWorld.Image = global::DirectZobEditor.Properties.Resources.rotate_W;
            this.btnRotateWorld.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnRotateWorld.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRotateWorld.Name = "btnRotateWorld";
            this.btnRotateWorld.Size = new System.Drawing.Size(23, 22);
            this.btnRotateWorld.Text = "RW";
            this.btnRotateWorld.Click += new System.EventHandler(this.btnRotateWorld_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnScale
            // 
            this.btnScale.AutoSize = false;
            this.btnScale.CheckOnClick = true;
            this.btnScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnScale.Image = global::DirectZobEditor.Properties.Resources.Scale;
            this.btnScale.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnScale.Name = "btnScale";
            this.btnScale.Size = new System.Drawing.Size(23, 22);
            this.btnScale.Text = "S";
            this.btnScale.ToolTipText = "Scale";
            this.btnScale.Click += new System.EventHandler(this.btnScale_Click);
            // 
            // toolStripSnap
            // 
            this.toolStripSnap.AutoSize = false;
            this.toolStripSnap.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSnap.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.noneToolStripMenuItem,
            this.toolStripMenuItem2,
            this.toolStripMenuItem3,
            this.toolStripMenuItem4});
            this.toolStripSnap.Image = global::DirectZobEditor.Properties.Resources.snap;
            this.toolStripSnap.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripSnap.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSnap.Name = "toolStripSnap";
            this.toolStripSnap.Size = new System.Drawing.Size(29, 22);
            this.toolStripSnap.Text = "Snap";
            this.toolStripSnap.DropDownItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStripSnap_DropDownItemClicked);
            // 
            // noneToolStripMenuItem
            // 
            this.noneToolStripMenuItem.Name = "noneToolStripMenuItem";
            this.noneToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.noneToolStripMenuItem.Text = "none";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem2.Text = "1";
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem3.Text = "0.5";
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem4.Text = "0.1";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.AutoSize = false;
            this.toolStripLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripLabel1.Image = global::DirectZobEditor.Properties.Resources.camera2;
            this.toolStripLabel1.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(22, 22);
            this.toolStripLabel1.Text = "Camera";
            // 
            // toolStripComboBoxCurrentCamera
            // 
            this.toolStripComboBoxCurrentCamera.Name = "toolStripComboBoxCurrentCamera";
            this.toolStripComboBoxCurrentCamera.Size = new System.Drawing.Size(120, 25);
            this.toolStripComboBoxCurrentCamera.SelectedIndexChanged += new System.EventHandler(this.toolStripComboBoxCurrentCamera_SelectedIndexChanged);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
            // 
            // btnWireframe
            // 
            this.btnWireframe.AutoSize = false;
            this.btnWireframe.CheckOnClick = true;
            this.btnWireframe.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnWireframe.Image = global::DirectZobEditor.Properties.Resources.wireframe;
            this.btnWireframe.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnWireframe.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnWireframe.Name = "btnWireframe";
            this.btnWireframe.Size = new System.Drawing.Size(23, 22);
            this.btnWireframe.Text = "Wf";
            this.btnWireframe.ToolTipText = "Wireframe";
            this.btnWireframe.Click += new System.EventHandler(this.btnWireframe_Click);
            // 
            // btnGizmos
            // 
            this.btnGizmos.Checked = true;
            this.btnGizmos.CheckOnClick = true;
            this.btnGizmos.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnGizmos.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnGizmos.Image = global::DirectZobEditor.Properties.Resources.gizmo2;
            this.btnGizmos.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnGizmos.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnGizmos.Name = "btnGizmos";
            this.btnGizmos.Size = new System.Drawing.Size(23, 22);
            this.btnGizmos.Text = "Gi";
            this.btnGizmos.Click += new System.EventHandler(this.btnGizmos_Click);
            // 
            // btnGrid
            // 
            this.btnGrid.Checked = true;
            this.btnGrid.CheckOnClick = true;
            this.btnGrid.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnGrid.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnGrid.Image = global::DirectZobEditor.Properties.Resources.grid;
            this.btnGrid.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnGrid.Name = "btnGrid";
            this.btnGrid.Size = new System.Drawing.Size(23, 22);
            this.btnGrid.Text = "Gr";
            this.btnGrid.Click += new System.EventHandler(this.btnGrid_Click);
            // 
            // btnNormals
            // 
            this.btnNormals.AutoSize = false;
            this.btnNormals.CheckOnClick = true;
            this.btnNormals.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnNormals.Image = global::DirectZobEditor.Properties.Resources.normals2;
            this.btnNormals.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnNormals.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNormals.Name = "btnNormals";
            this.btnNormals.Size = new System.Drawing.Size(25, 22);
            this.btnNormals.Text = "Ns";
            this.btnNormals.Click += new System.EventHandler(this.btnNormals_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = global::DirectZobEditor.Properties.Resources.text;
            this.toolStripButton1.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = global::DirectZobEditor.Properties.Resources.physic;
            this.toolStripButton2.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton2.Text = "toolStripButton2";
            this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click);
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton3.Image = global::DirectZobEditor.Properties.Resources.bbox;
            this.toolStripButton3.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton3.Text = "toolStripButton3";
            // 
            // EngineRendererPanel
            // 
            this.EngineRendererPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRendererPanel.Location = new System.Drawing.Point(3, 28);
            this.EngineRendererPanel.Name = "EngineRendererPanel";
            this.EngineRendererPanel.Size = new System.Drawing.Size(816, 635);
            this.EngineRendererPanel.TabIndex = 0;
            this.EngineRendererPanel.Resize += new System.EventHandler(this.EngineRendererPanel_Resize);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.MinimumSize = new System.Drawing.Size(0, 50);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(822, 101);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.textLog);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(814, 75);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // textLog
            // 
            this.textLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textLog.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textLog.Location = new System.Drawing.Point(3, 3);
            this.textLog.MinimumSize = new System.Drawing.Size(4, 50);
            this.textLog.Name = "textLog";
            this.textLog.Size = new System.Drawing.Size(808, 69);
            this.textLog.TabIndex = 0;
            this.textLog.Text = "";
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(814, 75);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabsControl
            // 
            this.tabsControl.Controls.Add(this.tabObject);
            this.tabsControl.Controls.Add(this.tabScene);
            this.tabsControl.Controls.Add(this.tabEngine);
            this.tabsControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabsControl.Location = new System.Drawing.Point(0, 0);
            this.tabsControl.Name = "tabsControl";
            this.tabsControl.SelectedIndex = 0;
            this.tabsControl.Size = new System.Drawing.Size(408, 504);
            this.tabsControl.TabIndex = 0;
            // 
            // tabObject
            // 
            this.tabObject.BackColor = System.Drawing.SystemColors.Control;
            this.tabObject.Controls.Add(this.propertiesPanel);
            this.tabObject.Location = new System.Drawing.Point(4, 22);
            this.tabObject.Name = "tabObject";
            this.tabObject.Padding = new System.Windows.Forms.Padding(3);
            this.tabObject.Size = new System.Drawing.Size(400, 478);
            this.tabObject.TabIndex = 0;
            this.tabObject.Text = "Object";
            // 
            // propertiesPanel
            // 
            this.propertiesPanel.AutoScroll = true;
            this.propertiesPanel.ColumnCount = 1;
            this.propertiesPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.propertiesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertiesPanel.Location = new System.Drawing.Point(3, 3);
            this.propertiesPanel.Name = "propertiesPanel";
            this.propertiesPanel.RowCount = 1;
            this.propertiesPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.propertiesPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 472F));
            this.propertiesPanel.Size = new System.Drawing.Size(394, 472);
            this.propertiesPanel.TabIndex = 1;
            // 
            // tabScene
            // 
            this.tabScene.BackColor = System.Drawing.SystemColors.Control;
            this.tabScene.Controls.Add(this.globalPropertiesPanel);
            this.tabScene.Location = new System.Drawing.Point(4, 22);
            this.tabScene.Name = "tabScene";
            this.tabScene.Padding = new System.Windows.Forms.Padding(3);
            this.tabScene.Size = new System.Drawing.Size(400, 478);
            this.tabScene.TabIndex = 1;
            this.tabScene.Text = "Scene";
            // 
            // globalPropertiesPanel
            // 
            this.globalPropertiesPanel.Controls.Add(this.SceneControlsFlowLayout);
            this.globalPropertiesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.globalPropertiesPanel.Location = new System.Drawing.Point(3, 3);
            this.globalPropertiesPanel.MinimumSize = new System.Drawing.Size(300, 0);
            this.globalPropertiesPanel.Name = "globalPropertiesPanel";
            this.globalPropertiesPanel.Size = new System.Drawing.Size(394, 472);
            this.globalPropertiesPanel.TabIndex = 1;
            // 
            // SceneControlsFlowLayout
            // 
            this.SceneControlsFlowLayout.AutoScroll = true;
            this.SceneControlsFlowLayout.BackColor = System.Drawing.SystemColors.Control;
            this.SceneControlsFlowLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SceneControlsFlowLayout.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.SceneControlsFlowLayout.Location = new System.Drawing.Point(0, 0);
            this.SceneControlsFlowLayout.MinimumSize = new System.Drawing.Size(300, 0);
            this.SceneControlsFlowLayout.Name = "SceneControlsFlowLayout";
            this.SceneControlsFlowLayout.Size = new System.Drawing.Size(394, 472);
            this.SceneControlsFlowLayout.TabIndex = 0;
            this.SceneControlsFlowLayout.WrapContents = false;
            // 
            // tabEngine
            // 
            this.tabEngine.BackColor = System.Drawing.SystemColors.Control;
            this.tabEngine.Controls.Add(this.panel2);
            this.tabEngine.Location = new System.Drawing.Point(4, 22);
            this.tabEngine.Name = "tabEngine";
            this.tabEngine.Padding = new System.Windows.Forms.Padding(3);
            this.tabEngine.Size = new System.Drawing.Size(400, 478);
            this.tabEngine.TabIndex = 2;
            this.tabEngine.Text = "Engine";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.EngineControlsFlowLayout);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.MinimumSize = new System.Drawing.Size(300, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(394, 472);
            this.panel2.TabIndex = 1;
            // 
            // EngineControlsFlowLayout
            // 
            this.EngineControlsFlowLayout.AutoScroll = true;
            this.EngineControlsFlowLayout.BackColor = System.Drawing.SystemColors.Control;
            this.EngineControlsFlowLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineControlsFlowLayout.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.EngineControlsFlowLayout.Location = new System.Drawing.Point(0, 0);
            this.EngineControlsFlowLayout.MinimumSize = new System.Drawing.Size(300, 0);
            this.EngineControlsFlowLayout.Name = "EngineControlsFlowLayout";
            this.EngineControlsFlowLayout.Size = new System.Drawing.Size(394, 472);
            this.EngineControlsFlowLayout.TabIndex = 0;
            this.EngineControlsFlowLayout.WrapContents = false;
            // 
            // loadAssetToolStripMenuItem
            // 
            this.loadAssetToolStripMenuItem.Name = "loadAssetToolStripMenuItem";
            this.loadAssetToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.loadAssetToolStripMenuItem.Text = "Load Asset ...";
            this.loadAssetToolStripMenuItem.Click += new System.EventHandler(this.loadAssetToolStripMenuItem_Click);
            // 
            // DirectZobEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1442, 552);
            this.Controls.Add(this.FirstPanelSplitter);
            this.Controls.Add(this.bottomStatusBar);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "DirectZobEditorForm";
            this.Text = "DirectZob";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.bottomStatusBar.ResumeLayout(false);
            this.bottomStatusBar.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.FirstPanelSplitter.Panel1.ResumeLayout(false);
            this.FirstPanelSplitter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.FirstPanelSplitter)).EndInit();
            this.FirstPanelSplitter.ResumeLayout(false);
            this.ViewAndLogsAndObjectControl.Panel1.ResumeLayout(false);
            this.ViewAndLogsAndObjectControl.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ViewAndLogsAndObjectControl)).EndInit();
            this.ViewAndLogsAndObjectControl.ResumeLayout(false);
            this.ViewAndLogs.Panel1.ResumeLayout(false);
            this.ViewAndLogs.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ViewAndLogs)).EndInit();
            this.ViewAndLogs.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.buttonBar.ResumeLayout(false);
            this.buttonBar.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabsControl.ResumeLayout(false);
            this.tabObject.ResumeLayout(false);
            this.tabScene.ResumeLayout(false);
            this.globalPropertiesPanel.ResumeLayout(false);
            this.tabEngine.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.StatusStrip bottomStatusBar;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.SplitContainer FirstPanelSplitter;
        private System.Windows.Forms.SplitContainer ViewAndLogsAndObjectControl;
        private System.Windows.Forms.SplitContainer ViewAndLogs;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.RichTextBox textLog;
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
        private System.Windows.Forms.ToolStripMenuItem spotToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pointToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem directionalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem LoadSpriteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createZobObjectToolStripMenuItem;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ToolStrip buttonBar;
        private System.Windows.Forms.ToolStripButton btnPlay;
        private System.Windows.Forms.ToolStripButton btnPause;
        private System.Windows.Forms.ToolStripButton btnStop;
        private System.Windows.Forms.ToolStripMenuItem setupToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton btnGrid;
        private System.Windows.Forms.ToolStripButton btnWireframe;
        private System.Windows.Forms.ToolStripButton btnGizmos;
        private System.Windows.Forms.ToolStripButton btnNormals;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton btnTranslateLocal;
        private System.Windows.Forms.ToolStripButton btnRotateLocal;
        private System.Windows.Forms.ToolStripButton btnScale;
        private System.Windows.Forms.ToolStripButton btnTranslateWorld;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton btnRotateWorld;
        private System.Windows.Forms.TabControl tabsControl;
        private System.Windows.Forms.TabPage tabObject;
        private System.Windows.Forms.TabPage tabScene;
        private System.Windows.Forms.TabPage tabEngine;
        private System.Windows.Forms.Panel globalPropertiesPanel;
        private System.Windows.Forms.FlowLayoutPanel SceneControlsFlowLayout;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.FlowLayoutPanel EngineControlsFlowLayout;
        private System.Windows.Forms.ToolStripMenuItem fixedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fPSToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem orbitalFreeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem orbitalToParentToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusEngineState;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBoxCurrentCamera;
        private System.Windows.Forms.ToolStripMenuItem regenerateZobIdsToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.TableLayoutPanel propertiesPanel;
        private System.Windows.Forms.ToolStripDropDownButton toolStripSnap;
        private System.Windows.Forms.ToolStripMenuItem noneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem materialsToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusScanePath;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.ToolStripMenuItem entitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem publishToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem packLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadAssetToolStripMenuItem;
    }
}

