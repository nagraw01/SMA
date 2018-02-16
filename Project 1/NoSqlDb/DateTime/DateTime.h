#pragma once
/////////////////////////////////////////////////////////////////////
// DateTime.h - represents clock time                              //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The DateTime class represents clock time, and supports:
 * - creating default instances and instances from specific time points
 * - return times as formatted strings
 * - building time points and durations from years, months, days, hours, ...
 * - performing addition and subtraction of times
 * - comparing times
 * - extracting counts of years, months, days, hours, minutes, and seconds
 *
 * Function Prologues:
 * prologues written next to function declarations
 *
 * Public Interface :
 * == == == == == == == == =
 * DateTime Class > ---- >
 *
Build commands
== == == ==  == == == == ==
- devenv NoSqlDb.sln

 * Required Files:
 * ---------------
 *   DateTime.h, DateTime.cpp
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 18 Feb 2017
*/

#include <chrono>
#include <ctime>
#include <string>
#include "../Utilities/StringUtilities/StringUtilities.h"

class DateTime
{
public:
  using SysClock = std::chrono::system_clock;
  using TimePoint = std::chrono::system_clock::time_point;
  using Duration = std::chrono::system_clock::duration;

  DateTime();                   //----< construct DateTime instance with current system time
  DateTime(std::string dtStr);     //----< makes a DateTime instance from a formatted string
  DateTime(const TimePoint& tp);     //----< construct DateTime instance from time point
  operator std::string();        //----< cast operator converts to time formatted string

  std::string now();         //----< set time to system time
  TimePoint timepoint();    //----< return internal time point
  size_t ticks();            //----< return seconds from Jan 1 1990 at midnight
  std::string time();         //----< return formatted time string
  static TimePoint makeTime(          //----< make a time from integral parts 
    size_t yrs, size_t mon, size_t day, 
    size_t hrs = 0, size_t min = 0, size_t sec = 0
  );
  static Duration makeDuration(              //----< make duration from integral parts
    size_t hrs, size_t min, size_t sec = 0, size_t millisec = 0
  );

  DateTime operator+=(const Duration& dt);   //----< add a duration to DateTime instance
  DateTime operator+(const Duration& dt);     //----< add a duration to DateTime instance
  DateTime operator-=(const Duration& dt);//----< subtract a duration from instance
  DateTime operator-(const Duration& dt);//---- make DateTime from instance time minus duration
  bool operator<(const DateTime& dt);        //----< compare DateTime instances
  bool operator>(const DateTime& dt);       //----< compare DateTime instances

  size_t year();//----< return year count
  size_t month();//----< return month count
  size_t day();//----< return day count
  size_t hour();//----< return hour count
  size_t minute();//----< return minutes count
  size_t second();//----< return seconds count
private:
  char* ctime(const std::time_t* pTime);    //----< replaces std::ctime using ctime_s
  std::tm* localtime(const time_t* pTime);   //----< replaces std::localtime using localtime_s
  TimePoint tp_;
};