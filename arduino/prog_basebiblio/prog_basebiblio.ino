//----------------------------------------librairies--------------------------------------

#include "capt_ultr_son.h"
#include "capt_capacitif.h"
#include "ambiances.h"
#include <bandeauLEDRGB.h>

/*******************************variables globales*****************************************/


//variables pour gerer la temporisation
unsigned int delais = 0 ;
unsigned int temps ; 

int ambiance = -1 ; //chaque valeur de la variable correspond à une ambiance
int increment = 0 ; // cette variable pernet de faire une routine par rapport au temps

//variables des mesures de couleur à envoyer
float valrouge ;
float valbleu ;
float valvert ;

//--------------------------------------bandeaux-----------------------------------------------
/*
 * ces variables servent à activer des routines qui vont se rajouter aux ambiances en fonction des 
 * demandes de l'ordinateur comme faire briller les led avec la musique. 
 */

bandeauLEDRGB bandeau_pilier_1(3,5,6, false) ;


/******************************************************************************************************/
// communication valeurs capteurs pc



void envoyer()
{
  
}

void setup()
{

  Serial.begin(115200);// début de la communication avec l'ordinateur avec une vitesse de 115200 (à synchroniser avec l'ordinateur)
 // bandeau_pilier_1.bandeauLEDRGB(3,5,6) ;
  petale1.set_CS_AutocaL_Millis(0xFFFFFFFF) ; // initialisation du capteur capacitif du lotus
  bandeau_pilier_1.istrue() ;
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
 * lors d'une temporisation, elle s'execute "à vide" en lieu et place de l'instruction delay pour rester à l'écoute des autre processus.
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
        valrouge = (increment%100)*0.4+capt1-20 ; // l'intensité varie périodiquement avec une phase décalée d'un tiers pour chaque couleur de led.
        valbleu = ((increment+33)%100)*0.4+capt1-20 ; // l'intensité des led est compris entre capt1-20 et capt1+20 
        valvert = ((increment+66)%100)*0.4+capt1-20 ;
        bandeau_pilier_1.ledpwm(int(valrouge),0) ; //  
        bandeau_pilier_1.ledpwm(int(valvert),1) ;
        bandeau_pilier_1.ledpwm(int(valbleu),2) ;
        
        delais = 50 ; 
        break ;
        
/*Scène de combat :
 * dans le lotus deux couleurs s'affrontent.
 * les deux couleurs sont bleu et rouge
 * on met un bandeau de chaque coté à la couleur voulu et les deux autres vont 
 * tour à tour changer de couleur.
 * les bandeaux du bas vont gagne en intensité en fonction du bruit
 */
       case 2 :
        

        
        valrouge = (sin((increment%100)*2*PI/100)+1)*capt1+capt2 ;      // on prend une fonction périodique entre 0 et 2 qu'on multiplie avec la distance du capteur principal.
        valbleu = (sin(((increment+50)%100)*2*PI/100)+1)*capt1+capt3 ;  // cettte fonction va faire fluctuer les couleurs rouges et bleu sur les bandeaux intermédiaires.
        
        
        delais : 100 ;
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
