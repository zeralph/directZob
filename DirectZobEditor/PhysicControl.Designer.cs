namespace DirectZobEditor
{
    partial class PhysicControl
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
            this.Physics = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.comboPhysicType = new System.Windows.Forms.ComboBox();
            this.Physics.SuspendLayout();
            this.SuspendLayout();
            // 
            // Physics
            // 
            this.Physics.Controls.Add(this.comboPhysicType);
            this.Physics.Controls.Add(this.label1);
            this.Physics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Physics.Location = new System.Drawing.Point(0, 0);
            this.Physics.Name = "Physics";
            this.Physics.Size = new System.Drawing.Size(340, 150);
            this.Physics.TabIndex = 0;
            this.Physics.TabStop = false;
            this.Physics.Text = "Physics";
            this.Physics.Enter += new System.EventHandler(this.Physics_Enter);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Type";
            // 
            // comboPhysicType
            // 
            this.comboPhysicType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboPhysicType.FormattingEnabled = true;
            this.comboPhysicType.Items.AddRange(new object[] {
            "None",
            "Collision shape",
            "Rigid body"});
            this.comboPhysicType.Location = new System.Drawing.Point(72, 46);
            this.comboPhysicType.Name = "comboPhysicType";
            this.comboPhysicType.Size = new System.Drawing.Size(168, 21);
            this.comboPhysicType.TabIndex = 1;
            this.comboPhysicType.SelectedIndexChanged += new System.EventHandler(this.comboPhysicType_SelectedIndexChanged);
            // 
            // PhysicControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.Physics);
            this.Name = "PhysicControl";
            this.Size = new System.Drawing.Size(340, 150);
            this.Physics.ResumeLayout(false);
            this.Physics.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox Physics;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboPhysicType;
    }
}
