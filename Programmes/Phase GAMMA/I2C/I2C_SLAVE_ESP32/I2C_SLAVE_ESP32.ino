#include "Wire.h"

#define I2C_DEV_ADDR 0x11

uint32_t i = 0;
volatile boolean receiveFlag = false;
char temp[32];
String command;

void onRequest(){
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
}

void onReceive(int len){
  for (int j = 0; j<len; j++)
  {
    temp[j]=Wire.read();
    temp[j+1] = '\0';
  }
  for (int j=0; j<len; j++)
  {
    temp[j] = temp[j+1];
  }
  receiveFlag = true;

}

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);


}

void loop() {
  delay(300);
  if(receiveFlag == true)
  {
    Serial.println(temp);
    receiveFlag = false;
  }
  

}
