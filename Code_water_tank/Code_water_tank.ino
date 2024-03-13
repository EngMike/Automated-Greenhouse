#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int TRIG_PIN   = 8; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN   = 7; // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int RELAY_PIN  = 2; // Arduino pin connected to Relay

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // create SSD1306 display object connected to I2C

String tempString;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT); // Set TRIG_PIN as OUTPUT
  pinMode(ECHO_PIN, INPUT);  // Set ECHO_PIN as INPUT
  pinMode(RELAY_PIN, OUTPUT); // Set RELAY_PIN as OUTPUT

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);      // text size
  oled.setTextColor(WHITE); // text color
  oled.setCursor(0, 10);    // position to display

  tempString.reserve(10);   // to avoid fragmenting memory when using String
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, LOW);  // Ensure the TRIG_PIN is LOW before the pulse
  delayMicroseconds(2);          // Small delay for safety
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  long duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  float distance_cm = duration_us * 0.0343 / 2; // Divide by 2 as we are calculating one-way distance

  // Control the relay based on distance
  if (distance_cm >= 50) {
    digitalWrite(RELAY_PIN, LOW); // Turn on relay if distance is 50 cm or above
  } else if (distance_cm <= 10) {
    digitalWrite(RELAY_PIN, HIGH);  // Turn off relay if distance is 10 cm or below
  }

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  tempString  = String(distance_cm, 2); // two decimal places
  tempString += " cm";

  Serial.println(tempString);    // print the distance in cm to Serial Monitor
  oledDisplayCenter(tempString); // display distance on OLED

  delay(1000);
}

void oledDisplayCenter(String text) {
  int16_t x1, y1;
  uint16_t width, height;

  oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // display on horizontal and vertical center
  oled.clearDisplay(); // clear display
  oled.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  oled.println(text); // text to display
  oled.display();
}
