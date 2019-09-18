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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.EngineRender = new System.Windows.Forms.PictureBox();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(960, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // menuToolStripMenuItem
            // 
            this.menuToolStripMenuItem.Name = "menuToolStripMenuItem";
            this.menuToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.menuToolStripMenuItem.Text = "menu";
            // 
            // EngineRender
            // 
            this.EngineRender.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.EngineRender.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRender.Location = new System.Drawing.Point(0, 24);
            this.EngineRender.Name = "EngineRender";
            this.EngineRender.Size = new System.Drawing.Size(960, 540);
            this.EngineRender.TabIndex = 1;
            this.EngineRender.TabStop = false;
            this.EngineRender.MouseDown += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseDown);
            this.EngineRender.MouseHover += new System.EventHandler(this.EngineRender_MouseHover);
            this.EngineRender.MouseMove += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseMove);
            this.EngineRender.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseWheel);
            // 
            // EngineWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.EngineRender);
            this.Controls.Add(this.menuStrip1);
            this.Name = "EngineWindow";
            this.Size = new System.Drawing.Size(960, 564);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuToolStripMenuItem;
        private System.Windows.Forms.PictureBox EngineRender;
    }
}
