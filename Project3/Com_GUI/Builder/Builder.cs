////////////////////////////////////////////////////////////////////////
// Builder.cs - Demonstrates the functionalities of a Source code    // 
//               builder part of a pool process started by mother     //
//               builder                                              //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 3       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Send ready message to the mother builder
 * Get test request from mother builder
 * Parse the test request using TestRequest
 * Ask repository to post source files to build storage
 * Get files from the build storage
 * Run the build process on the files
 * 
 * Public Interface
 * ----------------
 * Builder bldr = new Builder();
 * bldr.getFiles();
 * bldr.parseRequest();
 * bldr.checkCache();
 * bldr.runProcess();
 * 
 * 
 * Required Files:
 * ---------------
 * Builder.cs
 * TestRequest.cs
 * IcomPro.cs
 * ComProService.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 28 Sep 2017
 * ver 2.0 : 24 Oct 2017
 * - first release
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
using System.Diagnostics;

namespace Com_GUI
{
  public class Builder
  {
        public string builderPath { get; set; } = "..\\..\\..\\Builder\\BuilderStorage";
        public List<string> sourcefiles { get; set; } = new List<string>();
        public List<string> files { get; set; } = new List<string>();
        public TestRequest tr2 { get; set; }
        private Comm cm { get; set; } = null;
        public int port { get; set; } = 0;
        
        //builder constructor, starts comm and the thread to process the received messages from motherbuilder and repo
        public Builder(int portNum) {

            port = portNum;
            builderPath = Path.GetFullPath(builderPath + port.ToString());
            if (!Directory.Exists(builderPath))
                Directory.CreateDirectory(builderPath);
            cm = new Comm(port, "/Builder", builderPath);

            Thread threadProc = new Thread(threadFunction);
            threadProc.Start();
            sendReady();
        }

        //get files from the build storage and store in a list
        public void getFiles(string path, string pattern)
        {
            files.Clear();
            string[] tempFiles = Directory.GetFiles(path, pattern);
            for (int i = 0; i < tempFiles.Length; ++i)
            {
                tempFiles[i] = Path.GetFileName(tempFiles[i]);
            }
            files.AddRange(tempFiles);
        }
        
        //parse a request using the TestRequest package
        public void parseRequest(string bfileSpec)
        {
            Console.Write("\n Parsing the test request...\n----------------------------------------------------\n");
            tr2 = new TestRequest();
            tr2.loadXml(bfileSpec);
            tr2.parse("author");
            tr2.parse("dateTime");
            tr2.parseDr("test");
        }

        //runs the build process
        public void runProcess()
        {
            //delete the dlls from the builder before running the build process to create DLLs
            deleteDLLs();

            string output ="";
            foreach (TestDriver td in tr2.testDrivers)
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

        //function to check the local storage of the builder for the source files required to process the test request
        public void checkCache()
        {
            Console.WriteLine("\n\nChecking the cache of this builder for the source files, if already there, not added to the 'sendsourcefile' message to repo!!!\n-----------------------------------------\n");

            getFiles(builderPath, "*.cs");
            foreach (TestDriver td in tr2.testDrivers)
            {
                if (!files.Contains(td.driverName))
                {
                    Console.WriteLine("This file is not in cache, requested from repo: {0}", td.driverName);
                    sourcefiles.Add(td.driverName);
                }
                else
                {
                    Console.WriteLine("This file is already in cache: {0}", td.driverName);
                }

                foreach (string file in td.testedFiles)
                {
                    if (!files.Contains(file))
                    {
                        Console.WriteLine("This file is not in cache, requested from repo: {0}", file);
                        sourcefiles.Add(file);
                    }
                    else
                    {
                        Console.WriteLine("This file is already in cache: {0}", file);
                    }

                }                
            }
            
        }

        //thread function to process the received messages on the receiver host
        public void threadFunction()
        {
            while (true)
            {
                CommMessage rcvMsg = cm.getMessage();
                rcvMsg.show();
                if (rcvMsg.command == "xmlFileSent")
                {
                    string xmlfile = rcvMsg.arguments[0];
                    
                    //get the file from the storage and its full path
                    string[] xmlFile = Directory.GetFiles(builderPath, xmlfile);
                    string bfileSpec = System.IO.Path.GetFullPath(xmlFile[0]);
                    parseRequest(bfileSpec);
                    
                    //check for files in cache
                    checkCache();
                    sendRepoRequest();
                }
                else if (rcvMsg.command == "sourceFilesSent")
                {
                    string[] filePaths = System.IO.Directory.GetFiles(builderPath, "*.dll");
                    foreach (string filePath in filePaths)
                    {
                        System.IO.File.Delete(filePath);
                    }
                    Console.WriteLine("\nStarting build process for the current test request:\n======================================================== \n");
                    runProcess();
                    sendReady();
                }
                else if (rcvMsg.command == "quit")
                {
                    Process.GetCurrentProcess().Kill();
                }

            }
        }

        //function to send ready message to the mother builder
        public void sendReady()
        {
            Console.WriteLine("\nSending ready message to the mother builder.......\n");

            CommMessage csndMsg = new CommMessage(CommMessage.MessageType.request);
            csndMsg.command = "ready";
            csndMsg.author = "Nishant Agrawal";
            csndMsg.to = "http://localhost:8080/MotherBuilder";
            csndMsg.from = "http://localhost:" + port.ToString() + "/Builder";
            csndMsg.fromStorage = builderPath;
            cm.postMessage(csndMsg);
        }

        //send request message to the repo now for source files if they are not in cache
        public void sendRepoRequest()
        {
            CommMessage csndMsg = new CommMessage(CommMessage.MessageType.request);
            csndMsg.command = "sendsourcefiles";
            csndMsg.author = "Nishant Agrawal";
            csndMsg.to = "http://localhost:9090/RepoMock";
            csndMsg.from = "http://localhost:" + port.ToString() + "/Builder";
            csndMsg.fromStorage = builderPath;
            foreach (string file in sourcefiles)
            {
                string fileP = builderPath + file;
                if (!File.Exists(fileP))
                {
                    csndMsg.arguments.Add(file);
                }
            }

            //if there are no arguments to the message don't post it
            if (csndMsg.arguments.Count != 0)
            {
                cm.postMessage(csndMsg);
                Console.WriteLine("\nRequest message sent to the repository for source files..........\n");
            }
            else
            {
                Console.WriteLine("\nIf all files are already in cache, no request message is sent to the repository for source files since all source files are already in builder local storage.\n");
                runProcess();
                sendReady();
            }

            sourcefiles.Clear();
        }

        //function to previously present dll files from the buildstorage
        public void deleteDLLs()
        {
            string[] filePaths = System.IO.Directory.GetFiles(builderPath, "*.dll");
            foreach (string filePath in filePaths)
            {
                System.IO.File.Delete(filePath);
            }
        }

        static void Main(string[] args)
        {
            int port = Int32.Parse(args[0]);
            Console.Title = "Builder"+port;
            Console.BackgroundColor = ConsoleColor.White;
            Console.ForegroundColor = ConsoleColor.DarkBlue;

            Console.Write("\n  Demo Builder Process on port {0}", port);
            Console.Write("\n ====================\n");

            Builder b1 = new Builder(port);
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
      
      int port = Int32.Parse(args[0]);
      Builder bldr = new Builder(port);
    }
  }
#endif
}
