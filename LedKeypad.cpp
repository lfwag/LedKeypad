#include "LedKeypad.h"


char LedKeypad::onlineTime[2]={0,0};  ///< time:onlineTime[0],minute£»time_online[1]hour
unsigned char LedKeypad::dotFlag = 0; ///<
unsigned char LedKeypad::brightness = 0;  ///<The current brightness level
unsigned long LedKeypad::lastTime=0,LedKeypad::disTime=0,LedKeypad::ledTime=0;  ///<Timer
unsigned char LedKeypad::brightness7[8]={0x19,0x29,0x39,0x49,0x59,0x69,0x79,0x09};  ///<1 to 8 - level brightness£¨7-SEG LED£©
unsigned char LedKeypad::brightness8[8]={0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x01};  ///<1 to 8 - level brightness£¨8-SEG LED£©
unsigned char LedKeypad::ledVal[17]=
{
	0x3F, ///< 0
	0x06, ///< 1
	0x5B, ///< 2
	0x4F, ///< 3
	0x66, ///< 4
	0x6D, ///< 5
	0x7D, ///< 6
	0x07, ///< 7
	0x7F, ///< 8
	0x6F, ///< 9
	0x77, ///< A
	0x7C, ///< b
	0x39, ///< c
	0x5E, ///< d
	0x79, ///< E
	0x71, ///< F
	0x40  ///< -
};  ///<cathode
char LedKeypad::ledByte[4]={0X68,0X6A,0X6C,0X6E}; ///<Digital tube selection



/*!
* @brief IO configuration
*
* @brief  Configured for I2C IO
*
* @return void
*/ 

void LedKeypad::begin(void)
{
	pinMode(SDA_pin,OUTPUT);
	pinMode(SCL_pin,OUTPUT);
}

/*!
* @brief Sets the brightness level
*
* @brief  Sets the brightness level
*
* @return void
*/ 
void LedKeypad::setBrightness(unsigned char brightness)
{
	tm1650Send(0x48,brightness8[brightness]);
}
/*!
* @brief Show decimal point
*
* @brief  display the decimal point at temp
*
* @return void
*/ 
void LedKeypad::dotShow(char temp)
{
	
	unsigned char i = 1;
	i <<= temp;
	dotFlag |= i;
	tm1650Send(ledByte[temp],ledVal[ledByteVal_[temp]]|0x80);
}
/*!
* @brief Close the decimal point
*
* @brief  Do not display the decimal point at temp
*
* @return void
*/ 
void LedKeypad::dotVanish(char temp)
{
	unsigned char i = 1;
	i <<= temp;
	dotFlag &= ~i;
	tm1650Send(ledByte[temp],ledVal[ledByteVal_[temp]]);
}
/*!
* @brief Enable TM1650
*
* @brief  Enable TM1650
*
* @return void
*/ 
void LedKeypad::tm1650Begin(void)
{
	digitalWrite(SCL_pin,HIGH);
	digitalWrite(SDA_pin,HIGH);
	delayMicroseconds(2);
	digitalWrite(SDA_pin,LOW);
	delayMicroseconds(2);
	digitalWrite(SCL_pin,LOW);
}
/*!
* @brief Disable TM1650
*
* @brief  Disable TM1650
*
* @return void
*/ 
void LedKeypad::tm1650Stop(void)
{
	digitalWrite(SCL_pin,HIGH);
    digitalWrite(SDA_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(SDA_pin,HIGH);
    delayMicroseconds(2);
}
/*!
* @brief Wirte data
*
* @brief  Wirte oneByte
*
* @return void
*/
void LedKeypad::tm1650Write(unsigned char oneByte)
{
    for(int i=0;i<8;i++)
    {
      if(oneByte & 0x80)
      {
        digitalWrite(SDA_pin,HIGH);
        }
        else
      {
         digitalWrite(SDA_pin,LOW); 
        }
       delayMicroseconds(2); 
       digitalWrite(SCL_pin,HIGH);
       oneByte <<= 1; 
       delayMicroseconds(2);
       digitalWrite(SCL_pin,LOW);
    }
     digitalWrite(SDA_pin,HIGH); 
     delayMicroseconds(2); 
     digitalWrite(SCL_pin,HIGH);
     delayMicroseconds(2);
     digitalWrite(SCL_pin,LOW);
}
/*!
* @brief Wirte data
*
* @brief  TM1650 an address to write a byte
*
* @return void
*/
void LedKeypad::tm1650Send(unsigned char addr,unsigned char data)
{
	tm1650Begin();
    tm1650Write(addr);
    tm1650Write(data);
    tm1650Stop();	
}
/*!
* @brief Read the key value
*
* @brief  Read button analog voltage values
*
* @return char
*/
char LedKeypad::keyRead(void)
{
	unsigned int keyVal;
    keyVal=analogRead(A0);
    if(keyVal<150)return(KEY_DOWN);//0V-down
    else if(keyVal<350)return(KEY_LEFT);//1V-LEFT
    else if(keyVal<550)return(KEY_UP);//2V-up
    else if(keyVal<750)return(KEY_RIGHT);//3V-right
    else if(keyVal<950)return(KEY_SELECT);//4V-select
    else return(KEY_NULL);
}
/*!
* @brief Get key value
*
* @brief  Get key value
*
* @return char
*/
char LedKeypad::getKey(void)
{
  unsigned char keyNumber;
  unsigned long nowTime=millis();  
  keyOk = 0;
  if(nowTime-lastTime>5){
	lastTime=nowTime;
	keyNumber=keyRead();
	if(keyNumber){
		keyCount++;
		if(keyCount==10)keyOk=keyNumber;
		if(keyCount>10)keyCount=11;
	}else{
		keyCount=0;
	}
  }
  return(keyOk);
}

/*!
* @brief ASCLL code-switching
*
* @brief  Code will be converted to values in the this program ASCLL
*
* @return void
*/
char LedKeypad::letterTransform(char letter)
{
	
	if(letter < 10){
		return letter;
	}else if(letter <= 45){
		return 16;
	}else if(letter<58){
		return(letter-48);
	}else if(letter<91){
		return(letter-55);
	}else if(letter < 123){
		return(letter-87);
	}
}
/*!
* @brief Display data
*
* @brief  Display data
*
* @return void
*/
void LedKeypad::display(int data)
{
	char number[4]; 
	ledByteVal_[0] = data/1000;
	data %= 1000;
	ledByteVal_[1] = data/100;
	data %= 100;
	ledByteVal_[2]=data/10;
	ledByteVal_[3]=data%10;
	for(int i=0;i<4;i++){		
		display(i,ledByteVal_[i]);
	}
}
/*!
* @brief Display character
*
* @brief  Display character
*
* @return void
*/
void LedKeypad::display(int addr,char data)
{
	unsigned char i = 1;
	i <<= addr;
	data = letterTransform(data);
	Serial.println(dotFlag);
	ledByteVal_[addr]=data;
	if(i&dotFlag){
		tm1650Send(ledByte[addr],ledVal[data]+0x80);
	}else{
		tm1650Send(ledByte[addr],ledVal[data]);
	}	
	
}
/*!
* @brief Display string
*
* @brief  Display string
*
* @return void
*/
void LedKeypad::display(const char* buf_)
{
	for(int i=0;i<4;i++){
		ledByteVal_[i] = *buf_++;
		display(i,ledByteVal_[i]);
		ledByteVal_[i] = letterTransform(ledByteVal_[i]);
		tm1650Send(ledByte[i],ledVal[ledByteVal_[i]]);		
	}
}

LedKeypad ledkeypad;




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
 
 







