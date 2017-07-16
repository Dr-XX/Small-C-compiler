using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;



namespace WindowsFormsApplication1
{
    public partial class JordanForm : Form
    {
        public string fDirPath = ".\\";
        public JordanForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e){}
        private void button1_Click(object sender, EventArgs e){}
        private void openInputFile_FileOk(object sender, CancelEventArgs e){}


        /*
         * openFile Button
         * 打开源代码的文件
         */
        private void openFileButton_Click(object sender, EventArgs e)
        {
            openInputFile.InitialDirectory = "C:\\";
            openInputFile.Filter = "文本文件 (*.txt)|*.txt|All files (*.*)|*.*";
            if (this.openInputFile.ShowDialog() == DialogResult.OK){
                filePath.Text = openInputFile.FileName;
                System.IO.StreamReader sr = new System.IO.StreamReader(openInputFile.FileName, Encoding.GetEncoding("GB2312"));
                this.textSourceCode.Text = sr.ReadToEnd();
                sr.Close();
            }
            string fileName = fDirPath + "finput.txt";
            System.IO.File.WriteAllText(fileName, string.Empty);
            //StreamWriter FileWriter = new StreamWriter(fileName, true); //写文件
            //FileWriter.Close();
        }

        /*
         * 几个tooltip，显示button功能
         * 干啥用呢，看着爽啊 = =!
         */
        private void openFileButton_MouseHover_1(object sender, EventArgs e){
            toolTip_openFile.Show("Open File", buttonOpenFile);
        }
        private void button1_MouseHover(object sender, EventArgs e){
            toolTip_Run.Show("Build and Run", buttonRun);
        }
        private void button_Debug_MouseHover(object sender, EventArgs e){
            toolTip_Debug.Show("Debug", buttonDebug);
        }
        private void button_StopDebug_MouseHover(object sender, EventArgs e){
            toolTip_StopDebug.Show("Stop Debug", buttonStopDebug);
        }

        private void label1_Click(object sender, EventArgs e){}
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e){}
        private void textBox_Input_TextChanged(object sender, EventArgs e){}
        private void label_Output_Click(object sender, EventArgs e){}
        private void button_Build_Click(object sender, EventArgs e){}
        private void toolTip_openFile_Popup(object sender, PopupEventArgs e){}
        private void toolTip_openFile_Draw(object sender, DrawToolTipEventArgs e){}
        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e){}
        private void textBox_output3_TextChanged(object sender, EventArgs e){}

        private void JordanForm_KeyDown(object sender, KeyEventArgs e)
        {
            //if (e.Control ) openFileButton_Click(null, null);
        }

        public void textTOtextBox(string st, TextBox tb) {
            StreamReader sr = File.OpenText(st);
            for (; sr.Peek() != -1;){
                string tmp = sr.ReadLine().ToString();
                tb.AppendText(tmp);
                tb.AppendText("\n");
            }
            sr.Close();
        }

        private void button_Run_Click(object sender, EventArgs e)
        {
            
            textErrReport.Text = "";
            textCode.Text = "";
            textOutput.Text = "";
            textTable.Text = "";
        
                // 文件名
                string fileName = fDirPath+"in.txt";
                // 创建文件，准备写入
                FileStream fs = File.Open(fileName,
                        FileMode.Create,
                        FileAccess.Write);
                StreamWriter wr = new StreamWriter(fs);

                // 逐行将textBox1的内容写入到文件中
                foreach (string line in textSourceCode.Lines)
                {
                    wr.WriteLine(line);
                }
                wr.Flush();
                wr.Close();
                fs.Close();
            // 关闭文件
            
            Program.solve();//c++ 后台，编译器核心代码
            
            textTOtextBox(fDirPath + "foutput.txt", textErrReport);
            textTOtextBox(fDirPath + "fcode.txt", textCode);
            textTOtextBox(fDirPath + "fresult.txt", textOutput);
            textTOtextBox(fDirPath + "ftable.txt", textTable);
        }
       // 
        private void label_outputt_Click(object sender, EventArgs e)
        {

        }

        private void textBox_Code_TextChanged(object sender, EventArgs e){}

        private void buttonInputOK_Click(object sender, EventArgs e)
        {
            string fileName = fDirPath + "finput.txt";
            System.IO.File.WriteAllText(fileName, string.Empty);
            StreamWriter FileWriter = new StreamWriter(fileName, true); //写文件

            FileWriter.Write(textInput.Text);//将字符串写入

            FileWriter.Close(); //关闭StreamWriter对象
            

        }

        private void labelTitle_Click(object sender, EventArgs e)
        {

        }
    }

}