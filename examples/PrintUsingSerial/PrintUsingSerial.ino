#include <TinyTuner2.h>

static const uint8_t NO_PIN = (uint8_t)(-1);
static const uint8_t TogglePin = NO_PIN; // 3;

static void OutputOSCCALAssignment( void )
{
  TinyTuner2.tune();

  #if FLASHEND <= 1023
    Serial.println( OSCCAL, HEX );
  #else
    Serial.print( F( "\r\n\r\n  // Add the following line of code to the top of setup...\r\n  OSCCAL = 0x" ) );
    Serial.print( OSCCAL, HEX );
    Serial.println( F( ";" ) );
  #endif

  delay( 2500 );
}

void setup( void )
{
  Serial.begin( 250000 );

  if ( TogglePin != NO_PIN)
  {
    digitalWrite( TogglePin, HIGH );
    pinMode( TogglePin, OUTPUT );
  }

  #if FLASHEND <= 1023
  #else
    Serial.print( F( "\r\n\r\n  // Initial value of OSCCAL is 0x" ) );
    Serial.println( OSCCAL, HEX );
  #endif
  _delay_ms( 1000 );

  OutputOSCCALAssignment();
}

void loop( void )
{
  OutputOSCCALAssignment();
}

