#include <FastLED.h>

#define START_LED 10
#define END_LED 50
#define LED_PIN     6  // Pin where the LED strip is connected
#define NUM_LEDS    60 // Total number of LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define COMET_LENGTH 15  // Length of each comet
#define NUM_COMETS  (NUM_LEDS / COMET_LENGTH)  // Adjusted to eliminate gaps

CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(9600); // Initialize serial communication

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
    static int positions_rev[NUM_COMETS];
    static bool initialized = false;
    
    if (!initialized) {
        for (int i = 0; i < NUM_COMETS; i++) {
            positions[i] = START_LED + NUM_LEDS / 2 + ((i - NUM_COMETS / 2) * COMET_LENGTH);
            positions_rev[i] = START_LED - (NUM_LEDS / 2 + ((i - NUM_COMETS / 2) * COMET_LENGTH));
        }
        initialized = true;
    }

    FastLED.clear();
    hue += 2;  // Shift hue slightly for a rainbow effect
    
    for (int c = 0; c < NUM_COMETS; c++) {
        // Serial.print("Comet ");
        // Serial.print(c);
        // Serial.print(": Position ");
        // Serial.println(positions[c]);
        for (int i = 0; i < COMET_LENGTH; i++) {
            int pos = positions[c] - i;
            int pos_rev = positions_rev[c] + i;
            // Serial.print("pos ");
            // Serial.println(pos);
            // Serial.print("pos_rev ");
            // Serial.println(pos_rev);
            if (pos >= START_LED && pos <= END_LED) {
                leds[pos] = CHSV(hue + c * (255 / NUM_COMETS), 255, 255 - (i * (255 / COMET_LENGTH)));
            }
            if ((pos_rev >= 0 && pos_rev < START_LED) || (pos_rev >= END_LED && pos_rev < NUM_LEDS)) {
                leds[pos_rev] = CHSV(hue + c * (255 / NUM_COMETS), 255, 255 - (i * (255 / COMET_LENGTH)));
            }
        }
        
        positions[c]++; // Move outward
        if (positions[c] > END_LED ) positions[c] = START_LED; // Wrap around instead of resetting to center
        
        positions_rev[c]--; // Move rev
        
        if (positions_rev[c] < 0 ) positions_rev[c] = NUM_LEDS ; // Wrap around instead of resetting to center
        if (positions_rev[c] <= END_LED && positions_rev[c] > START_LED) positions_rev[c] = START_LED ; // Wrap around instead of resetting to center
    }
    
    FastLED.show();
    delay(50);
}
