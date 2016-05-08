#include <CapacitiveSensor.h>
#include <NewPing.h>
#include "capt_ultr_son.h"
#include "capt_capacitif.h"
//-------------------------------------bandeau led------------------------------------------------
const int ledVert = 5; // Configuration de la broche 3 (PWM) de l'Arduino à ledVert
const int ledRouge = 3; // Configuration de la broche 5 (PWM) de l'Arduino à ledRouge
const int ledBleu = 6; // Configuration de la broche 6 (PWM) de l'Arduino à ledBleu

//----------------------------------------lotus--------------------------------------



//-------------------------------------variables globales------------------------------------------

unsigned int delais = 0 ;
unsigned int temps ; 
volatile int ambiance = -1 ; //chaque valeur de la variable correspond à une ambiance
volatile int increment = -1 ;

int valBleu ;
int valRouge ;
int valVert ;

//------------------fonction pour colorer les led--------------------------------------
void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) // reçoit valeur 0-255 par couleur
{
  analogWrite(ledRouge, pwmRouge);
  analogWrite(ledVert, pwmVert);
  analogWrite(ledBleu, pwmBleu);
}

//-----------------------fonction pour prendre les mesures de chaqque capteur
void prendreMesures()
{
  capt1 = sonar1.ping_cm() ;
  capt2 = sonar2.ping_cm() ;
  capt3 = sonar3.ping_cm() ;
  capt4 = sonar4.ping_cm() ;
}



// communication de l'ambiance avec le PC
void envoyer_ambiance(int ambiance)
{
  
}
// communication valeurs capteurs pc
void envoyer()
{
  
}

void setup()
{

  Serial.begin(115200);// début de la communnication avec l'ordinateur
  
  pinMode (ledVert,OUTPUT); // Broche ledVert configurée en sortie
  pinMode (ledRouge,OUTPUT); // Broche ledRouge configurée en sortie
  pinMode (ledBleu,OUTPUT); // Broche ledBleu configurée en sortie

  temps = millis() ;
}






void loop() {
  if (temps+delais>millis())
  {
    prendreMesures() ;
    envoyer() ;
    switch(ambiance)
    {
      case 1 :
        
        
        /*
        
        on fait les traitements de la lumière en fonction des détections


        */
        
        break ;

       default : 
       prendreMesures() ;
       envoyer() ;
       valRouge = capt1 ;
       valVert = capt1 ;
       valBleu = capt1 ;
    }
  
  
  
    increment++ ;
    ledRVBpwm(valRouge, valVert, valBleu) ;
    temps = millis() ;
  }
  
}


void serialEvent() {

  envoyer() ;

}
