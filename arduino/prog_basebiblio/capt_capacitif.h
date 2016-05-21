/*-------------------------------principe----------------------------------------------------
 * le capteur capacitif calcule le temps de charge du systeme assimilé à un condensateur de très faible capacitée
 * celui-ci est modifié quand on touche le capteur, ce qui crée un deuxième condensateur et donc un effet de résonnance. 
 * le système aura donc un temps de chargement de 1 si on ne touche pas le capteur mais pourra dépasser 1000 dès lors qu'il sera touché
 * 
 * -----------------------------branchement--------------------------------------------------
 * connecter une résistance de 1MΩ entre deux pins de l'arduino, le pin d'entrée et le pin de sortie. 
 * connecter ensuite le capteur métallique sur le pin de sortie sans lui attribuer de masse.
 */

#ifndef CAPT_CAPACITIF
#define CAPT_CAPACITIF
#include <CapacitiveSensor.h>

#define TPS_DETECTION 100 //le temps de detection est à étallonner en fonction du capteur et le résistance. pour ça, veullez utiliser le code de démo fournis avec la librairie.

//---------------------------------définitions des pins des capteurs--------------------------------------
// i=on peut prendre sans aucun problème une seule alimentation pour tous les capteurs.
CapacitiveSensor petale1 = CapacitiveSensor(31,33) ; // résistance de 1MΩ entre pin 31(alim) et 33(sortie)
CapacitiveSensor petale2 = CapacitiveSensor(31,35) ; // résistance de 1MΩ entre pin 31 et 35
CapacitiveSensor petale3 = CapacitiveSensor(31,37) ; // résistance de 1MΩ entre pin 31 et 37
CapacitiveSensor petale4 = CapacitiveSensor(31,39) ; // résistance de 1MΩ entre pin 31 et 39

//----------------variables globales---------------------------------

bool pet1 = false ; // on cherche juste à savoir si le capteur à detectée quelque chose ou pas
bool pet2 = false ;
bool pet3 = false ;
bool pet4 = false ;

//
void mesure_tactile() 
{
  pet1 = petale1.capacitiveSensor(30)>TPS_DETECTION ;
  pet2 = petale2.capacitiveSensor(30)>TPS_DETECTION ;
  pet3 = petale3.capacitiveSensor(30)>TPS_DETECTION ;
  pet4 = petale4.capacitiveSensor(30)>TPS_DETECTION ;
}

#endif
