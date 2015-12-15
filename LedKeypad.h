/*!
* @file LedKeypad.h
* @brief LedKeypad.h detailed description for LedKeypad.cpp 
*
*  LedKeypad.h wrote the library file for Led Keypad Shield specially
* 
* @author linfeng(490289303@qq.com)
* @version  V1.0
* @date  2015-12-11
*/
#ifndef __LedKeypad__H
#define __LedKeypad__H

#include "Arduino.h"

#define KEY_DOWN  1
#define KEY_LEFT  2
#define KEY_UP   3
#define KEY_RIGHT 4
#define KEY_SELECT 5
#define KEY_NULL    0	


class LedKeypad
{
public:
	LedKeypad(){};
	void begin(void);
	char keyRead(void);	
	char getKey(void);
	void clrKey(void);
	char letterTransform(char letter);
	void tm1650Begin(void);
	void tm1650Stop(void);
	void tm1650Write(unsigned char oneByte);
	void tm1650Send(unsigned char addr,unsigned char data);
	void setBrightness(unsigned char brightness);
	void display(int data);
	void display(int addr,char data);
	void display(const char* buf_);
	void dotShow(char temp);
	void dotVanish(char temp);
	
private:
	#define SCL_pin 19
	#define SDA_pin 18	

	unsigned char keyOk;
	unsigned char keyCount;	
	char ledByteVal_[4]; ///<The displayed value
	static unsigned char dotFlag;
	static char onlineTime[2];  ///< time:onlineTime[0],minute；time_online[1]hour
	static unsigned char brightness;  ///<The current brightness level
	static unsigned long lastTime,disTime,ledTime;  ///<Timer
	static unsigned char brightness7[8];  ///<1 to 8 - level brightness（7-SEG LED）
	static unsigned char brightness8[8];  ///<1 to 8 - level brightness（8-SEG LED）
	static unsigned char ledVal[17];  ///<cathode
	static char ledByte[4]; ///<Digital tube selection
	


};
extern LedKeypad ledkeypad;

#endif

/******************************************************************************
  Copyright (C) <2015>  <linfeng>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  Contact: 490289303@qq.com
 ******************************************************************************/
 