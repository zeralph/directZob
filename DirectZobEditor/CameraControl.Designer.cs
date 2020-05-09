namespace DirectZobEditor
{
    partial class CameraControl
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
            this.camerasBox = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cameraBox = new System.Windows.Forms.ComboBox();
            this.camerasBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // camerasBox
            // 
            this.camerasBox.Controls.Add(this.label1);
            this.camerasBox.Controls.Add(this.cameraBox);
            this.camerasBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.camerasBox.Location = new System.Drawing.Point(0, 0);
            this.camerasBox.Margin = new System.Windows.Forms.Padding(10);
            this.camerasBox.Name = "camerasBox";
            this.camerasBox.Padding = new System.Windows.Forms.Padding(10);
            this.camerasBox.Size = new System.Drawing.Size(200, 84);
            this.camerasBox.TabIndex = 1;
            this.camerasBox.TabStop = false;
            this.camerasBox.Text = "Cameras";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Current camera";
            // 
            // cameraBox
            // 
            this.cameraBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cameraBox.FormattingEnabled = true;
            this.cameraBox.Location = new System.Drawing.Point(13, 36);
            this.cameraBox.Name = "cameraBox";
            this.cameraBox.Size = new System.Drawing.Size(162, 21);
            this.cameraBox.TabIndex = 0;
            this.cameraBox.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // CameraControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.camerasBox);
            this.MinimumSize = new System.Drawing.Size(200, 50);
            this.Name = "CameraControl";
            this.Size = new System.Drawing.Size(200, 84);
            this.camerasBox.ResumeLayout(false);
            this.camerasBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox camerasBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cameraBox;
    }
}
