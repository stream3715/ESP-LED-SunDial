#include <Arduino.h>
#include <FastLED.h>
#include "command.h"

#define LED_PIN 27
#define NUM_LEDS 96
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
#define WAIT_EACH_CYCLES 41

CRGB leds[NUM_LEDS];

#define MAX_DELAY 30 // Time (in milliseconds) to pause between pixels

bool isConfigured, isRunning;
long setTime, lightMode, brightness = 10;

uint64_t globalLoopSum = NUM_LEDS * 255;
uint8_t maxHue[] = {170, 255};
uint32_t waitTime;

uint8_t globalHue = 0;
int currentLed = 0;

void setup()
{
  Serial.begin(115200);
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  isConfigured = false;
  isRunning = false;
  setTime = 90;
  lightMode = 1;

  FastLED.clear(); // clear all pixel data
  leds[0] = CRGB(255, 0, 0);
  leds[NUM_LEDS - 1] = CRGB(255, 0, 0);
  FastLED.show();
}

uint8_t calcSinedVal(int raw)
{
  return 255 * sin(raw / 255.0);
}

uint8_t calcHue(int currentIndex)
{
  double step = maxHue[lightMode - 1] / (double)NUM_LEDS;
  return (uint8_t)currentIndex * step;
}

void resetState()
{
  isConfigured = false;
  isRunning = false;
  FastLED.clear(true);
  currentLed = 0;
}

void loop()
{
  if (!isConfigured)
  {
    ParserLoop(&isConfigured, &setTime, &lightMode, &brightness);
  }

  else if (!isRunning)
  {
    FastLED.setBrightness(brightness);
    waitTime = (setTime - 74) * WAIT_EACH_CYCLES;

    FastLED.clear(); // clear all pixel data
    isRunning = true;
  }

  else
  {
    for (int i = 1; i < 256; i++)
    {
      leds[currentLed] = CHSV(calcHue(currentLed), 255, calcSinedVal(i));
      if (currentLed != 0)
      {
        leds[currentLed - 1] = CHSV(calcHue(currentLed - 1), 255, calcSinedVal(255 - i));
      }
      FastLED.show();
      ets_delay_us(waitTime);
    }
    currentLed++;

    if (currentLed == NUM_LEDS)
    {
      resetState();
      Serial.println("Lighting finished. back to settings");
    }
  }
}
