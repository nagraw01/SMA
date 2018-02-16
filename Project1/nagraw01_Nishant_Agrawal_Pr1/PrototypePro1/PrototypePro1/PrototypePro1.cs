using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace PrototypePro1
{
    class PrototypePro1
    {
        public string batchPath { get; set; } = "../../TestFiles";
        public List<string> files { get; set; } = new List<string>();
        public void CompileCpp(string filePath)
        {
            
            string cppBatchFile = "test_cpp.bat";
            string destSpec = Path.Combine(batchPath, cppBatchFile);
           
            string cppExeFile = Path.GetFileNameWithoutExtension(filePath);

            runProcess(destSpec, filePath);
        }

        public void CompileCs(string filePath)
        {

            string csBatchFile = "test_cs.bat";
            string destSpec = Path.Combine(batchPath, csBatchFile);
            runProcess(destSpec, filePath);

        }

        private void getFilesHelper(string path)
        {
            
            string[] tempFiles = Directory.GetFiles(path);
            
            for (int i = 0; i < tempFiles.Length; ++i)
            {           
                tempFiles[i] = Path.GetFullPath(tempFiles[i]);
            }
            files.AddRange(tempFiles);

            string[] dirs = Directory.GetDirectories(path);
            foreach (string dir in dirs)
            {
                getFilesHelper(dir);
            }
        }

        public void getFiles(string projPath)
        {
            files.Clear();
            getFilesHelper(projPath);
        }

        public string ToLiteral(string input)
        {
            StringBuilder literal = new StringBuilder(input.Length + 2);
            literal.Append("\"");
            foreach (var c in input)
            {
                switch (c)
                {
                    case '\'': literal.Append(@"\'"); break;
                    case '\"': literal.Append("\\\""); break;
                    case '\\': literal.Append(@"\\"); break;
                    case '\0': literal.Append(@"\0"); break;
                    case '\a': literal.Append(@"\a"); break;
                    case '\b': literal.Append(@"\b"); break;
                    case '\f': literal.Append(@"\f"); break;
                    case '\n': literal.Append(@"\n"); break;
                    case '\r': literal.Append(@"\r"); break;
                    case '\t': literal.Append(@"\t"); break;
                    case '\v': literal.Append(@"\v"); break;
                    default:
                        // ASCII printable character
                        if (c >= 0x20 && c <= 0x7e)
                        {
                            literal.Append(c);
                            // As UTF16 escaped character
                        }
                        else
                        {
                            literal.Append(@"\u");
                            literal.Append(((int)c).ToString("x4"));
                        }
                        break;
                }
            }
            literal.Append("\"");
            return literal.ToString();
        }

        private void runProcess(string destSpec, string filePath)
        {
            string exeFile = Path.GetFileNameWithoutExtension(filePath);
            filePath = ToLiteral(filePath);

            Process p = new Process();
            ProcessStartInfo info = new ProcessStartInfo();

            info.FileName = destSpec;
            info.Arguments = string.Format("{0} {1}", filePath, exeFile);

            p.StartInfo = info;
            p.Start();
            p.WaitForExit();
        }

    }

    class TestPrototypePro1
    {
        static void Main(string[] args)
        {
            PrototypePro1 p = new PrototypePro1();
            Console.Write("\n Demonstration of Project Builder Prototype \n");
            Console.Write("\n ======================================================== \n");

            Console.WriteLine("Enter the path of a C++ or a C# Visual Studio Project:");
            string projPath = Console.ReadLine();
    
            string s1 = null;
            string s1Ext = null;
          

            p.getFiles(projPath);
           
            foreach (string file in p.files) {
                
                s1Ext = Path.GetExtension(file);
               if ((File.ReadLines(file).Any(line => line.Contains("main")) || File.ReadLines(file).Any(line => line.Contains("Main"))) && (s1Ext == ".cpp" || s1Ext == ".cs"))
               {
                    s1 = file;
                    goto BreakForeach;
                    
               }
                    
            }

            BreakForeach:

            if (!string.IsNullOrEmpty(s1)){
                Console.WriteLine("\n****Attemting to compile the source file: "+s1);
                if (s1Ext == ".cpp") p.CompileCpp(s1);
                else if (s1Ext == ".cs") p.CompileCs(s1);
                else Console.Write("\nThere is no cpp or cs file with a main function in it in the given path.\n");
            }

        }
    }
}
