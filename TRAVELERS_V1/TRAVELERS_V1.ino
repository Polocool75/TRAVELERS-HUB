#include <PS4Controller.h> 

// Agrandissement de la zone morte des joysticks (plus la zone morte est grande plus le joystick devra être décalé de son centre pour détecter une variation):
const int dead = 40; 

// Intensité de rouge, vert et bleu de la lumière de la manette PS4:
int r = 50;
int g = 50;
int b = 50;

// Les coefficients de vitesse des roues Avant, Milieu et aRrière:
float coefA = 1;
float coefM = 1;
float coefR = 1;

// Indicateur (booléen) si la touche carré, triangle ou cercle a été appuyé:
bool carre = false;
bool triangle = false;
bool cercle = false;

/* Pour les moteur, la numérotation suit ce schéma (vu du dessus):

     AVANT
    1     4
    2     5
    3     6

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

const int freq = 500; // Fréquence du PWM
const int resolution = 8; // Resolution (8 octets = 256 valeurs prenables pour le PWM, de 0 à 255)

void setup()
{
  Serial.begin(115200); //Pour quand il y a un problème
  PS4.begin("02:02:02:02:02:02"); //Lance la connection à la manette PS4 (le paramètre est l'adresse MAC de la manette)
  for (int i = 1; i <= 12; i++)
  {
    /* Fonctionnement du PWM avec l'ESP32:
     * Tout d'abord créer un "programme pré-enregistré" avec ledcSetup([numéro du programme (1 à 16)],[fq du PWM], [résolution du PWM])
     * Ensuite, on "attache" un pin à un programme avec ledcAttachPin([pin],[numéro du programme])
     * Attention, on ne peut pas attacher plusieurs pins à un même programme (ici, on a donc du créer 12 programmes différents ayant les mêmes caractéristiques)
     * 
     * Enfin, pour utiliser le PWM, on utiliser ledcWrite([numéro du programme],[valeur du PWM])
     * 
     */
    ledcSetup(i, freq, resolution); // Tous les programmes/channels de 1 à 12 auront donc comme fq 500hz et 8 de résolution
  }

  ledcAttachPin(Motor1A, 1);
  ledcAttachPin(Motor1R, 2);
  ledcAttachPin(Motor2A, 3);
  ledcAttachPin(Motor2R, 4);
  ledcAttachPin(Motor3A, 5);
  ledcAttachPin(Motor3R, 6);
  ledcAttachPin(Motor4A, 7);
  ledcAttachPin(Motor4R, 8);
  ledcAttachPin(Motor5A, 9);
  ledcAttachPin(Motor5R, 10);
  ledcAttachPin(Motor6A, 11);
  ledcAttachPin(Motor6R, 12);
}

void loop() {
  if (PS4.isConnected())// Le code ne se lance que si la manette PS4 est connecté
  {
    int RStick = PS4.RStickY(); // Valeur de l'intensité d'inclinaison du joystick droit (entre 127 et -128)
    bool R_A = PS4.RStickY() > 0; // Booléen indiquant si l'on veut avancer ou non (pour le joystick droit)
    int LStick = PS4.LStickY(); // idem pour joystick gauche
    bool L_A = PS4.LStickY() > 0; //idem pour joystick gauche
    
    if (PS4.Square()) //Si le bouton carré est pressé
    {
      carre = !carre;
      b = carre ? 210 : 50; //La valeur d'intensité passe à 210 si carre est pressé et 50 si le bouton avait déjà été pressé et qu'on represse
      delay(100);
    }
    if (PS4.Circle())//Idem avec cercle
    {
      cercle = !cercle;
      r = cercle ? 210 : 50;
      delay(100);
    }
    if (PS4.Triangle())//Idem avec triangle
    {
      triangle = !triangle;
      g = triangle ? 210 : 50;
      delay(100);
    }
    if (triangle && cercle && carre) // Si les 3 boutons sont appuyés cela veut dire qu'on reduit la vitesse de toutes les roues : c'est inutile donc on annule
    {
      triangle = false;
      carre = false;
      cercle = false;
      r = 50;
      g = 50;
      b = 50;
    }
    //Pour indiquer le mode, on utilise la lumière de la manette PS4
    PS4.setLed(r, g, b);
    PS4.sendToController();
    Move(LStick, RStick, L_A, R_A);
    delay(50);
  }
}

void Move(int gauche, int droite, bool L_A, bool R_A)
{
  if (PS4.isConnected())
  {
    if ((gauche <= -dead || gauche >= dead) || (droite <= -dead || droite >= dead))//
    {
      coefA = 1;
      coefM = 1;
      coefR = 1;
      if (cercle || triangle || carre) {
        coefA = cercle ? 1 : 0.80 ;
        coefM = triangle ? 1 : 0.80 ;
        coefR = carre ? 1 : 0.80 ;
      }
      
      int valueR = 0;
      int valueL = 0;
      if ((droite <= -dead || droite >= dead))
        valueR = min((int)map(abs(droite), 0, 110, 0, 255), 255);
      if (gauche <= -dead || gauche >= dead)
        valueL = min((int)map(abs(gauche), 0, 110, 0, 255), 255);

      if (L_A)
      {
        ledcWrite(1, round(valueL * coefA));
        ledcWrite(2, 0);
        ledcWrite(3, round(valueL * coefM));
        ledcWrite(4, 0);
        ledcWrite(5, round(valueL * coefR));
        ledcWrite(6, 0);
      }
      else
      {
        ledcWrite(1, 0);
        ledcWrite(2, round(valueL * coefA));
        ledcWrite(3, 0);
        ledcWrite(4, round(valueL * coefM));
        ledcWrite(5, 0);
        ledcWrite(6, round(valueL * coefR));
      }

      if (R_A)
      {
        ledcWrite(7, round(valueR * coefA));
        ledcWrite(8, 0);
        ledcWrite(9, round(valueR * coefM));
        ledcWrite(10, 0);
        ledcWrite(11, round(valueR * coefR));
        ledcWrite(12, 0);
      }
      else
      {
        ledcWrite(7, 0);
        ledcWrite(8, round(valueR * coefA));
        ledcWrite(9, 0);
        ledcWrite(10, round(valueR * coefM));
        ledcWrite(11, 0);
        ledcWrite(12, round(valueR * coefR));
      }
    }
    else
    {
      ledcWrite(1, 0);
      ledcWrite(2, 0);
      ledcWrite(3, 0);
      ledcWrite(4, 0);
      ledcWrite(5, 0);
      ledcWrite(6, 0);
      ledcWrite(7, 0);
      ledcWrite(8, 0);
      ledcWrite(9, 0);
      ledcWrite(10, 0);
      ledcWrite(11, 0);
      ledcWrite(12, 0);
    }
  }
}
