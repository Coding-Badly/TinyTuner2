/*==============================================================================

  Copyright 2012, 2013 Rowdy Dog Software.

  This file is part of TinyTuner2.

  TinyTuner2 is free software: you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option)
  any later version.

  TinyTuner2 is distributed in the hope that it will be useful, but WITHOUT 
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License 
  along with TinyTuner2.  If not, see <http://www.gnu.org/licenses/>.

  Inspired by the work of oPossum (Kevin Timmerman)...
  http://forums.adafruit.com/viewtopic.php?t=5078

==============================================================================*/

#ifndef TinyTuner2_h
#define TinyTuner2_h

#include <Arduino.h>
#include <inttypes.h>
#include <avr/io.h>


#if defined( TC_XTAL2 )

  #define TINY_TUNER_DEFAULT_PIN  TC_PIN(TC_XTAL2)
  #define TINY_TUNER_DEFAULT_BIT  TC_DIO_BIT(TC_XTAL2)

#else

  #if TC_VERSION >= 200
    #error TC_XTAL2 must be defined for the selected processor.  The definition goes in the processor specific header file.  See tc_hardware_mapping_t861.h for an example.
  #else

    #if defined( __AVR_ATtiny13__ ) || (__AVR_ATtiny13A__)
      #define TINY_TUNER_DEFAULT_PIN  PINB
      #define TINY_TUNER_DEFAULT_BIT  4
    #endif

    #if defined( __AVR_ATtiny25__ ) || defined( __AVR_ATtiny45__ ) || defined( __AVR_ATtiny85__ )
      #define TINY_TUNER_DEFAULT_PIN  PINB
      #define TINY_TUNER_DEFAULT_BIT  4
    #endif

    #if defined( __AVR_ATtiny24__ ) || defined( __AVR_ATtiny44__ ) || defined( __AVR_ATtiny84__ )
      #define TINY_TUNER_DEFAULT_PIN  PINB
      #define TINY_TUNER_DEFAULT_BIT  1
    #endif

    #if defined( __AVR_ATtiny261__ ) || defined( __AVR_ATtiny461__ ) || defined( __AVR_ATtiny861__ )
      #define TINY_TUNER_DEFAULT_PIN  PINB
      #define TINY_TUNER_DEFAULT_BIT  5
    #endif

    #if defined( __AVR_ATtiny2313__ ) || defined( __AVR_ATtiny4313__ )
      #define TINY_TUNER_DEFAULT_PIN  PINA
      #define TINY_TUNER_DEFAULT_BIT  1
    #endif

    #if defined( __AVR_ATmega328P__ ) || defined( __AVR_ATmega328__ ) || defined( __AVR_ATmega168__ )
      #define TINY_TUNER_DEFAULT_PIN  PINB
      #define TINY_TUNER_DEFAULT_BIT  7
    #endif

    #if ! defined( TINY_TUNER_DEFAULT_PIN ) || ! defined( TINY_TUNER_DEFAULT_BIT )
      #error TINY_TUNER_DEFAULT_PIN and TINY_TUNER_DEFAULT_BIT must be defined for the selected processor.  The definition goes in this header file.  See the section above for examples.
    #endif

  #endif // TC_VERSION

#endif // TC_XTAL2

#if ! defined( TINY_TUNER_PIN )
  #define TINY_TUNER_PIN  TINY_TUNER_DEFAULT_PIN
#endif

#if ! defined( TINY_TUNER_BIT )
  #define TINY_TUNER_BIT  TINY_TUNER_DEFAULT_BIT
#endif


typedef struct
{
  uint8_t               minimum;
  uint8_t               seed;
  uint8_t               maximum;
}
calibration_range_t;

typedef struct
{
  uint8_t               range_count;
  calibration_range_t   range[2];
}
calibration_t;

typedef struct
{
  uint16_t              best_error;
  uint8_t               best_OSCCAL;
}
calibration_results_t;


#if defined( __AVR_ATtiny13__ ) || (__AVR_ATtiny13A__)
  static const calibration_t calibration =
      { 1, { { 0x00, 0x5A, 0x7F }, { 0x00, 0x00, 0x00 } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if defined( __AVR_ATtiny25__ ) || defined( __AVR_ATtiny45__ ) || defined( __AVR_ATtiny85__ )
  static const calibration_t calibration =
      { 2, { { 0x00, 0x68, 0x7F }, { 0x80, 0x90, 0xFF } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if defined( __AVR_ATtiny24__ ) || defined( __AVR_ATtiny44__ ) || defined( __AVR_ATtiny84__ )
  static const calibration_t calibration =
      { 2, { { 0x00, 0x74, 0x7F }, { 0x80, 0xA8, 0xFF } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if defined( __AVR_ATtiny2313__ ) || defined( __AVR_ATtiny4313__ )
  static const calibration_t calibration =
      { 1, { { 0x00, 0x65, 0x7F }, { 0x00, 0x00, 0x00 } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if defined( __AVR_ATmega328P__ ) || defined( __AVR_ATmega328__ ) || defined( __AVR_ATmega168__ )
  static const calibration_t calibration =
      { 2, { { 0x00, 0x6E, 0x7F }, { 0x80, 0xB4, 0xFF } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if defined( __AVR_ATtiny261__ ) || defined( __AVR_ATtiny461__ ) || defined( __AVR_ATtiny861__ )
  static const calibration_t calibration =
      { 2, { { 0x00, 0x65, 0x7F }, { 0x80, 0x85, 0xFF } } };
  #define TINY_TUNER_HAVE_CALIBRATION  (1)
#endif

#if ! TINY_TUNER_HAVE_CALIBRATION
  #warning Calibration range information not available for the selected processor.  Assuming a split range.
  static const calibration_t calibration =
      { 2, { { 0x00, 0x40, 0x7F }, { 0x80, 0xC0, 0xFF } } };
#endif


class TinyTuner2Class
{
public:

  #if F_CPU == 1000000L
    // 16 ms pulse, 1 us cycle, 5 cycles per loop, 5 us per loop
    // 16 ms / 5 us = (16/1000)/(5/1000000) = 3200
    static const uint16_t CALIBRATION_GOAL  = 3200;
    #define TC_CLOCK_DIVIDED_BY_8  1
    
  #elif F_CPU == 1200000L
    // 16 ms pulse, 1/1.2 us cycle, 5 cycles per loop, 5/1.2 us per loop
    // 16 ms / (5/1.2) us = (16/1000)/((5/1.2)/1000000) = 3840
    static const uint16_t CALIBRATION_GOAL  = 3840;
    #define TC_CLOCK_DIVIDED_BY_8  1
    
  #elif  F_CPU == 8000000L
    // 16 ms pulse, 1/8 us cycle, 5 cycles per loop, 5/8 us per loop
    // 16 ms / (5/8) us = (16/1000)/((5/8)/1000000) = 25600
    #define TOP_FIXER 8
    static const uint16_t CALIBRATION_GOAL  = 3200;
    #define TC_CLOCK_BASE  1
    
  #elif F_CPU == 9600000L
    // 16 ms pulse, 1/9.6 us cycle, 5 cycles per loop, 5/9.6 us per loop
    // 16 ms / (5/9.6) us = (16/1000)/((5/9.6)/1000000) = 30720
    #define TOP_FIXER 8
    static const uint16_t CALIBRATION_GOAL  = 3840;
    #define TC_CLOCK_BASE  1
    
  #elif  F_CPU == 16000000L
    // 16 ms pulse, 1/16 us cycle, 5 cycles per loop, 5/16 us per loop
    // 16 ms / (5/16) us = (16/1000)/((5/16)/1000000) = 51200
    #define TOP_FIXER 16
    static const uint16_t CALIBRATION_GOAL  = 3200;
    #define TC_CLOCK_MULTIPLIED_BY_2  1
    
  #endif

  static uint16_t TimeOnePulse( void )
  {
    uint16_t Temp = 0;

    asm volatile
    (
    // Wait for the input to be low
    "L%=wfl: "
      "sbic  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfl"                            "\n\t"

    // No interrupts  
      "cli"                                     "\n\t"

    // Wait for the rising edge
    "L%=wfre: "
      "sbis  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfre"                           "\n\t"

    // Ensure all passes through the loop are 5 cycles
      "nop"                                     "\n\t"
      "nop"                                     "\n\t"

    // Time the pulse
    "L%=wffe: "
      "adiw  %[reseult], 1"                     "\n\t"
      "sbic  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wffe"                           "\n\t"

      "sei"                                     "\n\t"

      : 
        [reseult] "+w" ( Temp )
      : 
        [calreg] "I" ( _SFR_IO_ADDR( TINY_TUNER_PIN ) ),
        [calbit] "I" ( TINY_TUNER_BIT )
    );
    
    return( Temp );
  }

  static uint16_t MeasureCalibration( void )
  {
    uint16_t rv;
    #if TOP_FIXER
      uint16_t top;
    #endif
    
    rv = 0;

    for ( int8_t i=4; i > 0; --i )
    {
      #if TOP_FIXER
        top = (TimeOnePulse() + (TOP_FIXER/2)) / TOP_FIXER;
        rv += top;
      #else
        rv += TimeOnePulse();
      #endif
    }
    
    return( (rv + 2) / 4 );
  }

  __attribute__ ((noinline))
  static void SetOSCCAL( uint8_t NewValue )
  {
    uint8_t CurrentValue;
    
    CurrentValue = OSCCAL;
    
    while ( CurrentValue < NewValue )
    {
      ++CurrentValue;
      OSCCAL = CurrentValue;
      // fix? Short delay?
    }

    while ( CurrentValue > NewValue )
    {
      --CurrentValue;
      OSCCAL = CurrentValue;
      // fix? Short delay?
    }
    
    // fix? Short delay?
  }

  __attribute__ ((noinline))
  static void TuneInternalOscillatorLeftRight( 
      uint8_t minimum, uint8_t seed, uint8_t maximum,
      int8_t direction,
      calibration_results_t& results )
  {
    uint16_t Calibration;
    uint16_t Error;
    uint16_t BestError;
    int8_t BestCountdown;
    uint8_t CurrentValue;

    BestError = results.best_error;
    BestCountdown = 3;

    SetOSCCAL( seed );
    CurrentValue = OSCCAL;

    while ( true )
    {
      // Debug.print( CurrentValue, HEX ); // rmv
      // Debug.write( '\t' ); // rmv
    
      Calibration = MeasureCalibration();
    
      // Debug.print( Calibration ); // rmv
      // Debug.write( '\t' ); // rmv
    
      if ( Calibration < CALIBRATION_GOAL )
      {
        Error = CALIBRATION_GOAL - Calibration;
      }
      else if ( Calibration > CALIBRATION_GOAL )
      {
        Error = Calibration - CALIBRATION_GOAL;
      }
      else
      {
        Error = 0;
      }
    
      if ( Error < BestError )
      {
        BestError = Error;
        results.best_error = Error;
        results.best_OSCCAL = OSCCAL;
        BestCountdown = 3;
        // Debug.print( F( "closer\t" ) ); // rmv
      }
      else
      {
        --BestCountdown;
    
        if ( BestCountdown <= 0 )
        {
          return;
        }
      }
      
      if ( direction < 0 )
      {
        if ( CurrentValue > minimum )
        {
          --CurrentValue;
        }
        else
        {
          return;
        }
      }
      else // if ( direction > 0 )
      {
        if ( CurrentValue < maximum )
        {
          ++CurrentValue;
        }
        else
        {
          return;
        }
      }
    
      OSCCAL = CurrentValue;

      // Debug.println(); // rmv
    }
  }

  __attribute__ ((noinline))
  static void TuneInternalOscillatorRange( 
      uint8_t minimum, uint8_t seed, uint8_t maximum,
      calibration_results_t& results )
  {
    calibration_results_t temp;

    temp.best_error = (uint16_t)(-1);

    TuneInternalOscillatorLeftRight( minimum, seed, maximum, -1, temp );
    // Debug.println(); // rmv
    TuneInternalOscillatorLeftRight( minimum, seed, maximum, +1, temp );
    // Debug.println(); // rmv

    if ( temp.best_error < results.best_error )
    {
      results.best_error = temp.best_error;
      results.best_OSCCAL = temp.best_OSCCAL;
    }
  }

  static void tune( void )
  {
    if ( calibration.range_count == 2 )
    {
      uint8_t CurrentValue;

      CurrentValue = OSCCAL;

      if ( (CurrentValue >= calibration.range[1].minimum) && (CurrentValue <= calibration.range[1].maximum) )
      {
        SetOSCCAL( calibration.range[1].seed );
        OSCCAL = calibration.range[0].seed;
      }
    }

    calibration_results_t results;

    results.best_error = (uint16_t)(-1);

    TuneInternalOscillatorRange( calibration.range[0].minimum, calibration.range[0].seed, calibration.range[0].maximum, results );

    if ( calibration.range_count == 2 )
    {
      SetOSCCAL( calibration.range[0].seed );
      OSCCAL = calibration.range[1].seed;

      TuneInternalOscillatorRange( calibration.range[1].minimum, calibration.range[1].seed, calibration.range[1].maximum, results );
    }
    
    if ( calibration.range_count == 2 )
    {
      // Range 1 is in effect because of the code above

      // If Best OSCCAL is in Range 0 then switch to Range 0
      if ( (results.best_OSCCAL >= calibration.range[0].minimum) && (results.best_OSCCAL <= calibration.range[0].maximum) )
      {
        SetOSCCAL( calibration.range[1].seed );
        OSCCAL = calibration.range[0].seed;
      }
    }

    SetOSCCAL( results.best_OSCCAL );

/*rmv
    Debug.println();
    delay( 100 );
    Debug.println();
    Debug.print( results.best_OSCCAL, HEX );
    Debug.write( '\t' );
    Debug.print( F( "fini" ) );
    Debug.write( '\t' );
    Debug.print( results.best_error, DEC );
    Debug.write( '\t' );
    Debug.println();
    Debug.println();
*/
  }

  __attribute__ ((noinline))
  static void WriteByteToEEPROM( uint8_t offsetFromEnd, uint8_t value )
  {
/* fix
    eeprom_write_byte( (uint8_t*)(E2END-offsetFromEnd), value );
*/
  }

  static void SaveCurrentOSCCALToEEPROM( void )
  {
/**/
    WriteByteToEEPROM( 0, 0x59 );
    WriteByteToEEPROM( 1, OSCCAL );
    WriteByteToEEPROM( 2, 1 );
    WriteByteToEEPROM( 3, 0xC6 );
/**/
/*
    calibration_t calibration;

    calibration.guard1  = 0xC6;
    calibration.version = 1;
    calibration.osccal  = OSCCAL;
    calibration.guard2  = 0x59;

    eeprom_write_dword( (uint32_t*) ((E2END+1)-sizeof(calibration.raw)), calibration.raw );
*/
/*
    calibration_t calibration;

    calibration.guard1  = 0xC6;
    calibration.version = 1;
    calibration.osccal  = OSCCAL;
    calibration.guard2  = 0x59;

    eeprom_write_block( (void*) ((E2END+1)-sizeof(calibration)), &calibration, sizeof(calibration) );
*/
  }

};

extern TinyTuner2Class TinyTuner2;


/* fix
inline void ReadAndSetOsccal( void )
{
  TinyTuner::calibration_t calibration;

  calibration.raw = eeprom_read_dword( (uint32_t*) ((E2END+1)-sizeof(calibration.raw)) );

  if ( (calibration.raw & 0xFFFF00FF) == 0xC6010059 )
  {
    OSCCAL = calibration.osccal;
  }
}
*/

#endif
