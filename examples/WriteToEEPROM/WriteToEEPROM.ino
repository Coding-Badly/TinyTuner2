#include <TinyTuner2.h>
#include <avr/eeprom.h>

const uint8_t NO_PIN = (uint8_t)(-1);
const uint8_t TogglePin = NO_PIN; // 3; // 

#if FLASHEND <= 1023
  const bool ReportStatus = false;
#else
  const bool ReportStatus = true;
#endif

#if TC_CLOCK_BASE
  static uint8_t * const OSCCAL_ADDRESS = (uint8_t*const)(E2END -  0);
#elif TC_CLOCK_DIVIDED_BY_8
  static uint8_t * const OSCCAL_ADDRESS = (uint8_t*const)(E2END -  1);
#elif TC_CLOCK_MULTIPLIED_BY_2
  static uint8_t * const OSCCAL_ADDRESS = (uint8_t*const)(E2END -  2);
#endif

static void PrintStatus( const __FlashStringHelper * v )
{
  if ( ReportStatus )
  {
    Serial.println( v );
  }
}

static void ToggleLed( void )
{
  if ( TogglePin != NO_PIN)
  {
    digitalWrite( TogglePin, ! digitalRead( TogglePin ) );
  }
}

void setup( void )
{
  if ( ReportStatus )
  {
    Serial.begin( 250000 );
  }

  if ( TogglePin != NO_PIN)
  {
    digitalWrite( TogglePin, HIGH );
    pinMode( TogglePin, OUTPUT );
  }

  PrintStatus( F( "Tuning started..." ) );

  TinyTuner2.tune();

  ToggleLed();
  PrintStatus( F( "Tuning finished." ) );

  PrintStatus( F( "Writing OSCCAL to EEPROM..." ) );

  eeprom_write_byte( OSCCAL_ADDRESS, OSCCAL );

  ToggleLed();
  PrintStatus( F( "Writing OSCCAL to finished." ) );
}

void loop( void )
{
  ToggleLed();
  delay( 100 );
}
