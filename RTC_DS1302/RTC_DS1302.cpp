/*
* Real-time clock, DS1302; setArduino-Pins at init.
* Trickle-Charge is by default deactivated.
* CLK, DAT, CE
*/

#include "Arduino.h"
#include "RTC_DS1302.h"

RTC_DS1302::RTC_DS1302(int t_CLK, int t_DAT, int t_CE){
	//init Pins
	CLK = t_CLK;
	DAT = t_DAT;
	CE = t_CE;
	
	pinMode(CLK, OUTPUT);
	pinMode(CE, OUTPUT);
	pinMode(DAT, INPUT);
	
	delayMicroseconds(20);
	
	//init values
	
	  //write protect off
	write_byte(0x8E,0b00000000);
	// trickle charge off
	write_byte(0x90,0b00000000);
	
	write_byte(0x80,0b00000011);
	
	write_byte(0x82,0b00000000);
	write_byte(0x84,0b00000000);
	write_byte(0x86,0b00000001);
	write_byte(0x88,0b00000001);
	write_byte(0x8A,0b00000001);
	write_byte(0x8C,0b00000000);
	
	
}

unsigned char RTC_DS1302::read_byte(unsigned char addr){

	unsigned char ret_val=0;
	// CE high
	digitalWrite(CE, HIGH);
	delayMicroseconds(10);
	//DAT as input
	pinMode(DAT, OUTPUT);
	digitalWrite(DAT, LOW);
	delayMicroseconds(10);
	// write 8 Bits
	int i = 0;
    while (i <= 7) 
    {
		if(((addr & (1 << i))>>i)==0)
		{
			digitalWrite(DAT, LOW);
		}
		else
		{
			digitalWrite(DAT, HIGH);
		}
		
		delayMicroseconds(10);
		digitalWrite(CLK, HIGH);
		delayMicroseconds(10);
		// switch to read
		if (i==7)
		{
			digitalWrite(DAT, LOW);
			pinMode(DAT, INPUT);
		}
		digitalWrite(CLK, LOW);
		delayMicroseconds(10);
		i++;
	}
	//read 8 Bits
	i = 0;
    while (i <= 7) 
    {
		if(digitalRead(DAT)==0)
		{
			ret_val &= ~(1 << i);
		}
		else
		{
			ret_val |= (1<<i);
		}
		
		if(i<7){
			delayMicroseconds(10);
			digitalWrite(CLK, HIGH);
			delayMicroseconds(10);
			digitalWrite(CLK, LOW);
			delayMicroseconds(10);
			}
		i++;
	}
	delayMicroseconds(10);
	digitalWrite(CE, LOW);
	delayMicroseconds(10);
	return ret_val;
}


void RTC_DS1302::write_byte(unsigned char addr, unsigned char val){

	//CE high
	digitalWrite(CE, HIGH);
	delayMicroseconds(10);
	//DAT as input
	pinMode(DAT, OUTPUT);
	digitalWrite(DAT, LOW);
	delayMicroseconds(10);
	// write 8 Bits
	int i = 0;
    while (i <= 7) 
    {
		if(((addr & (1 << i))>>i)==0)
		{
			digitalWrite(DAT, LOW);
		}
		else
		{
			digitalWrite(DAT, HIGH);
		}
		
		delayMicroseconds(10);
		digitalWrite(CLK, HIGH);
		delayMicroseconds(10);
		digitalWrite(CLK, LOW);
		delayMicroseconds(10);
		i++;
	}
	//write data
	i = 0;
	while (i <= 7) 
	{
		if(((val & (1 << i))>>i)==0)
		{
			digitalWrite(DAT, LOW);
		}
		else
		{
			digitalWrite(DAT, HIGH);
		}
		
		delayMicroseconds(10);
		digitalWrite(CLK, HIGH);
		delayMicroseconds(10);
		digitalWrite(CLK, LOW);
		delayMicroseconds(10);
		i++;
	}
	//reset Data-pin to tristate  
	digitalWrite(DAT, LOW);
	pinMode(DAT, INPUT);
	//disable CE
	delayMicroseconds(10);
	digitalWrite(CE, LOW);
	delayMicroseconds(10);
}

void RTC_DS1302::get_timestamp(){
	unsigned char temp_val=0;
	//sec
	temp_val=read_byte(0x81);
	t_second=(temp_val & 0b00001111)+(((temp_val & 0b01110000)>>4)*10); 
	//min
	temp_val=read_byte(0x83);
	t_minute=(temp_val & 0b00001111)+(((temp_val & 0b01110000)>>4)*10); 
	//h
	temp_val=read_byte(0x85);
	t_hour=(temp_val & 0b00001111)+(((temp_val & 0b00110000)>>4)*10); 
	
	//date
	//day
	temp_val=read_byte(0x87);
	d_day=(temp_val & 0b00001111)+(((temp_val & 0b00110000)>>4)*10);
	//month
	temp_val=read_byte(0x89);
	d_month=(temp_val & 0b00001111)+(((temp_val & 0b00010000)>>4)*10); 
	//year
	temp_val=read_byte(0x8D);
	d_year=(temp_val & 0b00001111)+(((temp_val & 0b11110000)>>4)*10); 
	//weekday
	temp_val=read_byte(0x8B);
	d_weekday=(temp_val & 0b00000111);  
}

unsigned char RTC_DS1302::extract_one_digit(int dat){
	unsigned char ret_val=0;
	ret_val=dat%10;
	return ret_val;
}

unsigned char RTC_DS1302::extract_ten_digit(int dat){
	unsigned char ret_val=0;
	ret_val=(dat/10)%10;
	return ret_val;
}

void RTC_DS1302::set_seconds(int data){
	if((data>=0) and (data<=59)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  ((extract_ten_digit(data)<<4) & 0b01111111);
		write_byte(0x80,temp_val);
	}
}

void RTC_DS1302::set_minutes(int data){
	if((data>=0) and (data<=59)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  ((extract_ten_digit(data)<<4) & 0b01111111);
		write_byte(0x82,temp_val);
	}
}

void RTC_DS1302::set_hours(int data){
	if((data>=0) and (data<=23)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  ((extract_ten_digit(data)<<4) & 0b01111111);
		write_byte(0x84,temp_val);
	}
}

void RTC_DS1302::set_weekday(int data){
	if((data>=1) and (data<=7)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) & 0b00000111;
		write_byte(0x8A,temp_val);
	}
}

void RTC_DS1302::set_day(int data){
	if((data>=1) and (data<=31)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  ((extract_ten_digit(data)<<4) & 0b00111111);
		write_byte(0x86,temp_val);
	}
}

void RTC_DS1302::set_month(int data){
	if((data>=1) and (data<=12)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  ((extract_ten_digit(data)<<4) & 0b00011111);
		write_byte(0x88,temp_val);
	}
}

void RTC_DS1302::set_year(int data){
	if((data>=0) and (data<=99)){
		unsigned char temp_val=0;
		temp_val=extract_one_digit(data) +  (extract_ten_digit(data)<<4);
		write_byte(0x8C,temp_val);
	}
}


//return values
int RTC_DS1302::get_seconds(){
return t_second;
}
int RTC_DS1302::get_minutes(){
return t_minute;
}
int RTC_DS1302::get_hours(){
return t_hour;
}
int RTC_DS1302::get_weekday(){
return d_weekday;
}
int RTC_DS1302::get_day(){
return d_day;
}
int RTC_DS1302::get_month(){
return d_month;
}
int RTC_DS1302::get_year(){
return d_year;
}

