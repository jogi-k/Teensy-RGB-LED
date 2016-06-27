/***********************************/
/* Teensy RGB-LED                  */
/***********************************/
/* 
 * Simple program to control a single Neo-Pixel WS2812-RGB-Led
 * Very simple schematics: 
 * 
 * +5 V ------------------ LED +5v
 * GND  -------------------LED GND
 * Pin 17 --- 330 Ohm ---- LED Data In
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
 *  
 */ 
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 17
#define version "1.00.01"

int idle = 1;
int flashing = 0;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)  
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

enum flashmode
{
   start,
   do_it,
   stop
};


void setup() {


  strip.begin();
  strip.setBrightness( 70 );
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  idle = 1; 
}


void loop() 
{
   while( Serial.available() > 0 )
   {
      char inchar;
      int oldIdle = idle; // save idle-status. Will be restored in case of unknow character
      idle = 0; // for most commands that will be the result: No idle, we will display a color.
      inchar = Serial.read();
      switch (inchar)
      {
         case 'r':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(255, 0, 0), 100); // Red
            break;
         case 'g':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(0, 255, 0), 100); // Green
            break;
         case 'b':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(0, 0, 255), 100); // Blue
            break;
         case 'p':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(255, 0, 255), 100); // Purple
            break;
         case 't':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(0, 255, 255), 100); // Turquoise
            break;
         case 'y':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(255, 255, 0), 100); // Yellow
            break;
         case 'w':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(255, 255, 255), 100); // white
            break;
         case 'i':
            idle = 1;
            break;
         case 'o':
            flash( stop );
            flashing = 0;
            colorWipe(strip.Color(0, 0, 0), 50); // off
            break;
         case 'f':
            flashing = 1;
            flash( start );
            break;
         case 'n':
            flashing = 0;
            flash( stop );
            break; 
         case 'v':
            Serial.print("Version: ");
            Serial.println(version);
            idle = oldIdle;
            break;
         case 'h':
            Serial.println("Help: known commands:");
            Serial.println("i -> idle");
            Serial.println("r -> red");
            Serial.println("g -> green");
            Serial.println("b -> blue");
            Serial.println("y -> yellow");
            Serial.println("p -> pink");
            Serial.println("t -> turquoise");
            Serial.println("w -> white");
            Serial.println("o -> off");
            Serial.println("f -> flash current color");
            Serial.println("n -> no flash");
            Serial.println("v -> version");
            Serial.println("h -> this help");
            idle = oldIdle;
            break;
         default:
         
            idle = oldIdle; // unknown character, so restore status and do nothing else.
            break;
      }
   }
   if (idle == 1 )
   { 
      rainbowCycle( 20 );
   }
   if( flashing == 1 )
   {
      flash( do_it );
   }
}

#define HALF_SEC_COUNTER       50   // 50 intervals a DELAY_TIME_IN_LOOP = 10ms = half a second
#define DELAY_TIME_IN_LOOP     10   // ms
#define DELAY_FOR_ANIMATION    50   // ms 

void flash( flashmode what )
{
   static int count = 0;
   static int color = 0;
   static int flashing = 0;
   static int led_high = 0;
   if ( what == start )
   {
      // avoid flashing when already flashing, this would lead in 50% of all cases to switched-off LED, when latching the color when it is off...
      if( flashing == 0 )
      {
         color = strip.getPixelColor( 0 );
         count = HALF_SEC_COUNTER - 1;  // counter expires "soon" 
         flashing = 1;
         led_high = 1;
      } 
   }
   else if ( what == stop )
   {
      if( color != 0 )
      {
         colorWipe( color, DELAY_FOR_ANIMATION );
      }
      flashing = 0;
   }
   else
   {
      delay( DELAY_TIME_IN_LOOP );
      count++;
      if ( count >= HALF_SEC_COUNTER  )
      {
         count = 0;
         if( led_high == 1 )
         {
            colorWipe(strip.Color(0, 0, 0), DELAY_FOR_ANIMATION ); // off
            led_high = 0; 
         }
         else
         {
            colorWipe( color, 50 );
            led_high = 1;
         }
      }
   }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
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

