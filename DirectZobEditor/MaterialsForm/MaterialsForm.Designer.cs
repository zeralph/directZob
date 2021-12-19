
namespace DirectZobEditor.MaterialsForm
{
    partial class MaterialsForm
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
            this.MaterialsTableLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.SuspendLayout();
            // 
            // MaterialsTableLayout
            // 
            this.MaterialsTableLayout.AutoScroll = true;
            this.MaterialsTableLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MaterialsTableLayout.Location = new System.Drawing.Point(0, 0);
            this.MaterialsTableLayout.Name = "MaterialsTableLayout";
            this.MaterialsTableLayout.Size = new System.Drawing.Size(800, 450);
            this.MaterialsTableLayout.TabIndex = 0;
            // 
            // MaterialsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.MaterialsTableLayout);
            this.Name = "MaterialsForm";
            this.Text = "MaterialsForm";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel MaterialsTableLayout;
    }
}