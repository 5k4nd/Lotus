#include "communicationpy.h"



 inline void communicationpy::ArdToPC(String nomVar, uint8_t valVar )
{
  
  Serial.print(nomVar) ;
  Serial.println("var") ;
  Serial.println(valVar) ;
}


void communicationpy::CaptToPC(uint8_t capteur1, uint8_t capteur2, uint8_t capteur3, uint8_t capteur4, uint8_t lotus)
{
  communicationpy::ArdToPC("capteur1",capteur1) ;
  communicationpy::ArdToPC("capteur2",capteur2) ;
  communicationpy::ArdToPC("capteur3",capteur3) ;
  communicationpy::ArdToPC("capteur4",capteur4) ;
  communicationpy::ArdToPC("lotus",int(lotus)) ;
}

int communicationpy::PCToArd()
{ 
    if (Serial.available() > 0)
    {
      char var = Serial.read(); // a tester pour voir si anbiance.
      int ambiance = Serial.parseInt();
      return ambiance;
    }
}