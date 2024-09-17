
namespace DirectZobEditor
{
    partial class ZobPackerForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.buttonAddLevel = new System.Windows.Forms.Button();
            this.buttonPack = new System.Windows.Forms.Button();
            this.packLevels = new System.Windows.Forms.ListView();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.packLevels);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.buttonPack);
            this.splitContainer1.Panel2.Controls.Add(this.buttonAddLevel);
            this.splitContainer1.Size = new System.Drawing.Size(687, 332);
            this.splitContainer1.SplitterDistance = 274;
            this.splitContainer1.TabIndex = 0;
            // 
            // buttonAddLevel
            // 
            this.buttonAddLevel.Location = new System.Drawing.Point(100, 19);
            this.buttonAddLevel.Name = "buttonAddLevel";
            this.buttonAddLevel.Size = new System.Drawing.Size(75, 23);
            this.buttonAddLevel.TabIndex = 0;
            this.buttonAddLevel.Text = "Add Level";
            this.buttonAddLevel.UseVisualStyleBackColor = true;
            this.buttonAddLevel.Click += new System.EventHandler(this.buttonAddLevel_Click);
            // 
            // buttonPack
            // 
            this.buttonPack.Location = new System.Drawing.Point(311, 19);
            this.buttonPack.Name = "buttonPack";
            this.buttonPack.Size = new System.Drawing.Size(75, 23);
            this.buttonPack.TabIndex = 1;
            this.buttonPack.Text = "Pack !";
            this.buttonPack.UseVisualStyleBackColor = true;
            this.buttonPack.Click += new System.EventHandler(this.buttonPack_Click);
            // 
            // packLevels
            // 
            this.packLevels.AllowDrop = true;
            this.packLevels.CheckBoxes = true;
            this.packLevels.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packLevels.FullRowSelect = true;
            this.packLevels.HideSelection = false;
            this.packLevels.Location = new System.Drawing.Point(0, 0);
            this.packLevels.Name = "packLevels";
            this.packLevels.Size = new System.Drawing.Size(687, 274);
            this.packLevels.TabIndex = 0;
            this.packLevels.UseCompatibleStateImageBehavior = false;
            this.packLevels.View = System.Windows.Forms.View.List;
            // 
            // ZobPackerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(687, 332);
            this.Controls.Add(this.splitContainer1);
            this.Name = "ZobPackerForm";
            this.Text = "ZobPackerForm";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView packLevels;
        private System.Windows.Forms.Button buttonPack;
        private System.Windows.Forms.Button buttonAddLevel;
    }
}