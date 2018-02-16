/////////////////////////////////////////////////////////////////////////////////
// Query.cpp - Class for running Queries on the database, test stub included   //
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// copyright © Nishant Agrawal, 2018                                           //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //                                                                   //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio 2017                                 //
// Platform:    Windows 7                                                      //
// Application: Spring Project,CSE 687,2018                                    //
// Author:      Nishant Agrawal, Syracuse University                           //
//              SUID: 595031520, nagraw01@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////////

#include "Query.h"

using namespace NoSqlDb;

// test stub for Query class
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
	Query<Payload> query;
	std::cout << "\nQuery 1: to find an element using its key: Nishant";
	DbElement<Payload> demoElem = query.getElementByKey(db, "Nishant");
	showHeader(); showElem(demoElem);
	std::cout << "\nQuery 2: to find children of an element using its key: Nishant";
	Children cList = query.getChildrenByKey(db, "Nishant");
	std::cout << "\n  Children: "; showKeys(cList);
	std::cout << "\nQuery 3: to find set of keys by matching with a given key pattern 'ra': ";
	showHeader(); query.getKeysByKeyPattern(db, ".*ra.*"); std::cout << "\n";
	std::cout << "\nQuery 4a: to find set of keys by matching with a given name pattern 'ar': ";
	showHeader(); query.getKeysByNamePattern(db, ".*ar.*"); std::cout << "\n";
	std::cout << "\nQuery 4b: to find set of keys by matching with a given description pattern 'st': ";
	showHeader(); query.getKeysByDescriptionPattern(db, ".*st.*"); std::cout << "\n";
	std::cout << "\nQuery 4c: to find set of keys by checking whether a key pattern exists as child keys for pattern 'ar': ";
	showHeader(); query.getKeysByChildKeyPattern(db, ".*ar.*"); std::cout << "\n"; 
	std::cout << "\nQuery 4d: to find set of keys by matching with a given payload file path pattern 'ai': ";
	showHeader(); query.getKeysByPayloadFPPattern(db, ".*ai.*"); std::cout << "\n";
	std::cout << "\nQuery 4e: to find set of keys by matching with a given payload categories pattern '11': ";
	showHeader(); query.getKeysByPayloadCatPattern(db, ".*11.*"); std::cout << "\n";
	std::cout << "\nQuery 5a: to find set of keys with timedates between 2 given dates: '2017-04-18  13:16:10' and '2017-06-18  13:16:10'";
	showHeader(); query.getKeysByDateRange(db, "Thu Feb 1 13:16:10 2018", "Thu Feb 15 13:16:10 2018"); std::cout << "\n";
	std::cout << "\nQuery 5b: to find set of keys with dates between a given begin date: '2017-02-18  13:16:10' and current date: ";
	showHeader(); query.getKeysByDateRange(db, "Thu Feb 1 13:16:10 2018"); std::cout << "\n";
}
