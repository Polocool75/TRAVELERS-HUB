#include <ESP32_Servo.h> // Bibliothèque pour faire marcher un moteur servo avec un ESP32
#include<PS4Controller.h> // Bilbliothèque pour connecter la manette PS4

#define MIN_PULSE_LENGTH 1000 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 2000 // Maximum pulse length in µs
/* Ces pulsations peuvent être modifié mais ça peut peut-être abimer le moteur
  J'ai testé un max de 3000 ça marchait mais ça ne changeait rien (il plafonne)
  J'éviterai de toucher au min... ça marche, laissons ça comme ça */

Servo motA; // Declare le premier moteur (a renommer motR ou motL pour gauche droite)

const int dead = 30; // La zone morte du stick analogique PS4 (ça permet qu'une micro-variation de position du stick ne soit pas prise en compte)
int mode = 0; // pour l'instant, mode prendra 2 valeur dans le programme (0: phase d'initialisation, et 1 : mode normal)
bool active = false; // Booléen utilisé dans l'initialisation (au niveau de la touche triangle)

void setup() {
  PS4.begin("03:03:03:03:03:03");// Adresse IMAC de la manette PS4 à connecter, à laisser comme ça
  pinMode(32, OUTPUT); // Pin 32 de l'ESP (je crois c'est un PWM, jsp si ça marche avec tous les pins)
  motA.attach(32, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH); // Le moteur est alors "atttaché" au pin 32 avec un min et un max de pulse
}

void loop() {
  if (PS4.isConnected()) {
    if (mode == 0)
      // Ce bloc est la partie initialisation (on appuie sur cercle pour en sortir
      // MAIS il faut avoir fait la manip de mise en marche (voir le READ_ME)
    {
      if (PS4.Triangle()) {
        if (active == false)
        {
          motA.writeMicroseconds(MAX_PULSE_LENGTH);
          active = !active; // On change active comme ça quand on appuie de nouveau sur triangle on va dans l'autre bloc
        }
        else
        {
          motA.writeMicroseconds(MIN_PULSE_LENGTH);
          active = !active;
        }
      }
      if (PS4.Circle()) {
        mode = 1;
      }
      if (PS4.Square()) {
        motA.writeMicroseconds((MAX_PULSE_LENGTH + MIN_PULSE_LENGTH) / 2 );
      }

    }
    if (mode == 1) 
    // Mode normal ou l'on peut accelerer avec le joystick droit (pour l'instant)
    {
      int RStick = PS4.RStickY();
      bool R_A = PS4.RStickY() > dead; // Si la valeur est plus grande que la zone morte alors on avancera (true)
      int valueR = map(abs(RStick), 0, 129, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH); // On remap les valeurs
      if (R_A) {
        motA.writeMicroseconds(valueR);
      }
      else
      {
        motA.writeMicroseconds(MIN_PULSE_LENGTH);
      }
    }

  }
  delay(100); // Ce delai est à laisser
}
