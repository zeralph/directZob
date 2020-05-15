namespace DirectZobEditor
{
    partial class ZobLightControl
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
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.lightAngle = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lightDistance = new System.Windows.Forms.TextBox();
            this.lightIntensity = new System.Windows.Forms.TextBox();
            this.comboBoxLightType = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonColor = new System.Windows.Forms.Button();
            this.lightColorDialog = new System.Windows.Forms.ColorDialog();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 187);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Light properties";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(65, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Color";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.lightAngle);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.lightDistance);
            this.groupBox2.Controls.Add(this.lightIntensity);
            this.groupBox2.Controls.Add(this.comboBoxLightType);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.buttonColor);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Location = new System.Drawing.Point(0, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(340, 187);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Scene";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(25, 140);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 24;
            this.label5.Text = "Spot angle";
            // 
            // lightAngle
            // 
            this.lightAngle.Location = new System.Drawing.Point(104, 137);
            this.lightAngle.Name = "lightAngle";
            this.lightAngle.Size = new System.Drawing.Size(100, 20);
            this.lightAngle.TabIndex = 23;
            this.lightAngle.TextChanged += new System.EventHandler(this.valueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(25, 114);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(73, 13);
            this.label4.TabIndex = 22;
            this.label4.Text = "Light distance";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 88);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 13);
            this.label3.TabIndex = 21;
            this.label3.Text = "Light intensity";
            // 
            // lightDistance
            // 
            this.lightDistance.Location = new System.Drawing.Point(104, 111);
            this.lightDistance.Name = "lightDistance";
            this.lightDistance.Size = new System.Drawing.Size(100, 20);
            this.lightDistance.TabIndex = 20;
            this.lightDistance.TextChanged += new System.EventHandler(this.valueChanged);
            // 
            // lightIntensity
            // 
            this.lightIntensity.Location = new System.Drawing.Point(104, 85);
            this.lightIntensity.Name = "lightIntensity";
            this.lightIntensity.Size = new System.Drawing.Size(100, 20);
            this.lightIntensity.TabIndex = 19;
            this.lightIntensity.TextChanged += new System.EventHandler(this.valueChanged);
            // 
            // comboBoxLightType
            // 
            this.comboBoxLightType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxLightType.FormattingEnabled = true;
            this.comboBoxLightType.Items.AddRange(new object[] {
            "Point",
            "Spot",
            "Directional"});
            this.comboBoxLightType.Location = new System.Drawing.Point(104, 24);
            this.comboBoxLightType.Name = "comboBoxLightType";
            this.comboBoxLightType.Size = new System.Drawing.Size(121, 21);
            this.comboBoxLightType.TabIndex = 18;
            this.comboBoxLightType.SelectedIndexChanged += new System.EventHandler(this.comboBoxLightType_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(65, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 13);
            this.label2.TabIndex = 17;
            this.label2.Text = "Type";
            // 
            // buttonColor
            // 
            this.buttonColor.Location = new System.Drawing.Point(104, 55);
            this.buttonColor.Name = "buttonColor";
            this.buttonColor.Size = new System.Drawing.Size(75, 23);
            this.buttonColor.TabIndex = 0;
            this.buttonColor.UseVisualStyleBackColor = true;
            this.buttonColor.Click += new System.EventHandler(this.buttonColor_Click);
            // 
            // ZobLightControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobLightControl";
            this.Size = new System.Drawing.Size(340, 187);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonColor;
        private System.Windows.Forms.ColorDialog lightColorDialog;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox lightDistance;
        private System.Windows.Forms.TextBox lightIntensity;
        private System.Windows.Forms.ComboBox comboBoxLightType;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox lightAngle;
    }
}
