////////////////////////////////////////////////////////////////////////
// RepoMock.cs - Demonstrates the functionalities of a Mock repository// 
//                 for storing test request and source code files     //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 3       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Store and get files from repository storage
 * Send request files to motherbuilder
 * Send source files to builder processes after getting a request message
 *
 * Public Interface
 * ----------------
 * RepoMock rp = new RepoMock();
 * rp.sendRequests();
 * 
 * Required Files:
 * ---------------
 * RepoMock.cs
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

namespace Com_GUI
{
  ///////////////////////////////////////////////////////////////////
  // RepoMock class
  // - begins to simulate basic Repo operations

  public class RepoMock
  {
        public string storagePath { get; set; } = "..\\..\\..\\RepoMock\\RepoStorage";
        public List<string> files { get; set; } = new List<string>();
        private Comm cm { get; set; } = null;

        
        //repomock constructor, starts comm and the thread for receiving 'sendsourcefile' request from builder processes
        public RepoMock()
        {
            string absPath = Path.GetFullPath(storagePath);
            cm = new Comm(9090, "/RepoMock", absPath);
            Thread threadProc = new Thread(sendSourceFiles);
            threadProc.Start();
        }

        //function to get the files from repository storage
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

        //thread function to send source files to the builder processes 
        public void sendSourceFiles()
        {
            while (true)
            {
                CommMessage rcvmsg = cm.getMessage();
                rcvmsg.show();
                if (rcvmsg.command == "sendsourcefiles")
                {
                    //connect before postFile
                    CommMessage csndMsg = new CommMessage(CommMessage.MessageType.request);
                    csndMsg.command = "connect";
                    csndMsg.author = "Nishant Agrawal";
                    csndMsg.to = rcvmsg.from;
                    csndMsg.from = "http://localhost:" + "9090/RepoMock";
                    cm.postMessage(csndMsg);

                    Console.WriteLine("\nSending source files to builderProcess {0} from repomock checking the arguments of 'sendsourcefiles' message from builder using WCF...\n----------------------------------------------------\n", rcvmsg.from);
                    foreach (string fileName in rcvmsg.arguments)
                    {
                        cm.postFile(fileName, rcvmsg.from, rcvmsg.fromStorage);
                    }

                    csndMsg.command = "sourceFilesSent";
                    cm.postMessage(csndMsg);
                }
            }
        }

        //function to send xml files to the mother builder
        public void sendRequests()
        {
            Console.WriteLine("\nSending xml requests to mother builder from repomock using WCF...\n----------------------------------------------------\n");
            
            //connect before postFile
            CommMessage csndMsg = new CommMessage(CommMessage.MessageType.request);
            csndMsg.command = "connect";
            csndMsg.author = "Nishant Agrawal";
            csndMsg.to = "http://localhost:" + "8080/MotherBuilder";
            csndMsg.from = "http://localhost:" + "9090/RepoMock";
            cm.postMessage(csndMsg);

            getFiles(storagePath, "*.xml");
            foreach(string file in files)
            {
                cm.postFile(file, csndMsg.to, "..\\..\\..\\MotherBuilder\\mBuilderStorage");  
            }

            //send message after file sent
            CommMessage rsndMsg = new CommMessage(CommMessage.MessageType.request);
            rsndMsg.command = "allxmlsent";
            rsndMsg.author = "Nishant Agrawal";
            rsndMsg.to = "http://localhost:" + "8080/MotherBuilder";
            rsndMsg.from = "http://localhost:" + "9090/RepoMock";
            cm.postMessage(rsndMsg);
        }
        
        //function to send kill builder process message to the mother builder
        public void sendShutProcessMessage()
        {
            CommMessage csndMsg = new CommMessage(CommMessage.MessageType.request);
            csndMsg.command = "killBuilders";
            csndMsg.author = "Nishant Agrawal";
            csndMsg.to = "http://localhost:" + "8080/MotherBuilder";
            csndMsg.from = "http://localhost:" + "9090/RepoMock";
            cm.postMessage(csndMsg);
        }
        
    }

#if (TEST_REPOMOCK)

    ///////////////////////////////////////////////////////////////////
    // TestRepoMock class

    class TestRepoMock
    {
        static void Main(string[] args)
        {
            Console.Write("\n  Demonstration of Mock Repo");
            Console.Write("\n ============================");
            RepoMock repo = new RepoMock();
            repo.getFiles(repo.storagePath, "*.*");
            foreach (string file in repo.files)
                Console.Write("\nFiles in repo:  \"{0}\"", file);

            repo.sendRequests();
            Console.Write("\n\n");
        }
    }
#endif
}

