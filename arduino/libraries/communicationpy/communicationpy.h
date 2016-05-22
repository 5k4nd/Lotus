#ifndef PROTOCOLE_PYTHON
#define PROTOCOL_PYTHON

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, etc
#else
#include "WProgram.h"
#endif

class communicationpy
{
  public:
    communicationpy() {};
    static  int PCToArd() ;
    static void ArdToPC(String nomVar, uint8_t valVar ) ;
    static  void CaptToPC(uint8_t capteur1, uint8_t capteur2, uint8_t capteur3, uint8_t capteur4, uint8_t lotus) ;
    
} ;
















#endif