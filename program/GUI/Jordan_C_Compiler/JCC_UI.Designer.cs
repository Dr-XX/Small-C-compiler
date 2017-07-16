namespace WindowsFormsApplication1
{
    partial class JordanForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)){
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label labelOutput;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(JordanForm));
            this.buttonOpenFile = new System.Windows.Forms.Button();
            this.openInputFile = new System.Windows.Forms.OpenFileDialog();
            this.filePath = new System.Windows.Forms.TextBox();
            this.labelSource = new System.Windows.Forms.Label();
            this.textSourceCode = new System.Windows.Forms.TextBox();
            this.textCode = new System.Windows.Forms.TextBox();
            this.buttonStopDebug = new System.Windows.Forms.Button();
            this.buttonDebug = new System.Windows.Forms.Button();
            this.textOutput = new System.Windows.Forms.TextBox();
            this.labelTitle = new System.Windows.Forms.Label();
            this.toolTip_openFile = new System.Windows.Forms.ToolTip(this.components);
            this.toolTip_Run = new System.Windows.Forms.ToolTip(this.components);
            this.buttonRun = new System.Windows.Forms.Button();
            this.toolTip_Debug = new System.Windows.Forms.ToolTip(this.components);
            this.toolTip_StopDebug = new System.Windows.Forms.ToolTip(this.components);
            this.labelStack = new System.Windows.Forms.Label();
            this.labelOutputt = new System.Windows.Forms.Label();
            this.textErrReport = new System.Windows.Forms.TextBox();
            this.textTable = new System.Windows.Forms.TextBox();
            this.textInput = new System.Windows.Forms.TextBox();
            this.label_Input = new System.Windows.Forms.Label();
            this.label_Table = new System.Windows.Forms.Label();
            this.buttonInputOK = new System.Windows.Forms.Button();
            labelOutput = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelOutput
            // 
            labelOutput.AutoSize = true;
            labelOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            labelOutput.ForeColor = System.Drawing.Color.Blue;
            labelOutput.Location = new System.Drawing.Point(350, 87);
            labelOutput.Name = "labelOutput";
            labelOutput.Size = new System.Drawing.Size(86, 24);
            labelOutput.TabIndex = 7;
            labelOutput.Text = "目标代码";
            labelOutput.Click += new System.EventHandler(this.label_Output_Click);
            // 
            // buttonOpenFile
            // 
            this.buttonOpenFile.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonOpenFile.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonOpenFile.BackgroundImage")));
            this.buttonOpenFile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.buttonOpenFile.FlatAppearance.BorderSize = 0;
            this.buttonOpenFile.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonOpenFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonOpenFile.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.buttonOpenFile.Location = new System.Drawing.Point(721, 39);
            this.buttonOpenFile.Name = "buttonOpenFile";
            this.buttonOpenFile.Size = new System.Drawing.Size(40, 40);
            this.buttonOpenFile.TabIndex = 2;
            this.buttonOpenFile.UseVisualStyleBackColor = true;
            this.buttonOpenFile.Click += new System.EventHandler(this.openFileButton_Click);
            this.buttonOpenFile.MouseHover += new System.EventHandler(this.openFileButton_MouseHover_1);
            // 
            // openInputFile
            // 
            this.openInputFile.FileName = "openInputFile";
            this.openInputFile.FileOk += new System.ComponentModel.CancelEventHandler(this.openInputFile_FileOk);
            // 
            // filePath
            // 
            this.filePath.BackColor = System.Drawing.Color.White;
            this.filePath.Font = new System.Drawing.Font("Consolas", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.filePath.ForeColor = System.Drawing.SystemColors.WindowText;
            this.filePath.ImeMode = System.Windows.Forms.ImeMode.On;
            this.filePath.Location = new System.Drawing.Point(19, 49);
            this.filePath.Name = "filePath";
            this.filePath.Size = new System.Drawing.Size(683, 24);
            this.filePath.TabIndex = 3;
            // 
            // labelSource
            // 
            this.labelSource.AutoSize = true;
            this.labelSource.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelSource.ForeColor = System.Drawing.Color.Blue;
            this.labelSource.Location = new System.Drawing.Point(15, 87);
            this.labelSource.Name = "labelSource";
            this.labelSource.Size = new System.Drawing.Size(67, 24);
            this.labelSource.TabIndex = 4;
            this.labelSource.Text = "源代码";
            this.labelSource.Click += new System.EventHandler(this.label1_Click);
            // 
            // textSourceCode
            // 
            this.textSourceCode.BackColor = System.Drawing.Color.White;
            this.textSourceCode.Font = new System.Drawing.Font("Consolas", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textSourceCode.ForeColor = System.Drawing.SystemColors.WindowText;
            this.textSourceCode.Location = new System.Drawing.Point(19, 120);
            this.textSourceCode.Multiline = true;
            this.textSourceCode.Name = "textSourceCode";
            this.textSourceCode.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textSourceCode.Size = new System.Drawing.Size(321, 413);
            this.textSourceCode.TabIndex = 5;
            this.textSourceCode.TextChanged += new System.EventHandler(this.textBox_Input_TextChanged);
            // 
            // textCode
            // 
            this.textCode.BackColor = System.Drawing.Color.White;
            this.textCode.Font = new System.Drawing.Font("Consolas", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textCode.ForeColor = System.Drawing.SystemColors.WindowText;
            this.textCode.Location = new System.Drawing.Point(354, 119);
            this.textCode.Multiline = true;
            this.textCode.Name = "textCode";
            this.textCode.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textCode.Size = new System.Drawing.Size(196, 413);
            this.textCode.TabIndex = 6;
            this.textCode.TextChanged += new System.EventHandler(this.textBox_Code_TextChanged);
            // 
            // buttonStopDebug
            // 
            this.buttonStopDebug.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonStopDebug.BackgroundImage")));
            this.buttonStopDebug.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.buttonStopDebug.FlatAppearance.BorderSize = 0;
            this.buttonStopDebug.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonStopDebug.Location = new System.Drawing.Point(859, 41);
            this.buttonStopDebug.Name = "buttonStopDebug";
            this.buttonStopDebug.Size = new System.Drawing.Size(40, 40);
            this.buttonStopDebug.TabIndex = 8;
            this.buttonStopDebug.UseVisualStyleBackColor = true;
            this.buttonStopDebug.Click += new System.EventHandler(this.button_Build_Click);
            this.buttonStopDebug.MouseHover += new System.EventHandler(this.button_StopDebug_MouseHover);
            // 
            // buttonDebug
            // 
            this.buttonDebug.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonDebug.BackgroundImage")));
            this.buttonDebug.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.buttonDebug.FlatAppearance.BorderSize = 0;
            this.buttonDebug.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonDebug.Location = new System.Drawing.Point(813, 41);
            this.buttonDebug.Name = "buttonDebug";
            this.buttonDebug.Size = new System.Drawing.Size(40, 40);
            this.buttonDebug.TabIndex = 9;
            this.buttonDebug.UseVisualStyleBackColor = true;
            this.buttonDebug.MouseHover += new System.EventHandler(this.button_Debug_MouseHover);
            // 
            // textOutput
            // 
            this.textOutput.BackColor = System.Drawing.Color.White;
            this.textOutput.Font = new System.Drawing.Font("Consolas", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textOutput.ForeColor = System.Drawing.SystemColors.WindowText;
            this.textOutput.Location = new System.Drawing.Point(741, 119);
            this.textOutput.Multiline = true;
            this.textOutput.Name = "textOutput";
            this.textOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textOutput.Size = new System.Drawing.Size(265, 451);
            this.textOutput.TabIndex = 11;
            this.textOutput.TextChanged += new System.EventHandler(this.textBox_output3_TextChanged);
            // 
            // labelTitle
            // 
            this.labelTitle.AutoSize = true;
            this.labelTitle.Font = new System.Drawing.Font("Consolas", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTitle.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.labelTitle.Location = new System.Drawing.Point(12, 9);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(323, 37);
            this.labelTitle.TabIndex = 12;
            this.labelTitle.Text = "Jordan C Compiler";
            this.labelTitle.Click += new System.EventHandler(this.labelTitle_Click);
            // 
            // toolTip_openFile
            // 
            this.toolTip_openFile.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.toolTip_openFile.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.toolTip_openFile.Tag = "OpenFile";
            // 
            // toolTip_Run
            // 
            this.toolTip_Run.Tag = "Build and Run";
            // 
            // buttonRun
            // 
            this.buttonRun.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonRun.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonRun.BackgroundImage")));
            this.buttonRun.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.buttonRun.FlatAppearance.BorderSize = 0;
            this.buttonRun.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonRun.Location = new System.Drawing.Point(767, 39);
            this.buttonRun.Name = "buttonRun";
            this.buttonRun.Size = new System.Drawing.Size(40, 40);
            this.buttonRun.TabIndex = 13;
            this.buttonRun.UseVisualStyleBackColor = false;
            this.buttonRun.Click += new System.EventHandler(this.button_Run_Click);
            this.buttonRun.MouseHover += new System.EventHandler(this.button1_MouseHover);
            // 
            // toolTip_Debug
            // 
            this.toolTip_Debug.Tag = "Debug";
            // 
            // toolTip_StopDebug
            // 
            this.toolTip_StopDebug.Tag = "Stop Debug";
            // 
            // labelStack
            // 
            this.labelStack.AutoSize = true;
            this.labelStack.Font = new System.Drawing.Font("Consolas", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStack.ForeColor = System.Drawing.Color.Blue;
            this.labelStack.Location = new System.Drawing.Point(562, 89);
            this.labelStack.Name = "labelStack";
            this.labelStack.Size = new System.Drawing.Size(110, 22);
            this.labelStack.TabIndex = 15;
            this.labelStack.Text = "Err_Report";
            // 
            // labelOutputt
            // 
            this.labelOutputt.AutoSize = true;
            this.labelOutputt.Font = new System.Drawing.Font("Consolas", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelOutputt.ForeColor = System.Drawing.Color.Blue;
            this.labelOutputt.Location = new System.Drawing.Point(737, 89);
            this.labelOutputt.Name = "labelOutputt";
            this.labelOutputt.Size = new System.Drawing.Size(70, 22);
            this.labelOutputt.TabIndex = 16;
            this.labelOutputt.Text = "Output";
            this.labelOutputt.Click += new System.EventHandler(this.label_outputt_Click);
            // 
            // textErrReport
            // 
            this.textErrReport.BackColor = System.Drawing.Color.White;
            this.textErrReport.Location = new System.Drawing.Point(566, 119);
            this.textErrReport.Multiline = true;
            this.textErrReport.Name = "textErrReport";
            this.textErrReport.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textErrReport.Size = new System.Drawing.Size(162, 451);
            this.textErrReport.TabIndex = 10;
            // 
            // textTable
            // 
            this.textTable.Location = new System.Drawing.Point(354, 576);
            this.textTable.Multiline = true;
            this.textTable.Name = "textTable";
            this.textTable.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textTable.Size = new System.Drawing.Size(660, 155);
            this.textTable.TabIndex = 17;
            // 
            // textInput
            // 
            this.textInput.Location = new System.Drawing.Point(19, 569);
            this.textInput.Multiline = true;
            this.textInput.Name = "textInput";
            this.textInput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textInput.Size = new System.Drawing.Size(321, 162);
            this.textInput.TabIndex = 18;
            // 
            // label_Input
            // 
            this.label_Input.AutoSize = true;
            this.label_Input.Font = new System.Drawing.Font("Consolas", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Input.ForeColor = System.Drawing.Color.Blue;
            this.label_Input.Location = new System.Drawing.Point(15, 536);
            this.label_Input.Name = "label_Input";
            this.label_Input.Size = new System.Drawing.Size(60, 22);
            this.label_Input.TabIndex = 19;
            this.label_Input.Text = "Input";
            // 
            // label_Table
            // 
            this.label_Table.AutoSize = true;
            this.label_Table.Font = new System.Drawing.Font("Consolas", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Table.ForeColor = System.Drawing.Color.Blue;
            this.label_Table.Location = new System.Drawing.Point(350, 541);
            this.label_Table.Name = "label_Table";
            this.label_Table.Size = new System.Drawing.Size(60, 22);
            this.label_Table.TabIndex = 20;
            this.label_Table.Text = "Table";
            // 
            // buttonInputOK
            // 
            this.buttonInputOK.BackColor = System.Drawing.Color.WhiteSmoke;
            this.buttonInputOK.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.buttonInputOK.FlatAppearance.BorderSize = 5;
            this.buttonInputOK.FlatAppearance.MouseDownBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.buttonInputOK.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Gray;
            this.buttonInputOK.Font = new System.Drawing.Font("Consolas", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonInputOK.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.buttonInputOK.Location = new System.Drawing.Point(284, 539);
            this.buttonInputOK.Name = "buttonInputOK";
            this.buttonInputOK.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.buttonInputOK.Size = new System.Drawing.Size(56, 24);
            this.buttonInputOK.TabIndex = 21;
            this.buttonInputOK.Text = "OK";
            this.buttonInputOK.UseVisualStyleBackColor = false;
            this.buttonInputOK.Click += new System.EventHandler(this.buttonInputOK_Click);
            // 
            // JordanForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.ClientSize = new System.Drawing.Size(1024, 738);
            this.Controls.Add(this.buttonInputOK);
            this.Controls.Add(this.label_Table);
            this.Controls.Add(this.label_Input);
            this.Controls.Add(this.textInput);
            this.Controls.Add(this.textTable);
            this.Controls.Add(this.labelOutputt);
            this.Controls.Add(this.labelStack);
            this.Controls.Add(this.buttonRun);
            this.Controls.Add(this.labelTitle);
            this.Controls.Add(this.textOutput);
            this.Controls.Add(this.textErrReport);
            this.Controls.Add(this.buttonDebug);
            this.Controls.Add(this.buttonStopDebug);
            this.Controls.Add(labelOutput);
            this.Controls.Add(this.textCode);
            this.Controls.Add(this.textSourceCode);
            this.Controls.Add(this.labelSource);
            this.Controls.Add(this.filePath);
            this.Controls.Add(this.buttonOpenFile);
            this.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "JordanForm";
            this.Text = "Jordan_C_Compiler";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.JordanForm_KeyDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonOpenFile;
        private System.Windows.Forms.OpenFileDialog openInputFile;
        private System.Windows.Forms.TextBox filePath;
        private System.Windows.Forms.Label labelSource;
        private System.Windows.Forms.TextBox textSourceCode;
        private System.Windows.Forms.TextBox textCode;
        private System.Windows.Forms.Button buttonStopDebug;
        private System.Windows.Forms.Button buttonDebug;
        private System.Windows.Forms.TextBox textOutput;
        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.ToolTip toolTip_openFile;
        private System.Windows.Forms.ToolTip toolTip_Run;
        private System.Windows.Forms.Button buttonRun;
        private System.Windows.Forms.ToolTip toolTip_Debug;
        private System.Windows.Forms.ToolTip toolTip_StopDebug;
        private System.Windows.Forms.Label labelStack;
        private System.Windows.Forms.Label labelOutputt;
        private System.Windows.Forms.TextBox textErrReport;
        private System.Windows.Forms.TextBox textTable;
        private System.Windows.Forms.TextBox textInput;
        private System.Windows.Forms.Label label_Input;
        private System.Windows.Forms.Label label_Table;
        private System.Windows.Forms.Button buttonInputOK;
    }
}

