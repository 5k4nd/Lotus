#include <CapacitiveSensor.h>
#include <NewPing.h>

//-----------------------capteurs---------------------------------------
// capt1
const int capt1_trigger_pin = 46 ;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int capt1_echo_pin = 52 ; // Arduino pin tied to echo pin on the ultrasonic sensor.
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
CapacitiveSensor petale = CapacitiveSensor(28,30) ; // résistance de 2,2K entre pin 50 et 52

const int MAX_DISTANCE  = 200 ;// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(capt1_trigger_pin, capt1_echo_pin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//NewPing sonar2(capt2_trigger_pin,capt2_echo_pin, MAX_DISTANCE);
//NewPing sonar3(capt3_trigger_pin,capt3_echo_pin, MAX_DISTANCE);
//NewPing sonar4(capt4_trigger_pin,capt4_echo_pin, MAX_DISTANCE);

const int ledVert = 3; // Configuration de la broche 3 (PWM) de l'Arduino à ledVert
const int ledRouge = 5; // Configuration de la broche 5 (PWM) de l'Arduino à ledRouge
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

int increment = 1 ;
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
//  pinMode (petale, INPUT) ; // interrupteur pétale en entrée
//  digitalWrite(petale, HIGH) ; // on met une résistance de rappel
  pinMode (ledVert, OUTPUT); // Broche ledVert configurée en sortie
  pinMode (ledRouge, OUTPUT); // Broche ledRouge configurée en sortie
  pinMode (ledBleu, OUTPUT); // Broche ledBleu configurée en sortie
  Serial.begin(115200) ;
  petale.set_CS_AutocaL_Millis(0xFFFFFFFF) ;
 }



void loop() {
   if (petale.capacitiveSensor(30)>100) 
   {
    increment++ ;
    delay(500) ;
    if (increment == 4) increment = 1 ;
   }

  if (increment == 1)
  {  
    prendreMesures() ; // on mesure la distance du capteur
    ledRVBpwm(255, 255, 255) ;  // on éteint le bandeau
    delay(capt1) ;  // on attend la valeur du capteur
    ledRVBpwm(0, 0, 0) ;  // on rallume le bandeau
    delay(capt1) ;   // on attends de la valeur du capteur
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");
  }
  else if (increment == 2)
  {
    prendreMesures() ;
    valVert = capt1 ;
    valBleu = capt1 ;
    valRouge = capt1 ;
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");

    ledRVBpwm(255-capt1*3, 255-capt1*3, 255-capt1*3) ; // on allume le bandeau avec une intensité proportionnelle au capteur
    delay(200) ;
  }
  else
  {
    int i ;
    for( i = 0 ; i < 255 ; i+=10)
    {
      prendreMesures() ;
      ledRVBpwm(i, 0,0) ;
      delay(capt1) ;
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");     
    }
      for(i = 0 ; i < 255 ; i+=10)
    {
      prendreMesures() ;
      ledRVBpwm(0,0,i) ;
      delay(capt1) ;
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");
    }
        for( i = 0 ; i < 255 ; i+=10)
    {
      prendreMesures() ;
      ledRVBpwm(0, i,0) ;
      delay(capt1) ;
      
    Serial.print("{'capteur1':" );
    Serial.print(capt1);
    Serial.println("}");
    }
  }
}
