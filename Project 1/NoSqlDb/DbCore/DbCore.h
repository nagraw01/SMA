#pragma once
/////////////////////////////////////////////////////////////////////////////////
// DcCore.cpp - Implementats the                                             //
//              NoSql database                                                 //
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
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
* - saving to db
* - updating a key/value pair
* - editing/updating/deleting metadata, relationships and payload data
* - Payload provides the payload or data required for the db
*   It contains fields for file-path and categories.

Public Interface :
== == == == == == == == =
DbCore Class > ---- >db getter, setter, access and modify methods
DbElement Class > ---- >getter and setter, add remove child
Payload Class > ---- >getter and setter

//Function Prologues
/* prologues written next to function declarations

Build commands
== == == ==  == == == == ==
- devenv NoSqlDb.sln

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* StringUtilities.h, StringUtilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

namespace NoSqlDb
{
	using Key = std::string;
	using Children = std::vector<Key>;
	using Keys = std::vector<Key>;

	///////////////////////////////////////////////////////////////////////
	// Payload class
	// - provides the payload or data required for the db

	class Payload
	{

	public:
		// methods to get and set Payload fields

		std::string& FilePath() { return FilePath_; }
		std::string FilePath() const { return FilePath_; }
		void FilePath(const std::string& name) { FilePath_ = name; }

		std::vector<std::string>& Cat() { return Categories_; }
		std::vector<std::string> Cat() const { return Categories_; }
		void Cat(const std::vector<std::string>& cat) { Categories_ = cat; }

	private:
		std::string FilePath_;
		std::vector<std::string> Categories_;
	};

  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:


    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	void addChild(Key);  //function to add child relationship
	void removeChild(Key);  //function to remove child relationship


  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    T payLoad_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbElement<T> methods

  //function to add child relationship
  template<typename T>
  void DbElement<T>::addChild(Key child)
  {
	  children_.push_back(child);
  }
  // function to remove child relationship
  template<typename T>
  void DbElement<T>::removeChild(Key child)
  {
	  Children::iterator result = find(children_.begin(), children_.end(), child);
	  if (result == children_.end())
		  std::cout << "This is not a child of this db element!" << std::endl;
	  else
		  children_.erase(result);
  }

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();     //returns list of db element keys
    bool contains(const Key& key); //returns whether db has a key
    size_t size(); //returns size of db
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }//throws exception
    DbElement<T>& operator[](const Key& key);//access db element using key
    DbElement<T> operator[](const Key& key) const;//access db element using key
    typename iterator begin() { return dbStore_.begin(); }//helper method to iterate on the unordered map using key
    typename iterator end() { return dbStore_.end(); }//helper method to iterate on the unordered map using key

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	//NA
	bool saveItem(Key key, DbElement<T>); // function to save an item to database
	bool updateItem(Key key, DbElement<T>); //function to update an item's details in the database
	bool removeItem(Key key);  //// function to remove an item from the database

	DbElement<T> value(Key key); //function to get the value from the keyof a key value pair

  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<T>());
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }

  //----< display database key set >-----------------------------------

  void showKeys(Keys keys, std::ostream& out = std::cout)
  {
	  out << "\n  ";
	  for (auto key : keys)
	  {
		  out << key << " ";
	  }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-----------------------";
  }
  //----< display a DbElement >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el)
  {
	 std::cout << "\n  ";
	 std::cout << std::setw(26) << std::left << std::string(el.dateTime());
	 std::cout << std::setw(10) << std::left << el.name();
	 std::cout << std::setw(25) << std::left << el.descrip();

    Children children = el.children();
    if (children.size() > 0)
    {
		std::cout << "\n    child keys: ";
      for (auto key : children)
      {
		  std::cout << " " << key;
      }
    }
	std::cout << std::setw(25) << std::left << std::string(elem.payLoad());
  }

  //----< display a DbElement specialization for payload class >-----------------------------------------

  template<>
  void showElem(const DbElement<Payload>& el)
  {
	  std::cout << "\n\n\n  ";
	  std::cout << std::setw(26) << std::left << std::string(el.dateTime());
	  std::cout << std::setw(10) << std::left << el.name();
	  std::cout << std::setw(25) << std::left << el.descrip();
	  std::cout << std::setw(25) << std::left << el.payLoad().FilePath();
	  Children children = el.children();
	  if (children.size() > 0)
	  {
		  std::cout << "\n\n    child keys: ";
		  for (auto key : children)
		  {
			  std::cout << " " << key;
		  }
	  }
	  Children categ = el.payLoad().Cat();
	  if (categ.size() > 0)
	  {
		  std::cout << "\n\n    Payload Categories: ";
	  	for (auto key : categ)
	  	{
			std::cout << " " << key;
	  	}
	  }
  }

  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second);
    }
  }

  //----< to save a new record in database >----------------------------
  template<typename T>
  bool DbCore<T>::saveItem(Key key, DbElement<T> elem)
  {
	  if (dbStore_.find(key) != dbStore_.end())
		  return false;
	  dbStore_[key] = elem;
	  return true;
  }

  //----< to update/edit an existing record in database >----------------------------
  template<typename T>
  bool DbCore<T>::updateItem(Key key, DbElement<T> elem) {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  dbStore_[key] = elem;
	  return true;
  }
  
  //----< to remove a record from the database >----------------------------
  template<typename T>
  bool DbCore<T>::removeItem(Key key)
  {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  dbStore_.erase(key);

	  for (Key k : keys())
	  {

		  Children cList = dbStore_[k].children();
		  Children::iterator result = find(cList.begin(), cList.end(), key);
		  if (result != cList.end())
			  cList.erase(result);

		  dbStore_[k].children(cList);
	  }

	  return true;
  }

  //----< function that returns the element mapped to the provided key from the database >----------------------------
  template<typename T>
  DbElement<T> DbCore<T>::value(Key key)
  {

	  if (dbStore_.find(key) != dbStore_.end())
		  return dbStore_[key];
	  return DbElement<T>();
  }

}
