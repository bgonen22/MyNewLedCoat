#include <FastLED.h>

#define LED_PIN     6  // Pin where the LED strip is connected
#define NUM_LEDS    60 // Total number of LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define COMET_LENGTH 15  // Length of each comet
#define NUM_COMETS  (NUM_LEDS / COMET_LENGTH)  // Adjusted to eliminate gaps

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    multiCometEffect();
}

void multiCometEffect() {
    static uint8_t hue = 0;
    static int positions[NUM_COMETS];
    static bool initialized = false;
    
    if (!initialized) {
        for (int i = 0; i < NUM_COMETS; i++) {
            positions[i] = NUM_LEDS / 2 + ((i - NUM_COMETS / 2) * COMET_LENGTH);
        }
        initialized = true;
    }

    FastLED.clear();
    hue += 2;  // Shift hue slightly for a rainbow effect
    
    for (int c = 0; c < NUM_COMETS; c++) {
        for (int i = 0; i < COMET_LENGTH; i++) {
            int pos = NUM_LEDS/2 + positions[c] - i;
            int pos_rev = NUM_LEDS/2 - positions[c] + i;
            if (pos >= 0 && pos < NUM_LEDS) {
                leds[pos] = CHSV(hue + c * (255 / NUM_COMETS), 255, 255 - (i * (255 / COMET_LENGTH)));
                leds[pos_rev] = CHSV(hue + c * (255 / NUM_COMETS), 255, 255 - (i * (255 / COMET_LENGTH)));
            }
        }
        
        positions[c]++; // Move outward
        if (positions[c] >= NUM_LEDS ) positions[c] = 0; // Wrap around instead of resetting to center
    }
    
    FastLED.show();
    delay(50);
}
