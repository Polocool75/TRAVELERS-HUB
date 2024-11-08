#include <PS4Controller.h>

//#include <WiFi.h>
//#include <WiFiUdp.h>

const short inputPin_left = 25;
const short inputPin_right = 22;
const short sens_left = 32;
const short sens_right = 21;
const short entraxe = 210; // a verifier
const short diametre  = 63; // a verifier

/*const char * networkName = "ntw_TRAVELERS";
  const char * networkPswd = "TRAVELERS";
  const char * udpAddress = "10.3.141.50";
  const short udpPort = 3333;
  boolean connected = false;
  WiFiUDP udp;*/

int tikleft = 0;
int tikright = 0;
double x = 0;
double y = 0;
double teta = 0;
int timerT = 0;

const short dead = 40; //zone morte de la manette
const short freq = 500; // Fréquence du PWM
const short resolution = 8; // Resolution (8 octets = 256 valeurs prenables pour le PWM, de 0 à 255)

const short Motor1A = 13;
const short Motor1R = 27;

const short Motor2A = 18;
const short Motor2R = 17;

const short On_Off = 2;

bool carre = false;

void setup() {
  PS4.begin("02:02:02:02:02:02"); //Lance la connection à la manette PS4 (le paramètre est l'adresse MAC de la manette)
  Serial.begin(115200);
  //connectToWiFi(networkName, networkPswd);

  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1R, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2R, OUTPUT);
  pinMode(On_Off, OUTPUT);
  pinMode(inputPin_left, INPUT_PULLDOWN);
  pinMode(inputPin_right, INPUT_PULLDOWN);
  pinMode(sens_left, INPUT_PULLDOWN);
  pinMode(sens_right, INPUT_PULLDOWN);

  digitalWrite(On_Off, LOW);

  attachInterrupt(digitalPinToInterrupt(inputPin_left), interruptionleft, RISING);
  attachInterrupt(digitalPinToInterrupt(inputPin_right), interruptionright, RISING);

  for (int i = 1; i <= 4; i++)
  {
    /* Fonctionnement du PWM avec l'ESP32:
       Tout d'abord créer un "programme pré-enregistré" avec ledcSetup([numéro du programme (1 à 16)],[fq du PWM], [résolution du PWM])
       Ensuite, on "attache" un pin à un programme avec ledcAttachPin([pin],[numéro du programme])
       Attention, on ne peut pas attacher plusieurs pins à un même programme (ici, on a donc du créer 12 programmes différents ayant les mêmes caractéristiques)

       Enfin, pour utiliser le PWM, on utiliser ledcWrite([numéro du programme],[valeur du PWM])

    */
    ledcSetup(i, freq, resolution); // Tous les programmes/channels de 1 à 12 auront donc comme fq 500hz et 8 de résolution
  }

  ledcAttachPin(Motor1A, 1);
  ledcAttachPin(Motor1R, 2);
  ledcAttachPin(Motor2A, 3);
  ledcAttachPin(Motor2R, 4);
  timerT = millis();
}

void loop() {
  
  if (PS4.isConnected())// Le code ne se lance que si la manette PS4 est connecté
  {
    short RStick = PS4.RStickY(); // Valeur de l'intensité d'inclinaison du joystick droit (entre 127 et -128)
    bool R_A = PS4.RStickY() > 0; // Booléen indiquant si l'on veut avancer ou non (pour le joystick droit)
    short LStick = PS4.LStickY(); // idem pour joystick gauche
    bool L_A = PS4.LStickY() > 0; //idem pour joystick gauche

    if (PS4.Square()) //Si le bouton carré est pressé
    {
      carre = !carre;
      if (carre)
        digitalWrite(On_Off, HIGH);
      else
        digitalWrite(On_Off, LOW);
      delay(100);
    }
    Move(LStick, RStick, L_A, R_A);
    position(PI * diametre * tikleft / 341, PI * diametre * tikright / 341, &x, &y, &teta); // 2*pi*rayon d'une roue / 1024 (nb d'impulsions par tour)
    if (millis() - timerT>1000) {
      Serial.print("[");
      Serial.print(x);
      Serial.print(" , ");
      Serial.print(y);
      Serial.print(" , ");
      Serial.print(teta);
      Serial.println("]");
      timerT = millis();
    }
    /*if (connected) {
      //Send a packet
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("Seconds since boot: %u", millis() / 1000);
      udp.endPacket();
      }
      delay(50);
      }*/
  }
  delay(50);
}

void Move(int gauche, int droite, bool L_A, bool R_A)
{
  if (PS4.isConnected())
  {
    if ((gauche <= -dead || gauche >= dead) || (droite <= -dead || droite >= dead))//
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
/*void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
  }

  //wifi event handler
  void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(WiFi.localIP(), udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
  }
  }*/

void interruptionleft() {
  if (digitalRead(sens_left)) {
    tikleft++;
  }
  else {
    tikleft--;
  }
}

void interruptionright() {
  if (!digitalRead(sens_right)) {
    tikright++;
  }
  else {
    tikright--;
  }
}

void position(double dleft, double dright, double * px, double * py, double * pteta) {
  double dcenter = (dleft + dright) / 2;
  double phi = (dright - dleft) / entraxe;
  *px = x + dcenter * cos(*pteta + phi / 2);
  *py = y + dcenter * sin(*pteta + phi / 2);
  *pteta = teta + phi;
  tikright = 0;
  tikleft = 0;
}
