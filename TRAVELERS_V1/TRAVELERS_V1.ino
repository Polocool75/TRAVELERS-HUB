#include <PS4Controller.h>

const int dead = 50; // Agrandissement de la zone morte 

//Pour les moteur, la numérotation suit ce schéma
/* 
 * AVANT
 * 1   4
 * 2   5
 * 3   6
 *  
 */
const int Motor1A = 15;
const int Motor1R = 4;

const int Motor2A = 13;
const int Motor2R = 12;

const int Motor3A = 26;
const int Motor3R = 25;

const int Motor4A = 5;
const int Motor4R = 19;

const int Motor5A = 14;
const int Motor5R = 27;

const int Motor6A = 33;
const int Motor6R = 32;

const int freq = 500; //fq du PWM
const int resolution = 8; //Resolution (8 octets = 256 valeurs prenables pour le PWM

void setup() 
{
  
  Serial.begin(115200);
  PS4.begin("01:01:01:01:01:01");

  for(int i= 1; i<=12;i++)
  {
     ledcSetup(i,freq, resolution); //Le channel 0 aura donc comme fq 5000hz et 8 de résolution
  }
  
  ledcAttachPin(Motor1A,1);
  ledcAttachPin(Motor1R,2);
  ledcAttachPin(Motor2A,3);
  ledcAttachPin(Motor2R,4); 
  ledcAttachPin(Motor3A,5);
  ledcAttachPin(Motor3R,6);
  ledcAttachPin(Motor4A,7);
  ledcAttachPin(Motor4R,8);
  ledcAttachPin(Motor5A,9);
  ledcAttachPin(Motor5R,10);
  ledcAttachPin(Motor6A,11); 
  ledcAttachPin(Motor6R,12);
}

void Move(int gauche, int droite, bool L_A, bool R_A)
{
  if (PS4.isConnected()) 
  {
  //Une ou deux roues en mouvement
  if((gauche<=-dead || gauche >= dead) || (droite<= -dead ||droite >= dead))
  {
    int valueR =0;
    int valueL =0;
    if((droite<= -dead ||droite >= dead))
      valueR =map(abs(droite),0,128,0,255);
    if(gauche<=-dead || gauche >= dead)
      valueL= map(abs(gauche),0,129,0,255);
      
    //Serial.println(valueR);
    //Serial.println(valueL);
    
    if(L_A)
     {
        ledcWrite(1,valueL);
        ledcWrite(2,0);
        ledcWrite(3,valueL);
        ledcWrite(4,0);
        ledcWrite(5,valueL);
        ledcWrite(6,0);
     }
     else
     {
        ledcWrite(1,0);
        ledcWrite(2,valueL);
        ledcWrite(3,0);
        ledcWrite(4,valueL);
        ledcWrite(5,0);
        ledcWrite(6,valueL);
     }

     if(R_A)
     {
        ledcWrite(7,valueR);
        ledcWrite(8,0);
        ledcWrite(9,valueR);
        ledcWrite(10,0);
        ledcWrite(11,valueR);
        ledcWrite(12,0);
     }
     else
     {
        ledcWrite(7,0);
        ledcWrite(8,valueR);
        ledcWrite(9,0);
        ledcWrite(10,valueR);
        ledcWrite(11,0);
        ledcWrite(12,valueR);
     }
  }
  else
  {
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(4,0);
    ledcWrite(5,0);
    ledcWrite(6,0);
    ledcWrite(7,0);
    ledcWrite(8,0);
    ledcWrite(9,0);
    ledcWrite(10,0);
    ledcWrite(11,0);
    ledcWrite(12,0);
  }
}
}

void loop() {
if (PS4.isConnected()) 
{
    int RStick = PS4.RStickY();
    bool R_A = PS4.RStickY()>0;
    int LStick = PS4.LStickY();
    bool L_A = PS4.LStickY()>0;

    /*
    if(RStick<=-dead || RStick >= dead)
    {
      //Serial.println(RStick);
    }
    if(LStick<=-dead || LStick >= dead)
    {
      //Serial.println(LStick);
    }
    */
    Move(LStick,RStick,L_A,R_A);
    
    delay(50);
}
}
