#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows
DHT dht(DHTPIN, DHTTYPE);

#define RELAY_PIN_PUMP    7  // Arduino pin that controls the pump via relay
#define MOISTURE_PIN A0 // Arduino pin that connects to AOUT pin of moisture sensor
#define THRESHOLD 530   // => CHANGE YOUR THRESHOLD HERE

const int LIGHT_SENSOR_PIN = A1; // Arduino pin connected to light sensor's  pin
const int RELAY_PIN_LIGHT  = 8;  // Arduino pin connected to Relay's pin
const int ANALOG_THRESHOLD = 500;
int analogValue;


void setup()
{
  dht.begin();     // initialize the sensor
  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight
  pinMode(RELAY_PIN_PUMP, OUTPUT);
  pinMode(RELAY_PIN_LIGHT, OUTPUT); 
  Serial.begin(9600);
}

void loop()
{
  delay(1000); // wait a few seconds between measurements

// Temperature and humidity measurement
int humi  = dht.readHumidity();    // read humidity
int tempC = dht.readTemperature(); // read temperature


// Soil monitorization and watering soil
int value = analogRead(MOISTURE_PIN); // read the analog value from sensor

  if (value > THRESHOLD) {
    Serial.print("The soil moisture is DRY => activate pump");
    digitalWrite(RELAY_PIN_PUMP, LOW);
  } else {
    Serial.print("The soil moisture is WET => deactivate the pump");
    digitalWrite(RELAY_PIN_PUMP, HIGH);
  }

  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");

  // UV Light mode
analogValue = analogRead(LIGHT_SENSOR_PIN); // read the input on analog pin

  if(analogValue < ANALOG_THRESHOLD)
    digitalWrite(RELAY_PIN_LIGHT, LOW); // turn on Relay
  else
    digitalWrite(RELAY_PIN_LIGHT, HIGH);  // turn off Relay


// LCD displayed values
  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("T:");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");

    lcd.setCursor(8, 0);  // start to print at the second row
    lcd.print("H:");
    lcd.print(humi);      // print the humidity
    lcd.print("%");
  }
   lcd.setCursor(0, 1);  // start to print at the second row
   lcd.print("Soil:");
   lcd.print(value);      // print the humidity
}