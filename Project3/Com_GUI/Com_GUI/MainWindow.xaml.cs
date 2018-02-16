
////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for getting repo files, creating test    // 
//                 request, starting and shutting building process    //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 3       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Initialize window component and diplay the GUI
 * testExec() function to show all the requirements
 * button functionalities to get repo files, create test request, start and shut building process
 *
 * Public Interface
 * ----------------
 * testExec();
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml.cs
 * App.xaml.cs
 * RepoMock.cs
 * TestRequest.cs
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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.IO;

namespace Com_GUI
{
    //Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        private RepoMock rp { get; set; }
        private List<TestDriver> testD { get; set; }
        public MainWindow()
        {
            InitializeComponent();
            rp = new RepoMock();
            testD = new List<TestDriver>();
            processnumber.Text = "3";
        }

        //executive function to display that all requirements has been met
        public void testExec()
        {
            Console.Write("\n Demonstration of Requirement 1:\n======================================================== \n");
            Console.Write("\n This project has been prepared using C#, the .Net Frameowrk, and Visual Studio 2017 \n");
            Console.Write("\n Demonstration of Requirement 2:\n======================================================== \n");
            Console.Write("\n This project includes a Message-Passing Communication Service built with WCF\n");
            Console.Write("\n Demonstration of Requirement 3:\n======================================================== \n");
            Console.Write("\n The Communication Service supports sending build requests to Pool Processes from the mother Builder process and sending and receiving files from repository\n");
            Console.Write("\n Demonstration of Requirement 4:\n======================================================== \n");
            Console.Write("\n This provides a Process Pool component(function allProcess())that creates a user inputted number of processes on command\n\n\n");
            Console.Write("\n Demonstration of Requirement 5:\n======================================================== \n");
            Console.Write("\n Pool Processes are using Communication prototype to access messages and xml request files from the mother Builder process\n");
            Console.Write("\n Demonstration of Requirement 6:\n======================================================== \n");
            Console.Write("\n Includes a Graphical User Interface, built using WPF\n");
            Console.Write("\n Demonstration of Requirement 7:\n======================================================== \n");
            Console.Write("\n The GUI provides 'Start Builder' button to start the main Builder (mother process), specifying the number of child builders to be started and provides a button 'Stop Pool Processes' to ask the mother Builder to shut down its Pool Processes\n\n\n");
            Console.Write("\n Demonstration of Requirement 8:\n======================================================== \n");
            Console.Write("\n The GUI enables building test requests by selecting file names from the Mock Repository using 'Add driver' and 'Build Test Request' buttons\n\n\n");
            Console.Write("\n Demonstration of Requirement 9:\n======================================================== \n");
            Console.Write("\n This project integrates all three prototypes into a single functional Visual Studio Solution, with a Visual Studio project for each\n----------------------------------------------------\n\n");
            getfiles();
            TestDriver td1 = new TestDriver();
            td1.driverName = "TestDriver1.cs";
            List<string> tested1 = new List<string>();
            tested1.Add("TestedOne.cs");
            tested1.Add("TestedTwo.cs");
            td1.testedFiles = tested1;

            TestDriver td2 = new TestDriver();
            td2.driverName = "TestDriver2.cs";
            List<string> tested2 = new List<string>();
            tested2.Add("TestedOne.cs");
            tested2.Add("TestedTwo.cs");
            td2.testedFiles = tested2;

            testD.Add(td1);
            testD.Add(td2);
            buildrequestfunc();
            Console.Write("\n Your test request has been built and saved to the repository.\n---------------------------------------------------- \n");

            string buildProcess = processnumber.Text;
            startBuilder(buildProcess);
            rp.sendRequests();
        }

        //button function to get driver and tested files from repository
        private void Button_getfiles(object sender, RoutedEventArgs e)
        {
            Console.Write("\n Get Files Button clicked.\n");
            getfiles();
        }

        //function to get driver and tested files from repository
        private void getfiles()
        {
            Console.Write("\n Test Drivers and tested files list are now updated in the List boxes.\n---------------------------------------------------- \n");

            rp.getFiles(rp.storagePath, "*dri*");
            foreach (string file in rp.files)
                driverListBox.Items.Add(System.IO.Path.GetFileName(file));

            rp.getFiles(rp.storagePath, "*tested*");
            foreach (string file in rp.files)
                testFilesListBox.Items.Add(System.IO.Path.GetFileName(file));
        }

        //button function to add driver after choosing a driver and its corresponding tested files
        private void Button_adddriver(object sender, RoutedEventArgs e)
        {
            //when nothing is selected           
            if (driverListBox.SelectedIndex == -1 || testFilesListBox.SelectedIndex == -1)
            {
                Console.Write("\n >>>Please select a test driver and corresponding tested files and then press this button.\n");
                return;
            }

            TestDriver td1 = new TestDriver();
            td1.driverName = driverListBox.SelectedItem.ToString();
            List<string> tested = new List<string>();
            for (int i = 0; i < testFilesListBox.SelectedItems.Count; i++)
            {
                tested.Add(testFilesListBox.SelectedItems[i].ToString());
            }
            td1.testedFiles = tested;
            testD.Add(td1);
            Console.Write("\n Test Driver:{0} and its tested files have been added to the request. Press the 'Build Request' button when you are done adding all drivers.\n----------------------------------------------------\n", td1.driverName);

        }

        //button function to create a request after desired drivers and tested files have been added
        private void Button_buildrequest(object sender, RoutedEventArgs e)
        {
            if (testD.Count == 0)
            {
                Console.Write("\n >>>Add drivers and tested files first to your test request .\n");
            }
            else
            {
                buildrequestfunc();
                Console.Write("\n Your test request has been built and saved to the repository.\n---------------------------------------------------- \n");
            }

        }

        //function to create a request after desired drivers and tested files have been added
        private void buildrequestfunc()
        {
            TestRequest tr = new TestRequest();
            string fileName = "TestRequest.xml";
            string fileSpec = System.IO.Path.Combine(rp.storagePath, fileName);
            fileSpec = System.IO.Path.GetFullPath(fileSpec);

            tr.author = "Nishant Agrawal";
            tr.testDrivers = testD;
            tr.makeRequest();
            tr.saveXml(fileSpec);
            
            testD.Clear();
        }

        //button function to start the motherbuilder and sending it the no. of builder processes to start
        private void Button_startbuilder(object sender, RoutedEventArgs e)
        {

            string buildProcess = processnumber.Text;
            startBuilder(buildProcess);
            rp.sendRequests();

        }

        //button function to shut the builder processes
        private void Button_shutbuilder(object sender, RoutedEventArgs e)
        {
            Console.Write("\n Shutting down the builder processes by sending a message to the mother Builder.\n======================================================== \n");
            rp.sendShutProcessMessage();
        }

        //function to start motherbuilder process
        private bool startBuilder(string i)
        {
            Process proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo();

            info.FileName = "..\\..\\..\\MotherBuilder\\bin\\debug\\MotherBuilder.exe";
            string absFileSpec = System.IO.Path.GetFullPath(info.FileName);

            Console.Write("\nAttempting to start Mother Builder process.........\n");

            info.Arguments = i;
            proc.StartInfo = info;
            try
            {
                proc.Start();
            }
            catch (Exception ex)
            {
                Console.Write("\n  {0}", ex.Message);
                return false;
            }
            return true;
        }

    }
}
