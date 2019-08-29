namespace DirectZobEditor
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
            this.EngineWIndow = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.EngineWIndow)).BeginInit();
            this.SuspendLayout();
            // 
            // EngineWIndow
            // 
            this.EngineWIndow.Location = new System.Drawing.Point(115, 12);
            this.EngineWIndow.Name = "EngineWIndow";
            this.EngineWIndow.Size = new System.Drawing.Size(572, 370);
            this.EngineWIndow.TabIndex = 0;
            this.EngineWIndow.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1278, 796);
            this.Controls.Add(this.EngineWIndow);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.EngineWIndow)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox EngineWIndow;
    }
}

