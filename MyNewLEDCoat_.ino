#include <FastLED.h>

#define START_LED 50
#define END_LED 21
#define LED_PIN     6  // Pin where the LED strip is connected
#define NUM_LEDS    60 // Total number of LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define COMET_LENGTH 5  // Length of each comet
// #define NUM_COMETS  ((END_LED-START_LED) / COMET_LENGTH)  // Adjusted to eliminate gaps
#define NUM_COMETS  ((NUM_LEDS / COMET_LENGTH) + 1)/2  // Adjusted to eliminate gaps

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
            // positions[i] = START_LED + NUM_LEDS / 2 + ((i - NUM_COMETS / 2) * COMET_LENGTH);
            // positions_rev[i] = START_LED - (NUM_LEDS / 2 + ((i - NUM_COMETS / 2) * COMET_LENGTH));
            positions[i] = (START_LED + (i * COMET_LENGTH))% NUM_LEDS;
            positions_rev[i] = (START_LED - (i * COMET_LENGTH)) % NUM_LEDS;
            // Serial.print("i ");
            // Serial.print(i);
            // Serial.print(" position ");
            // Serial.print(positions[i]);
            // Serial.print(" position_rev ");
            // Serial.println(positions_rev[i]);
        }
        initialized = true;
    }

    FastLED.clear();
    hue += 2;  // Shift hue slightly for a rainbow effect
    
    // Serial.print(":1 Position ");
    // Serial.println(positions_rev[0]);
    for (int c = 0; c < NUM_COMETS ; c++) {
        // Serial.print("Comet ");
        // Serial.print(c);
        // Serial.print(": Position ");
        // Serial.println(positions[c]);
        for (int i = 0; i < COMET_LENGTH; i++) {
        // for (int i = 0; i < 1; i++) {
            int pos = positions[c] - i;
            int pos_rev = positions_rev[c] + i;
            // Serial.print("pos ");
            // Serial.println(pos);
            // Serial.print("pos_rev ");
            // Serial.println(pos_rev);
            CRGB color = CHSV(hue + c * (255 / (2*NUM_COMETS)), 255, 255 - (i * (255 / COMET_LENGTH)));
            if (END_LED > START_LED) {
              if (pos >= START_LED && pos <= END_LED) {
                  leds[pos] = color;
              }
              if ((pos_rev >= 0 && pos_rev < START_LED) || (pos_rev >= END_LED && pos_rev < NUM_LEDS)) {
                  leds[pos_rev] = color;
              }
            } else {
              if ((pos >= START_LED && pos <= NUM_LEDS) || (pos >= 0 && pos < END_LED)) {
                  leds[pos] = color;
              }
              if ((pos_rev < START_LED) && (pos_rev >= END_LED)) {
                  leds[pos_rev] = color;
              }
            }
        }
        
        positions[c]++; // Move outward
        positions_rev[c]--; // Move rev
        if (END_LED > START_LED) {
          if (positions[c] == END_LED + 1) positions[c] = START_LED + 1; // Wrap around instead of resetting to center
          if (positions_rev[c] < 0 ) { 
            if (END_LED == NUM_LEDS) {
              positions_rev[c] = START_LED ; // Wrap around instead of resetting to center
            } else {
              positions_rev[c] = NUM_LEDS;
            }
          }
          if (positions_rev[c] == END_LED) positions_rev[c] = START_LED; // Wrap around instead of resetting to center
        } else {
          if (positions[c] == NUM_LEDS) positions[c] = 0; // Wrap around instead of resetting to center
          if (positions[c] == END_LED) positions[c] = START_LED; // Wrap around instead of resetting to center
          if (positions_rev[c] == END_LED) positions_rev[c] = START_LED; // Wrap around instead of resetting to center          
        }
        
    }
    // Serial.print(":2 Position_rev ");
    //   Serial.println(positions_rev[0]);
    
    FastLED.show();
    delay(100);
}
