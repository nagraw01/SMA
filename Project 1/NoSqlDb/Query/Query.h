#pragma once
/////////////////////////////////////////////////////////////////////////////////
// Query.h - Class for running Queries on the database                         //
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// copyright © Nishant Agrawal, 2018                                          //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018          //                                                                   //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio 2017                                //
// Platform:    Windows 7                                                      //
// Application: Spring Project,CSE 687,2018                                    //
// Author:      Nishant Agrawal, Syracuse University                           //
//              SUID: 595031520, nagraw01@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////////
/*
//Package Operations
//== == == == == == ==

/*This Package mainly deals with the query mechanism and supports queries for the value of a specifies keys.
Queries to return the children of a specified key is done. Along with these the queries,the queries
to return the set of all keys matching a specified pattern is also done. The Queries that return the key in that
contain a specified string in their category name,item name,template data section is done. Along with these all the
queries that return a specified time date value is also included.This package structure also supports the union and
intersection of the keys that are present. This usually is very important and is demonstrated in this package.
These are the set of Queries that are being used here to determine the specific value which the user wants.
Using these Query mechanism it would become easy for the user to determine the specific value/data that he/she is
searching for.*/


//USE OF REGULAR EXPRESSION
/*To demonstrate the queries and hence to search the specific pattern in the database and then to retrieve it.
The package also makes use of the regular expressions. This is an extra feature that is being demonstrated in this
package.Regular expression is something that is being used to demonstrate the similarity and helps in retrieving
the data much easily.This feature is being implemented in this package.

//Module Operations :
//== == == == == == == == ==

//Function Prologues
/* prologues written next to function declarations

Public Interface :
== == == == == == == == =
Query Class > ---- >

Build Process :
== == == == == == ==
Required files
- DbCore.h, DbCore.cpp
- DateTime.h, DateTime.cpp
- StringUtilities.h, StringUtilities.cpp

Build commands
== == == ==  == == == == ==
- devenv NoSqlDb.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 :
- first release
-----------------------------
== == == == == == == == == == */

#include "../DbCore/DbCore.h"

#include <functional>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <regex>

using namespace NoSqlDb;

template<typename T>
class Query
{
public:

	bool hasKey(DbCore<T>& db, Key);  //function to check whether a key is there in the database
	DbElement<T> getElementByKey(DbCore<T>& , Key);//function to get element value by a key

	bool hasChildrenByKey(DbCore<T>&, Key);//function to check whether an item has children or not using key
	Children getChildrenByKey(DbCore<T>&, Key);//function to get children using a key

	Keys getKeysByKeyPattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a key pattern
	Keys getKeysByNamePattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a name pattern
	Keys getKeysByDescriptionPattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a description pattern
	Keys getKeysByChildKeyPattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a child key pattern
	Keys getKeysByPayloadFPPattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a payload filepath pattern
	Keys getKeysByPayloadCatPattern(DbCore<T>&, Key);//function to get keyset my regular expression matching on a payload categories pattern
	Keys getKeysByDateRange(DbCore<T>&, std::string, std::string);//function to get keyset with a timedate value between a date range
	Keys getKeysByDateRange(DbCore<T>&, std::string);//function to get keyset with a timedate value between a date and current date
	Keys andQuery(DbCore<T>& pDb_, Keys keySet1, Keys keySet2);//function to get the intersection of 2 keysets
	Keys orQuery(DbCore<T>& pDb_, Keys keySet1, Keys keySet2);//function to get the union of 2 keysets
};

//function to check whether a key is there in the database
template<typename T>
bool Query<T>::hasKey(DbCore<T>& pDb_, Key key) {
	Keys keys = pDb_.keys();
	for (Key k : keys) {
		if (k == key) {
			return true;
		}
	}
	return false;
}

//function to get element value by a key
template<typename T>
DbElement<T> Query<T>::getElementByKey(DbCore<T>& pDb_, Key key) {
	if (hasKey(pDb_, key)) {
		return pDb_.value(key);
	}
	else {
		std::cout << "No item with this key!!!" << std::endl;
		return DbElement<T>();
	}
}

//function to check whether an item has children or not using key
template<typename T>
bool Query<T>::hasChildrenByKey(DbCore<T>& pDb_, Key key) {
	Children childlist;
	if (hasKey(pDb_, key)) {

		DbElement<T> elem = pDb_.value(key);
		childlist = elem.children();
		if (!childlist.empty()) {
			return true;
		}
	}
	else {
		std::cout << "No item with this key!!!" << std::endl;
	}
	return false;
}

//function to get children using a key
template<typename T>
Children Query<T>::getChildrenByKey(DbCore<T>& pDb_, Key key) {

	Children childlist;
	if (hasChildrenByKey(pDb_, key)) {

		DbElement<T> elem = pDb_.value(key);
		childlist = elem.children();
	}
	else {
		std::cout << "This item has no children!!!" << std::endl;
	}
	return childlist;
}

//function to get keyset my regular expression matching on a key pattern
template<typename T>
Keys Query<T>::getKeysByKeyPattern(DbCore<T>& pDb_, Key keyPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regKey(keyPat);

	for (Key k : keys) {
		if (std::regex_match(k, regKey)) {
			keyList.push_back(k);

		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset my regular expression matching on a name pattern
template<typename T>
Keys Query<T>::getKeysByNamePattern(DbCore<T>& pDb_, Key namePat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regName(namePat);

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);

		if (std::regex_match(static_cast<std::string>(elem.name()), regName)) {
			keyList.push_back(k);

		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset my regular expression matching on a description pattern
template<typename T>
Keys Query<T>::getKeysByDescriptionPattern(DbCore<T>& pDb_, Key catPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regCat(catPat);

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		if (std::regex_match(static_cast<std::string>(elem.descrip()), regCat)) {
			keyList.push_back(k);

		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset my regular expression matching on a child key pattern
template<typename T>
Keys Query<T>::getKeysByChildKeyPattern(DbCore<T>& pDb_, Key dataPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regData(dataPat);

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		for (Key k1 : elem.children()) {
			if (std::regex_match(k1, regData)) {
				keyList.push_back(k);
				break;
			}
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset my regular expression matching on a payload filepath pattern
template<typename T>
Keys Query<T>::getKeysByPayloadFPPattern(DbCore<T>& pDb_, Key dataPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regData(dataPat);

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		if (std::regex_match(Convert<T>::toString(elem.payLoad()), regData)) {
			keyList.push_back(k);
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset my regular expression matching on a payload categories pattern
template<typename T>
Keys Query<T>::getKeysByPayloadCatPattern(DbCore<T>& pDb_, Key dataPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regData(dataPat);

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		if (std::regex_match(std::string(elem.payLoad()), regData)) {
			keyList.push_back(k);
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k)); 
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//template specialization for class Payload- function to get keyset my regular expression matching on a payload filepath pattern
template<>
Keys Query<Payload>::getKeysByPayloadFPPattern(DbCore<Payload>& pDb_, Key dataPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regData(dataPat);

	for (Key k : keys) {

		DbElement<Payload> elem = pDb_.value(k);
		if (std::regex_match(elem.payLoad().FilePath(), regData)) {
			keyList.push_back(k);
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//template specialization for class Payload-//function to get keyset my regular expression matching on a payload categories pattern
template<>
Keys Query<Payload>::getKeysByPayloadCatPattern(DbCore<Payload>& pDb_, Key dataPat) {

	Keys keyList;

	Keys keys = pDb_.keys();
	std::regex regData(dataPat);

	for (Key k : keys) {

		DbElement<Payload> elem = pDb_.value(k);
		for (Key k1 : elem.payLoad().Cat()) {
			if (std::regex_match(k1, regData)) {
				keyList.push_back(k);
				break;
			}
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given pattern: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset with a timedate value between a date range
template<typename T>
Keys Query<T>::getKeysByDateRange(DbCore<T>& pDb_, std::string bDate, std::string eDate) {

	DateTime begDate = DateTime(bDate);
	DateTime endDate = DateTime(eDate);

	Keys keyList;

	Keys keys = pDb_.keys();

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		DateTime date = elem.dateTime();

		if (date < endDate && date > begDate) {
			keyList.push_back(k);
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given date range: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get keyset with a timedate value between a date and current date
template<typename T>
Keys Query<T>::getKeysByDateRange(DbCore<T>& pDb_, std::string bDate) {
	
	DateTime begDate = DateTime(bDate);

	Keys keyList;

	Keys keys = pDb_.keys();

	for (Key k : keys) {

		DbElement<T> elem = pDb_.value(k);
		DateTime date = elem.dateTime();

		if (date < DateTime() && date > begDate) {
			keyList.push_back(k);
		}
	}

	if (keyList.size() > 0) {
		std::cout << "\n Keys found for the given date range: ";
		for (Key k : keyList) {
			std::cout << "\n";
			std::cout << k << ": ";
			showElem(pDb_.value(k));
		}
	}
	else {
		std::cout << "\n No matching keys found!!!";
	}

	return keyList;
}

//function to get the intersection of 2 keysets
template<typename T>
Keys Query<T>::andQuery(DbCore<T>& pDb_, Keys keySet1, Keys keySet2) {

	Keys resultSet;
	for (Key key1 : keySet1)
	{
		for (Key key2 : keySet2)
		{
			if (key1 == key2) {
				resultSet.push_back(key1);
				break;
			}
		}
	}
	return resultSet;
}

//function to get the union of 2 keysets
template<typename T>
Keys Query<T>::orQuery(DbCore<T>& pDb_, Keys keySet1, Keys keySet2) {

	int count = 0;
	int toggle = 0;
	for (Key key2 : keySet2)
	{
		toggle = 0;
		for (Key key1 : keySet1)
		{
			if (key2 == key1) {
				count++;
				toggle = 1;
				break;
			}
		}
		if (count != keySet1.size() && toggle != 1) {
			keySet1.push_back(key2);
		}
	}
	return keySet1;
}