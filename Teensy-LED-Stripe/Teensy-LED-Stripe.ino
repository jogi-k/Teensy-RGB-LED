/***********************************/
/* Teensy LED-Stripe               */
/***********************************/
/* 
 * Simple program to control a stripe of Neo-Pixel WS2812-RGB-Led
 * Very simple schematics: 
 * 
 * +5 V ------------------ +5v ----LED1 ----------- +5V --- LED2 ---- and so on
 * GND  -------------------GND ----LED1------------ GND ----LED2 ---- and so on
 * Pin 17 --- 330 Ohm ---- Data In-LED1-Data-Out--- Data-In-LED2 ---- and so on
 *  
 *  The Software is based on the demonstrator 
 *  that is coming with the Adafruit-Neopixel Library
 *  
 *  You can control the color of the LED via USB-Serial with sending a single lowercase char
 *  r => red
 *  g => green
 *  b => blue
 *  p => pink
 *  t => turquoise
 *  y => yellow
 *  w => white
 *  o => off
 *  i => idle, thats a rainbow 
 *  In addition single LEDs can be controlled, precede the color with the number of the LED, seperated
 *  by a colon.
 *  e.g. "2:g" will set LED Nr 2 (counting from 0) to green
 *  In additon you can also send the "number" a for all LEDs.
 *  This means you can either send "a:r" or "r" to set all LEDs to red.
 *  
 */ 
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 17
int idle = 1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void setup() {

  strip.begin();
  strip.setBrightness( 70 );
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  idle = 1; 
}


/*
  String definition :
  LedNr:Color
  LedNr = 0 ... 8 | a
  Color = red | blue | green | yellow | white | off | r-val,g-val,b-val 
  (r-val,g-val,b-val not yet implemented...)
*/

void loop() 
{
   String content = "";
   char character;
   int AllLeds = 0;
   int LedNr = 0;

   if ( Serial.available() > 0 )
   { 
      content = "";
      while(Serial.available()) 
      {
         character = Serial.read();
         content.concat(character);
      }
      if (content != "") 
      {
         char ColorByte ;
         int commaIndex = content.indexOf(':');
         if ( commaIndex >= 0 )
         {
            String LedNrStr = content.substring( 0, commaIndex);
            if( LedNrStr == "a" )
            {
               AllLeds = 1;
            }
            else
            {
               AllLeds = 0;
               LedNr = LedNrStr.toInt();
            }
         
            String Color = content.substring( commaIndex + 1 );
            ColorByte = Color[0];
         }
         else // did not find a ':' This means : old behaviour, no selectable led and only simple colors 
         {
            ColorByte = content[0];
            AllLeds = 1;
         }
         int oldidle = idle;
         idle = 0;
         switch (ColorByte)
         {
            case 'r':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(255, 0, 0), 100); // Red
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(255,0,0));
               }
               break;
            case 'g':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(0, 255, 0), 100); // Green 
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(0, 255, 0)); // Green
               }
               break;
            case 'b':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(0, 0, 255), 100); // Blue
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(0,0,255)); // Blue
                }
               break;
            case 'p':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(255, 0, 255), 100); // Red + Blue
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(255,0,255)); // Red + Blue
               }
               break;
            case 't':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(0, 255, 255), 100); // Green + Blue
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(0 , 255, 255)); // Green + Blue
               }
               break;
            case 'y':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(255, 255, 0 ), 100); // Red + Green
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(255, 255, 0 )); // Red + Green
               }
               break;
            case 'w':
               if( AllLeds )
               {
                  AnimatedColorWipe( strip.Color(255, 255, 255), 100); // All 3
               }
               else
               {
                  ShowOneLed( LedNr, strip.Color(255,255,255)); // all 3
               }
               break;
            case 'i':
               idle = 1;
               break;
            case 'o':
               colorWipeInvers(strip.Color(0, 0, 0), 50); // off
               break;
            default:
               idle = oldidle;
               break;
         }
      }
   }
   if (idle == 1 )
   {   
      rainbowCycle( 20 );
   }
}

void ShowOneLed( uint16_t LedNr, uint32_t color )
{
   strip.setPixelColor( LedNr, color );
   strip.show();
}

void AnimatedColorWipe( uint32_t color, uint8_t wait)
{
   colorWipeInvers( strip.Color(0, 0, 0), 10); // off
   colorWipe( color, wait); 
}
    

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipeInvers(uint32_t c, uint8_t wait) {
  for(uint16_t i=strip.numPixels() ; i > 0; i--) {
      strip.setPixelColor(i-1, c);
      strip.show();
      delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      // as the two loops can take a rather long time, we will watch in the inner loop the serial port
      // this is fast enough for "immediate" reaction and avoids the installation of interrupts ... 
      if ( Serial.available() > 0  )  
      {
         return;
      }
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

