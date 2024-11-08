#include "Wire.h"

#define I2C_DEV_ADDR 0x11

/* Structure des messages I2C (String):
    "[1],[2],[3],[4]"
  Pour se déplacer Terrestre / Aquatique:
    [1] : "M" / "A" -> Terrestre / Aquatique
    [2] : int() -> Valeurs du PWM ([0,255])
    [3] : "L" ou "R" -> Moteurs gauche (L) ou droit (R)
    [4] : "A" ou "R" -> Sens avant (A) ou arrière (R)
  Boutons :
    [1] : "B" -> Boutons
    [2] : "1", "2", "3", "4", "5" -> Carré, Triangle, Rond, Croix, Start
*/

const int freq = 1000;             // Fréquence du PWM
const int resolution = 8;        // Resolution (8 octets = 256 valeurs prenables pour le PWM, de 0 à 255)

const int Motor1A = 23;
const int Motor1R = 17;

const int Motor2A = 5;
const int Motor2R = 19;

const int Motor3A = 27;
const int Motor3R = 13;

const int Motor4A = 25;
const int Motor4R = 26;

const int enable = 2;
bool carre = false;

volatile boolean receiveFlag = false;
char temp[32];
int gauche, droite;
String dir;

void onRequest() {
}

void onReceive(int len) {
  for (int j = 0; j < len; j++)
  {
    temp[j] = Wire.read();
    temp[j + 1] = '\0';
  }
  for (int j = 0; j < len; j++)
  {
    temp[j] = temp[j + 1];
  }
  Serial.println("interruption");
  receiveFlag = true;

}

void setup()
{
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest); // On mettra la les données de l'odo
  Wire.begin((uint8_t)I2C_DEV_ADDR);

  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1R, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2R, OUTPUT);
  pinMode(Motor3A, OUTPUT);
  pinMode(Motor3R, OUTPUT);
  pinMode(Motor4A, OUTPUT);
  pinMode(Motor4R, OUTPUT);
  pinMode(enable, OUTPUT);

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
  Serial.begin(9600);

}

void MoveValues(String input_str, int& val1, int& val2, String& val3) {
  int index = 2; // initialise l'index à 2 pour pas compter M
  String temp_str = ""; // initialise la chaîne temporaire
  int count = 0; // initialise le compteur de valeurs
  while (index < input_str.length()) { // tant qu'il y a des caractères dans la chaîne
    char current_char = input_str.charAt(index); // récupère le caractère actuel
    if (current_char == ',') { // si c'est une virgule
      count++; // incrémente le compteur de valeurs
      if (count == 1) { // si c'est la première valeur/deuxième virgule
        val1 = temp_str.toInt(); // convertit la chaîne en entier et l'assigne à la première valeur
      } 
      else if (count == 2) { // si c'est la deuxième valeur/3eme Virgule
        val2 = temp_str.toInt(); // convertit la chaîne en entier et l'assigne à la deuxième valeur                     
      }
      temp_str = ""; // réinitialise la chaîne temporaire
    } 
    else { // si ce n'est pas une virgule
      temp_str += current_char; // ajoute le caractère à la chaîne temporaire
    }
    index++; // incrémente l'index
  }
  if (input_str.length() > 5) // Sinon c'est qu'on est a pwm = 0 ou Motor Aqua
    val3 = String(temp_str);
  else
  {
    val3 = "N";
  }
  temp_str = "";

}

void ButtonAction(String input_str)
{
  int index = 2; // initialise l'index
  String button_str = "";
  int button;
  while (index < input_str.length())
  {
    button_str += input_str.charAt(index);
    index++;
  }
  button = button_str.toInt();
  switch (button)
  {
    case 1: // Carré
      carre = !carre;
      if (carre)
        digitalWrite(enable, HIGH);
      else
        digitalWrite(enable, LOW);
      break;

    case 2: // Triangle
      break;

    case 3: // Rond
      break;

    case 4: // Croix
      break;

    case 5: // Start
      break;

    default:
      break;
  }
}

void loop() {
  delay(10);
  String mess = String(temp);
  if (receiveFlag == true)
  {
    if (mess.charAt(0) == 'M' || mess.charAt(0) == 'A')
    {
      MoveValues(mess, gauche, droite, dir);
        if (dir == "A") // Avance
        {
          Serial.println("LA");
          ledcWrite(1, round(gauche));
          ledcWrite(2, 0);
          ledcWrite(5, round(gauche));
          ledcWrite(6, 0);
        }
        else // Recule
        {
          if (dir == "R")
          {
            Serial.println("LR");
            ledcWrite(1, 0);
            ledcWrite(2, round(gauche));
            ledcWrite(5, 0);
            ledcWrite(6, round(gauche));
          }
          else
          {
            Serial.println("LS");
            ledcWrite(1, 0);
            ledcWrite(2, 0);
            ledcWrite(5, 0);
            ledcWrite(6, 0);
          }
        }
      
        if (dir == "A") // Avance
        {
          Serial.println("RA");
          ledcWrite(3, round(droite));
          ledcWrite(4, 0);
          ledcWrite(7, round(droite));
          ledcWrite(8, 0);
        }
        else // Recule
        {
          if (dir == "R")
          {
            Serial.println("RR");
            ledcWrite(3, 0);
            ledcWrite(4, round(droite));
            ledcWrite(7, 0);
            ledcWrite(8, round(droite));
          }
          else
          {
            Serial.println("RS");
            ledcWrite(3, 0);
            ledcWrite(4, 0);
            ledcWrite(7, 0);
            ledcWrite(8, 0);
          }
        }
      }
    }

    //On sait dans quelle direction, sur quelle roue et a quelle puissance on va
  else
  {
    ButtonAction(mess);
  }
  receiveFlag = false;
}
