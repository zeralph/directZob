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
            this.EngineRenderPanel = new System.Windows.Forms.Panel();
            this.bCenter = new System.Windows.Forms.Label();
            this.bTZ = new System.Windows.Forms.Label();
            this.bTY = new System.Windows.Forms.Label();
            this.bTX = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).BeginInit();
            this.EngineRenderPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // EngineRender
            // 
            this.EngineRender.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.EngineRender.Location = new System.Drawing.Point(0, 190);
            this.EngineRender.Name = "EngineRender";
            this.EngineRender.Size = new System.Drawing.Size(669, 374);
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
            // EngineRenderPanel
            // 
            this.EngineRenderPanel.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.EngineRenderPanel.Controls.Add(this.bCenter);
            this.EngineRenderPanel.Controls.Add(this.bTZ);
            this.EngineRenderPanel.Controls.Add(this.bTY);
            this.EngineRenderPanel.Controls.Add(this.bTX);
            this.EngineRenderPanel.Controls.Add(this.EngineRender);
            this.EngineRenderPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EngineRenderPanel.Location = new System.Drawing.Point(0, 0);
            this.EngineRenderPanel.Name = "EngineRenderPanel";
            this.EngineRenderPanel.Size = new System.Drawing.Size(960, 564);
            this.EngineRenderPanel.TabIndex = 2;
            // 
            // bCenter
            // 
            this.bCenter.BackColor = System.Drawing.Color.Black;
            this.bCenter.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Bold);
            this.bCenter.ForeColor = System.Drawing.Color.White;
            this.bCenter.Location = new System.Drawing.Point(734, 325);
            this.bCenter.Name = "bCenter";
            this.bCenter.Size = new System.Drawing.Size(30, 25);
            this.bCenter.TabIndex = 9;
            this.bCenter.Text = "O";
            this.bCenter.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bCenter.MouseMove += new System.Windows.Forms.MouseEventHandler(this.bCenter_MouseMove);
            // 
            // bTZ
            // 
            this.bTZ.BackColor = System.Drawing.Color.Blue;
            this.bTZ.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Bold);
            this.bTZ.ForeColor = System.Drawing.Color.White;
            this.bTZ.Location = new System.Drawing.Point(734, 404);
            this.bTZ.Name = "bTZ";
            this.bTZ.Size = new System.Drawing.Size(30, 25);
            this.bTZ.TabIndex = 8;
            this.bTZ.Text = "Z";
            this.bTZ.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bTZ.MouseMove += new System.Windows.Forms.MouseEventHandler(this.bTZ_MouseMove);
            // 
            // bTY
            // 
            this.bTY.BackColor = System.Drawing.Color.Green;
            this.bTY.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Bold);
            this.bTY.ForeColor = System.Drawing.Color.White;
            this.bTY.Location = new System.Drawing.Point(734, 283);
            this.bTY.Name = "bTY";
            this.bTY.Size = new System.Drawing.Size(30, 25);
            this.bTY.TabIndex = 7;
            this.bTY.Text = "Y";
            this.bTY.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bTY.MouseMove += new System.Windows.Forms.MouseEventHandler(this.bTY_MouseMove);
            // 
            // bTX
            // 
            this.bTX.BackColor = System.Drawing.Color.Red;
            this.bTX.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Bold);
            this.bTX.ForeColor = System.Drawing.Color.White;
            this.bTX.Location = new System.Drawing.Point(734, 365);
            this.bTX.Name = "bTX";
            this.bTX.Size = new System.Drawing.Size(30, 25);
            this.bTX.TabIndex = 6;
            this.bTX.Text = "X";
            this.bTX.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bTX.MouseDown += new System.Windows.Forms.MouseEventHandler(this.bTX_MouseDown);
            this.bTX.MouseHover += new System.EventHandler(this.bTX_MouseHover);
            this.bTX.MouseMove += new System.Windows.Forms.MouseEventHandler(this.bTX_MouseMove);
            // 
            // EngineWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.EngineRenderPanel);
            this.Name = "EngineWindow";
            this.Size = new System.Drawing.Size(960, 564);
            ((System.ComponentModel.ISupportInitialize)(this.EngineRender)).EndInit();
            this.EngineRenderPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.PictureBox EngineRender;
        private System.Windows.Forms.Panel EngineRenderPanel;
        private System.Windows.Forms.Label bTX;
        private System.Windows.Forms.Label bTZ;
        private System.Windows.Forms.Label bTY;
        private System.Windows.Forms.Label bCenter;
    }
}
