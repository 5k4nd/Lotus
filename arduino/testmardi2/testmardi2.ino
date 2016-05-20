#include <CapacitiveSensor.h>
#include <NewPing.h>

//-----------------------capteurs ultrason---------------------------------------
// les capteur à ultrasons sont alimentés en 5V
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

const int MAX_DISTANCE  = 200 ;// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(capt1_trigger_pin, capt1_echo_pin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//NewPing sonar2(capt2_trigger_pin,capt2_echo_pin, MAX_DISTANCE);
//NewPing sonar3(capt3_trigger_pin,capt3_echo_pin, MAX_DISTANCE);
//NewPing sonar4(capt4_trigger_pin,capt4_echo_pin, MAX_DISTANCE);


//------------------------------------lotus-----------------------------------------------------------
// connecter le capteur métallique sur la broche de sortie (ici 52) sans masse
CapacitiveSensor petale = CapacitiveSensor(28,30) ; // résistance de 1MΩ entre pin 28(alimentation) et 52(sortie).

//--------------------------------------bandeau led------------------------------------------------------
/*
 * le bandeau est alimenté en 12V et peux tirer plusieurs ampere, il faut donc mettre des transistors pour faire un circuit de puissance. 
 * Avec le transistor coté non métallique, 
 * brancher le pin de droite sur une résistance de 10KΩ relié à l'arduino
 *          le pin central sur le bandeau
 *          le pin de gauche sur la masse ( commune entre l'alim 12V et l'arduino )
 */

const int ledVert = 3; // Configuration de la broche 3 (PWM) de l'Arduino à ledVert
const int ledRouge = 5; // Configuration de la broche 5 (PWM) de l'Arduino à ledRouge
const int ledBleu = 6; // Configuration de la broche 6 (PWM) de l'Arduino à ledBleu

//--------------------------------------variables globales :--------------------------------------------- 

unsigned int capt1 = 0 ;
//unsigned int capt2 = 0 ;
//unsigned int capt3 = 0 ;
//unsigned int capt4 = 0 ;

int increment = 1 ; // variable pour passer d'un programme à l'autre

int i ; // variable de la troisieme séquence

//----------------------------------fonction de commande du bandeau---------------------------------------
/*
 * La luminosité des led varie en fonction de la tension entre l'entrée et la sortie.
 * L'entrée est réglée sur 12V et les sortie (chaque couleur de led)
 * La tension sur les sorties est commandée par l'arduino aux moyen d'une tension gérée par modulations d'impulsions (PWM)
 * la tension des led est la différence entre le 12V et la sortie de l'arduino [ ex : alim 12V / arduino 3V ==> 9V ]
 * l'arduino ne pouvant sortir que du 5V et une petite intensité, 
 * 
 * -----------------------------------------PWM--------------------------------------------
 * le PWM crée une onde carrée dont on peut choisir le temps qu'il est à 1 par rapport au temps ou il est à 0. Pour l'arduino, 
 * une valeur de 255 donne une onde toujours à 1 et une valeur de 0 donne une onde toujours à 0, une valeur de 127 donne une onde carrée équilibrée
 * la fréquence du signal est suffisament rapide pour approximer la sortie à une tension moyenne : 12V, 0V et 6V pour l'exemple précédent (avec l'amplification)
 */

 

void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) // reçoit valeur 0-255 par couleur
{
  // boucles de sécurité pour ne pas dépasser les valeurs
  if (pwmRouge > 255) pwmRouge = 255 ;
  if (pwmRouge < 0) pwmRouge = 0 ;
  if (pwmRouge > 255) pwmVert = 255 ;
  if (pwmRouge < 0) pwmVert = 0 ;
  if (pwmRouge > 255) pwmBleu = 255 ;
  if (pwmRouge < 0) pwmBleu = 0 ;
  //----------------------------------------
  analogWrite(ledRouge, pwmRouge);// on envoi le signal carré avec une proportion de pwmrouge/255 tmps à 5V par période. 
  analogWrite(ledVert, pwmVert);
  analogWrite(ledBleu, pwmBleu);
}


//---------------------------fonction pour prendre les mesures des capteurs----------------------------------

void prendreMesures()
{
  capt1 = sonar1.ping_cm() ;// fonction de la pibliothèque newping qui renvoi une valeur en cm
  // capt2 = sonar2.ping_cm() ;
  // capt3 = sonar3.ping_cm() ;
  // capt4 = sonar4.ping_cm() ;
}

//---------------------------Fonction d'initialisation--------------------------------------------------
void setup() {
  pinMode (ledVert, OUTPUT); // Broche ledVert configurée en sortie
  pinMode (ledRouge, OUTPUT); // Broche ledRouge configurée en sortie
  pinMode (ledBleu, OUTPUT); // Broche ledBleu configurée en sortie
  Serial.begin(115200) ; // début de la communication avec l'ordinateur avec une vitesse de 115200 (à synchroniser avec l'ordinateur)
  petale.set_CS_AutocaL_Millis(0xFFFFFFFF) ; // initialisation du apteur capacitif du lotus
 }



void loop() {
  
  // le capteur capacitif calcule le temps de charge du systeme assimilé à un condensateur
  // celui-ci est modifié quand on touche le capteur qui crée un deuxième condensateur et crée un effet de résonnance. 
   if (petale.capacitiveSensor(30)>100) // la valeur 100 est donc à étalonner en fonction du capteur.
   {
    increment++ ;
    delay(500) ;
    if (increment == 4) increment = 1 ;
   }
   // la boucle change la valeur d'incrément pour modifier le programme executé.

  switch(increment)
  {
    case 1 :
      prendreMesures() ; // on mesure la distance du capteur
      ledRVBpwm(255, 255, 255) ;  // on éteint le bandeau
      delay(capt1) ;  // on attend la valeur du capteur
      ledRVBpwm(0, 0, 0) ;  // on rallume le bandeau
      delay(capt1) ;   // on attends de la valeur du capteur
      Serial.print("{'capteur1':" );
      Serial.print(capt1);
      Serial.println("}");
    break ;
    
    case 2 : 
      prendreMesures() ;
      Serial.print("{'capteur1':" );
      Serial.print(capt1);
      Serial.println("}");
  
      ledRVBpwm(255-capt1*3, 255-capt1*3, 255-capt1*3) ; // on allume le bandeau avec une intensité proportionnelle au capteur
      delay(200) ;
    break ;

    case 3 :
      
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
        
      Serial.print("{'capteur1':");
      Serial.print(capt1);
      Serial.println("}");
      }
    break ;

    default : increment = 1 ;
  }
}
