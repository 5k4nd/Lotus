//----------------------------------fonction de commande du bandeau---------------------------------------
/*
 * La luminosité des led varie en fonction de la tension entre l'entrée et la sortie.
 * L'entrée est réglée sur 12V et les sortie (chaque couleur de led)
 * La tension sur les sorties est commandée par l'arduino aux moyen d'une tension gérée par modulations d'impulsions (PWM)
 * la tension des led est la différence entre le 12V et la sortie de l'arduino [ ex : alim 12V / arduino 3V ==> 9V ]
 * l'arduino ne pouvant sortir que du 5V et une petite intensité, on utilise des transistors pour generer un circuit de puissance. 
 * 
 * -----------------------------------------PWM--------------------------------------------
 * le PWM crée une onde carrée dont dont on peut choisir le rapport entre le temps haut et le temps bas. Celui-ci est codé sur un octet. Ainsi, 
 * une valeur de 255 enverra un signal continu à 1(5V), une valeur de 0 un signal continu à 0 (0V) et un signal à 127 donne une onde carrée de rapport 0.5 (2.5V)  
 * 
 * 
 *-----------------------------------------Amplification--------------------------------------- 
 * 
 * le bandeau est alimenté en 12V et peux tirer plusieurs amperes, il faut donc mettre des transistors pour faire un circuit de puissance. 
 * Avec le transistor coté non métallique, 
 * brancher le pin de droite sur une résistance de 10KΩ relié à l'arduino
 *          le pin central sur le bandeau
 *          le pin de gauche sur la masse ( commune entre l'alim 12V et l'arduino )
 *
 * 
 * 0   ==> __________________________________________ ==> 0V    ==> amplification ==> 0V
 *          _     _     _     _     _     _     _
 * 63  ==> | |___| |___| |___| |___| |___| |___| |___ ==> 1.25V ==> amplification ==> 3V
 *          __    __    __    __    __    __    __
 * 127 ==> |  |__|  |__|  |__|  |__|  |__|  |__|  |__ ==> 2.5V  ==> amplification ==> 6V
 *          ___   ___   ___   ___   ___   ___   ___
 * 191 ==> |   |_|   |_|   |_|   |_|   |_|   |_|   |_ ==> 3.75V ==> amplification ==> 9V
 *          ________________________________________
 * 255 ==> |                                        | ==> 5V    ==> amplification ==> 12V
 * 
 * : 12V, 0V et 6V pour l'exemple précédent (avec l'amplification)
 */

#ifndef BANDEAU_LED
#define BANDEAU_LED


//----------------------definitions des broches associée au bandeau------------------------
#define LEDVERT 5 // Configuration de la broche 3 (PWM) de l'Arduino à ledVert
#define LEDROUGE 3 // Configuration de la broche 5 (PWM) de l'Arduino à ledRouge
#define LEDBLEU 6 // Configuration de la broche 6 (PWM) de l'Arduino à ledBleu



//------------------fonction pour colorer les led--------------------------------------
void ledpwm(int pwm, int bandeau) // reçoit valeur 0-255 par couleur
{
  analogWrite(bandeau, pwm);// la fonction crée l'onde carrée de rapport pwm sur la sortie bandeau. l'onde sera modifiée au prochain appel de la fonction.
}






#endif
