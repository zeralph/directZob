namespace DirectZobEditor
{
    partial class ZobObjectControl
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
            this.groupBoxZobObject = new System.Windows.Forms.GroupBox();
            this.ZobObjectLayout = new System.Windows.Forms.TableLayoutPanel();
            this.TransformVars = new System.Windows.Forms.GroupBox();
            this.zobId = new System.Windows.Forms.TextBox();
            this.checkBoxLinkScale = new System.Windows.Forms.CheckBox();
            this.zobRotZ = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.zobRotY = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.zobScaleX = new System.Windows.Forms.TextBox();
            this.zobName = new System.Windows.Forms.TextBox();
            this.zobRotX = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.zobScaleY = new System.Windows.Forms.TextBox();
            this.zobPosX = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.zobPosY = new System.Windows.Forms.TextBox();
            this.zobScaleZ = new System.Windows.Forms.TextBox();
            this.zobPosZ = new System.Windows.Forms.TextBox();
            this.groupBoxZobObject.SuspendLayout();
            this.ZobObjectLayout.SuspendLayout();
            this.TransformVars.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxZobObject
            // 
            this.groupBoxZobObject.AutoSize = true;
            this.groupBoxZobObject.Controls.Add(this.ZobObjectLayout);
            this.groupBoxZobObject.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxZobObject.Location = new System.Drawing.Point(0, 0);
            this.groupBoxZobObject.Margin = new System.Windows.Forms.Padding(0);
            this.groupBoxZobObject.Name = "groupBoxZobObject";
            this.groupBoxZobObject.Size = new System.Drawing.Size(340, 180);
            this.groupBoxZobObject.TabIndex = 0;
            this.groupBoxZobObject.TabStop = false;
            this.groupBoxZobObject.Text = "ZobObject";
            // 
            // ZobObjectLayout
            // 
            this.ZobObjectLayout.AutoSize = true;
            this.ZobObjectLayout.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ZobObjectLayout.ColumnCount = 1;
            this.ZobObjectLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.ZobObjectLayout.Controls.Add(this.TransformVars, 0, 0);
            this.ZobObjectLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ZobObjectLayout.Location = new System.Drawing.Point(3, 16);
            this.ZobObjectLayout.Name = "ZobObjectLayout";
            this.ZobObjectLayout.RowCount = 2;
            this.ZobObjectLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 160F));
            this.ZobObjectLayout.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.ZobObjectLayout.Size = new System.Drawing.Size(334, 161);
            this.ZobObjectLayout.TabIndex = 47;
            // 
            // TransformVars
            // 
            this.TransformVars.Controls.Add(this.zobId);
            this.TransformVars.Controls.Add(this.checkBoxLinkScale);
            this.TransformVars.Controls.Add(this.zobRotZ);
            this.TransformVars.Controls.Add(this.label4);
            this.TransformVars.Controls.Add(this.label1);
            this.TransformVars.Controls.Add(this.zobRotY);
            this.TransformVars.Controls.Add(this.label7);
            this.TransformVars.Controls.Add(this.zobScaleX);
            this.TransformVars.Controls.Add(this.zobName);
            this.TransformVars.Controls.Add(this.zobRotX);
            this.TransformVars.Controls.Add(this.label2);
            this.TransformVars.Controls.Add(this.zobScaleY);
            this.TransformVars.Controls.Add(this.zobPosX);
            this.TransformVars.Controls.Add(this.label3);
            this.TransformVars.Controls.Add(this.zobPosY);
            this.TransformVars.Controls.Add(this.zobScaleZ);
            this.TransformVars.Controls.Add(this.zobPosZ);
            this.TransformVars.Location = new System.Drawing.Point(3, 3);
            this.TransformVars.Name = "TransformVars";
            this.TransformVars.Size = new System.Drawing.Size(328, 154);
            this.TransformVars.TabIndex = 46;
            this.TransformVars.TabStop = false;
            this.TransformVars.Text = "Values";
            // 
            // zobId
            // 
            this.zobId.Location = new System.Drawing.Point(73, 19);
            this.zobId.Name = "zobId";
            this.zobId.ReadOnly = true;
            this.zobId.Size = new System.Drawing.Size(174, 20);
            this.zobId.TabIndex = 44;
            // 
            // checkBoxLinkScale
            // 
            this.checkBoxLinkScale.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLinkScale.AutoSize = true;
            this.checkBoxLinkScale.Location = new System.Drawing.Point(253, 118);
            this.checkBoxLinkScale.Name = "checkBoxLinkScale";
            this.checkBoxLinkScale.Size = new System.Drawing.Size(23, 23);
            this.checkBoxLinkScale.TabIndex = 45;
            this.checkBoxLinkScale.Text = "L";
            this.checkBoxLinkScale.UseVisualStyleBackColor = true;
            // 
            // zobRotZ
            // 
            this.zobRotZ.Location = new System.Drawing.Point(193, 96);
            this.zobRotZ.Name = "zobRotZ";
            this.zobRotZ.Size = new System.Drawing.Size(54, 20);
            this.zobRotZ.TabIndex = 38;
            this.zobRotZ.Text = "100.00";
            this.zobRotZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(33, 128);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 39;
            this.label4.Text = "Scale";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(32, 50);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 29;
            this.label1.Text = "Name";
            // 
            // zobRotY
            // 
            this.zobRotY.Location = new System.Drawing.Point(133, 97);
            this.zobRotY.Name = "zobRotY";
            this.zobRotY.Size = new System.Drawing.Size(54, 20);
            this.zobRotY.TabIndex = 37;
            this.zobRotY.Text = "100.00";
            this.zobRotY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(51, 24);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(16, 13);
            this.label7.TabIndex = 43;
            this.label7.Text = "Id";
            // 
            // zobScaleX
            // 
            this.zobScaleX.Location = new System.Drawing.Point(73, 121);
            this.zobScaleX.Name = "zobScaleX";
            this.zobScaleX.Size = new System.Drawing.Size(54, 20);
            this.zobScaleX.TabIndex = 40;
            this.zobScaleX.Text = "100.00";
            this.zobScaleX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobName
            // 
            this.zobName.Location = new System.Drawing.Point(73, 45);
            this.zobName.Name = "zobName";
            this.zobName.Size = new System.Drawing.Size(174, 20);
            this.zobName.TabIndex = 30;
            this.zobName.Leave += new System.EventHandler(this.zobName_Leave);
            // 
            // zobRotX
            // 
            this.zobRotX.Location = new System.Drawing.Point(73, 96);
            this.zobRotX.Name = "zobRotX";
            this.zobRotX.Size = new System.Drawing.Size(54, 20);
            this.zobRotX.TabIndex = 36;
            this.zobRotX.Text = "100.00";
            this.zobRotX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(23, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 31;
            this.label2.Text = "Position";
            // 
            // zobScaleY
            // 
            this.zobScaleY.Location = new System.Drawing.Point(133, 121);
            this.zobScaleY.Name = "zobScaleY";
            this.zobScaleY.Size = new System.Drawing.Size(54, 20);
            this.zobScaleY.TabIndex = 41;
            this.zobScaleY.Text = "100.00";
            this.zobScaleY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobPosX
            // 
            this.zobPosX.Location = new System.Drawing.Point(73, 71);
            this.zobPosX.Name = "zobPosX";
            this.zobPosX.Size = new System.Drawing.Size(54, 20);
            this.zobPosX.TabIndex = 32;
            this.zobPosX.Text = "100.00";
            this.zobPosX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 102);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 35;
            this.label3.Text = "Rotation";
            // 
            // zobPosY
            // 
            this.zobPosY.Location = new System.Drawing.Point(133, 71);
            this.zobPosY.Name = "zobPosY";
            this.zobPosY.Size = new System.Drawing.Size(54, 20);
            this.zobPosY.TabIndex = 33;
            this.zobPosY.Text = "100.00";
            this.zobPosY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobScaleZ
            // 
            this.zobScaleZ.Location = new System.Drawing.Point(193, 121);
            this.zobScaleZ.Name = "zobScaleZ";
            this.zobScaleZ.Size = new System.Drawing.Size(54, 20);
            this.zobScaleZ.TabIndex = 42;
            this.zobScaleZ.Text = "100.00";
            this.zobScaleZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobPosZ
            // 
            this.zobPosZ.Location = new System.Drawing.Point(193, 71);
            this.zobPosZ.Name = "zobPosZ";
            this.zobPosZ.Size = new System.Drawing.Size(54, 20);
            this.zobPosZ.TabIndex = 34;
            this.zobPosZ.Text = "100.00";
            this.zobPosZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // ZobObjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.groupBoxZobObject);
            this.Name = "ZobObjectControl";
            this.Size = new System.Drawing.Size(340, 180);
            this.groupBoxZobObject.ResumeLayout(false);
            this.groupBoxZobObject.PerformLayout();
            this.ZobObjectLayout.ResumeLayout(false);
            this.TransformVars.ResumeLayout(false);
            this.TransformVars.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxZobObject;
        private System.Windows.Forms.CheckBox checkBoxLinkScale;
        private System.Windows.Forms.TextBox zobId;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox zobName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox zobPosX;
        private System.Windows.Forms.TextBox zobPosY;
        private System.Windows.Forms.TextBox zobPosZ;
        private System.Windows.Forms.TextBox zobScaleZ;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox zobScaleY;
        private System.Windows.Forms.TextBox zobRotX;
        private System.Windows.Forms.TextBox zobScaleX;
        private System.Windows.Forms.TextBox zobRotY;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox zobRotZ;
        private System.Windows.Forms.GroupBox TransformVars;
        private System.Windows.Forms.TableLayoutPanel ZobObjectLayout;
    }
}
