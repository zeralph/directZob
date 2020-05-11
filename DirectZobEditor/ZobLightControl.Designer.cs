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
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lightDistance = new System.Windows.Forms.TextBox();
            this.lightIntensity = new System.Windows.Forms.TextBox();
            this.comboBoxLightType = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textFogDistance = new System.Windows.Forms.TextBox();
            this.textFogIntensity = new System.Windows.Forms.TextBox();
            this.comboFogType = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.trackBarFogDistance = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.trackBarFogIntensity = new System.Windows.Forms.TrackBar();
            this.buttonColor = new System.Windows.Forms.Button();
            this.lightColorDialog = new System.Windows.Forms.ColorDialog();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogDistance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogIntensity)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 150);
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
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.lightDistance);
            this.groupBox2.Controls.Add(this.lightIntensity);
            this.groupBox2.Controls.Add(this.comboBoxLightType);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.textFogDistance);
            this.groupBox2.Controls.Add(this.textFogIntensity);
            this.groupBox2.Controls.Add(this.comboFogType);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.trackBarFogDistance);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.trackBarFogIntensity);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.buttonColor);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Location = new System.Drawing.Point(0, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(340, 150);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Scene";
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
            // 
            // lightIntensity
            // 
            this.lightIntensity.Location = new System.Drawing.Point(104, 85);
            this.lightIntensity.Name = "lightIntensity";
            this.lightIntensity.Size = new System.Drawing.Size(100, 20);
            this.lightIntensity.TabIndex = 19;
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
            // textFogDistance
            // 
            this.textFogDistance.Location = new System.Drawing.Point(240, 240);
            this.textFogDistance.Name = "textFogDistance";
            this.textFogDistance.ReadOnly = true;
            this.textFogDistance.Size = new System.Drawing.Size(58, 20);
            this.textFogDistance.TabIndex = 16;
            // 
            // textFogIntensity
            // 
            this.textFogIntensity.Location = new System.Drawing.Point(240, 188);
            this.textFogIntensity.Name = "textFogIntensity";
            this.textFogIntensity.ReadOnly = true;
            this.textFogIntensity.Size = new System.Drawing.Size(58, 20);
            this.textFogIntensity.TabIndex = 15;
            // 
            // comboFogType
            // 
            this.comboFogType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboFogType.FormattingEnabled = true;
            this.comboFogType.Items.AddRange(new object[] {
            "None",
            "Linear",
            "Exp",
            "Exp2"});
            this.comboFogType.Location = new System.Drawing.Point(104, 160);
            this.comboFogType.Name = "comboFogType";
            this.comboFogType.Size = new System.Drawing.Size(121, 21);
            this.comboFogType.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(50, 163);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(48, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Fog type";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(30, 240);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(68, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "Fog distance";
            // 
            // trackBarFogDistance
            // 
            this.trackBarFogDistance.Location = new System.Drawing.Point(104, 240);
            this.trackBarFogDistance.Name = "trackBarFogDistance";
            this.trackBarFogDistance.Size = new System.Drawing.Size(130, 45);
            this.trackBarFogDistance.TabIndex = 10;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(32, 188);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Fog Intensity";
            // 
            // trackBarFogIntensity
            // 
            this.trackBarFogIntensity.Location = new System.Drawing.Point(104, 188);
            this.trackBarFogIntensity.Name = "trackBarFogIntensity";
            this.trackBarFogIntensity.Size = new System.Drawing.Size(130, 45);
            this.trackBarFogIntensity.TabIndex = 8;
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
            this.Size = new System.Drawing.Size(340, 150);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogDistance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogIntensity)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox textFogDistance;
        private System.Windows.Forms.TextBox textFogIntensity;
        private System.Windows.Forms.ComboBox comboFogType;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TrackBar trackBarFogDistance;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar trackBarFogIntensity;
        private System.Windows.Forms.Button buttonColor;
        private System.Windows.Forms.ColorDialog lightColorDialog;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox lightDistance;
        private System.Windows.Forms.TextBox lightIntensity;
        private System.Windows.Forms.ComboBox comboBoxLightType;
        private System.Windows.Forms.Label label2;
    }
}
