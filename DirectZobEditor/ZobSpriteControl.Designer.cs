namespace DirectZobEditor
{
    partial class ZobSpriteControl
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonTexture = new System.Windows.Forms.Button();
            this.textBoxTexturePath = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxTexturePath);
            this.groupBox1.Controls.Add(this.buttonTexture);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 150);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Sprite";
            // 
            // buttonTexture
            // 
            this.buttonTexture.Location = new System.Drawing.Point(42, 40);
            this.buttonTexture.Name = "buttonTexture";
            this.buttonTexture.Size = new System.Drawing.Size(75, 23);
            this.buttonTexture.TabIndex = 0;
            this.buttonTexture.Text = "Texture";
            this.buttonTexture.UseVisualStyleBackColor = true;
            this.buttonTexture.Click += new System.EventHandler(this.buttonTexture_Click);
            // 
            // textBoxTexturePath
            // 
            this.textBoxTexturePath.Location = new System.Drawing.Point(123, 41);
            this.textBoxTexturePath.Name = "textBoxTexturePath";
            this.textBoxTexturePath.Size = new System.Drawing.Size(189, 20);
            this.textBoxTexturePath.TabIndex = 1;
            // 
            // ZobSpriteControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobSpriteControl";
            this.Size = new System.Drawing.Size(340, 150);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBoxTexturePath;
        private System.Windows.Forms.Button buttonTexture;
    }
}
