
/////////////////////////////////////////////////////////////////////////////////
// Persist.cpp - persist database contents to and from an XML file, test stub included//
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// copyright © Nishant Agrawal, 2018                                           //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018          //                                                                   //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio 2017                                //
// Platform:    Windows 7                                                      //
// Application: Spring Project,CSE 687,2018                                    //
// Author:      Nishant Agrawal, Syracuse University                           //
//              SUID: 595031520, nagraw01@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////////


#include "Persist.h"

using namespace NoSqlDb;

// test stub for Persist class
int main()
{
	Utilities::Title("Demonstrating NoSql Queries");
	DbCore<Payload> db;
	DbElement<Payload> elem1;
	elem1.name("Jake");
	elem1.descrip("test");
	elem1.dateTime(DateTime().now());
	db.saveItem(elem1.name(), elem1);
	elem1.name("Trip");
	db.saveItem(elem1.name(), elem1);
	elem1.name("Nishant");
	elem1.addChild("Jake");
	elem1.addChild("Trip");
	db.saveItem(elem1.name(), elem1);
	
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
	return 0;
}
