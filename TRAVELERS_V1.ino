#include <PS4Controller.h>

const int dead = 50; // Agrandissement de la zone morte 

//Pour les moteur, utiliser des matrices dim 2 (numero,sens)
const int Motor1A = 12;
const int Motor1R = 13;

const int Motor2A = 14;
const int Motor2R = 27;

const int Motor3A = 26;
const int Motor3R = 25;

const int Motor4A = 33;
const int Motor4R = 32;

const int Motor5A = 15;
const int Motor5R = 4;

const int Motor6A = 19;
const int Motor6R = 5;

const int freq = 500;
const int resolution = 8;

//Vu que l'on appelle par channel, l'appelation "MotorXY" sera peut-être utilisé pour les channel

void setup() 
{
  
  Serial.begin(115200);
  PS4.begin("01:01:01:01:01:01");

  for(int i= 1; i<=12;i++)
  {
     ledcSetup(i,freq, resolution); //Le channel 0 aura donc comme fq 5000hz et 8 de résolution
  }
  
  ledcAttachPin(Motor5A,1);
  ledcAttachPin(Motor5R,2);
  ledcAttachPin(Motor6A,3);
  ledcAttachPin(Motor6R,4);

}

void Move(int gauche, int droite, bool L_A, bool R_A)
{
  if (PS4.isConnected()) 
  {
  //Une ou deux roues en mouvement
  if((gauche<=-dead || gauche >= dead) || (droite<= -dead ||droite >= dead))
  {
    int valueR = map(abs(droite),0,129,0,255);
    int valueL= map(abs(gauche),0,129,0,255);
    Serial.println(valueR);
    Serial.println(valueL);
    
    if(L_A)
     {
        ledcWrite(1,valueL);
        ledcWrite(2,0);
        Serial.println("Zone A");
        
        
        //ledcWrite();
        //ledcWrite();

        //ledcWrite();
        //ledcWrite();
     }
     else
     {
        Serial.println("Zone B");
        ledcWrite(2,valueL);
        ledcWrite(1,0);
     }

     if(R_A)
     {
        Serial.println("Zone C");
        ledcWrite(3,valueR);
        ledcWrite(4,0);
     }
     else
     {
        Serial.println("Zone D");
        ledcWrite(4,valueR);
        ledcWrite(3,0);
     }
  }
  else
  {
    ledcWrite(1,0);
    ledcWrite(2,0);
    ledcWrite(3,0);
    ledcWrite(4,0);
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
    
    if(RStick<=-dead || RStick >= dead)
    {
      Serial.println(RStick);
      Serial.println(R_A);
    }
    if(LStick<=-dead || LStick >= dead)
    {
      Serial.println(LStick);
      Serial.println(L_A);
    }
    
    Move(LStick,RStick,L_A,R_A);
    
    delay(50);
}
}
