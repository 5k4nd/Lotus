#include <TimerOne.h>

//----------------------------------------librairies--------------------------------------
/*******************************communicationpy********************************************/
#include <communicationpy.h>


/******************************************************************************************/


/*******************************variables globales*****************************************/


int ambiance = 0 ; //chaque valeur de la variable correspond à une ambiance
byte increment = 0 ; // cette variable pernet de faire une routine par rapport au temps


byte increment_evenement = 0 ;

//variables des mesures de couleur à envoyer
float valrouge ;
float valbleu ;
float valvert ;
/**********************************************************************************************/
/******************************************capteur ultrason*******************************************
   les capteurs à ultrasons sont alimentés en 5V
*/
#ifndef CAPT_ULTR_SON
#define CAPT_ULTR_SON
#include <NewPing.h>

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

NewPing sonar1(CAPT1_TRIGGER_PIN, CAPT1_ECHO_PIN, MAX_DISTANCE); // configuration du capteur.
NewPing sonar2(CAPT2_TRIGGER_PIN, CAPT2_ECHO_PIN, MAX_DISTANCE);
NewPing sonar3(CAPT3_TRIGGER_PIN, CAPT3_ECHO_PIN, MAX_DISTANCE);
NewPing sonar4(CAPT4_TRIGGER_PIN, CAPT4_ECHO_PIN, MAX_DISTANCE);

//-------------------------------------variables globales des capteurs ultrason------------------------------------------
unsigned int capt1 = MAX_DISTANCE ;
unsigned int capt2 = MAX_DISTANCE ;
unsigned int capt3 = MAX_DISTANCE ;
unsigned int capt4 = MAX_DISTANCE ;

//-----------------------fonction pour prendre les mesures de chaque capteur-----------------
// les fonctions sont directement implementées dans la librairie
inline void mesure_ultrason()
{
  capt1 = sonar1.ping_cm() ; // renvoie la distance en cm
  capt2 = sonar2.ping_cm() ;
  capt3 = sonar3.ping_cm() ;
  capt4 = sonar4.ping_cm() ;
}

#endif
/******************************************************************************************************/
/******************************************BANDEAUX****************************************************/
/* La luminosité des led varie en fonction de la tension entre l'entrée et la sortie.
   L'entrée est réglée sur 12V.
   La tension sur les sorties(chaque couleur de led) est commandée par l'arduino aux moyen d'une tension gérée par modulations d'impulsions (PWM)
   la tension des led est la différence entre le 12V et la sortie de l'arduino [ ex : alim 12V / arduino 3V ==> 9V ]
   l'arduino ne pouvant sortir que du 5V et une petite intensité, on utilise des transistors pour faire un circuit de puissance.

   -----------------------------------------PWM--------------------------------------------
   le PWM crée une onde carrée dont dont on peut choisir le rapport entre le temps haut et le temps bas. Celui-ci est codé sur un octet. Ainsi,
   une valeur de 255 enverra un signal continu à 1(5V), une valeur de 0 un signal continu à 0 (0V) et un signal à 127 donne une onde carrée de rapport 0.5 (2.5V)


  -----------------------------------------Amplification---------------------------------------

   le bandeau est alimenté en 12V et peux tirer plusieurs amperes, il faut donc mettre des transistors pour faire un circuit de puissance.
   Avec le transistor coté non métallique,
   brancher le pin de droite sur une résistance de 10KΩ relié à l'arduino
            le pin central sur le bandeau
            le pin de gauche sur la masse ( commune entre l'alim 12V et l'arduino )


   0   ==> __________________________________________ ==> 0V    ==> amplification ==> 0V
            _     _     _     _     _     _     _
   63  ==> | |___| |___| |___| |___| |___| |___| |___ ==> 1.25V ==> amplification ==> 3V
            __    __    __    __    __    __    __
   127 ==> |  |__|  |__|  |__|  |__|  |__|  |__|  |__ ==> 2.5V  ==> amplification ==> 6V
            ___   ___   ___   ___   ___   ___   ___
   191 ==> |   |_|   |_|   |_|   |_|   |_|   |_|   |_ ==> 3.75V ==> amplification ==> 9V
            ________________________________________
   255 ==> |                                        | ==> 5V    ==> amplification ==> 12V


*/
#include <bandeauLEDRGB.h>
//-------------------------------on crée tous les bandeaux de led----------------------------
// prototypes constructeur : nombandeau(unsigned int pinrouge,unsigned int pinvert,unsigned int pinbleu, bool activé ?[(optionnel) false par défaut] )

bandeauLEDRGB bandeau_pilier_1(3, 5, 6) ;
bandeauLEDRGB bandeau_pilier_2(3, 5, 6) ;
bandeauLEDRGB bandeau_pilier_3(3, 5, 6) ;

bandeauLEDRGB bandeau_pf_ext_1(3, 5, 6) ; //plateforme
bandeauLEDRGB bandeau_pf_ext_2(3, 5, 6) ;
bandeauLEDRGB bandeau_pf_ext_3(3, 5, 6) ;
bandeauLEDRGB bandeau_pf_ext_4(3, 5, 6) ;

bandeauLEDRGB bandeau_pf_int_1(3, 5, 6) ;
bandeauLEDRGB bandeau_pf_int_2(3, 5, 6) ;
bandeauLEDRGB bandeau_pf_int_3(3, 5, 6) ;
bandeauLEDRGB bandeau_pf_int_4(3, 5, 6) ;

bandeauLEDRGB bandeau_lotus_1(3, 5, 6) ;
bandeauLEDRGB bandeau_lotus_2(3, 5, 6) ;
bandeauLEDRGB bandeau_lotus_3(3, 5, 6) ;
bandeauLEDRGB bandeau_lotus_4(3, 5, 6) ;


/*
   ces variables servent à activer des routines qui vont se rajouter aux ambiances en fonction des
   demandes de l'ordinateur comme faire briller les led avec la musique.
*/

/*******************************************************************************************************/
/*******************************************capteur capacitif*******************************************/
/*-------------------------------principe----------------------------------------------------
   le capteur capacitif calcule le temps de charge du systeme assimilé à un condensateur de très faible capacitée
   celui-ci est modifié quand on touche le capteur, ce qui crée un deuxième condensateur et donc un effet de résonnance.
   le système aura donc un temps de chargement de 1 si on ne touche pas le capteur mais pourra dépasser 1000 dès lors qu'il sera touché

   -----------------------------branchement--------------------------------------------------
   connecter une résistance de 1MΩ entre deux pins de l'arduino, le pin d'entrée et le pin de sortie.
   connecter ensuite le capteur métallique sur le pin de sortie sans lui attribuer de masse.
*/

#ifndef CAPT_CAPACITIF
#define CAPT_CAPACITIF

#include <CapacitiveSensor.h>

#define TPS_DETECTION 50 //le temps de detection est à étalonner en fonction du capteur et de la résistance. pour ça, veuillez utiliser le code de démo fourni avec la librairie.

//---------------------------------définitions des pins des capteurs--------------------------------------
// on peut prendre sans aucun problème une seule alimentation pour tous les capteurs.
CapacitiveSensor petale1 = CapacitiveSensor(31, 33) ; // résistance de 1MΩ entre pin 31(alim) et 33(sortie)

//----------------variables globales capteurs---------------------------------

/*
   0 ==> aucun pétale
   1 ==> petale1 activé
   2 ==> petale2 activé
   3 ==> petale1 ET petale2 activés
   4 ==> petale3 activé
   8 ==> petale4 activé...
*/



//
inline bool mesure_tactile()
{
  return petale1.capacitiveSensor(30) > TPS_DETECTION ;
}
#endif
/******************************************************************************************************/


#include <Thread.h>
#include <ThreadController.h>

ThreadController controle = ThreadController() ;

Thread communication = Thread() ;
Thread evenement = Thread() ;

communicationpy command = communicationpy() ;


void setup()
{
  
  Serial.begin(115200);// début de la communication avec l'ordinateur avec une vitesse de 115200 (à synchroniser avec l'ordinateur)
  petale1.set_CS_AutocaL_Millis(0xFFFFFFFF) ; // initialisation du capteur capacitif du lotus


  communication.onRun(commande_bandeau) ;
  communication.setInterval(500) ;

  controle.add(&communication) ;
}


/**********************************problématique du temps réel multitache en monoprocessus********************************
   le programme devant faire un certain nombre d'action en simultané( allumage des led, prise de mesure des capteurs, dialogue avec l'ordinateur)
   on ne peut utiliser la fonction delay pour gerer le temps d'allumage des leds, celle-ci bloqurais le programme.

   On utilise donc une alternative à cette fonction en créant une variable qui fera tourner un certain nombre de fois la boucle principale pour "attendre"
   On utilise par ailleurs la fonction serialEvent, déjà implémentées dans le langage arduino qui s'éxecute entre chaque boucle loop.
   On va lui déleguer les prise de mesures et communication pour ne mettre que la gestion des led dans la boucle principale.


 * **********************************************principe du programme des leds********************************************
   la boucle principale est un choix entre les différentes ambiance que l'on aura codé au préalable.
   lors d'une temporisation, elle s'execute "à vide" en lieu et place de l'instruction delay pour rester à l'écoute des autre processus.

*/
void init_ambiance()
/*
   cette fonction se lance à chaque changement d'ambiance, elle dit quels bandeaux seront utilisé ou pas par la routine principale. elle définie aussi
   la valeur standard des bandeaux inutilisé. ( défini comme true pour die qu'ils sont disponibles pour la réponse secondaire) ;
*/
{

  switch (ambiance)
  {
    case 0 :
      bandeau_pilier_1.settrue(false) ;
      bandeau_pilier_2.settrue(false) ;
      bandeau_pilier_3.settrue(false) ;

      bandeau_pf_ext_1.settrue(false) ;
      bandeau_pf_ext_2.settrue(false) ;
      bandeau_pf_ext_3.settrue(false) ;
      bandeau_pf_ext_4.settrue(false) ;

      bandeau_pf_int_1.settrue(false) ;
      bandeau_pf_int_2.settrue(false) ;
      bandeau_pf_int_3.settrue(false) ;
      bandeau_pf_int_4.settrue(false) ;

      bandeau_lotus_1.settrue(false) ;
      bandeau_lotus_2.settrue(false) ;
      bandeau_lotus_3.settrue(false) ;
      bandeau_lotus_4.settrue(false) ;
      break ;

    case 1 :

      bandeau_pilier_1.settrue(true) ; // ces trois bandeaux sont activables pour la routine secondaire
      bandeau_pilier_2.settrue(true) ;
      bandeau_pilier_3.settrue(true) ;

      bandeau_pf_ext_1.settrue(false) ;
      bandeau_pf_ext_2.settrue(false) ;
      bandeau_pf_ext_3.settrue(false) ;
      bandeau_pf_ext_4.settrue(false) ;

      bandeau_pf_int_1.settrue(false) ;
      bandeau_pf_int_2.settrue(false) ;
      bandeau_pf_int_3.settrue(false) ;
      bandeau_pf_int_4.settrue(false) ;

      bandeau_lotus_1.settrue(false) ;
      bandeau_lotus_2.settrue(false) ;
      bandeau_lotus_3.settrue(false) ;
      bandeau_lotus_4.settrue(false) ;

      bandeau_pilier_1.ledpwmRGB(200, 200, 200) ; // petite lumière blanchâtre diffuse
      bandeau_pilier_2.ledpwmRGB(200, 200, 200) ; // petite lumière blanchâtre diffuse
      bandeau_pilier_3.ledpwmRGB(200, 200, 200) ; // petite lumière blanchâtre diffuse
      break ;

  }
}




void loop()
/*
   Le programme qui tourne en boucle ne fait que discuter avec le PC. il et régulièrement interrompu pour modifier les valeurs du bandeau led
*/
{
  
  int temp = ambiance ;
  noInterrupts() ;
  ambiance = command.PCToArd() ;
  interrupts() ;
  if (ambiance >= 10) // on passe dans les routines secondaires
  {
    evenement.onRun(evenement_bandeau) ;
    evenement.setInterval(1) ;

    controle.add(&evenement) ;
  }
  if (ambiance != temp) init_ambiance() ;

  mesure_ultrason() ;
  int lotus = mesure_tactile() ;
  noInterrupts() ;
  command.CaptToPC(capt1, capt2, capt3, capt4, lotus) ;
  interrupts() ;
 
}


void commande_bandeau()
/*cette fonction est appelée à intervales réguliers pour effectuer les modifications sur le bandeau led.
*/
{

 switch (ambiance)
  {
    /* ce programme fait fluctuer la couleur des bandeaux led en fonction du temps.
       l'intensité globale est fonction de la distance des gens avec le capteur ultrason.

    */
    case 0 ://avant d'avoir été touché une première fois


      increment++ ;
      valrouge = (increment % 255) * 0.2 + capt1 - 25 ; // l'intensité varie périodiquement avec une phase décalée d'un tiers pour chaque couleur de led.
      valbleu = (increment + 85) % 255 * 0.2 + capt1 - 25 ; // l'intensité des led est compris entre capt1-25 et capt1+25
      valvert = (increment + 170) % 255 * 0.2 + capt1 - 25 ;
      bandeau_pilier_1.ledpwmRGB(int(valrouge), int(valvert), int(valbleu)) ;
      communication.setInterval(50) ;
      controle.add(&communication) ;
      break ;

    /*Scène de combat :
       dans le lotus deux couleurs s'affrontent.
       les deux couleurs sont bleu et rouge
       on met un bandeau de chaque coté à la couleur voulu et les deux autres vont
       tour à tour changer de couleur.
       les bandeaux du bas vont gagne en intensité en fonction du bruit
    */
    case 1 :

      valrouge = (sin(increment * 2 * PI / 255) + 1) * capt1 + capt2 ; // on prend une fonction périodique entre 0 et 2 qu'on multiplie avec la distance du capteur principal.
      valbleu = (sin(byte(increment + 123) * 2 * PI / 255) + 1) * capt1 + capt3 ; // cette fonction va faire fluctuer les couleurs rouges et bleu sur les bandeaux intermédiaires.
      bandeau_pilier_1.ledpwm(int(valrouge), Couleur::rouge) ;
      bandeau_pilier_1.ledpwm(int(valrouge), Couleur::bleu) ;
      communication.setInterval(50) ;
      controle.add(&communication) ;
      break ;


  } 
}

void evenement_bandeau()
/* cette fonction sert eventuellement à rajouter des surcouches lumineuses sur le programme principal, si on veut le synchroniser avec la musique ou faire une chose précise.
    l'idée c'est de le faire agir seulement sur les bandeaux qui ne sont pas utilisés sur la routine principale, ceux qui ont leur valeur active à true. ( false par défaut)
    On peut par exemple implémenter une breve impulsion lumineuse blanche :
*/

{
  switch (ambiance)
  {
    case 20 :

      if(increment++ == 255) 
      {
        controle.remove(&evenement) ;
        return;  
      }
      int val = (sin(increment * 2 * PI / 255) + 1)*255;
      
      if(bandeau_pilier_1.istrue()) bandeau_pilier_1.ledpwmRGB(val, val, val) ; 
      if(bandeau_pilier_2.istrue() ) bandeau_pilier_2.ledpwmRGB(val, val, val) ;
      if(bandeau_pilier_3.istrue() ) bandeau_pilier_3.ledpwmRGB(val, val, val) ;

      if(bandeau_pf_ext_1.istrue()) bandeau_pf_int_1.ledpwmRGB(val, val, val) ;
      if(bandeau_pf_ext_2.istrue()) bandeau_pf_ext_2.ledpwmRGB(val, val, val)  ;
      if(bandeau_pf_ext_3.istrue()) bandeau_pf_ext_3.ledpwmRGB(val, val, val)  ;
      if(bandeau_pf_ext_4.istrue()) bandeau_pf_ext_4.ledpwmRGB(val, val, val)  ;

      if(bandeau_pf_int_1.istrue()) bandeau_pf_int_1.ledpwmRGB(val, val, val)  ;
      if(bandeau_pf_int_2.istrue()) bandeau_pf_int_2.ledpwmRGB(val, val, val)  ;
      if(bandeau_pf_int_3.istrue()) bandeau_pf_int_3.ledpwmRGB(val, val, val)  ;
      if(bandeau_pf_int_4.istrue()) bandeau_pf_int_4.ledpwmRGB(val, val, val)  ;

      if(bandeau_lotus_1.istrue()) bandeau_lotus_1.ledpwmRGB(val, val, val)  ;
      if(bandeau_lotus_2.istrue()) bandeau_lotus_2.ledpwmRGB(val, val, val)  ;
      if(bandeau_lotus_3.istrue()) bandeau_lotus_3.ledpwmRGB(val, val, val)  ;
      if(bandeau_lotus_4.istrue()) bandeau_lotus_4.ledpwmRGB(val, val, val)  ;      
    

  }

}

