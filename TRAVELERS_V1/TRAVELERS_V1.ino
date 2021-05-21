#include <PS4Controller.h>

const int dead = 40; // Agrandissement de la zone morte 
int r = 50;
int g = 50;
int b = 50;

float coefA=1;
float coefM=1;
float coefR=1;    

int slow= 0;
bool carre = false;
bool triangle = false;
bool cercle = false;

/* Pour les moteur, la numérotation suit ce schéma 
 * 
 *   AVANT
 *  1    4
 *  2    5
 *  3    6
 *  
 */

// Définition des pins de chaque moteur :
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

const int freq = 500; // Fq du PWM
const int resolution = 8; // Resolution (8 octets = 256 valeurs prenables pour le PWM)

void setup() 
{
  Serial.begin(115200);
  PS4.begin("01:01:01:01:01:01");
  for(int i= 1; i<=12;i++)
  {
     ledcSetup(i,freq, resolution); //Tous les channels auront donc comme fq 5000hz et 8 de résolution
     //nb: 
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
   
  if((gauche<=-dead || gauche >= dead) || (droite<= -dead ||droite >= dead))
  {
    coefA=1;
    coefM=1;
    coefR=1;    
    if(cercle || triangle || carre){
      coefA = cercle ? 1:0.80 ;
      coefM = triangle ? 1:0.80 ;
      coefR = carre ? 1:0.80 ;
    }
    Serial.println(coefA);
    Serial.println(coefM);
    Serial.println(coefR);
    
    int valueR = 0;
    int valueL = 0;
    if((droite<= -dead ||droite >= dead))
      valueR =min((int)map(abs(droite),0,110,0,255),255);
    if(gauche<=-dead || gauche >= dead)
      valueL= min((int)map(abs(gauche),0,110,0,255),255);
      
    if(L_A)
     {
        ledcWrite(1,round(valueL*coefA));
        ledcWrite(2,0); 
        ledcWrite(3,round(valueL*coefM));
        ledcWrite(4,0);             
        ledcWrite(5,round(valueL*coefR));
        ledcWrite(6,0);
     }
     else
     {
        ledcWrite(1,0);
        ledcWrite(2,round(valueL*coefA));
        ledcWrite(3,0);
        ledcWrite(4,round(valueL*coefM));      
        ledcWrite(5,0);
        ledcWrite(6,round(valueL*coefR));
     }

     if(R_A)
     {
        ledcWrite(7,round(valueR*coefA));
        ledcWrite(8,0);
        ledcWrite(9,round(valueR*coefM));
        ledcWrite(10,0);      
        ledcWrite(11,round(valueR*coefR));
        ledcWrite(12,0);
     }
     else
     {
        ledcWrite(7,0);
        ledcWrite(8,round(valueR*coefA));
        ledcWrite(9,0);
        ledcWrite(10,round(valueR*coefM));       
        ledcWrite(11,0);
        ledcWrite(12,round(valueR*coefR));
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
    if(PS4.Square())
    {
      carre = !carre;
      b = carre ? 210:50;
      delay(100);
    }
    if(PS4.Circle())
    {
      cercle = !cercle;
      r = cercle ? 210:50;
      delay(100);
    }
    if(PS4.Triangle())
    {
      triangle= !triangle;
      g = triangle ? 210:50;
      delay(100);
    }
    if(triangle && cercle && carre)
    {
      triangle=false;
      carre = false;
      cercle = false;
      r=50;
      g=50;
      b=50;     
    }
    PS4.setLed(r, g, b);
    PS4.sendToController();
    Move(LStick,RStick,L_A,R_A);    
    delay(50);
}

}
