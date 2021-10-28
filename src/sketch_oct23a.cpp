#include <Arduino.h>
#include <FastLED.h>
#include "command.h"

#define LED_PIN 22
#define NUM_LEDS 96
#define BRIGHTNESS 10
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define MAX_DELAY 30 // Time (in milliseconds) to pause between pixels

bool isRunning;
long setTime;
long lightMode;

void setup()
{
  Serial.begin(115200);
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  isRunning = false;
  setTime = 60;
  lightMode = 1;

  FastLED.clear(); // clear all pixel data
  leds[0] = CRGB(255, 0, 0);
  leds[NUM_LEDS - 1] = CRGB(255, 0, 0);
}

void loop()
{
  if (!isRunning)
  {
    ParserLoop(&isRunning, &setTime, &lightMode);
  }

  else
  {
    FastLED.clear(); // clear all pixel data
    for (int i = 0; i < NUM_LEDS; i++)
    { // For each pixel...
      leds[i] = CRGB(0, 255, 0);
      FastLED.show();
      delay(MAX_DELAY); // Pause before next pass through loop
    }
    Serial.println("Lighting finished. back to settings");
    isRunning = false;
  }
}
