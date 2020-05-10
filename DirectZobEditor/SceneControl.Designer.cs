namespace DirectZobEditor
{
    partial class SceneControl
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
            this.comboFogType = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.trackBarFogDistance = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.trackBarFogIntensity = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.trackBarAmbientIntensity = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonAmbientColor = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonFogColor = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonClearColor = new System.Windows.Forms.Button();
            this.clearColorDialog = new System.Windows.Forms.ColorDialog();
            this.fogColorDialog = new System.Windows.Forms.ColorDialog();
            this.ambientColorDialog = new System.Windows.Forms.ColorDialog();
            this.textAmbientIntensity = new System.Windows.Forms.TextBox();
            this.textFogIntensity = new System.Windows.Forms.TextBox();
            this.textFogDistance = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogDistance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogIntensity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAmbientIntensity)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textFogDistance);
            this.groupBox1.Controls.Add(this.textFogIntensity);
            this.groupBox1.Controls.Add(this.textAmbientIntensity);
            this.groupBox1.Controls.Add(this.comboFogType);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.trackBarFogDistance);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.trackBarFogIntensity);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.trackBarAmbientIntensity);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.buttonAmbientColor);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.buttonFogColor);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.buttonClearColor);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 290);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Scene";
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
            this.comboFogType.SelectedIndexChanged += new System.EventHandler(this.comboFogType_SelectedIndexChanged);
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
            this.trackBarFogDistance.Scroll += new System.EventHandler(this.trackBarFogDistance_Scroll);
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
            this.trackBarFogIntensity.Scroll += new System.EventHandler(this.trackBarFogIntensity_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 108);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(87, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Ambient Intensity";
            // 
            // trackBarAmbientIntensity
            // 
            this.trackBarAmbientIntensity.Location = new System.Drawing.Point(104, 108);
            this.trackBarAmbientIntensity.Name = "trackBarAmbientIntensity";
            this.trackBarAmbientIntensity.Size = new System.Drawing.Size(130, 45);
            this.trackBarAmbientIntensity.TabIndex = 6;
            this.trackBarAmbientIntensity.Scroll += new System.EventHandler(this.trackBarAmbientIntensity_Scroll);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(27, 83);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Ambient color";
            // 
            // buttonAmbientColor
            // 
            this.buttonAmbientColor.Location = new System.Drawing.Point(104, 78);
            this.buttonAmbientColor.Name = "buttonAmbientColor";
            this.buttonAmbientColor.Size = new System.Drawing.Size(75, 23);
            this.buttonAmbientColor.TabIndex = 4;
            this.buttonAmbientColor.UseVisualStyleBackColor = true;
            this.buttonAmbientColor.Click += new System.EventHandler(this.buttonAmbientColor_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(48, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Fog color";
            // 
            // buttonFogColor
            // 
            this.buttonFogColor.Location = new System.Drawing.Point(104, 48);
            this.buttonFogColor.Name = "buttonFogColor";
            this.buttonFogColor.Size = new System.Drawing.Size(75, 23);
            this.buttonFogColor.TabIndex = 2;
            this.buttonFogColor.UseVisualStyleBackColor = true;
            this.buttonFogColor.Click += new System.EventHandler(this.buttonFogColor_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(41, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Clear color";
            // 
            // buttonClearColor
            // 
            this.buttonClearColor.Location = new System.Drawing.Point(104, 18);
            this.buttonClearColor.Name = "buttonClearColor";
            this.buttonClearColor.Size = new System.Drawing.Size(75, 23);
            this.buttonClearColor.TabIndex = 0;
            this.buttonClearColor.UseVisualStyleBackColor = true;
            this.buttonClearColor.Click += new System.EventHandler(this.buttonClearColor_Click);
            // 
            // textAmbientIntensity
            // 
            this.textAmbientIntensity.Location = new System.Drawing.Point(240, 108);
            this.textAmbientIntensity.Name = "textAmbientIntensity";
            this.textAmbientIntensity.ReadOnly = true;
            this.textAmbientIntensity.Size = new System.Drawing.Size(58, 20);
            this.textAmbientIntensity.TabIndex = 14;
            // 
            // textFogIntensity
            // 
            this.textFogIntensity.Location = new System.Drawing.Point(240, 188);
            this.textFogIntensity.Name = "textFogIntensity";
            this.textFogIntensity.ReadOnly = true;
            this.textFogIntensity.Size = new System.Drawing.Size(58, 20);
            this.textFogIntensity.TabIndex = 15;
            // 
            // textFogDistance
            // 
            this.textFogDistance.Location = new System.Drawing.Point(240, 240);
            this.textFogDistance.Name = "textFogDistance";
            this.textFogDistance.ReadOnly = true;
            this.textFogDistance.Size = new System.Drawing.Size(58, 20);
            this.textFogDistance.TabIndex = 16;
            // 
            // SceneControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "SceneControl";
            this.Size = new System.Drawing.Size(340, 290);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogDistance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFogIntensity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAmbientIntensity)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TrackBar trackBarFogDistance;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar trackBarFogIntensity;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar trackBarAmbientIntensity;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonAmbientColor;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonFogColor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonClearColor;
        private System.Windows.Forms.ComboBox comboFogType;
        private System.Windows.Forms.ColorDialog clearColorDialog;
        private System.Windows.Forms.ColorDialog fogColorDialog;
        private System.Windows.Forms.ColorDialog ambientColorDialog;
        private System.Windows.Forms.TextBox textFogDistance;
        private System.Windows.Forms.TextBox textFogIntensity;
        private System.Windows.Forms.TextBox textAmbientIntensity;
    }
}
