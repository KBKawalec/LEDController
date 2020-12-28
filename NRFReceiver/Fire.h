#ifndef _FIRE_H
#define _FIRE_H
#include <Arduino.h>
#include <FastLED.h>
#include "NRFReceiver.h"

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 35
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING  40

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100



//CRGBPalette16 gPal;


// This first palette is the basic 'black body radiation' colors,
// which run from black to red to bright yellow to white.
//gPal = HeatColors_p;

// These are other ways to set up the color palette for the 'fire'.
// First, a gradient from black to red to yellow to white -- similar to HeatColors_p
//   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

// Second, this palette is like the heat colors, but blue/aqua instead of red/yellow

extern void  doFire(int red, int green, int blue, int beginningStrip, int endStrip);
extern void Fire2012WithPalette(int beginningStrip, int endStrip);

//  = CRGBPalette16( CRGB::Black, CRGB::HotPink , CRGB::Amethyst ,  CRGB::White);

#endif
