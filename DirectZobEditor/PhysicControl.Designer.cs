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
            this.textRollingResistance = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textMassDensity = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textFrictionCoeff = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBounciness = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textHalExtendsY = new System.Windows.Forms.TextBox();
            this.textHalExtendsZ = new System.Windows.Forms.TextBox();
            this.textHalExtendsX = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBHeight = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textRadius = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonSet = new System.Windows.Forms.Button();
            this.comboBoxPhysicShape = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboPhysicType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBoxScaleWithObject = new System.Windows.Forms.CheckBox();
            this.PhysicsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // PhysicsGroupBox
            // 
            this.PhysicsGroupBox.Controls.Add(this.checkBoxScaleWithObject);
            this.PhysicsGroupBox.Controls.Add(this.textRollingResistance);
            this.PhysicsGroupBox.Controls.Add(this.label9);
            this.PhysicsGroupBox.Controls.Add(this.textMassDensity);
            this.PhysicsGroupBox.Controls.Add(this.label8);
            this.PhysicsGroupBox.Controls.Add(this.textFrictionCoeff);
            this.PhysicsGroupBox.Controls.Add(this.label7);
            this.PhysicsGroupBox.Controls.Add(this.textBounciness);
            this.PhysicsGroupBox.Controls.Add(this.label5);
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
            this.PhysicsGroupBox.Size = new System.Drawing.Size(340, 386);
            this.PhysicsGroupBox.TabIndex = 0;
            this.PhysicsGroupBox.TabStop = false;
            this.PhysicsGroupBox.Text = "Physics";
            this.PhysicsGroupBox.Enter += new System.EventHandler(this.Physics_Enter);
            // 
            // textRollingResistance
            // 
            this.textRollingResistance.Location = new System.Drawing.Point(96, 253);
            this.textRollingResistance.Name = "textRollingResistance";
            this.textRollingResistance.Size = new System.Drawing.Size(174, 20);
            this.textRollingResistance.TabIndex = 23;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(-2, 256);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(92, 13);
            this.label9.TabIndex = 22;
            this.label9.Text = "RollingResistance";
            // 
            // textMassDensity
            // 
            this.textMassDensity.Location = new System.Drawing.Point(96, 227);
            this.textMassDensity.Name = "textMassDensity";
            this.textMassDensity.Size = new System.Drawing.Size(174, 20);
            this.textMassDensity.TabIndex = 21;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(23, 230);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(67, 13);
            this.label8.TabIndex = 20;
            this.label8.Text = "MassDensity";
            this.label8.Click += new System.EventHandler(this.label8_Click);
            // 
            // textFrictionCoeff
            // 
            this.textFrictionCoeff.Location = new System.Drawing.Point(96, 201);
            this.textFrictionCoeff.Name = "textFrictionCoeff";
            this.textFrictionCoeff.Size = new System.Drawing.Size(174, 20);
            this.textFrictionCoeff.TabIndex = 19;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(24, 204);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(66, 13);
            this.label7.TabIndex = 18;
            this.label7.Text = "FrictionCoeff";
            // 
            // textBounciness
            // 
            this.textBounciness.Location = new System.Drawing.Point(96, 175);
            this.textBounciness.Name = "textBounciness";
            this.textBounciness.Size = new System.Drawing.Size(174, 20);
            this.textBounciness.TabIndex = 17;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(28, 178);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(62, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Bounciness";
            // 
            // textHalExtendsY
            // 
            this.textHalExtendsY.Location = new System.Drawing.Point(156, 149);
            this.textHalExtendsY.Name = "textHalExtendsY";
            this.textHalExtendsY.Size = new System.Drawing.Size(54, 20);
            this.textHalExtendsY.TabIndex = 15;
            // 
            // textHalExtendsZ
            // 
            this.textHalExtendsZ.Location = new System.Drawing.Point(216, 149);
            this.textHalExtendsZ.Name = "textHalExtendsZ";
            this.textHalExtendsZ.Size = new System.Drawing.Size(54, 20);
            this.textHalExtendsZ.TabIndex = 14;
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
            this.label4.Location = new System.Drawing.Point(52, 126);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Height";
            // 
            // textRadius
            // 
            this.textRadius.Location = new System.Drawing.Point(96, 97);
            this.textRadius.Name = "textRadius";
            this.textRadius.Size = new System.Drawing.Size(174, 20);
            this.textRadius.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(50, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Radius";
            // 
            // buttonSet
            // 
            this.buttonSet.Location = new System.Drawing.Point(96, 308);
            this.buttonSet.Name = "buttonSet";
            this.buttonSet.Size = new System.Drawing.Size(174, 23);
            this.buttonSet.TabIndex = 4;
            this.buttonSet.Text = "Set";
            this.buttonSet.UseVisualStyleBackColor = true;
            this.buttonSet.Click += new System.EventHandler(this.buttonSet_Click);
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
            this.label2.Location = new System.Drawing.Point(52, 73);
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
            this.comboPhysicType.Location = new System.Drawing.Point(96, 43);
            this.comboPhysicType.Name = "comboPhysicType";
            this.comboPhysicType.Size = new System.Drawing.Size(174, 21);
            this.comboPhysicType.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(59, 46);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Type";
            // 
            // checkBoxScaleWithObject
            // 
            this.checkBoxScaleWithObject.AutoSize = true;
            this.checkBoxScaleWithObject.Checked = true;
            this.checkBoxScaleWithObject.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxScaleWithObject.Location = new System.Drawing.Point(96, 285);
            this.checkBoxScaleWithObject.Name = "checkBoxScaleWithObject";
            this.checkBoxScaleWithObject.Size = new System.Drawing.Size(107, 17);
            this.checkBoxScaleWithObject.TabIndex = 24;
            this.checkBoxScaleWithObject.Text = "Scale with object";
            this.checkBoxScaleWithObject.UseVisualStyleBackColor = true;
            this.checkBoxScaleWithObject.CheckedChanged += new System.EventHandler(this.checkBoxScaleWithObject_CheckedChanged);
            // 
            // PhysicControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PhysicsGroupBox);
            this.Name = "PhysicControl";
            this.Size = new System.Drawing.Size(340, 386);
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
        private System.Windows.Forms.TextBox textRollingResistance;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textMassDensity;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textFrictionCoeff;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBounciness;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox checkBoxScaleWithObject;
    }
}
