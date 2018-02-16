
/////////////////////////////////////////////////////////////////////////////////
// Test.cpp - Test stub for Test package where all the test methods are created//
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

#include "Test.h"

using namespace NoSqlDb;

// test stub for this package

int main()
{
	Utilities::Title("Testing No Sql Database");
	putLine();
	TestExecutive ex;
	// define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Requirement# 1" };
	TestExecutive::TestStr ts2{ testR2, "Requirement# 2" };
	TestExecutive::TestStr ts3a{ testR3a, "Requirement# 3a" };
	TestExecutive::TestStr ts3b{ testR3b, "Requirement# 3b" };
	TestExecutive::TestStr ts4{ testR4, "Requirement# 4" };
	TestExecutive::TestStr ts5{ testR5, "Requirement# 5" };
	TestExecutive::TestStr ts6{ testR6, "Requirement# 6" };
	TestExecutive::TestStr ts7{ testR7, "Requirement# 7" };
	TestExecutive::TestStr ts8{ testR8, "Requirement# 8" };
	TestExecutive::TestStr ts9{ testR9, "Requirement# 9" };
	TestExecutive::TestStr ts10{ testR10, "Requirement# 10" };
	TestExecutive::TestStr ts11{ testR11, "Requirement# 11" };
	TestExecutive::TestStr ts12{ testR12, "Requirement# 12" };
	TestExecutive::TestStr ts13{ testR13, "Requirement# 13" };
	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);
	ex.registerTest(ts12);
	ex.registerTest(ts13);
	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	putLine(2);
	return 0;
}