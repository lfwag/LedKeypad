#include "LedKeypad.h"
#include <avr/pgmspace.h>

char LedKeypad::onlineTime[2]={0,0};  ///< time:onlineTime[0],minute time_online[1]hour
uint8_t  LedKeypad::dotFlag = 0; ///<
uint8_t  LedKeypad::brightness = 0;  ///<The current brightness level
uint16_t LedKeypad::lastTime=0,LedKeypad::disTime=0,LedKeypad::ledTime=0;  ///<Timer
uint8_t  LedKeypad::brightness7[8]={0x19,0x29,0x39,0x49,0x59,0x69,0x79,0x09};  ///<1 to 8 - level brightness 7-SEG LED
uint8_t  LedKeypad::brightness8[8]={0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x01};  ///<1 to 8 - level brightness 8-SEG LED

/*
          a
         ---
       f| g |b
         ---
       e|   |c
         --- . x
          d

Note: The TM1650 chip uses gfedcba encoding, x is the dot (MSb)

 */
                                 // xgfedcba     Char  Displays  gfedcba-hex
const uint8_t C00 PROGMEM = 0b00111111; //   0     0        0x3F
const uint8_t C01 PROGMEM = 0b00000110; //   1     1        0x06
const uint8_t C02 PROGMEM = 0b01011011; //   2     2        0x5B
const uint8_t C03 PROGMEM = 0b01001111; //   3     3        0x4F
const uint8_t C04 PROGMEM = 0b01100110; //   4     4        0x66
const uint8_t C05 PROGMEM = 0b01101101; //   5     5        0x6D
const uint8_t C06 PROGMEM = 0b01111101; //   6     6        0x7D
const uint8_t C07 PROGMEM = 0b00000111; //   7     7        0x04
const uint8_t C08 PROGMEM = 0b01111111; //   8     8        0x7F
const uint8_t C09 PROGMEM = 0b01101111; //   9     9        0x6F
const uint8_t C10 PROGMEM = 0b01110111; //   A     A        0x77
const uint8_t C11 PROGMEM = 0b01111100; //   B     b        0x7C
const uint8_t C12 PROGMEM = 0b00111001; //   C     C        0x39
const uint8_t C13 PROGMEM = 0b01011110; //   D     d        0x5E
const uint8_t C14 PROGMEM = 0b01111001; //   E     E        0x79
const uint8_t C15 PROGMEM = 0b01110001; //   F     F        0x71
const uint8_t C16 PROGMEM = 0b00111101; //   G     G        0x3D
const uint8_t C17 PROGMEM = 0b01110110; //   H     H        0x76
const uint8_t C18 PROGMEM = 0b00110000; //   I     I        0x30
const uint8_t C19 PROGMEM = 0b00001110; //   J     J        0x0E
const uint8_t C20 PROGMEM = 0b00000000; //   K     K        0x00 blank
const uint8_t C21 PROGMEM = 0b00111000; //   L     L        0x38
const uint8_t C22 PROGMEM = 0b00000000; //   M     M        0x00 blank
const uint8_t C23 PROGMEM = 0b01010100; //   N     n        0x54
const uint8_t C24 PROGMEM = 0b01011100; //   O     o        0x5C
const uint8_t C25 PROGMEM = 0b01110001; //   P     P        0x71
const uint8_t C26 PROGMEM = 0b01100111; //   Q     q        0x67
const uint8_t C27 PROGMEM = 0b01010000; //   R     r        0x50
const uint8_t C28 PROGMEM = 0b01101101; //   S     S        0x6D (same as 5)
const uint8_t C29 PROGMEM = 0b01111000; //   T     t        0x78
const uint8_t C30 PROGMEM = 0b00111110; //   U     U        0x3E
const uint8_t C31 PROGMEM = 0b00000000; //   V     V        0x00 blank
const uint8_t C32 PROGMEM = 0b00000000; //   W     W        0x00 blank
const uint8_t C33 PROGMEM = 0b00000000; //   X     X        0x00 blank
const uint8_t C34 PROGMEM = 0b01101110; //   Y     Y        0x6E
const uint8_t C35 PROGMEM = 0b00000000; //   Z     Z        0x00 blank
const uint8_t C36 PROGMEM = 0b01100011; //   *     Degree   0x63
const uint8_t C37 PROGMEM = 0b01000000; //   -     -        0x40
const uint8_t C38 PROGMEM = 0b00001000; //   _     _        0x08
const uint8_t C39 PROGMEM = 0b00100000; //   '     '        0x20
const uint8_t C40 PROGMEM = 0b00100010; //   "     "        0x22

const uint8_t ledVal[41] PROGMEM =
{
    C00,C01,C02,C03,C04,C05,
    C06,C07,C08,C09,C10,C11,
    C12,C13,C14,C15,C16,C17,
    C18,C19,C20,C21,C22,C23,
    C24,C25,C26,C27,C28,C29,
    C30,C31,C32,C33,C34,C35,
    C36,C37,C38,C39,C40
};  ///<cathode
uint8_t LedKeypad::ledByte[4]={0x68,0x6A,0x6C,0x6E}; ///<Digital tube selection

uint8_t LedKeypad::getLedVal(uint8_t index){
    return pgm_read_byte_near (&ledVal[index]);
}

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
void LedKeypad::dotShow(uint8_t addr)
{
    uint8_t i = 1;
    i <<= addr;
    dotFlag |= i;
    display(addr,ledByteVal_[addr]);
}
/*!
* @brief Close the decimal point
*
* @brief  Do not display the decimal point at temp
*
* @return void
*/ 
void LedKeypad::dotVanish(uint8_t addr)
{
    uint8_t i = 1;
    i <<= addr;
    dotFlag &= ~i;
    display(addr,ledByteVal_[addr]);
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
void LedKeypad::tm1650Write(uint8_t oneByte)
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
void LedKeypad::tm1650Send(uint8_t addr,uint8_t data)
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
* @return byte
*/
uint8_t LedKeypad::keyRead(void)
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

    if(letter == 32){
        return 35; //Z no char
    } else if(letter == 42){
        return 36;
    }else if(letter == 45){
        return 37;
    }else if(letter == 95){
        return 38;
    }else if(letter == 39){
        return 39;
    }else if(letter == 34){
        return 40;
    }else if(letter < 10){
        return letter;
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
void LedKeypad::display(uint8_t addr,char data)
{
    unsigned char i = 1;
    i <<= addr;
    data = letterTransform(data);
    ledByteVal_[addr]=data;
    if(i&dotFlag){
        tm1650Send(ledByte[addr],getLedVal(data)|0x80);
    }else{
        tm1650Send(ledByte[addr],getLedVal(data));
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
    }
}

void LedKeypad::clear()
{
    for(int i=0;i<4;i++){
        tm1650Send(ledByte[i],0x00);
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
 
 







