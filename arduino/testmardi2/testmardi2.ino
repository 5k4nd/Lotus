#include <CapacitiveSensor.h>
#include <NewPing.h>

//-----------------------capteurs---------------------------------------
// capt1
const int capt1_trigger_pin = 22 ;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int capt1_echo_pin = 24 ; // Arduino pin tied to echo pin on the ultrasonic sensor.
/*
  // capt2
  const int capt2_trigger_pin = 26 ;
  const int capt2_echo_pin = 28 ;

  // capt3
  const int capt3_trigger_pin = 30 ;
  const int capt3_echo_pin = 32 ;

  // capt4
  const int capt4_trigger_pin = 34 ;
  const int capt4_echo_pin = 36 ;
*/
//pétale
//CapacitiveSensor petale = CapacitiveSensor(50,52) ; // résistance de 2,2K entre pin 50 et 52
const int petale = 30 ;

const int MAX_DISTANCE  = 200 ;// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(capt1_trigger_pin, capt1_echo_pin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//NewPing sonar2(capt2_trigger_pin,capt2_echo_pin, MAX_DISTANCE);
//NewPing sonar3(capt3_trigger_pin,capt3_echo_pin, MAX_DISTANCE);
//NewPing sonar4(capt4_trigger_pin,capt4_echo_pin, MAX_DISTANCE);

const int ledVert = 5; // Configuration de la broche 3 (PWM) de l'Arduino à ledVert
const int ledRouge = 3; // Configuration de la broche 5 (PWM) de l'Arduino à ledRouge
const int ledBleu = 6; // Configuration de la broche 6 (PWM) de l'Arduino à ledBleu

unsigned int capt1 = 0 ;
unsigned int capt2 = 0 ;
unsigned int capt3 = 0 ;
unsigned int capt4 = 0 ;

void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) // reçoit valeur 0-255 par couleur
{
  if (pwmRouge > 255) pwmRouge = 255 ;
  if (pwmRouge < 0) pwmRouge = 0 ;
  if (pwmRouge > 255) pwmVert = 255 ;
  if (pwmRouge < 0) pwmVert = 0 ;
  if (pwmRouge > 255) pwmBleu = 255 ;
  if (pwmRouge < 0) pwmBleu = 0 ;
  analogWrite(ledRouge, pwmRouge);
  analogWrite(ledVert, pwmVert);
  analogWrite(ledBleu, pwmBleu);
}

bool increment = false ;
int valBleu ;
int valRouge ;
int valVert ;

void prendreMesures()
{
  capt1 = sonar1.ping_cm() ;
  // capt2 = sonar2.ping_cm() ;
  // capt3 = sonar3.ping_cm() ;
  // capt4 = sonar4.ping_cm() ;
}


void setup() {
  pinMode (petale, INPUT) ; // interrupteur pétale en entrée
  digitalWrite(petale, HIGH) ; // on met une résistance de rappel
  pinMode (ledVert, OUTPUT); // Broche ledVert configurée en sortie
  pinMode (ledRouge, OUTPUT); // Broche ledRouge configurée en sortie
  pinMode (ledBleu, OUTPUT); // Broche ledBleu configurée en sortie
  Serial.begin(115200) ;
  // petale.set_CS_AutocaL_Millis(0xFFFFFFFF) ;
 }



void loop() {
  // if (petale.capacitiveSensor(30)>100) increment=!increment ;
  //if (digitalRead(petale)) increment = !increment ;
 // if (digitalRead(petale))
 // {
 //   increment = !increment ;// si on détecte le bouton, on inverse le programme
 //   delay(300) ;
 // }
  if (increment)
  {
    Serial.println("mode 2") ;

    prendreMesures() ; // on mesure la distance du capteur
    ledRVBpwm(255, 255, 255) ;  // on éteint le bandeau
    delay(capt1) ;  // on attend la valeur du capteur
    ledRVBpwm(0, 0, 0) ;  // on rallume le bandeau
    delay(capt1) ;   // on attends de la valeur du capteur
  }
  else
  {
    //Serial.println("mode 1 :") ;
    prendreMesures() ;
    //valVert = capt1 ;
    //valBleu = capt1 ;
    //valRouge = capt1 ;
    if (1 == 1) { Serial.println(42); }
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");

    //ledRVBpwm(valRouge * 3, valBleu * 3, valVert * 3) ; // on allume le bandeau avec une intensité proportionnelle au capteur
    delay(200) ;
  }

}