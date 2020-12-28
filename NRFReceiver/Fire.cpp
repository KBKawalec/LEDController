#include "Fire.h"

CRGBPalette16 gPal;
bool gReverseDirection = false;
void doFire(int red, int green, int blue, int beginningStrip, int endStrip)
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());
  gPal = CRGBPalette16( CRGB::Black, CRGB(red, green, blue));

  //
  //    static uint8_t hue = 0;
  //    //hue++;
  //     CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //     CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //     gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);


  Fire2012WithPalette( beginningStrip, endStrip); // run simulation frame, using palette colors

  // FastLED.delay(1000 / FRAMES_PER_SECOND);
}

//



void Fire2012WithPalette(int beginningStrip, int endStrip)
{
  // Array of temperature readings at each simulation cell
  int ledSize = endStrip - beginningStrip;
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < ledSize; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = ledSize - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = beginningStrip; j < endStrip; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j - beginningStrip], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
