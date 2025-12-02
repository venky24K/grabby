#include <Ucglib.h>
#include "HX711.h"

// ------------------------------
// Pins
// ------------------------------
#define DC_PIN 2
#define CS_PIN 14
#define RESET_PIN 4
#define SCK_PIN 18
#define MOSI_PIN 19

// IR sensors
#define IR1 26
#define IR2 32
#define IR3 34

// Relays (ACTIVE-LOW)
#define RELAY1 27
#define RELAY2 25

// HX711 (Load cell)
#define LOADCELL_DOUT 12
#define LOADCELL_SCK 13

// ------------------------------
// Objects
// ------------------------------
Ucglib_ST7735_18x128x160_SWSPI ucg(SCK_PIN, MOSI_PIN, DC_PIN, CS_PIN, RESET_PIN);
HX711 scale;

// ------------------------------
// Relay & Display Timers / Flags
// ------------------------------
unsigned long relay2Start = 0;
bool relay2Running = false;
bool displayShown = false;  // Display flag

// ------------------------------
// Display Functions
// ------------------------------
void clearDisplay() {
  ucg.setColor(0,0,0);
  ucg.drawBox(0,0,128,160);
}

void showDisplay() {
  ucg.setColor(0,0,0);
  ucg.drawBox(0,0,128,160);

  ucg.setColor(0,255,0);
  ucg.setFont(ucg_font_fur14_tr);

  ucg.setPrintPos(5, 30);  ucg.print("SRMAP");
  ucg.setPrintPos(5, 60);  ucg.print("Team 21");
  ucg.setPrintPos(5, 90);  ucg.print("Simulation");
  ucg.setPrintPos(5, 120); ucg.print("Completed");
}

// ------------------------------
// Setup
// ------------------------------
void setup() {
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Active-low relays → OFF
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  // HX711 initialization
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  scale.set_scale(2280.0 / 4.0); // Example factor (adjusted to your reading)
  scale.tare();                  // Zero the load cell

  clearDisplay();  // OLED blank at start
}

// ------------------------------
// Loop
// ------------------------------
void loop() {
  // Read weight (average of 5 readings for stability)
  float weight = scale.get_units(5);

  // ===== RELAY1 (IR1 → Water Pump) =====
  if (digitalRead(IR1) == LOW) {
    // IR1 detected → turn motor ON
    digitalWrite(RELAY1, LOW); // Active-low ON
  }

  // Keep motor ON until weight >= 125g
  if (weight >= 125.0) {
    digitalWrite(RELAY1, HIGH); // OFF
  }

  // ===== RELAY2 (IR3 priority, IR2 one-shot) =====
  if (digitalRead(IR3) == LOW) {
    // IR3 detected → Relay2 fully ON, refresh display
    digitalWrite(RELAY2, LOW); // Active-low ON
    relay2Running = false;      // cancel IR2 timer

    if (!displayShown) {
      showDisplay();
      displayShown = true;      // Lock display ON forever
    }
  }
  else {
    // IR3 not detected → normal IR2 control
    if (!relay2Running && digitalRead(IR2) == LOW) {
      digitalWrite(RELAY2, LOW); // ON
      relay2Start = millis();
      relay2Running = true;
    }

    if (relay2Running && millis() - relay2Start >= 4000) {
      digitalWrite(RELAY2, HIGH); // OFF
      relay2Running = false;
    }
  }
}