////////////////////////////////////////////////////////////////////////
// RepoMock.cs - Demonstrates the functionalities of a Source code    // 
//                 builder                                            //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 2       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Get test request from build storage
 * Parse the test request using TestRequest
 * Ask repository for source files
 * Get files from the build storage
 * Run the build process on the files
 * 
 * Required Files:
 * ---------------
 * Builder.cs
 * TestRequest.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 28 Sep 2017
 * - first release
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace Federation
{
  public class Builder
  {
        public string builderPath { get; set; } = "../../BuilderStorage";
        public List<string> files { get; set; } = new List<string>();
        public List<TestRequest> testRequests { get; set; } = new List<TestRequest>();
        public Builder() { }

        //helper function to get files from the build storage
        private void getFilesHelper(string path, string pattern)
        {
            string[] tempFiles = Directory.GetFiles(path, pattern);
            for (int i = 0; i < tempFiles.Length; ++i)
            {
                tempFiles[i] = Path.GetFullPath(tempFiles[i]);
            }
            files.AddRange(tempFiles);

            string[] dirs = Directory.GetDirectories(path);
            foreach (string dir in dirs)
            {
                getFilesHelper(dir, pattern);
            }
        }

        //get files from the build storage and store in a list
        public void getFiles(string path, string pattern)
        {
            files.Clear();
            getFilesHelper(path, pattern);
        }

        //parse a request using the TestRequest package
        public void parseRequest(string bfileSpec)
        {

            TestRequest tr2 = new TestRequest();
            tr2.loadXml(bfileSpec);
            tr2.parse("author");
            tr2.parse("dateTime");
            tr2.parseDr("test");

            testRequests.Add(tr2);
        }

        //runs the build process
        public void runProcess()
        {
            string output="";
            foreach (TestDriver td in testRequests[0].testDrivers)
            {
                Console.WriteLine("\n\nBuilding for test with {0}!!!\n===========================================\n\n", td.driverName);
                StringBuilder sb = new StringBuilder();
                sb.Append(td.driverName);
                foreach (string file in td.testedFiles)
                {
                    sb.Append(" ");
                    sb.Append(file);
                }
                String entirePath = sb.ToString();
        
                Process p = new Process();
                ProcessStartInfo info = new ProcessStartInfo();

                info.FileName = "cmd.exe";
                info.WindowStyle = ProcessWindowStyle.Hidden;

                info.Arguments = string.Format("/Ccsc /target:library {0}", entirePath);

                info.WorkingDirectory = builderPath;
                info.RedirectStandardError = true;
                info.RedirectStandardOutput = true;
                info.UseShellExecute = false;

                p.StartInfo = info;
                p.Start();
                p.WaitForExit();
              
                output = p.StandardOutput.ReadToEnd();
                if (output.Contains("error"))
                {
                    Console.WriteLine("\n\nBuild Failed!!!\n-----------------------------------------\n");
                }
                else
                {
                    Console.WriteLine("\n Build successful!!!!\n-----------------------------------------\n\n");
                }
                Console.WriteLine("{0}", output);
            }
        }
    }
#if (TEST_BUILDER)

  ///////////////////////////////////////////////////////////////////
  // TestBuilder class

  class TestBuilder
  {
    static void Main(string[] args)
    {
      Console.Write("\n  Demonstration of souce code Builder");
      Console.Write("\n ============================");

      Builder bldr = new Builder();
      bldr.getFiles(bldr.builderPath, "*.xml");
      string bfileSpec = System.IO.Path.GetFullPath(bldr.files[0]);
      bldr.parseRequest(bfileSpec);

      string[] filePaths = System.IO.Directory.GetFiles(@bldr.builderPath, "*.dll");
      foreach (string filePath in filePaths)
      {
        System.IO.File.Delete(filePath);
      }
      bldr.runProcess();

      Console.Write("\n\n");
    }
  }
#endif
}
