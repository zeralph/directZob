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
            this.PhysicsGroupBox = new System.Windows.Forms.GroupBox();
            this.comboBoxPhysicShape = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboPhysicType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonSet = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textRadius = new System.Windows.Forms.TextBox();
            this.textBHeight = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textHalExtendsX = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textHalExtendsZ = new System.Windows.Forms.TextBox();
            this.textHalExtendsY = new System.Windows.Forms.TextBox();
            this.PhysicsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // PhysicsGroupBox
            // 
            this.PhysicsGroupBox.Controls.Add(this.textHalExtendsY);
            this.PhysicsGroupBox.Controls.Add(this.textHalExtendsZ);
            this.PhysicsGroupBox.Controls.Add(this.textHalExtendsX);
            this.PhysicsGroupBox.Controls.Add(this.label6);
            this.PhysicsGroupBox.Controls.Add(this.textBHeight);
            this.PhysicsGroupBox.Controls.Add(this.label4);
            this.PhysicsGroupBox.Controls.Add(this.textRadius);
            this.PhysicsGroupBox.Controls.Add(this.label3);
            this.PhysicsGroupBox.Controls.Add(this.buttonSet);
            this.PhysicsGroupBox.Controls.Add(this.comboBoxPhysicShape);
            this.PhysicsGroupBox.Controls.Add(this.label2);
            this.PhysicsGroupBox.Controls.Add(this.comboPhysicType);
            this.PhysicsGroupBox.Controls.Add(this.label1);
            this.PhysicsGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PhysicsGroupBox.Location = new System.Drawing.Point(0, 0);
            this.PhysicsGroupBox.Name = "PhysicsGroupBox";
            this.PhysicsGroupBox.Size = new System.Drawing.Size(340, 218);
            this.PhysicsGroupBox.TabIndex = 0;
            this.PhysicsGroupBox.TabStop = false;
            this.PhysicsGroupBox.Text = "Physics";
            this.PhysicsGroupBox.Enter += new System.EventHandler(this.Physics_Enter);
            // 
            // comboBoxPhysicShape
            // 
            this.comboBoxPhysicShape.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPhysicShape.FormattingEnabled = true;
            this.comboBoxPhysicShape.Items.AddRange(new object[] {
            "None",
            "Box",
            "Sphere",
            "Capsule",
            "Mesh"});
            this.comboBoxPhysicShape.Location = new System.Drawing.Point(96, 70);
            this.comboBoxPhysicShape.Name = "comboBoxPhysicShape";
            this.comboBoxPhysicShape.Size = new System.Drawing.Size(174, 21);
            this.comboBoxPhysicShape.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(52, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Shape";
            // 
            // comboPhysicType
            // 
            this.comboPhysicType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboPhysicType.FormattingEnabled = true;
            this.comboPhysicType.Items.AddRange(new object[] {
            "None",
            "Static",
            "Dynamic"});
            this.comboPhysicType.Location = new System.Drawing.Point(96, 46);
            this.comboPhysicType.Name = "comboPhysicType";
            this.comboPhysicType.Size = new System.Drawing.Size(174, 21);
            this.comboPhysicType.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(59, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Type";
            // 
            // buttonSet
            // 
            this.buttonSet.Location = new System.Drawing.Point(96, 175);
            this.buttonSet.Name = "buttonSet";
            this.buttonSet.Size = new System.Drawing.Size(174, 23);
            this.buttonSet.TabIndex = 4;
            this.buttonSet.Text = "Set";
            this.buttonSet.UseVisualStyleBackColor = true;
            this.buttonSet.Click += new System.EventHandler(this.buttonSet_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(50, 97);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Radius";
            // 
            // textRadius
            // 
            this.textRadius.Location = new System.Drawing.Point(96, 97);
            this.textRadius.Name = "textRadius";
            this.textRadius.Size = new System.Drawing.Size(174, 20);
            this.textRadius.TabIndex = 6;
            // 
            // textBHeight
            // 
            this.textBHeight.Location = new System.Drawing.Point(96, 123);
            this.textBHeight.Name = "textBHeight";
            this.textBHeight.Size = new System.Drawing.Size(174, 20);
            this.textBHeight.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(50, 123);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Height";
            // 
            // textHalExtendsX
            // 
            this.textHalExtendsX.Location = new System.Drawing.Point(96, 149);
            this.textHalExtendsX.Name = "textHalExtendsX";
            this.textHalExtendsX.Size = new System.Drawing.Size(54, 20);
            this.textHalExtendsX.TabIndex = 13;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(24, 152);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(66, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Half extends";
            // 
            // textHalExtendsZ
            // 
            this.textHalExtendsZ.Location = new System.Drawing.Point(216, 149);
            this.textHalExtendsZ.Name = "textHalExtendsZ";
            this.textHalExtendsZ.Size = new System.Drawing.Size(54, 20);
            this.textHalExtendsZ.TabIndex = 14;
            // 
            // textHalExtendsY
            // 
            this.textHalExtendsY.Location = new System.Drawing.Point(156, 149);
            this.textHalExtendsY.Name = "textHalExtendsY";
            this.textHalExtendsY.Size = new System.Drawing.Size(54, 20);
            this.textHalExtendsY.TabIndex = 15;
            // 
            // PhysicControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PhysicsGroupBox);
            this.Name = "PhysicControl";
            this.Size = new System.Drawing.Size(340, 218);
            this.PhysicsGroupBox.ResumeLayout(false);
            this.PhysicsGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox PhysicsGroupBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboPhysicType;
        private System.Windows.Forms.ComboBox comboBoxPhysicShape;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonSet;
        private System.Windows.Forms.TextBox textHalExtendsY;
        private System.Windows.Forms.TextBox textHalExtendsZ;
        private System.Windows.Forms.TextBox textHalExtendsX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textRadius;
        private System.Windows.Forms.Label label3;
    }
}
