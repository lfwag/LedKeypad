#include <LedKeypad.h>

char brightness =0;
char buf[5]="2456";

void setup() {
  ledkeypad.begin(); /*Enable*/
  ledkeypad.setBrightness(0);/*Sets the brightness level*/  
  ledkeypad.display(2015);/*Display data*/
  delay(5000);
  ledkeypad.display(buf);/*Display character for testing*/
  for(int i=0;i<4;i++){/*for testing*/
    ledkeypad.dotShow(i);
    delay(1000);
    ledkeypad.dotVanish(i);
  }
}

void loop() { 
  unsigned char keyValue=0;
  keyValue = ledkeypad.getKey();/*Get key value*/
  switch(keyValue){
    case KEY_DOWN:
      buf[0]++;
      if(buf[0] > '9')
        buf[0] = '0';
      ledkeypad.display(0,buf[0]);   
      break;
   
    case KEY_LEFT:
      buf[1]++;
      if(buf[1] > '9')
        buf[1] = '0';
      ledkeypad.display(1,buf[1]);
      break;
   
    case KEY_UP:
      buf[2]++;
      if(buf[2] > '9')
        buf[2] = '0';
      ledkeypad.display(2,buf[2]);
      break;
   
    case KEY_RIGHT:
      buf[3]++;
      if(buf[3] > '9')
        buf[3] = '0';
      ledkeypad.display(3,buf[3]);
      break;
   
    case KEY_SELECT:
      brightness++;
      if(brightness > 7)
        brightness = 0;
      ledkeypad.setBrightness(brightness);
      break;
     
    default:
      break;
 } 
}

