////////////////////////////////////////////////////////////////////////
// RepoMock.cs - Demonstrates the requirements for the Project 2 and  // 
//                 acts as test executive for Project 2               //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 2       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Test executive
 * 
 * Required Files:
 * ---------------
 * Executive.cs
 * TestRequest.cs
 * RepoMock.cs
 * Builder.cs
 * TestHarness.cs
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

namespace Federation
{
    class Executive
    {
        RepoMock rp { get; set; }
        Builder bldr { get; set; }
        TestHarness th { get; set; }

        Executive()
        {
            rp = new RepoMock();
            bldr = new Builder();
            th = new TestHarness();
        }

        //a helper method to create test request
        public void buildDriverList(string fileName)
        {
            List<TestDriver> testD = new List<TestDriver>();

            TestDriver td1 = new TestDriver();
            td1.driverName = "TestDriver1.cs";
            List<string> tested = new List<string>();
            tested.Add("TestedOne.cs");
            tested.Add("TestedTwo.cs");
            td1.testedFiles = tested;

            TestDriver td2 = new TestDriver();
            td2.driverName = "TestDriver2.cs";
            List<string> tested2 = new List<string>();
            tested2.Add("TestedOne.cs");
            tested2.Add("TestedTwo.cs");
            td2.testedFiles = tested2;

            TestDriver td3 = new TestDriver();
            td3.driverName = "TestDriver3.cs";
            List<string> tested3 = new List<string>();
            tested3.Add("TestedThree.cs");
            td3.testedFiles = tested3;

            testD.Add(td1);
            testD.Add(td2);
            testD.Add(td3);

            createRequest(fileName, testD);
        }

        //method to create test request
        public void createRequest(string fileName, List<TestDriver> driver)
        {
            TestRequest tr = new TestRequest();

            string fileSpec = System.IO.Path.Combine(rp.storagePath, fileName);
            fileSpec = System.IO.Path.GetFullPath(fileSpec);

            tr.author = "Nishant Agrawal";
            tr.testDrivers = driver;

            tr.makeRequest();
            tr.saveXml(fileSpec);
        }

        //a method which waits till it gets the yes to execute further statments
        public void askForCommand(string message)
        {
            while (true)
            {
                Console.Write("\n {0}\n", message);
                if (System.Text.RegularExpressions.Regex.IsMatch(Console.ReadLine(), "y", System.Text.RegularExpressions.RegexOptions.IgnoreCase))
                {
                    break;
                }
            }
        }

        //send the request from the repository to the build storage
        public void sendRequest(string pattern)
        {
            rp.getFiles(rp.storagePath, pattern);
            rp.sendFile(rp.files[0]);
        }

        //parse the test request at the builder
        public void parseAtBuilder(string pattern)
        {
            //get the test request file from build storage
            bldr.getFiles(bldr.builderPath, pattern);
            string bfileSpec = System.IO.Path.GetFullPath(bldr.files[0]);

            //parse the request at the builder
            bldr.parseRequest(bfileSpec);
        }

        //copy the source files from repo storage to the build storage
        public bool copySourceFiles()
        {
            foreach (TestDriver td in bldr.testRequests[0].testDrivers)
            {
                rp.getFiles(rp.storagePath, td.driverName);
                if (rp.files.Count == 0) return false;
                rp.sendFile(rp.files[0]);

                foreach (string file in td.testedFiles)
                {
                    rp.getFiles(rp.storagePath, file);
                    if (rp.files.Count == 0) return false;
                    rp.sendFile(rp.files[0]);
                }
            }
            return true;

        }

        //start the build process
        public void startBuilding()
        {
            //remove the previous dll files from build storage
            string[] filePaths = System.IO.Directory.GetFiles(@bldr.builderPath, "*.dll");
            foreach (string filePath in filePaths)
            {
                System.IO.File.Delete(filePath);
            }

            //run the build process
            bldr.runProcess();

        }

        //execute the dll files at the test harness
        public void executeTest()
        {
            th.runDllTests();
        }

        static void Main(string[] args)
        {
           Console.Write("\n Demonstration of Requirement 1:\n======================================================== \n");
            Console.Write("\n This project has been prepared using C#, the .Net Frameowrk, and Visual Studio 2017 \n");
            
            Console.Write("\n Demonstration of Requirement 2:\n======================================================== \n");
            Console.Write("\n This project include packages for an Executive, mock Repository, mock Test Harness and other packages like ....for the Core Project Builder\n");

            Console.Write("\n Demonstration of Requirement 3:\n======================================================== \n");
            Console.Write("\n This exec package constructs fixed sequence of operations of the repository, build server and the test harness.\n");

            Executive ex = new Executive();
            Console.Write("\n Creating a test request with test drivers and tested files....\n----------------------------------------------------\n");
            ex.buildDriverList("TestRequest.xml");
            ex.askForCommand("Start the building process by sending the request to the builder ? Press 'y' for yes ");

            Console.Write("\n Copying the test request to the build storage...\n----------------------------------------------------\n");
            ex.sendRequest("TestRequest.xml");

            Console.Write("\n Parsing the test request at the builder...\n----------------------------------------------------\n");
            ex.parseAtBuilder("TestRequest.xml");

            Console.Write("\n Demonstration of Requirement 4:\n======================================================== \n");
            Console.Write("\n Copying the required files for building to the build storage.\n\n\n");

            Console.Write("\n Requesting the repository for the source files...\n----------------------------------------------------\n");
            ex.askForCommand("Send the source code files to the builder? Press 'y' for yes");
            if (ex.copySourceFiles() == false)
            {
                Console.WriteLine("\nThe source code files are not there in the repository!!!\n\n");
                return;
            }

            Console.Write("\n Demonstration of Requirement 5:\n======================================================== \n");
            Console.Write("\n Building the project files sent by repository using build server.\n");

            Console.Write("\n Demonstration of Requirement 6:\n======================================================== \n");
            Console.Write("\n The Build server shows reports of build success and failures alongwith warnings to the console.\n");

            Console.Write("\n Demonstration of Requirement 7:\n======================================================== \n");
            Console.Write("\n Build server delivers the built libraries to the build storage which is known to the test harness.\n\n\n");
            Console.Write("\n Start the building process...\n----------------------------------------------------\n");
            Console.Write("\n 3 test cases taken for 3 tests in the test request...\n");
            Console.Write("\n 1)No build or execution fails for the test with TestDriver1.\n");
            Console.Write("\n 2)Build fail for the test with TestDriver2.\n");
            Console.Write("\n 3)Execution fail for the test with TestDriver3.\n\n\n");
            ex.startBuilding();

            Console.Write("\n Demonstration of Requirement 8:\n======================================================== \n");
            Console.Write("\n The test libraries are executed using the test harness.\n\n\n");
            Console.Write("\n Start the test execution at test harness....\n----------------------------------------------------\n\n");
            ex.executeTest();
        }
    }
}
