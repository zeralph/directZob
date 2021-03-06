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
            this.transparent = new System.Windows.Forms.CheckBox();
            this.zbuffered = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.comboLighting = new System.Windows.Forms.ComboBox();
            this.groupMesh.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupMesh
            // 
            this.groupMesh.Controls.Add(this.transparent);
            this.groupMesh.Controls.Add(this.zbuffered);
            this.groupMesh.Controls.Add(this.label5);
            this.groupMesh.Controls.Add(this.comboLighting);
            this.groupMesh.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupMesh.Location = new System.Drawing.Point(0, 0);
            this.groupMesh.Name = "groupMesh";
            this.groupMesh.Size = new System.Drawing.Size(340, 150);
            this.groupMesh.TabIndex = 0;
            this.groupMesh.TabStop = false;
            this.groupMesh.Text = "Mesh properties";
            // 
            // transparent
            // 
            this.transparent.AutoSize = true;
            this.transparent.Location = new System.Drawing.Point(78, 69);
            this.transparent.Name = "transparent";
            this.transparent.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.transparent.Size = new System.Drawing.Size(83, 17);
            this.transparent.TabIndex = 18;
            this.transparent.Text = "Transparent";
            this.transparent.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.transparent.UseVisualStyleBackColor = true;
            this.transparent.CheckedChanged += new System.EventHandler(this.transparent_CheckedChanged);
            // 
            // zbuffered
            // 
            this.zbuffered.AutoSize = true;
            this.zbuffered.Location = new System.Drawing.Point(86, 46);
            this.zbuffered.Name = "zbuffered";
            this.zbuffered.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.zbuffered.Size = new System.Drawing.Size(75, 17);
            this.zbuffered.TabIndex = 17;
            this.zbuffered.Text = "Z buffered";
            this.zbuffered.UseVisualStyleBackColor = true;
            this.zbuffered.CheckedChanged += new System.EventHandler(this.zbuffered_CheckedChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(97, 22);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Lighting";
            // 
            // comboLighting
            // 
            this.comboLighting.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboLighting.FormattingEnabled = true;
            this.comboLighting.Items.AddRange(new object[] {
            "None",
            "Flat",
            "Gouraud",
            "Phong",
            "Flat phong"});
            this.comboLighting.Location = new System.Drawing.Point(147, 19);
            this.comboLighting.Name = "comboLighting";
            this.comboLighting.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.comboLighting.Size = new System.Drawing.Size(117, 21);
            this.comboLighting.TabIndex = 15;
            this.comboLighting.SelectedIndexChanged += new System.EventHandler(this.comboLighting_SelectedIndexChanged);
            // 
            // ZobMeshControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupMesh);
            this.Name = "ZobMeshControl";
            this.Size = new System.Drawing.Size(340, 150);
            this.groupMesh.ResumeLayout(false);
            this.groupMesh.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupMesh;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboLighting;
        private System.Windows.Forms.CheckBox transparent;
        private System.Windows.Forms.CheckBox zbuffered;
    }
}
