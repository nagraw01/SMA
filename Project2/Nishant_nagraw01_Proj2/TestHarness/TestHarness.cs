////////////////////////////////////////////////////////////////////////
// RepoMock.cs - Demonstrates the functionalities of a Mock Test      // 
//               harness where the tests are executed as dll files    //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 2       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Gets the lib .dll files from the build storage which is known to itself
 * Execute the dll files by invoking the class methods
 * 
 * Required Files:
 * ---------------
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
using System.IO;
using System.Reflection;


namespace Federation
{
  public class TestHarness
  {
        public string testPath { get; set; } = "../../BuilderStorage";
        public List<string> files { get; set; } = new List<string>();
        public TestHarness() { }

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
        /*----< find all the files in TestHarness.testPath >-----------*/
        /*
        *  Finds all the files, matching pattern, in the entire 
        *  directory tree
        */
        public void getFiles(string path, string pattern)
        {
            files.Clear();
            getFilesHelper(path, pattern);
        }

        //finds all the dll files in the build storage and invokes the main methods 
        //of the test drivers
        public void runDllTests()
        {
            getFiles(testPath, "*.dll");
            if (files.Count == 0)
            {
                Console.WriteLine("There are no library dll files to execute. All builds failed!!!\n\n");
            }
            else
            {
                foreach (string library in files)
                {
                    Console.WriteLine("\n\nExecuting test for {0}!!!\n===========================================", library);
                    Console.WriteLine();
                    Assembly assem = Assembly.LoadFrom(library);
                    Type[] types = assem.GetExportedTypes();
                    foreach (Type t in types)
                    {
                        if (t.IsClass && !t.IsAbstract)
                        {
                            object obj = Activator.CreateInstance(t);

                            MethodInfo[] mis = t.GetMethods(BindingFlags.NonPublic | BindingFlags.Static);
                            foreach (MethodInfo mi in mis)
                            {
                                // don't call base members

                                if (mi.DeclaringType != typeof(object))
                                {

                                    // don't call if method has arguments
                                    try
                                    {
                                        if (mi.GetParameters().Length != 0/* && mi.Name != "create"*/)
                                        {
                                            mi.Invoke(null, new Object[] { new string[] { "" } });
                                        }
                                    }
                                    catch (Exception e) { Console.WriteLine("\n\nException while execution: {0}\n\n", e.Message);/* continue on error - calling factory function create throws */ }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

#if (TEST_TESTHARNESS)

  ///////////////////////////////////////////////////////////////////
  // TestTestHarness class

  class TestTestHarness
  {
    static void Main(string[] args)
    {
      Console.Write("\n  Demonstration of Mock test harness");
      Console.Write("\n ============================");

      TestHarness tr = new TestHarness();
      tr.runDllTests();

      Console.Write("\n\n");
    }
  }
#endif
}
