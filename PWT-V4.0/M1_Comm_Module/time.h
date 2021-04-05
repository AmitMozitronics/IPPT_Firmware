/////////////////////////////////////////////////////////////////////////////
///                                                                       ///
///                               time.h                                  ///
///                                                                       ///
/// Time algorithms.  This follows the standard C API, with the following ///
/// exceptions:                                                           ///
///   * ctime() has another input parameter which is the pointer to where ///
///     the input string is.  Normally ctime() and asctime() write the    ///
///     output to a globally allocated string and return a pointer to     ///
///     this string.  This library doesn't want to make this assumption   ///
///     so the library doesn't allocate the space needed for those        ///
///     functions.                                                        ///
///   * asctime() has nother input parameter which is the pointer to      ///
///     where the input string is.  See the above paragraph.              ///
///   * strftime() is not supported.                                      ///
///   * SetTime() is added that initializes/set the current time.         ///
///   * GetTime() is added so you can read directly into a struct_tm,     ///
///      instead of having to use localtime(time()) which is usually      ///
///       innefecient.                                                    ///
///   * TimeInit() added for any real time clock devices that may need    ///
///      to be initialized first.                                         ///
///   * Not all timebases have a tick system, so it's not recommended     ///
///      to use clock().  If you need clock(), look at CCS's              ///
///      #use timer() library.                                            ///
///                                                                       ///
/// This file only provides the prototypes and definitions needed to      ///
/// proved a time alogrithm that follows the C standard library.  You     ///
/// also need to include/link the actual library that performs the time   ///
/// base.  As of this writing CCS provides the following compatible       ///
/// timebase libraries:                                                   ///
///      rtcperipheral.c - for PICs with internal real time clock.        ///
///      ds1305.c - external DS1305 real time clock.                      ///
///      rtcticks.c - Use a PIC's timer with CCS #use timer() library.    ///
///                                                                       ///
/// API:                                                                  ///
///                                                                       ///
/// Variable definitions:                                                 ///
///   c - clock timer (clock_t), number of ticks since powerup.  See      ///
///       CLOCKS_PER_SECOND to determine clock rate.                      ///
///                                                                       ///
///   t - second timer (time_t), number of seconds since Jan 1st, 1970.   ///
///                                                                       ///
///   ts - time struct (struct_tm), a structure that holds time in        ///
///        descriptive format (seconds, minutes, hours, day, month, etc). ///
///                                                                       ///
/// CLOCKS_PER_SECOND - This is a constant which needs to be defined that ///
///   configures the timebase used by the clock timer and clock().        ///
///   If you are not using clock() then you don't need to define this.    ///
///   If you are using a method such a PIC's timer for the timebase then  ///
///   you will need to set this.                                          ///
///                                                                       ///
/// c = clock() - Return current clock timer.                             ///
///                                                                       ///
/// t = time(*t) - Return current second timer.  Returns twice (as a      ///
///         a return, and saves to input pointer).                        ///
///                                                                       ///
/// SetTime(*tm) - Initializes the current time with a struct_tm          ///
///                                                                       ///
/// SetTimeSec(t) - Initializes the current time with a seconds time      ///
///                                                                       ///
/// t = mktime(*tm) - Converts a time struct to a second timer.           ///
///                                                                       ///
/// t = difftime(t,t) - Returns difference between two second timers.     ///
///                                                                       ///
/// *char = ctime(*t, *char) - Converts second timer to a readable string ///
///                            Www Mmm dd hh:mm:ss yyyy                   ///
///                                                                       ///
/// *char = asctime(*ts, *char) - Converts time struct to a readable      ///
///                            string.                                    ///
///                            Www Mmm dd hh:mm:ss yyyy                   ///
///                                                                       ///
/// *ts = localtime(*t) - Converts second timer to a time struct.         ///
///                  *ts points to a global time struct and will be       ///
///                  corrupted in future calls to localtime().            ///
///                                                                       ///
/// GetTime(*tm) - Returns the current time as a time struct              ///
///                                                                       ///
/// TimeInit() - Initializes the timing device                            ///
///                                                                       ///
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2011 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#ifndef __TIME_H__
#define __TIME_H__

#include <stdlibm.h>

/* API Types*/
typedef signed int32 time_t;
typedef unsigned int32 clock_t;

typedef enum
{
   SUNDAY = 0,
   MONDAY,
   TUESDAY,
   WEDNESDAY,
   THURSDAY,
   FRIDAY,
   SATURDAY   
}  Weekday;

typedef enum
{
   JANUARY = 0,
   FEBRUARY,
   MARCH,
   APRIL,
   MAY,
   JUNE,
   JULY,
   AUGUST,
   SEPTEMBER,
   OCTOBER,
   NOVEMBER,
   DECEMBER
}  Month;

typedef struct
{
   unsigned int8 tm_sec;   // seconds after the minute (0-59)
   unsigned int8 tm_min;   // minutes after the hour (0-59)
   unsigned int8 tm_hour;  // hours since midnight (0-23)
   unsigned int8 tm_mday;  // day of the month (0-30)
   Month tm_mon;           // month of the year (0-11)
   unsigned int16 tm_year; // years since 1900
   Weekday tm_wday;        // day of the week (0-6) (Sunday=0)
   unsigned int16 tm_yday; // day of the year (0-365)
} struct_tm;


/* Functions */
clock_t clock(void);
time_t time(time_t * timer);
signed int32 difftime(time_t later, time_t earlier);
time_t mktime(struct_tm * timeT);

char * asctime(struct_tm * timeptr, char *szTime);
char * ctime(time_t * timer, char *szTime);
struct_tm * localtime ( time_t * timer );
void SetTime(struct_tm * nTime);
void SetTimeSec(time_t sTime);
void GetTime(struct_tm *pRetTm);
void TimeInit(void);

////////////////////////////////////////////////////////////////////////////////
//                                          time.c                            //
////////////////////////////////////////////////////////////////////////////////

/* Returns the number of days in a given month, leap year dependent
 */
unsigned int8 DaysInMonth(unsigned int8 month, int1 IsLeapYear)
{
    unsigned int8 ui8ReturnValue = 0;
    switch(month)
    {
       case JANUARY:
       case MARCH:
       case MAY:
       case JULY:
       case AUGUST:
       case OCTOBER:
       case DECEMBER:
           ui8ReturnValue = 31;
           break;
       case FEBRUARY:
            if(IsLeapYear)
            {
                ui8ReturnValue = 29;
            }
            else
            {
                ui8ReturnValue = 28;
            }
            break;
       case APRIL:
       case JUNE:
       case SEPTEMBER:
       case NOVEMBER:
          ui8ReturnValue = 30;
          break;
       // error
       default:
           break;
    }
    
    return ui8ReturnValue;
} 

/* Formats a given week day (as an integer) to a string
 */
void WeekdayAbbreviations(unsigned int8 day, char* wString)
{
   switch(day)
   {
      case SUNDAY:
      memcpy(wString,"Sun\0",4);
      break;
      
      case MONDAY:
      memcpy(wString,"Mon\0",4);
      break;
      
      case TUESDAY:
      memcpy(wString,"Tue\0",4);
      break;
      
      case WEDNESDAY:
      memcpy(wString,"Wed\0",4);
      break;
      
      case THURSDAY:
      memcpy(wString,"Thu\0",4);
      break;
      
      case FRIDAY:
      memcpy(wString,"Fri\0",4);
      break;
      
      case SATURDAY:
      memcpy(wString,"Sat\0",4);
      break;
      
      default:
      memcpy(wString,"Invalid\0",8);
      break;
   }
}

/* Formats a given month (as an integer) to a string
 */
void MonthAbbreviations(unsigned int8 month, char* mString)
{
   switch(month)
   {
      case JANUARY:
      memcpy(mString,"Jan\0",4);
      break;
      
      case FEBRUARY:
      memcpy(mString,"Feb\0",4);
      break;
      
      case MARCH:
      memcpy(mString,"Mar\0",4);
      break;
      
      case APRIL:
      memcpy(mString,"Apr\0",4);
      break;
      
      case MAY:
      memcpy(mString,"May\0",4);
      break;
      
      case JUNE:
      memcpy(mString,"Jun\0",4);
      break;
      
      case JULY:
      memcpy(mString,"Jul\0",4);
      break;
      
      case AUGUST:
      memcpy(mString,"Aug\0",4);
      break;
      
      case SEPTEMBER:
      memcpy(mString,"Sep\0",4);
      break;
      
      case OCTOBER:
      memcpy(mString,"Oct\0",4);
      break;
      
      case NOVEMBER:
      memcpy(mString,"Nov\0",4);
      break;
      
      case DECEMBER:
      memcpy(mString,"Dec\0",4);
      break;
      
      default:
      memcpy(mString,"Invalid\0",8);
      break;
   }
}
/* Determines if a year is a leap year
 * 'year' is the number of years since 1900
 * A year is a leap year if is evenly divisible by 4 while not divisible by 100
 * or if it is evenly divisible by 400. (Ex. 2000 is a leap year, 2100 is not)
 * Returns TRUE if the year is a leap year, FALSE if not
 */
int1 LeapYear(unsigned int16 year)
{
    if( ((year + 1900) % 400 == 0) || 
        (((year + 1900) % 4 == 0) && ((year + 1900) % 100 != 0)) )
    {
       return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Returns the difference in seconds between two times
 * Times later and earlier are expressed in seconds 
 */
signed int32 difftime(time_t later, time_t earlier)
{
   return (later - earlier);
}

/* Calculates the Unix Time from a standard time format
 * Returns the Unix Time (time in seconds since Jan 1, 1970 00:00:00)
 */
time_t mktime(struct_tm * timeT)
{
   time_t unixTime = 0;
   int1 isLeapYear = FALSE;
   unsigned int16 i = 0;
   
   if(timeT != NULL)
   {
   
      unixTime += timeT->tm_sec;
      unixTime += (unsigned int32)(timeT->tm_min) * 60;
      unixTime += (unsigned int32)(timeT->tm_hour) * 3600;
      
      isLeapYear = LeapYear(timeT->tm_year);
       /* Clamp the month to [0,11) */
      timeT->tm_mon %= 12;
      for(i = 1;i <= timeT->tm_mon;i++)
      {
         unixTime += (DaysInMonth(i - 1,isLeapYear) * 86400);
      }
      
      /* Clamp the days in the month */
      timeT->tm_mday %= DaysInMonth(timeT->tm_mon,isLeapYear);
      unixTime += (timeT->tm_mday) * 86400;
      
      if(isLeapYear)
      {
         timeT->tm_yday = (unixTime / 86400) % 366;
      }
      else
      {
         timeT->tm_yday = (unixTime / 86400) % 365;
      }
      
      i = 1970;  // Change done by Amit, initially it was i = 70;
      if(timeT->tm_year - 1970 >= 0)
      {
         while(i < (timeT->tm_year))
         {
            isLeapYear = LeapYear(i);
            if(isLeapYear)
            {
               unixTime += (31622400); // seconds in 366 days
            }
            else
            {
               unixTime += (31536000); // seconds in 365 days
            }
            i++;
         }
      }
      // To match GMT to IST we have to adjust 5hrs 30 mints..
      unixTime = unixTime - 19800;  // 19800 is the 
      timeT->tm_wday = ((unixTime / 86400) + 4) % 7;
   }

   return unixTime;
}

/* Returns the given time as a string of the form: 
 *  Day Mon X HH:MM:SS YYYY\0 
 */
char * asctime ( struct_tm * timeptr, char * szTime)
{
   char szDay[8];
   char szMon[8];
   
   WeekdayAbbreviations(timeptr->tm_wday, szDay);
   MonthAbbreviations(timeptr->tm_mon, szMon);
   
   sprintf(szTime,"%s %s %d %02d:%02d:%02d %04Lu",
      szDay,
      szMon,
      timeptr->tm_mday + 1,
      timeptr->tm_hour,
      timeptr->tm_min,
      timeptr->tm_sec,
      (timeptr->tm_year + 1900));
      
   return szTime;
}

/* Converts the given calendar time (in seconds) to local time 
 * and returns the equivalent string. 
 */
char * ctime ( time_t * timer, char *szTime )
{
   return (asctime(localtime(timer),szTime));
}

/* Global local time variable */
struct_tm g_lTime;

/* Converts the given calendar time (in seconds) to local time
 * and sets this time in the global g_lTime
 * Returns a pointer to g_lTime
 */
struct_tm * localtime(time_t * timer)
{
   time_t timeCounter;
   int1 done = FALSE;
   int1 isLeapYear = FALSE;//1970 is not a leap year

   if(timer != NULL)
   {
      timeCounter = *timer;
      g_lTime.tm_wday = ((timeCounter / 86400) + 4) % 7;//fill in the weekday
      g_lTime.tm_year = 70;//we are starting in 1970
      
      while(!done)
      {
         if(timeCounter < (31622400) && isLeapYear) // seconds in 366 days
         {
            g_lTime.tm_yday = (timeCounter / 86400);
            break;
         }
         else if(timeCounter < (31536000)) // seconds in 365 days
         {
            g_lTime.tm_yday = (timeCounter / 86400);
            break;
         }
         
         if(isLeapYear)
         {
            timeCounter -= 31622400; // seconds in 366 days
         }
         else
         {
            timeCounter -= 31536000; // seconds in 365 days
         }
         
         g_lTime.tm_year++;
         isLeapYear = LeapYear(g_lTime.tm_year);
      }
      
      g_lTime.tm_mon = 0;
      while(!done)
      {         
         if(timeCounter < DaysInMonth(g_lTime.tm_mon,isLeapYear) * 86400)
         {
            break;
         }
         else if(timeCounter >= DaysInMonth(g_lTime.tm_mon,isLeapYear) * 86400)
         {
            timeCounter -= DaysInMonth(g_lTime.tm_mon,isLeapYear) * 86400;
            g_lTime.tm_mon++;
         }
      }  

      g_lTime.tm_mday = (timeCounter / (86400));
      timeCounter -= (g_lTime.tm_mday * (86400));
      
      g_lTime.tm_hour = (timeCounter / (3600));
      timeCounter -= ((unsigned int32)g_lTime.tm_hour) * 3600;
      
      g_lTime.tm_min = (timeCounter / 60);
      timeCounter -= (((unsigned int16)g_lTime.tm_min) * 60);
     
      g_lTime.tm_sec = timeCounter;
   }
   
   return &g_lTime;
}
#endif
