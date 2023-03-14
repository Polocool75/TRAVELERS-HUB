#include <PS4Controller.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795

//------------------------------- Variables motorisation ---------------------------------------------
const int dead = 40;

const int freq = 500;                                      // Fréquence du PWM
const int resolution = 8;                                  // Resolution (8 octets = 256 valeurs prenables pour le PWM, de 0 à 255)

const int Motor1A = 23;
const int Motor1R = 22;

const int Motor2A = 21;
const int Motor2R = 19;

const int Motor3A = 27;
const int Motor3R = 13;

const int Motor4A = 25;
const int Motor4R = 26;

const int enable = 2;

bool carre = false;

//------------------------------- Variables odométrie générales ---------------------------------------------

double nbDeTicks = 341.2;   // à définir par mesures             // nombre de ticks par tour de l'encodeur
float diametre = 6.5;                                      // diamètre de la roue en cm
float entraxe = 21;                                        // distance entre les roues en cm
int incertitude = 1;                                       // nombre de d'appels de la méthode ISR pour le calcul de la vitesse (plus la valeur est petite, plus le bruit de la mesure est important)
int tailleVecteur = 10;                                    // taille du vecteur de moyenne
int k = 10;                                                // temps minimal de mesure


float distanceCentre = 0;                                  // distance parcourue au point central
float x = 0;
float y = 0;
float phi = 0;                                             //angle

volatile unsigned echActuel = 0;                           // mesure actuelle du temps depuis le début de l'execution du programme
volatile unsigned echPrecedent = 0;                        // mesure précédente du temps depuis le début de l'execution du programme
volatile unsigned echDelta = 0;                            // delta de temps entre la mesure actuelle et la précédente

///------------------------------- Variables du moteur droit ---------------------------------------------

volatile unsigned echActuelInterruptionD = 0;              // variables pour la définition du temps d'interruption et le calcul de la bonne vitesse du moteur

volatile unsigned echPrecedentInterruptionD = 0;
double echDeltaInterruptionD = 0;

int encodeurD = 18;                                         // pin de l'encodeur  (nous en avons deux par roue techniquement)

double frequenceD = 0;                                     // fréquence d'interruption des roues
double Wd = 0;                                             // Vitesse angulaire
double Vd = 0;                                             // vitesse linéaire
int CD = 0;                                                // compteur d'appels de la méthode ISR
float vecteurD[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};         // vecteur de stockage de données pour calculer la moyenne des temps d'interruptions

float distanceD = 0;                                       // distance parcourue
int compteurTicksActuelD = 0;                              // ticks de l'encodeur
int compteurTicksPrecedentD = 0;                           // ticks de l'encodeur  précédents
int deltaTicksD = 0;

//------------------------------  Variables du moteur gauche ------------------------------------------------

volatile unsigned echActuelInterruptionG = 0;
volatile unsigned echPrecedentInterruptionG = 0;
double echDeltaInterruptionG = 0;

int encodeurG = 32;

double frequenceG = 23;
double Wg = 0;
double Vg = 0;
int CG = 0;
float vectorG[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

float distanceG = 0;
int compteurTicksActuelG = 0;
int compteurTicksPrecedentG = 0;
int deltaTicksG = 0;

void setup()
{
  PS4.begin("02:02:02:02:02:02");                        // Lance la connection à la manette PS4 (le paramètre est l'adresse MAC de la manette)
  Serial.begin(115200);                                  // Initialement 9600 pour l'odométrie

  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1R, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2R, OUTPUT);
  pinMode(Motor3A, OUTPUT);
  pinMode(Motor3R, OUTPUT);
  pinMode(Motor4A, OUTPUT);
  pinMode(Motor4R, OUTPUT);
  pinMode(enable, OUTPUT);

  // A vérifier pour ça :
  pinMode(encodeurD, INPUT);
  pinMode(encodeurG, INPUT);

  digitalWrite(enable, LOW);

  for (int i = 1; i <= 8; i++)
  {
    /* Fonctionnement du PWM avec l'ESP32:
       Tout d'abord créer un "programme pré-enregistré" avec ledcSetup([numéro du programme (1 à 16)],[fq du PWM], [résolution du PWM])
       Ensuite, on "attache" un pin à un programme avec ledcAttachPin([pin],[numéro du programme])
       Attention, on ne peut pas attacher plusieurs pins à un même programme (ici, on a donc du créer 12 programmes différents ayant les mêmes caractéristiques)

       Enfin, pour utiliser le PWM, on utiliser ledcWrite([numéro du programme],[valeur du PWM])

    */
    ledcSetup(i, freq, resolution);                    // Tous les programmes/channels de 1 à 12 auront donc comme fq 500hz et 8 de résolution
  }

  ledcAttachPin(Motor1A, 1);
  ledcAttachPin(Motor1R, 2);
  ledcAttachPin(Motor2A, 3);
  ledcAttachPin(Motor2R, 4);
  
  ledcAttachPin(Motor3A, 5);
  ledcAttachPin(Motor3R, 6);
  ledcAttachPin(Motor4A, 7);
  ledcAttachPin(Motor4R, 8);

  attachInterrupt(digitalPinToInterrupt(encodeurD), ISRdroite, FALLING); // ligne pour ajouter une interruption à un pin précisé
  attachInterrupt(digitalPinToInterrupt(encodeurG), ISRgauche, FALLING);
}

void loop()
{
  if (PS4.isConnected())                                 // Le code ne se lance que si la manette PS4 est connecté
  {
    int RStick = PS4.RStickY();                        // Valeur de l'intensité d'inclinaison du joystick droit (entre 127 et -128)
    bool R_A = PS4.RStickY() > 0;                      // Booléen indiquant si l'on veut avancer ou non (pour le joystick droit)
    int LStick = PS4.LStickY();                        // idem pour joystick gauche
    bool L_A = PS4.LStickY() > 0;                      // idem pour joystick gauche

    if (PS4.Square())                                  // Si le bouton carré est pressé
    {
      carre = !carre;
      if (carre)
        digitalWrite(enable, HIGH);
      else
        digitalWrite(enable, LOW);
      delay(100);
    }
    Move(LStick, RStick, L_A, R_A);
    delay(50);

    echActuel = millis();                                  // Temps de la mesure
    echActuelInterruptionD = millis();                     // les temps d'interruption sont définis ici pour que le temps d'échantillonnage soit le même pour les deux moteurs
    echActuelInterruptionG = millis();

    echDelta = (double)echActuel - echPrecedent;           // delta de la mesure
    if (echDelta >= k)                                     // On s'assure ici que la mesure fait au moins 10 millisecondes
    {
      echDeltaInterruptionD = echActuelInterruptionD - echPrecedentInterruptionD; // on définit les deux delta d'interruption
      echDeltaInterruptionG = echActuelInterruptionG - echPrecedentInterruptionG;

      if (echDeltaInterruptionD >= 200 * incertitude)    // On vérifie que le moteur est à l'arrêt
      {
        frequenceD = 0;                                // 40 mS est le temps maximum que prend un tick à la vitesse du moteur la plus basse
      }
      if (echDeltaInterruptionG >= 200 * incertitude)
      {
        frequenceG = 0;
      }

      Wd = incertitude * ((2 * PI) / nbDeTicks) * frequenceD; // vitesse angulaire en rad/s
      Vd = Wd * (diametre / 2);                               // vitesse linéraire en cm/s
      Wg = incertitude * ((2 * PI) / nbDeTicks) * frequenceG;
      Vg = Wg * (diametre / 2);

      odometrie();

      Serial.print(x);
      Serial.print(" ");
      Serial.println(y);
      echPrecedent = echActuel;
    }
  }
}

void Move(int gauche, int droite, bool L_A, bool R_A)
{
  if (PS4.isConnected())
  {
    if ((gauche <= -dead || gauche >= dead) || (droite <= -dead || droite >= dead))
    {
      int valueR = 0;
      int valueL = 0;
      if ((droite <= -dead || droite >= dead))
        valueR = min((int)map(abs(droite), 0, 110, 0, 255), 255);
      if (gauche <= -dead || gauche >= dead)
        valueL = min((int)map(abs(gauche), 0, 110, 0, 255), 255);

      if (L_A)
      {
        ledcWrite(1, round(valueL));
        ledcWrite(2, 0);
      }
      else
      {
        ledcWrite(1, 0);
        ledcWrite(2, round(valueL));
      }

      if (R_A)
      {
        ledcWrite(3, round(valueR));
        ledcWrite(4, 0);
      }
      else
      {
        ledcWrite(3, 0);
        ledcWrite(4, round(valueR));
      }
    }
    else
    {
      ledcWrite(1, 0);
      ledcWrite(2, 0);
      ledcWrite(3, 0);
      ledcWrite(4, 0);
    }
  }
}

void ISRdroite()                                           // Interruption service routine : la routine associée au traitement des interruptions
{
  compteurTicksActuelD++;                                // on incrémente le compteur de ticks
  CD++;
  if (CD == incertitude)                                 // si le compteur d'appels atteint l'incertitude
  {
    float moyenne = 0;
    for (int i = 0; i < tailleVecteur - 1; i++)        // on décale les données du vecteur vers la gauche pour faire de la place au nouveau
    {
      vecteurD[i] = vecteurD[i + 1];
    }
    vecteurD[tailleVecteur - 1] = echDeltaInterruptionD; // on ajoute donc la nouvelle valeur au plus à droite

    for (int i = 0; i < tailleVecteur; i++)
    {
      moyenne += vecteurD[i] ;
    }
    moyenne = moyenne / tailleVecteur;                 // calcul final de la moyenne
    echDeltaInterruptionD = moyenne;                   // on actualise la valeur du delta de l'interruption actuelle
    frequenceD = (1000) / echDeltaInterruptionD;       // fréquence d'interruption
    echPrecedentInterruptionD = echActuelInterruptionD;// on actualise la valeur du delta de l'interruption précédente
    CD = 0;                                            // On réinitialise le compteur d'appels
  }
}

void ISRgauche()
{
  compteurTicksActuelG++;
  CG++;
  if (CG == incertitude)
  {
    float moyenne = 0;
    for (int i = 0; i < tailleVecteur - 1; i++)
    {
      vectorG[i] = vectorG[i + 1];
    }
    vectorG[tailleVecteur - 1] = echDeltaInterruptionG;

    for (int i = 0; i < tailleVecteur; i++)
    {
      moyenne += vectorG[i];
    }
    moyenne = moyenne / tailleVecteur;
    echDeltaInterruptionG = moyenne;

    echPrecedentInterruptionG = echActuelInterruptionG;
    CG = 0;
  }
}

void odometrie()
{

  deltaTicksD = compteurTicksActuelD - compteurTicksPrecedentD;    // définition du delta
  distanceD = PI * diametre * (deltaTicksD / (double)20);          // distance parcourue par la roue droite depuis le dernier calcul

  deltaTicksG = compteurTicksActuelG - compteurTicksPrecedentG;
  distanceG = PI * diametre * (deltaTicksG / (double)20);          // distance parcourue par la roue gauche depuis le dernier calcul

  distanceCentre = (distanceD + distanceG) / 2;                    // distance parcourue par le point central depuis le dernier calcul

  x += distanceCentre * cos(phi);                               // position en X du point central actuel
  y += distanceCentre * sin(phi);                               // position en Y du point central actuel

  phi  += ((distanceD - distanceG) / entraxe);                 // position angulaire du point central actuel
  phi = atan2(sin(phi), cos(phi));                                 // transformation de la position angulaire entre -PI et PI

  compteurTicksPrecedentD = compteurTicksActuelD;                  // actualisation de la variable précédente droite
  compteurTicksPrecedentG = compteurTicksActuelG;                  // actualisation de la variable précédente gauche
}
