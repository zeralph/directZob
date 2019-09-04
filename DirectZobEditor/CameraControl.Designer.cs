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
            this.cameraTarZ = new System.Windows.Forms.TextBox();
            this.cameraTarY = new System.Windows.Forms.TextBox();
            this.cameraTarX = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cameraPosZ = new System.Windows.Forms.TextBox();
            this.cameraPosY = new System.Windows.Forms.TextBox();
            this.cameraPosX = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.camerasBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // camerasBox
            // 
            this.camerasBox.Controls.Add(this.cameraTarZ);
            this.camerasBox.Controls.Add(this.cameraTarY);
            this.camerasBox.Controls.Add(this.cameraTarX);
            this.camerasBox.Controls.Add(this.label3);
            this.camerasBox.Controls.Add(this.cameraPosZ);
            this.camerasBox.Controls.Add(this.cameraPosY);
            this.camerasBox.Controls.Add(this.cameraPosX);
            this.camerasBox.Controls.Add(this.label2);
            this.camerasBox.Controls.Add(this.label1);
            this.camerasBox.Controls.Add(this.comboBox1);
            this.camerasBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.camerasBox.Location = new System.Drawing.Point(0, 0);
            this.camerasBox.Margin = new System.Windows.Forms.Padding(10);
            this.camerasBox.Name = "camerasBox";
            this.camerasBox.Padding = new System.Windows.Forms.Padding(10);
            this.camerasBox.Size = new System.Drawing.Size(200, 200);
            this.camerasBox.TabIndex = 1;
            this.camerasBox.TabStop = false;
            this.camerasBox.Text = "Cameras";
            // 
            // cameraTarZ
            // 
            this.cameraTarZ.Location = new System.Drawing.Point(125, 115);
            this.cameraTarZ.Name = "cameraTarZ";
            this.cameraTarZ.Size = new System.Drawing.Size(50, 20);
            this.cameraTarZ.TabIndex = 9;
            this.cameraTarZ.Text = "100.00";
            // 
            // cameraTarY
            // 
            this.cameraTarY.Location = new System.Drawing.Point(69, 115);
            this.cameraTarY.Name = "cameraTarY";
            this.cameraTarY.Size = new System.Drawing.Size(50, 20);
            this.cameraTarY.TabIndex = 8;
            this.cameraTarY.Text = "100.00";
            // 
            // cameraTarX
            // 
            this.cameraTarX.Location = new System.Drawing.Point(13, 115);
            this.cameraTarX.Name = "cameraTarX";
            this.cameraTarX.Size = new System.Drawing.Size(50, 20);
            this.cameraTarX.TabIndex = 7;
            this.cameraTarX.Text = "100.00";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "target";
            // 
            // cameraPosZ
            // 
            this.cameraPosZ.Location = new System.Drawing.Point(125, 76);
            this.cameraPosZ.Name = "cameraPosZ";
            this.cameraPosZ.Size = new System.Drawing.Size(50, 20);
            this.cameraPosZ.TabIndex = 5;
            this.cameraPosZ.Text = "100.00";
            // 
            // cameraPosY
            // 
            this.cameraPosY.Location = new System.Drawing.Point(69, 76);
            this.cameraPosY.Name = "cameraPosY";
            this.cameraPosY.Size = new System.Drawing.Size(50, 20);
            this.cameraPosY.TabIndex = 4;
            this.cameraPosY.Text = "100.00";
            // 
            // cameraPosX
            // 
            this.cameraPosX.Location = new System.Drawing.Point(13, 76);
            this.cameraPosX.Name = "cameraPosX";
            this.cameraPosX.Size = new System.Drawing.Size(50, 20);
            this.cameraPosX.TabIndex = 3;
            this.cameraPosX.Text = "100.00";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 60);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Position";
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
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(13, 36);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(162, 21);
            this.comboBox1.TabIndex = 0;
            // 
            // Camera
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.camerasBox);
            this.MinimumSize = new System.Drawing.Size(200, 200);
            this.Name = "Camera";
            this.Size = new System.Drawing.Size(200, 200);
            this.camerasBox.ResumeLayout(false);
            this.camerasBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox camerasBox;
        private System.Windows.Forms.TextBox cameraTarZ;
        private System.Windows.Forms.TextBox cameraTarY;
        private System.Windows.Forms.TextBox cameraTarX;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox cameraPosZ;
        private System.Windows.Forms.TextBox cameraPosY;
        private System.Windows.Forms.TextBox cameraPosX;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}
