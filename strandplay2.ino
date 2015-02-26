#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);

// For 36mm LED pixels: these pixels internally represent color in a
// different format.  Either of the above constructors can accept an
// optional extra parameter: WS2801_RGB is 'conventional' RGB order
// WS2801_GRB is the GRB order required by the 36mm pixels.  Other
// than this parameter, your code does not need to do anything different;
// the library will handle the format change.  Examples:
//Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin, WS2801_GRB);
//Adafruit_WS2801 strip = Adafruit_WS2801(25, WS2801_GRB);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

  setRainbowSegment(0, 360, 1.0, 0.01);
  delay( 3000);
}


void loop() {
  // Some example procedures showing how to display to the pixels
  
//  colorWipe(Color(255, 0, 0), 50);
//  colorWipe(Color(0, 255, 0), 50);
//  colorWipe(Color(0, 0, 255), 50);
//  rainbow(20);
//  rainbowCycle(20);

//  color_chase( Color( 8, 0, 0));
//  color_chase( Color( 0, 8, 0));
//  color_chase( Color( 0, 0, 8));

  /* float hue; */
  /* float r, g, b; */
  /* int i; */
  /* for (i = 0; i < 360; i += 10) */
  /* { */
  /*   hsl2rgb( i/360.0, 1, 0.1, &r, &g, &b); */
  /*   uint32_t color = Color( r*256, g*256, b*256);   */
  /*   pixel_move( color, 25); */
  /*   color_chase( color, 35); */
  /* } */

  rainbowCycleFadeInOut();
}

void rainbowCycleFadeInOut()
{
   // Loop: hue circle segment, say 60 degrees, step by 30 degrees,
   // set into pixels.  Full saturation.
   // Loop: sine wave lightness [0..1]
   
   float maxLightness = 0.9; // Keep the pixels from blinding us.
   int hueAngle;
   int hueAngleStep = 60;
   float sat = 1.0;
   float light = 0;
   float theta;
   float pi = 3.14159;
   int lightStep = 100;
   int lightCycleTime = 3;      /* Seconds */
   int delayMillis = (int) (lightCycleTime * 1000.0 / lightStep);
   float sinTheta01; // sin(theta) scaled to interval [0..1]
   float powSin; // sinTheta01 raised to some power (remains in interval [0..1]).
   
   // Plain white
    for (theta = 0; theta < pi; theta += pi/200.0)
    {
       sinTheta01 = (sin( theta) + 1.0)/2.0;
       powSin = pow( sinTheta01, 6);
       light = powSin * maxLightness;
       setRainbowSegment( 0.0, 0.0, 0.0, light); // sat, light);
       delay( 20);
    }

   for (hueAngle = 0; hueAngle < 360; hueAngle += hueAngleStep)
   {
      for (theta = 0; theta < pi; theta += pi/200.0)
      {
         sinTheta01 = (sin( theta) + 1.0)/2.0;
         powSin = pow( sinTheta01, 6);
         light = powSin * maxLightness;
         setRainbowSegment( hueAngle, hueAngle + 2 * hueAngleStep, 1.0, light); // sat, light);
         delay( 20);
      }
   }
}

/* ---------------------------------------------------------------- */
/*  setRainbowSegment */
/* ---------------------------------------------------------------- */

/* angles in degrees; saturation and lightness in range [0..1].
 */
void setRainbowSegment( int aStartAngle, int aStopAngle, float aSaturation, float aLightness)
{
   int stopAngle;
   int angle;
   float dAngle;
   int i;
   float hue;
   float r,g,b;
   int r256, g256, b256;
   uint32_t color;
   
   while (stopAngle < aStartAngle)
     stopAngle += 360;
   dAngle = (float) (stopAngle - aStartAngle) / 25.0;
   
   for (i = 0; i < 25; i++)
   {
      angle = (int) (aStartAngle + i * dAngle + 0.5) % 360;
      hue = (float) (angle % 360)/360.0;
      hsl2rgb( hue, aSaturation, aLightness, &r, &g, &b);
      r256 = constrain( (int) (r * 256), 0, 255);
      g256 = constrain( (int) (g * 256), 0, 255);
      b256 = constrain( (int) (b * 256), 0, 255);
      color = Color( (byte) r256, (byte) g256, (byte) b256);
      strip.setPixelColor( i, color);
   }
   strip.show();
}

/* ---------------------------------------------------------------- */
/*  pixel_move */
/* ---------------------------------------------------------------- */

/* A single pixel moves along the strip once, from start to finish.
 * Does even-numbered lights, then odd, as if the strip is still folded up.
 */
void pixel_move( uint32_t aColor, int aDelay)
{
  int i;

  // --------------------------------------- evens -------------------
  strip.setPixelColor( 0, aColor);
  strip.show();
  delay( aDelay);
  for (i = 2; i < 25; i += 2)
  {
    strip.setPixelColor( i - 2, Color( 0, 0, 0));
    strip.setPixelColor( i, aColor);
    strip.show();
    delay( aDelay);
  }
  strip.setPixelColor( 24, Color( 0, 0, 0));

  // -------------------------------------- odds ----------------------
  strip.setPixelColor( 1, aColor);
  strip.show();
  delay( aDelay);
  for (i = 3; i < 25; i += 2)
  {
    strip.setPixelColor( i - 2, Color( 0, 0, 0));
    strip.setPixelColor( i, aColor);
    strip.show();
    delay( aDelay);
  }
  strip.setPixelColor( 23, Color( 0, 0, 0));

  strip.show();
}

/* ---------------------------------------------------------------- */
/*  color_chase */
/* ---------------------------------------------------------------- */

/* Color/off chase each other.
 */
void color_chase(uint32_t aColor, int aDelay)
{
  int i;

  for (i = 0; i < 25; i++)
  {
    strip.setPixelColor(i, aColor);
    strip.show();
    delay( aDelay);
  }

  for (i = 0; i < 25; i++)
  {
    strip.setPixelColor(i, Color(0, 0, 0));
    strip.show();
    delay( aDelay);
  }
}

/* ---------------------------------------------------------------------------------------------- */
/*  hsl2rgb */
/* ---------------------------------------------------------------------------------------------- */

/* CSS3 spec algorithm: http://www.w3.org/TR/2011/REC-css3-color-20110607/#hsl-color
 * All params in range [0.0..1.0]
 */
void hsl2rgb( float hue, float sat, float light, float *r, float *g, float *b)
{
   float m1, m2;
   
   if (light < 0.5)
      m2 = light * (sat + 1.0);
   else
      m2 = light + sat - light * sat;
   m1 = light * 2.0 - m2;
   *r = hue2rgb( m1, m2, hue + 1.0/3.0);
   *g = hue2rgb( m1, m2, hue);
   *b = hue2rgb( m1, m2, hue - 1.0/3.0);
}

/* ---------------------------------------------------------------------------------------------- */
/*  hue2rgb */
/* ---------------------------------------------------------------------------------------------- */

/* CSS3 spec algorithm: http://www.w3.org/TR/2011/REC-css3-color-20110607/#hsl-color
 */
float hue2rgb( float m1, float m2, float hue )
{
   float retval;
   float h = hue;
   
   if (hue < 0.0)
      h = hue + 1.0;
   else if (hue > 1.0)
      h = hue - 1.0;
   else
      h = hue;

   if (h * 6.0 < 1.0)
      retval = m1 + (m2 - m1) * h * 6.0;
   else if (h * 2.0 < 1.0)
      retval = m2;
   else if (h * 3.0 < 2.0)
      retval = m1 + (m2 - m1) * (2.0 / 3.0 - h) * 6.0;
   else
      retval = m1;

   return retval;
}

//--------------------------------------------------------------------------------------

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
