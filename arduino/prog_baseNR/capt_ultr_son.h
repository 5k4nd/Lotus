#ifndef CAPT_ULTR_SON
#define CAPT_ULTR_SON
#include <NewPing.h>

/************************* les capteur à ultrasons sont alimentés en 5V**************************/

// capt1
#define CAPT1_TRIGGER_PIN 22   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define CAPT1_ECHO_PIN 24  // Arduino pin tied to echo pin on the ultrasonic sensor.

// capt2
#define CAPT2_TRIGGER_PIN 26 
#define CAPT2_ECHO_PIN 28 

// capt3
#define CAPT3_TRIGGER_PIN 30 
#define CAPT3_ECHO_PIN 32 

// capt4
#define CAPT4_TRIGGER_PIN 34 
#define CAPT4_ECHO_PIN 36 

#define MAX_DISTANCE  200 // Distance maximum de detection souhaitée en cm. distance max capteur 400-500cm



//-------------------------------------initialisation des capteurs-----------------------------------

NewPing sonar1(CAPT1_TRIGGER_PIN,CAPT1_ECHO_PIN, MAX_DISTANCE); // configuration du capteur.
NewPing sonar2(CAPT2_TRIGGER_PIN,CAPT2_ECHO_PIN, MAX_DISTANCE);
NewPing sonar3(CAPT3_TRIGGER_PIN,CAPT3_ECHO_PIN, MAX_DISTANCE);
NewPing sonar4(CAPT4_TRIGGER_PIN,CAPT4_ECHO_PIN, MAX_DISTANCE);

//-------------------------------------variables globales des capteurs ultrason------------------------------------------
unsigned int capt1 = 0 ;
unsigned int capt2 = 0 ;
unsigned int capt3 = 0 ;
unsigned int capt4 = 0 ;

//-----------------------fonction pour prendre les mesures de chaque capteur-----------------
// les fonctions sont directement implementées dans la librairie
void mesure_ultrason()
{
  capt1 = sonar1.ping_cm() ;
  capt2 = sonar2.ping_cm() ;
  capt3 = sonar3.ping_cm() ;
  capt4 = sonar4.ping_cm() ;
}




#endif
