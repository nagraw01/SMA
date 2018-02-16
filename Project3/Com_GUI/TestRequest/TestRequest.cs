////////////////////////////////////////////////////////////////////////
// RepoMock.cs - This package helps building xml test requests and    // 
//                 later parsing them on builder                      //
//                                                                    //
// Author: Nishant Agrawal, email-nagraw01@syr.edu                    //
// Application: CSE681 - Software Modelling Analysis, Project 3       //
// Environment: C# console                                            //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * Creates and parses TestRequest XML messages using XDocument
 * 
 * Public Interface
 * ----------------
 * TestRequest tr = new TestRequest();
 * tr.makeRequest();
 * 
 * Required Files:
 * ---------------
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
using System.Xml.Linq;

namespace Com_GUI
{
    ///////////////////////////////////////////////////////////////////
    // TestRequest class
    public class TestDriver
    {
        public string driverName { get; set; } = "";
        public List<string> testedFiles { get; set; } = new List<string>();
    }
  public class TestRequest
  {
    public string requestFileName { get; set; } = "TestRequest";
    public string author { get; set; } = "";
    public string dateTime { get; set; } = "";
    public List<TestDriver> testDrivers { get; set; } = new List<TestDriver>();
    public XDocument doc { get; set; } = new XDocument();

    /*----< build XML document that represents a test request >----*/

    public void makeRequest()
    {
      XElement testRequestElem = new XElement("testRequest");
      doc.Add(testRequestElem);

      XElement authorElem = new XElement("author");
      authorElem.Add(author);
      testRequestElem.Add(authorElem);

      XElement dateTimeElem = new XElement("dateTime");
      dateTimeElem.Add(DateTime.Now.ToString());
      testRequestElem.Add(dateTimeElem);



        foreach (TestDriver dr in testDrivers)
        {
            XElement testElem = new XElement("test");
            testRequestElem.Add(testElem);

            XElement driverElem = new XElement("testDriver");
            driverElem.Add(dr.driverName);
            testElem.Add(driverElem);
            foreach (string tfile in dr.testedFiles)
            {
                XElement testedElem = new XElement("tested");
                testedElem.Add(tfile);
                testElem.Add(testedElem);
            }
        }

  
    }
    /*----< load TestRequest from XML file >-----------------------*/

    public bool loadXml(string path)
    {
      try
      {
        doc = XDocument.Load(path);
        return true;
      }
      catch(Exception ex)
      {
        Console.Write("\n--{0}--\n", ex.Message);
        return false;
      }
    }
    /*----< save TestRequest to XML file >-------------------------*/

    public bool saveXml(string path)
    {
      try
      {
        doc.Save(path);
        return true;
      }
      catch(Exception ex)
      {
        Console.Write("\n--{0}--\n", ex.Message);
        return false;
      }
    }
    /*----< parse document for property value >--------------------*/

    public string parse(string propertyName)
    {

      string parseStr = doc.Descendants(propertyName).First().Value;
      if (parseStr.Length > 0)
      {
        switch (propertyName)
        {
          case "author":
            author = parseStr;
            break;
          case "dateTime":
            dateTime = parseStr;
            break;
          default:
            break;
        }
        return parseStr;
      }
      return "";
    }
    
        /*----< parse document for property list >---------------------*/
    /*
    * - now, there is only one property list for test drivers, where each test driver has a list 
    *   of tested files
    */

        public List<TestDriver> parseDr(string propertyName)
        {
            List<TestDriver> values = new List<TestDriver>();

            IEnumerable<XElement> parseElems = doc.Descendants(propertyName);

            if (parseElems.Count() > 0)
            {
                switch (propertyName)
                {
                    case "test":
                        foreach (XElement elem in parseElems)
                        {
                            foreach (XElement elemd in elem.Elements("testDriver"))
                            {
                                TestDriver v = new TestDriver();
                                Console.WriteLine("Driver file {0}", elemd.Value);
                                v.driverName = elemd.Value;
                                IEnumerable<XElement> parseElems2 = elem.Elements("tested");
                                foreach (XElement elem2 in parseElems2)
                                {
                                    Console.WriteLine("Tested files for this driver {0}", elem2.Value);
                                    v.testedFiles.Add(elem2.Value);
                                }
                                Console.WriteLine("");
                                values.Add(v);
                            }
                            
                        }
                        testDrivers = values;
                        break;
                    default:
                        break;
                }
            }
            return values;
        }
    }
  ///////////////////////////////////////////////////////////////////
  // test_TestRequest class

  class Test_TestRequest
  {
#if (TEST_X)
    static void Main(string[] args)
    {
      Console.Write("\n  Testing TestRequest");
      Console.Write("\n =====================");
      string savePath = "../../test/";
      string fileName = "TestRequest1.xml";
      if (!System.IO.Directory.Exists(savePath))
        System.IO.Directory.CreateDirectory(savePath);
      string fileSpec = System.IO.Path.Combine(savePath, fileName);
      fileSpec = System.IO.Path.GetFullPath(fileSpec);
      TestRequest tr = new TestRequest();
      tr.author = "Nishant Agrawal";
        TestDriver td1 = new TestDriver();
        td1.driverName = "td1.cs";
        td1.testedFiles.Add("tf1.cs");
        td1.testedFiles.Add("tf2.cs");
        tr.testDrivers.Add(td1);
      tr.makeRequest();
      Console.Write("\n{0}", tr.doc.ToString());
      Console.Write("\n  saving to \"{0}\"", fileSpec);
      tr.saveXml(fileSpec);
      Console.Write("\n  reading from \"{0}\"", fileSpec);
      TestRequest tr2 = new TestRequest();
      tr2.loadXml(fileSpec);
      Console.Write("\n{0}", tr2.doc.ToString());
      Console.Write("\n");
      tr2.parse("author");
      Console.Write("\n  author is \"{0}\"", tr2.author);
      tr2.parse("dateTime");
      Console.Write("\n  dateTime is \"{0}\"", tr2.dateTime);
      tr2.parseDr("test");
      Console.Write("\n  testedFiles are:");
      foreach(TestDriver td in tr2.testDrivers)
      {                Console.Write("\n  testDriver is \"{0}\"", td.driverName);
                foreach (string file in td.testedFiles)
            {
                Console.Write("\n    \"{0}\"", file);
            }           
      }
      Console.Write("\n\n");
    }
  }
#endif
}

