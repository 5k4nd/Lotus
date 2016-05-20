//----------------------------------------librairies--------------------------------------

#include "capt_ultr_son.h"
#include "capt_capacitif.h"
#include "bandeau_led.h"
#include "ambiances.h"

//-------------------------------------variables globales------------------------------------------

unsigned int delais = 0 ;
unsigned int temps ; 
int ambiance = -1 ; //chaque valeur de la variable correspond à une ambiance
int increment = 0 ;


float valrouge ;petale.set_CS_AutocaL_Millis(0xFFFFFFFF) ; // initialisation du capteur capacitif du lotus
float valbleu ;
float valvert ;





// communication valeurs capteurs pc
void envoyer()
{
  
}

void setup()
{

  Serial.begin(115200);// début de la communication avec l'ordinateur avec une vitesse de 115200 (à synchroniser avec l'ordinateur)
  
  pinMode (LEDVERT,OUTPUT); // Broche ledVert configurée en sortie
  pinMode (LEDROUGE,OUTPUT); // Broche ledRouge configurée en sortie
  pinMode (LEDBLEU,OUTPUT); // Broche ledBleu configurée en sortie
  petale.set_CS_AutocaL_Millis(0xFFFFFFFF) ; // initialisation du capteur capacitif du lotus
  temps = millis() ;
}


/**********************************problématique du temps réel multitache en monoprocessus********************************
 * le problème devant faire un certain nombre d'action en simultané( allumage des led, prise de mesure des capteurs, dialogue avec l'ordinateur)
 * on ne peut utiliser la fonction delay pour gerer le temps d'allumage des leds, celle-ci bloqurais le programme.
 * 
 * On utilise donc une alternative à cette fonction en créant une variable qui fera tourner un certain nombre de fois la boucle principale pour "attendre"
 * On utilise par ailleurs la fonction serialEvent, déjà implémentées dans le langage arduino qui s'éxecute entre chaque boucle loop. 
 * On va lui déleguer les prise de mesures et communication pour ne mettre que la gestion des led dans la boucle principale. 
 * 
 * 
 * **********************************************principe du programme des leds********************************************
 * la boucle principale est un choix entre les différentes ambiance que l'on aura codé au préalable.
 * 
 */



void loop() {
  if (temps+delais>millis())
  {
    switch(ambiance)
    {
/* ce programme fait fluctuer la couleur des bandeaux led en fonction du temps. 
 * l'intensité globale est fonction de la distance des gens avec le capteur ultrason.
 * 
 */
      case -1 ://avant d'avoir été touché une première fois

       
        increment++ ;
        valrouge = (increment%100)*0.4)+capt1-20 ; // l'intensité varie périodiquement avec une phase décalée d'un tiers pour chaque couleur de led.
        valbleu = ((increment+33)%100)*0.4+capt1-20 ; // l'intensité des led est compris entre capt1-20 et capt1+20 
        valvert = ((increment+66)%100)*0.4+capt1-20 ;
        ledpwm(int(valrouge),LEDROUGE) ; 
        ledpwm(int(valvert),LEDVERT) ;
        ledpwm(int(valbleu),LEDBLEU) ;
        
        delais = 50 ; 
        break ;
/*
 * 
 * 
 * 
 * 
 */
       case 2 :




        break ;

    }
  
  temps = millis() ;

  }
  
}


void serialEvent() {
  mesure_ultrason() ;
  mesure_tactile() ;

  envoyer() ;

}
