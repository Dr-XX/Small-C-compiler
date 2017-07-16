using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public static class Program
    {
        [DllImport("..\\..\\Release\\DLLtest.dll", EntryPoint = "solve", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = false, CallingConvention = CallingConvention.StdCall)]
        //[DllImport("D:\\Program\\jordan_c\\Jordan_C_Compiler\\Release\\DLLtest.dll", EntryPoint = "solve", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = false, CallingConvention = CallingConvention.StdCall)]
        public static extern void solve();

        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new JordanForm());

            //string str = System.Environment.CurrentDirectory;
            //Console.WriteLine(str);
            
            //solve();
        }
    }
}
