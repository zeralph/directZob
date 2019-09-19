namespace DirectZobEditor
{
    partial class EngineWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EngineWindow));
            this.EngineRender = new System.Windows.Forms.PictureBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.TransformButton = new System.Windows.Forms.ToolStripButton();
            this.RotateButton = new System.Windows.Forms.ToolStripButton();
            this.ScaleButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.renderMode = new System.Windows.Forms.ToolStripComboBox();
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // EngineRender
            // 
            this.EngineRender.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.EngineRender.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRender.Location = new System.Drawing.Point(0, 0);
            this.EngineRender.Name = "EngineRender";
            this.EngineRender.Size = new System.Drawing.Size(960, 564);
            this.EngineRender.TabIndex = 1;
            this.EngineRender.TabStop = false;
            this.EngineRender.MouseClick += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseClick);
            this.EngineRender.MouseDown += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseDown);
            this.EngineRender.MouseHover += new System.EventHandler(this.EngineRender_MouseHover);
            this.EngineRender.MouseMove += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseMove);
            this.EngineRender.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseWheel);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TransformButton,
            this.RotateButton,
            this.ScaleButton,
            this.toolStripSeparator1,
            this.renderMode});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(960, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // TransformButton
            // 
            this.TransformButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.TransformButton.Image = ((System.Drawing.Image)(resources.GetObject("TransformButton.Image")));
            this.TransformButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.TransformButton.Name = "TransformButton";
            this.TransformButton.Size = new System.Drawing.Size(23, 22);
            this.TransformButton.Text = "T";
            this.TransformButton.Click += new System.EventHandler(this.TRSButton_Click);
            // 
            // RotateButton
            // 
            this.RotateButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.RotateButton.Image = ((System.Drawing.Image)(resources.GetObject("RotateButton.Image")));
            this.RotateButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.RotateButton.Name = "RotateButton";
            this.RotateButton.Size = new System.Drawing.Size(23, 22);
            this.RotateButton.Text = "R";
            this.RotateButton.Click += new System.EventHandler(this.TRSButton_Click);
            // 
            // ScaleButton
            // 
            this.ScaleButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ScaleButton.Image = ((System.Drawing.Image)(resources.GetObject("ScaleButton.Image")));
            this.ScaleButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ScaleButton.Name = "ScaleButton";
            this.ScaleButton.Size = new System.Drawing.Size(23, 22);
            this.ScaleButton.Text = "S";
            this.ScaleButton.Click += new System.EventHandler(this.TRSButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // renderMode
            // 
            this.renderMode.Items.AddRange(new object[] {
            "Render",
            "ZBuffer",
            "OBuffer"});
            this.renderMode.Name = "renderMode";
            this.renderMode.Size = new System.Drawing.Size(121, 25);
            // 
            // EngineWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.EngineRender);
            this.Name = "EngineWindow";
            this.Size = new System.Drawing.Size(960, 564);
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.PictureBox EngineRender;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton TransformButton;
        private System.Windows.Forms.ToolStripButton RotateButton;
        private System.Windows.Forms.ToolStripButton ScaleButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripComboBox renderMode;
    }
}
