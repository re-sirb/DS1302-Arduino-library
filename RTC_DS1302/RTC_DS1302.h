/*
* Real-time clock, DS1302; setArduino-Pins at init.
* Trickle-Charge is by default deactivated.
* CLK, DAT, CE
*/

#ifndef RTC_DS1302_h
#define RTC_DS1302_h

//#include "Arduino.h"

class RTC_DS1302
{
  public:
		RTC_DS1302(int t_CLK, int t_DAT, int t_CE);
		
		//Set time
		void set_seconds(int data);
		void set_minutes(int data);
		void set_hours(int data);
		
		//Set date
		void set_weekday(int data);
		void set_day(int data);
		void set_month(int data);
		void set_year(int data);
		
		//Get timestamp
		void get_timestamp();
		
		//Get time
		int get_seconds();
		int get_minutes();
		int get_hours();
		
		//Get date
		int get_weekday();
		int get_day();
		int get_month();
		int get_year();
		
	private:
		// Data I/O
		unsigned char read_byte(unsigned char addr);
		void write_byte(unsigned char addr, unsigned char val);
		
		//conversion
		unsigned char extract_one_digit(int dat);
		unsigned char extract_ten_digit(int dat);
		
		//var
		int CLK;
		int DAT;
		int CE;
		
		int t_second=0;
		int t_minute=0;
		int t_hour=0;
		
		int d_weekday=0;
		int d_day=0;
		int d_month=0;
		int d_year=0;
		
};

#endif
