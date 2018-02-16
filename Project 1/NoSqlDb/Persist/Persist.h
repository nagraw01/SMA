#pragma once
/////////////////////////////////////////////////////////////////////////////////
// PersistHelp.h - persist database contents to and from an XML file           //
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// copyright © Nishant Agrawal, 2017                                           //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //                                                                   //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio 2015                                 //
// Platform:    Windows 7                                                      //
// Application: Spring Project,CSE 687,2017                                    //
// Author:      Nishant Agrawal, Syracuse University                           //
//              SUID: 595031520, nagraw01@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////////
/*
//Package Operations
//== == == == == == ==

/*This Package demonstrates the parsing of an XML File that is being used here.This package mainly contains of toXml
functions and fromXml functions that are being used to retrieve the contents from the database using the fromXML function
and displays it on the screen. The package also makes use of the toXml function that is being used to demonstrate adding
of all the contents into the database.This package uses two databases in which the contents that is to be added are put
into the database and from this database it is given to the xml file to write it.Once it is loaded into the xml, fromXml
function is called to retrive the contents of the Xml file and then store it in the newdatabase.The main operations in
this package structure is toXml() and fromXml().*/


//Function Prologues
/* prologues written next to function declarations

Public Interface :
== == == == == == == == =
toXmlFile() > ---- >
fromXmlFile()

Build Process :
== == == == == == ==
Required files
- DbCore.h, DbCore.cpp
- XmlDocument.h, XmlDocument.cpp
- XmlElement.h, XmlElement.cpp
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
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../DbCore/DbCore.h"

namespace NoSqlDb
{

	template<typename T>
	class PersistHelp
	{

	public:
		using SPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
		using SPtrs = std::vector<SPtr>;
		using Data = DbElement<T>;

		void toXmlFile(DbCore<T>& db_, const std::string&);    // sets nosql db contents to xml file
		void fromXmlFile(DbCore<T>& db_, const std::string&); //restores db contents from an xml file


	private:

		std::string getTextElement(SPtr sPtr);  //helper function to access children of tag elements
		std::string removeSpaces(std::string);  //helper function to remove extra spaces from text elements
	};

	// sets nosql db contents to xml file
	template<typename T>
	void PersistHelp<T>::toXmlFile(DbCore<T>& db_, const std::string& fileName)
	{
		std::ofstream oFile(fileName);
		SPtr pRoot = XmlProcessing::makeTaggedElement("NoSqlDb");
		XmlProcessing::XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
		Keys keys = db_.keys();
		for (Key k : keys)
		{
			SPtr pItem = XmlProcessing::makeTaggedElement("Item");
			SPtr pItemKey = XmlProcessing::makeTaggedElement("Key");
			SPtr pTextKey = XmlProcessing::makeTextElement(k);
			pItemKey->addChild(pTextKey);
			pItem->addChild(pItemKey);
			DbElement<T> e = db_.value(k);
			SPtr pName = XmlProcessing::makeTaggedElement("Name");
			SPtr pTextName = XmlProcessing::makeTextElement(e.name());
			pName->addChild(pTextName);
			pItem->addChild(pName);
			SPtr pDescription = XmlProcessing::makeTaggedElement("Description");
			SPtr pTextDesc = XmlProcessing::makeTextElement(e.descrip());
			pDescription->addChild(pTextDesc);
			pItem->addChild(pDescription);
			SPtr pTimeDate = XmlProcessing::makeTaggedElement("TimeDate");
			SPtr pTextTD = XmlProcessing::makeTextElement(e.dateTime());
			pTimeDate->addChild(pTextTD);
			pItem->addChild(pTimeDate);
			SPtr pChildren = XmlProcessing::makeTaggedElement("Children");
			Keys childrenArray = e.children();
			for (Key child : childrenArray)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("Child");
				SPtr pTextChild = XmlProcessing::makeTextElement(child);
				pChild->addChild(pTextChild);
				pChildren->addChild(pChild);
			}
			pItem->addChild(pChildren);
			Data data = e.payLoad();
			SPtr pData = XmlProcessing::makeTaggedElement("Payload");
			SPtr pTextData = XmlProcessing::makeTextElement(std::string(data));
			pData->addChild(pTextData);
			pItem->addChild(pData);
			pRoot->addChild(pItem);
		}
		oFile << doc.toString();
		oFile.close();
	}


	//restores db contents from an xml file
	template<typename T>
	void PersistHelp<T>::fromXmlFile(DbCore<T>& db_, const std::string& fileName)
	{
		std::ifstream iFile(fileName);
		std::string xmlString((std::istreambuf_iterator<char>(iFile)),
			std::istreambuf_iterator<char>());
		iFile.close();
		XmlProcessing::XmlDocument doc(xmlString, XmlProcessing::XmlDocument::str);
		SPtrs pAllItems = doc.descendents("Item").select();
		for (SPtr pItem : pAllItems)
		{
			XmlProcessing::XmlDocument doc1(XmlProcessing::makeDocElement(pItem));
			Key k = getTextElement(pItem->children()[0]);
			std::string n = getTextElement(pItem->children()[1]);
			DateTime t = DateTime(getTextElement(pItem->children()[3]));
			std::string d = getTextElement(pItem->children()[2]);
			Children children;
			SPtrs pChildren = pItem->children()[4]->children();
			for (SPtr pChild : pChildren)
			{
				children.push_back(removeSpaces(pChild->children()[0]->value()));
			}
			T t = T(getTextElement(pItem->children()[5]));
			DbElement<T> elem;
			elem.name(n);
			elem.dateTime(t);
			elem.descrip(d);
			elem.children(children);
			elem.payLoad(t);
			bool flag = db_.saveItem(k, elem);
			if (!flag)
				db_.updateItem(k, elem);
		}
	}
	
	//helper function to access children of tag elements
	template<typename T>
	std::string PersistHelp<T>::getTextElement(SPtr sPtr)
	{
		if (sPtr->children().size() == 0)
			return "";
		return sPtr->children()[0]->value();
	}

	//helper function to remove extra spaces from text elements
	template<typename T>
	std::string PersistHelp<T>::removeSpaces(std::string str)
	{

		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return str;
	}

	// sets nosql db contents to xml file--template specialization for payload class
	template<>
	void PersistHelp<Payload>::toXmlFile(DbCore<Payload>& db_, const std::string& fileName)
	{
		std::ofstream oFile(fileName);
		SPtr pRoot = XmlProcessing::makeTaggedElement("NoSqlDb");
		XmlProcessing::XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
		Keys keys = db_.keys();
		for (Key k : keys)
		{
			SPtr pItem = XmlProcessing::makeTaggedElement("Item"), pItemKey = XmlProcessing::makeTaggedElement("Key"), pTextKey = XmlProcessing::makeTextElement(k);
			pItemKey->addChild(pTextKey);
			pItem->addChild(pItemKey);
			DbElement<Payload> e = db_.value(k);
			SPtr pName = XmlProcessing::makeTaggedElement("Name"), pTextName = XmlProcessing::makeTextElement(e.name());
			pName->addChild(pTextName);
			pItem->addChild(pName);
			SPtr pDescription = XmlProcessing::makeTaggedElement("Description"), pTextDesc = XmlProcessing::makeTextElement(e.descrip());
			pDescription->addChild(pTextDesc);
			pItem->addChild(pDescription);
			SPtr pTimeDate = XmlProcessing::makeTaggedElement("TimeDate"), pTextTD = XmlProcessing::makeTextElement(e.dateTime());
			pTimeDate->addChild(pTextTD);
			pItem->addChild(pTimeDate);
			SPtr pChildren = XmlProcessing::makeTaggedElement("Children");
			Keys childrenArray = e.children();
			for (Key child : childrenArray)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("Child"), pTextChild = XmlProcessing::makeTextElement(child);
				pChild->addChild(pTextChild);
				pChildren->addChild(pChild);
			}
			pItem->addChild(pChildren);
			Payload data = e.payLoad();
			SPtr pData = XmlProcessing::makeTaggedElement("Payload"), pFilePath = XmlProcessing::makeTaggedElement("File Path"), pTextData = XmlProcessing::makeTextElement(data.FilePath());
			pFilePath->addChild(pTextData);
			pData->addChild(pFilePath);
			SPtr catData = XmlProcessing::makeTaggedElement("Categories");
			for (Key cat : data.Cat())
			{
				SPtr catChild = XmlProcessing::makeTaggedElement("Category"), cChild = XmlProcessing::makeTextElement(cat);
				catChild->addChild(cChild);
				catData->addChild(catChild);
			}
			pData->addChild(catData);pItem->addChild(pData);pRoot->addChild(pItem);
		}
		oFile << doc.toString();
		oFile.close();
	}


	// sets nosql db contents from xml file--template specialization for payload class
	template<>
	void PersistHelp<Payload>::fromXmlFile(DbCore<Payload>& db_, const std::string& fileName)
	{
		std::ifstream iFile(fileName);
		std::string xmlString((std::istreambuf_iterator<char>(iFile)),
			std::istreambuf_iterator<char>());
		iFile.close();
		XmlProcessing::XmlDocument doc(xmlString, XmlProcessing::XmlDocument::str);
		SPtrs pAllItems = doc.descendents("Item").select();
		for (SPtr pItem : pAllItems)
		{
			XmlProcessing::XmlDocument doc1(XmlProcessing::makeDocElement(pItem));
			Key k = getTextElement(pItem->children()[0]);
			std::string n = getTextElement(pItem->children()[1]);
			DateTime t;
			if (getTextElement(pItem->children()[3]) != "")
			   t = DateTime(getTextElement(pItem->children()[3]));
			std::string d = getTextElement(pItem->children()[2]);
			Children children;
			SPtrs pChildren = pItem->children()[4]->children();
			for (SPtr pChild : pChildren)
			{
				children.push_back(pChild->children()[0]->value());
			}
			std::string fileP = getTextElement(pItem->children()[5]->children()[0]);
			Children categ;
			SPtrs pCateg = pItem->children()[5]->children()[1]->children();
			for (SPtr pC : pCateg)
			{
				categ.push_back(pC->children()[0]->value());
			}
			DbElement<Payload> elem;
			elem.name(n);
			elem.dateTime(t);
			elem.descrip(d);
			elem.children(children);
			elem.payLoad().FilePath(fileP);
			elem.payLoad().Cat(categ);
			bool flag = db_.saveItem(k, elem);
			if (!flag)
				db_.updateItem(k, elem);
		}
	}
}
