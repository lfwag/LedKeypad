# LedKeypad
Library to drive DFRobot DFR0382 Shield for Arduino

Supports full Latin alphabet per ASCII namely:
* 0-9
* A-Z
* a-z

With some exceptions given limitations of 7-segment. These will be blank:
* K or k
* M or m
* V,W,X and Z

These special characters are provided:
* \* will display the degree symbol º
* \- will display the dash
* _ will display the underscore
* ' will display a single quote
* " will display a double quote

See example code for actual usage.

# Author
linfeng(490289303@qq.com)\
Original version  V1.0\
Date  2015-12-11

# Contributions and Changes
Alejandro Imass\
Version 1.1
Date 2021-09-23 
* extended charset to full latin ascii alphabet
* some useful special chars
* changed RAM char table to PROGMEM
* added a clear method to clear and shut off display
* changed unsigned char to uint8_t for portability, clarity and avoid confusion with char
* more re-use of the display(uint8_t addr,char data) method to simplify code
* tabs to spaces
* added README