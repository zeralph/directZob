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
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.FirstPanelSplitter = new System.Windows.Forms.SplitContainer();
            this.ZobObjectListPanel = new System.Windows.Forms.Panel();
            this.SecondPanelSplitter = new System.Windows.Forms.SplitContainer();
            this.ThirdPanelSPlitter = new System.Windows.Forms.SplitContainer();
            this.EngineRendererPanel = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.textLog = new System.Windows.Forms.RichTextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.propertiesPanel = new System.Windows.Forms.Panel();
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
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
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
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1278, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
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
            this.SecondPanelSplitter.Panel2.Controls.Add(this.propertiesPanel);
            this.SecondPanelSplitter.Size = new System.Drawing.Size(1074, 750);
            this.SecondPanelSplitter.SplitterDistance = 876;
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
            this.ThirdPanelSPlitter.Panel1.Controls.Add(this.EngineRendererPanel);
            // 
            // ThirdPanelSPlitter.Panel2
            // 
            this.ThirdPanelSPlitter.Panel2.Controls.Add(this.tabControl1);
            this.ThirdPanelSPlitter.Size = new System.Drawing.Size(876, 750);
            this.ThirdPanelSPlitter.SplitterDistance = 416;
            this.ThirdPanelSPlitter.TabIndex = 0;
            // 
            // EngineRendererPanel
            // 
            this.EngineRendererPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRendererPanel.Location = new System.Drawing.Point(0, 0);
            this.EngineRendererPanel.Name = "EngineRendererPanel";
            this.EngineRendererPanel.Size = new System.Drawing.Size(874, 414);
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
            this.tabControl1.Size = new System.Drawing.Size(874, 328);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.textLog);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(866, 302);
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
            this.textLog.Size = new System.Drawing.Size(860, 296);
            this.textLog.TabIndex = 0;
            this.textLog.Text = "";
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(866, 303);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // propertiesPanel
            // 
            this.propertiesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertiesPanel.Location = new System.Drawing.Point(0, 0);
            this.propertiesPanel.Name = "propertiesPanel";
            this.propertiesPanel.Size = new System.Drawing.Size(192, 748);
            this.propertiesPanel.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1278, 796);
            this.Controls.Add(this.FirstPanelSplitter);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
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
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
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
    }
}

