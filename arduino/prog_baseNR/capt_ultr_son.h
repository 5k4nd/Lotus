#ifndef CAPT_ULTR_SON
#define CAPT_ULTR_SON


// capt1
const int capt1_trigger_pin = 22 ;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int capt1_echo_pin = 24 ; // Arduino pin tied to echo pin on the ultrasonic sensor.

// capt2
const int capt2_trigger_pin = 26 ;
const int capt2_echo_pin = 28 ;

// capt3
const int capt3_trigger_pin = 30 ;
const int capt3_echo_pin = 32 ;

// capt4
const int capt4_trigger_pin = 34 ; 
const int capt4_echo_pin = 36 ;

const int MAX_DISTANCE  = 200 ;// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(capt1_trigger_pin,capt1_echo_pin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(capt2_trigger_pin,capt2_echo_pin, MAX_DISTANCE);
NewPing sonar3(capt3_trigger_pin,capt3_echo_pin, MAX_DISTANCE);
NewPing sonar4(capt4_trigger_pin,capt4_echo_pin, MAX_DISTANCE);

//-------------------------------------variables globales------------------------------------------
unsigned int capt1 = 0 ;
unsigned int capt2 = 0 ;
unsigned int capt3 = 0 ;
unsigned int capt4 = 0 ;


#endif
