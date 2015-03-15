#include <TinyDebugKnockBang.h>
#include <TinyTuner2.h>
#include <util/delay.h>

static const uint8_t NO_PIN = (uint8_t)(-1);
static const uint8_t TogglePin = NO_PIN; // 3;

static void OutputOSCCALAssignment( void )
{
  TinyTuner2.tune();

  #if FLASHEND <= 1023
    Debug.println( OSCCAL, HEX );
  #else
    Debug.print( F( "\r\n\r\n  // Add the following line of code to the top of setup...\r\n  OSCCAL = 0x" ) );
    Debug.print( OSCCAL, HEX );
    Debug.println( F( ";" ) );
  #endif

//  delay( 2500 );
  _delay_ms( 2500 );
}

void setup( void )
{
  Debug.begin( 250000 );

  if ( TogglePin != NO_PIN)
  {
    digitalWrite( TogglePin, HIGH );
    pinMode( TogglePin, OUTPUT );
  }

  #if FLASHEND <= 1023
  #else
    Debug.print( F( "\r\n\r\n  // Initial value of OSCCAL is 0x" ) );
    Debug.println( OSCCAL, HEX );
  #endif
  _delay_ms( 1000 );

  OutputOSCCALAssignment();
}

void loop( void )
{
  OutputOSCCALAssignment();
}

