namespace DirectZobEditor
{
    partial class ZobMeshControl
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
            this.groupMesh = new System.Windows.Forms.GroupBox();
            this.SuspendLayout();
            // 
            // groupMesh
            // 
            this.groupMesh.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupMesh.Location = new System.Drawing.Point(0, 0);
            this.groupMesh.Name = "groupMesh";
            this.groupMesh.Size = new System.Drawing.Size(340, 150);
            this.groupMesh.TabIndex = 0;
            this.groupMesh.TabStop = false;
            this.groupMesh.Text = "Mesh properties";
            this.groupMesh.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // ZobMeshControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupMesh);
            this.Name = "ZobMeshControl";
            this.Size = new System.Drawing.Size(340, 150);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupMesh;
    }
}
