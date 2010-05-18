namespace laptop
{
    partial class Form1
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.lbCoordinatenRobot = new System.Windows.Forms.Label();
            this.lbCoordinaatPersoon = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.btZendCoordinaten = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btKomTerug = new System.Windows.Forms.Button();
            this.btVolgendeStap = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBox4);
            this.groupBox1.Controls.Add(this.textBox3);
            this.groupBox1.Controls.Add(this.lbCoordinatenRobot);
            this.groupBox1.Controls.Add(this.lbCoordinaatPersoon);
            this.groupBox1.Controls.Add(this.textBox2);
            this.groupBox1.Controls.Add(this.btZendCoordinaten);
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.Location = new System.Drawing.Point(12, 144);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(146, 242);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "coordinaten";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 160);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(12, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "y";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 134);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(12, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "x";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 80);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "y";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(12, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "x";
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(30, 131);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(75, 20);
            this.textBox4.TabIndex = 9;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(30, 77);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(75, 20);
            this.textBox3.TabIndex = 8;
            // 
            // lbCoordinatenRobot
            // 
            this.lbCoordinatenRobot.AutoSize = true;
            this.lbCoordinatenRobot.Location = new System.Drawing.Point(15, 105);
            this.lbCoordinatenRobot.Name = "lbCoordinatenRobot";
            this.lbCoordinatenRobot.Size = new System.Drawing.Size(90, 13);
            this.lbCoordinatenRobot.TabIndex = 7;
            this.lbCoordinatenRobot.Text = "coordinaten robot";
            // 
            // lbCoordinaatPersoon
            // 
            this.lbCoordinaatPersoon.AutoSize = true;
            this.lbCoordinaatPersoon.Location = new System.Drawing.Point(15, 35);
            this.lbCoordinaatPersoon.Name = "lbCoordinaatPersoon";
            this.lbCoordinaatPersoon.Size = new System.Drawing.Size(104, 13);
            this.lbCoordinaatPersoon.TabIndex = 6;
            this.lbCoordinaatPersoon.Text = "coordinaten persoon";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(30, 51);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(75, 20);
            this.textBox2.TabIndex = 1;
            // 
            // btZendCoordinaten
            // 
            this.btZendCoordinaten.Location = new System.Drawing.Point(15, 195);
            this.btZendCoordinaten.Name = "btZendCoordinaten";
            this.btZendCoordinaten.Size = new System.Drawing.Size(110, 23);
            this.btZendCoordinaten.TabIndex = 5;
            this.btZendCoordinaten.Text = "zend coordinaten";
            this.btZendCoordinaten.UseVisualStyleBackColor = true;
            this.btZendCoordinaten.Click += new System.EventHandler(this.btZendCoordinaten_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(30, 157);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(75, 20);
            this.textBox1.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox2);
            this.groupBox3.Location = new System.Drawing.Point(12, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(146, 374);
            this.groupBox3.TabIndex = 13;
            this.groupBox3.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btKomTerug);
            this.groupBox2.Controls.Add(this.btVolgendeStap);
            this.groupBox2.Location = new System.Drawing.Point(0, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(146, 93);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "comands";
            // 
            // btKomTerug
            // 
            this.btKomTerug.Location = new System.Drawing.Point(6, 19);
            this.btKomTerug.Name = "btKomTerug";
            this.btKomTerug.Size = new System.Drawing.Size(99, 23);
            this.btKomTerug.TabIndex = 3;
            this.btKomTerug.Text = "kom terug";
            this.btKomTerug.UseVisualStyleBackColor = true;
            this.btKomTerug.Click += new System.EventHandler(this.btKomTerug_Click);
            // 
            // btVolgendeStap
            // 
            this.btVolgendeStap.Location = new System.Drawing.Point(6, 48);
            this.btVolgendeStap.Name = "btVolgendeStap";
            this.btVolgendeStap.Size = new System.Drawing.Size(99, 23);
            this.btVolgendeStap.TabIndex = 4;
            this.btVolgendeStap.Text = "volgende stap";
            this.btVolgendeStap.UseVisualStyleBackColor = true;
            this.btVolgendeStap.Click += new System.EventHandler(this.btVolgendeStap_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(690, 394);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox3);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label lbCoordinatenRobot;
        private System.Windows.Forms.Label lbCoordinaatPersoon;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button btZendCoordinaten;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btKomTerug;
        private System.Windows.Forms.Button btVolgendeStap;

    }
}

