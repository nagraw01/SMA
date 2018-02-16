#pragma once
/////////////////////////////////////////////////////////////////////////////////
// Test.h - All the requirements are created as test methods here to be executed in 
//         a try-catch block                                                  //
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// copyright © Nishant Agrawal, 2018                                          //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //                                                                   //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio 2017                                //
// Platform:    Windows 7                                                      //
// Application: Spring Project,CSE 687,2018                                   //
// Author:      Nishant Agrawal, Syracuse University                           //
//              SUID: 595031520, nagraw01@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - DbProvider provides provides mechanism to share a static test database between test functions
* The package also provides test methods to be executed in try-catch blocks

Public Interface :
== == == == == == == == =
DbProvider Class > ---- >getter function for static db 

//Function Prologues
/* prologues written next to functions

Build commands
== == == ==  == == == == ==
- devenv NoSqlDb.sln

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* StringUtilities.h, StringUtilities.cpp
* Query.h, Query.cpp
* Persist.h, Persist.cpp
* TestUtilities.h, TestUtilities.cpp

* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
* - first release
*/

#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Persist/Persist.h"
#include <iostream>
#include <iomanip>
#include <functional>

namespace NoSqlDb
{


	//----< reduce the number of characters to type >----------------------

	auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
	{
		Utilities::putline(n, out);
	};

	///////////////////////////////////////////////////////////////////////
	// DbProvider class
	// - provides mechanism to share a test database between test functions
	//   without explicitly passing as a function argument.

	class DbProvider
	{
	public:
		DbCore<Payload>& db() { return db_; }
	private:
		static DbCore<Payload> db_;
	};

	DbCore<Payload> DbProvider::db_;
	///////////////////////////////////////////////////////////////////////
	// test functions

	//----< demo requirement #1 >------------------------------------------

	bool testR1()
	{
		Utilities::title("Demonstrating Requirement #1");
		std::cout << "\n  " << typeid(std::function<bool()>).name()
			<< ", declared in this function, "
			<< "\n  is only valid for C++11 and later versions.";
		putLine();
		return true; // would not compile unless C++11
	}

	//----< demo requirement #2 >------------------------------------------

	bool testR2()
	{
		Utilities::title("Demonstrating Requirement #2");
		std::cout << "\n  A visual examination of all the submitted code "
			<< "will show only\n  use of streams and operators new and delete.";
		putLine();
		return true;
	}

	//----< demo first part of requirement #3 >----------------------------

	bool testR3a()
	{
		Utilities::title("Demonstrating Requirement #3a - creating DbElement");
		std::cout << "\n  Creating a db element with key \"Fawcett\":";

		// create database to hold std::string payload

		DbCore<Payload> db;
		DbProvider dbp;
		dbp.db() = db;

		// create some demo elements and insert into db

		DbElement<Payload> demoElem;
		Payload demoLoad;
		demoLoad.FilePath("This is the file path...");
		std::vector<std::string> categ;
		categ.push_back("Cat 1");
		categ.push_back("Cat 2");

		demoLoad.Cat(categ);

		demoElem.name("Jim");
		demoElem.descrip("Instructor for CSE687");
		demoElem.dateTime(DateTime().now());
		demoElem.payLoad(demoLoad);

		if (demoElem.name() != "Jim")
			return false;
		if (demoElem.descrip() != "Instructor for CSE687")
			return false;
		if (demoElem.dateTime().now() != DateTime().now())
			return false;
		if (demoElem.payLoad().FilePath() != "This is the file path...")
			return false;

		showHeader();
		showElem(demoElem);

		db["Fawcett"] = demoElem;
		dbp.db() = db;
		putLine();
		return true;
	}
	
	//----< demo second part of requirement #3 >---------------------------
	bool testR3b()
	{
		Utilities::title("Demonstrating Requirement #3b - creating DbCore");
		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		DbElement<Payload> demoElem = db["Fawcett"];
		Payload demoLoad;
		demoLoad.FilePath("This is the file path1...");
		demoElem.name("Ammar");
		demoElem.descrip("TA for CSE687");
		demoElem.payLoad(demoLoad);
		db["Salman"] = demoElem;
		demoElem.name("Jianan");
		demoLoad.FilePath("This is the file path2...");
		demoElem.payLoad(demoLoad);
		db["Sun"] = demoElem;
		demoLoad.FilePath("This is the file path3...");
		demoElem.payLoad(demoLoad);
		demoElem.name("Nikhil");
		db["Prashar"] = demoElem;
		demoLoad.FilePath("This is the file path4...");
		demoElem.payLoad(demoLoad);
		demoElem.name("Pranjul");
		db["Arora"] = demoElem;
		demoLoad.FilePath("This is the file path5...");
		demoElem.payLoad(demoLoad);
		demoElem.name("Akash");
		db["Anjanappa"] = demoElem;
		if (db.size() != 6)
			return false;
		std::cout << "\n  after adding elements with keys: ";
		showKeys(db);
		putLine();
		std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
		db["Fawcett"].children().push_back("Salman");
		db["Fawcett"].children().push_back("Sun");
		db["Fawcett"].children().push_back("Prashar");
		db["Fawcett"].children().push_back("Arora");
		db["Fawcett"].children().push_back("Anjanappa");
		showHeader();
		showElem(db["Fawcett"]);
		putLine();
		std::cout << "\n  showing all the database elements:";
		showDb(db);
		putLine();
		dbp.db() = db;
		return true;
	}

	//----< demo of requirement #4 >---------------------------

	bool testR4()
	{
		Utilities::title("Demonstrating Requirement #4 - adding/deleting key value pairs");

		DbProvider dbp;
		DbCore<Payload> db = dbp.db();

		DbElement<Payload> demoElem = db["Fawcett"];
		Children clist;
		demoElem.name("Nishant");
		demoElem.descrip("student for CSE687");
		demoElem.children(clist);

		db["Nishant"] = demoElem;

		if (!db.contains("Nishant"))
			return false;

		if (db.size() != 7)
			return false;
		std::cout << "\n  Keys after adding new key/value pair: Nishant";
		DbCore<Payload>::Keys keys = db.keys();
		showKeys(db);
		putLine();

		db.removeItem("Salman");
		if (db.size() != 6)
			return false;

		std::cout << "\n  Keys after removing a key/value pair with key Salman: ";
		keys = db.keys();
		showKeys(db);
		putLine();
		std::cout << "\n  showing all the database elements: as Salman is also removed from children of other keys";
		showDb(db);
		putLine();
		dbp.db() = db;
		return true;
	}

	//----< demo second part of requirement #5 >---------------------------

	bool testR5()
	{
		Utilities::title("Demonstrating Requirement #5 - editing/deleting of relationships and text metadata.");
		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		DbElement<Payload> demoElem = db["Nishant"];
		Payload demoLoad;
		std::cout << "\n  Before editing payload and its categories for key: Nishant ";
		showHeader();
		showElem(db["Nishant"]);
		putLine();		
		std::cout << "\n  After editing metadata, payload and its categories for key: Nishant and adding 2 children";
		demoLoad.FilePath("This is the main file path....");
		std::vector<std::string> categ;
		categ.push_back("Cat 10");
		categ.push_back("Cat 11");
		demoLoad.Cat(categ);
		demoElem.name("Nash");
		demoElem.descrip("student in SU");
		demoElem.payLoad(demoLoad);
		demoElem.addChild("Sun");
		demoElem.addChild("Prashar");
		if (!db.updateItem("Nishant", demoElem))
			return false;

		showHeader();
		showElem(db["Nishant"]);
		putLine();
		demoElem.removeChild("Prashar");
		demoElem.addChild("Arora");
		demoElem.addChild("Anjanappa");
		if (!db.updateItem("Nishant", demoElem))
			return false;

		std::cout << "\n After editing/deleting the child relationships for key: Nishant";
		showHeader();
		showElem(db["Nishant"]);
		putLine();
		dbp.db() = db;
		return true;
	}

	//----< demo second part of requirement #6 >---------------------------

	bool testR6()
	{
		Utilities::title("Demonstrating Requirement #6- Queries functionalities");
		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		Query<Payload> query;
		std::cout << "\nQuery 1: to find an element using its key: Nishant";
		DbElement<Payload> demoElem = query.getElementByKey(db, "Nishant");
		showHeader();showElem(demoElem);putLine();
		std::cout << "\nQuery 2: to find children of an element using its key: Nishant";
		Children cList = query.getChildrenByKey(db, "Nishant");
		std::cout << "\n  Children: ";showKeys(cList);putLine();
		std::cout << "\nQuery 3: to find set of keys by matching with a given key pattern 'ra': ";
		showHeader(); query.getKeysByKeyPattern(db, ".*ra.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 4a: to find set of keys by matching with a given name pattern 'ar': ";
		showHeader();query.getKeysByNamePattern(db, ".*ar.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 4b: to find set of keys by matching with a given description pattern 'st': ";
		showHeader();query.getKeysByDescriptionPattern(db, ".*st.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 4c: to find set of keys by checking whether a key pattern exists as child keys for pattern 'ar': ";
		showHeader();query.getKeysByChildKeyPattern(db, ".*ar.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 4d: to find set of keys by matching with a given payload file path pattern 'ai': ";
		showHeader();query.getKeysByPayloadFPPattern(db, ".*ai.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 4e: to find set of keys by matching with a given payload categories pattern '11': ";
		showHeader();query.getKeysByPayloadCatPattern(db, ".*11.*");std::cout << "\n";putLine();
		std::cout << "\nQuery 5a: to find set of keys with timedates between 2 given dates: '2017-04-18  13:16:10' and '2017-06-18  13:16:10'";
		showHeader();query.getKeysByDateRange(db, "Thu Feb 1 13:16:10 2018", "Thu Feb 15 13:16:10 2018");std::cout << "\n";putLine();
		std::cout << "\nQuery 5b: to find set of keys with dates between a given begin date: '2017-02-18  13:16:10' and current date: ";
		showHeader();query.getKeysByDateRange(db, "Thu Feb 1 13:16:10 2018");std::cout << "\n";putLine();
		return true;
	}

	//----< demo second part of requirement #7 >---------------------------

	bool testR7()
	{
		Utilities::title("Demonstrating Requirement #7 - Complex 'and'ing and 'or'ing of queries");

		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		Query<Payload> query;

		std::cout << "\nGet 2 keysets by applying two queries and apply 'and' query on the returned keyset to get intersection: ";
		putLine();
		std::cout << "\n1st query: Find set of keys with a key pattern matching 'ra':\n";
		Keys keys = query.getKeysByKeyPattern(db, ".*ra.*");
		std::cout << "\n";
		putLine();

		std::cout << "\n2nd compounded query: Find children of 'Nishant' key in the given keyset:\n";
		Children children = query.getChildrenByKey(db, "Nishant");
		std::cout << "Children: ";
		showKeys(children);
		std::cout << "\n";

		std::cout << "\nIntersection of the queries:\n";
		Keys result = query.andQuery(db, keys, children);
		showKeys(result);
		std::cout << "\n";

		std::cout << "\nUnion of the queries:\n";
		result = query.orQuery(db, keys, children);
		showKeys(result);
		std::cout << "\n";
		return true;
	}

	//----< demo second part of requirement #8 >---------------------------

	bool testR8()
	{
		Utilities::title("Demonstrating Requirement #8 - Persist to and from an xml file:");

		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		PersistHelp<Payload> persist;
		persist.toXmlFile(db, "test.xml");

		std::cout << "\n Reading and displaying the created xml file: test.xml\n";
		std::ifstream iFile("test.xml");
		std::string xmlString((std::istreambuf_iterator<char>(iFile)),
			std::istreambuf_iterator<char>());
		iFile.close();
		std::cout << xmlString << std::endl;

		DbCore<Payload> newDb;
		persist.fromXmlFile(newDb, "test.xml");
		std::cout << "\n Reading and displaying the database created from xml file: test.xml\n";
		showHeader();
		showDb(newDb);

		putLine();
		return true;
	}

	//----< demo second part of requirement #9 >---------------------------

	bool testR9()
	{
		Utilities::title("Demonstrating Requirement #9 - a Payload class is implemented and used as data for npsqldb which has a string FilePath and a string vector categories:");

		DbProvider dbp;
		DbCore<Payload> db = dbp.db();
		showDb(db);
		putLine();
		return true;
	}

	//----< demo second part of requirement #10 >---------------------------

	bool testR10()
	{
		Utilities::title("Demonstrating Requirement #10 - All the required 4 packages are there in project apart from others: Executive, DBCore, Query, Test");
		putLine();
		return true;
	}

	//----< demo second part of requirement #11 >---------------------------

	bool testR11()
	{
		Utilities::title("Demonstrating Requirement #11 - Project package structure is in 'nishant.xml' and is loaded from as a db and displayed here.");

		DbCore<Payload> db;
		PersistHelp<Payload> persist;
		
		std::cout << "\n Reading and displaying the package structure xml file: nishant.xml\n";
		std::cout << "\n In this structure a package has all the packages it is dependent upon as its children. Eg. DbCore is dependent on StringUtilities and DateTime packages and in xml has them as its children. \n";

		std::ifstream iFile("nishant.xml");
		std::string xmlString((std::istreambuf_iterator<char>(iFile)),
			std::istreambuf_iterator<char>());
		iFile.close();
		std::cout << xmlString << std::endl;

		std::cout << "\n\n Loading the package structure xml and displaying on console as a db.\n";
		persist.fromXmlFile(db, "nishant.xml");

		showHeader();
		showDb(db);
		putLine();
		return true;
	}

	//----< demo second part of requirement #12 >---------------------------

	bool testR12()
	{
		Utilities::title("Demonstrating Requirement #12 - Test executive demonstrates that all the functional requirements have been met. ");

		putLine();
		return true;
	}

	//----< demo second part of requirement #13 >---------------------------

	bool testR13()
	{
		Utilities::title("Demonstrating Requirement #13 - Project folder includes a pdf with class diagram.");

		putLine();
		return true;
	}
}
