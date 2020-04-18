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
            this.EngineRender = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // EngineRender
            // 
            this.EngineRender.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.EngineRender.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRender.Location = new System.Drawing.Point(0, 0);
            this.EngineRender.Name = "EngineRender";
            this.EngineRender.Size = new System.Drawing.Size(960, 564);
            this.EngineRender.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.EngineRender.TabIndex = 1;
            this.EngineRender.TabStop = false;
            this.EngineRender.MouseClick += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseClick);
            this.EngineRender.MouseDown += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseDown);
            this.EngineRender.MouseHover += new System.EventHandler(this.EngineRender_MouseHover);
            this.EngineRender.MouseMove += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseMove);
            this.EngineRender.MouseUp += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseUp);
            this.EngineRender.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.EngineRender_MouseWheel);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.EngineRender);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(960, 564);
            this.panel1.TabIndex = 2;
            // 
            // EngineWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "EngineWindow";
            this.Size = new System.Drawing.Size(960, 564);
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).EndInit();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.PictureBox EngineRender;
        private System.Windows.Forms.Panel panel1;
    }
}
