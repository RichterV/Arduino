#include <dht11.h>
#define DHT11PIN 4

#include "LiquidCrystal_I2C.h"
#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"

#define printIIC(args) Wire.write(args)

inline size_t LiquidCrystal_I2C::write(uint8_t value) {
    send(value, Rs);
    return 1;
}
#endif

LiquidCrystal_I2C lcd(0x27, 16, 2);

dht11 DHT11;
int ledPin = 2;

static float last_temp = -1000;
static float last_hum = -1;
bool ledState = false;

void setup()
{
  pinMode(ledPin, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {
  int chk = DHT11.read(DHT11PIN);

  float current_temp = (float)DHT11.temperature;
  float current_hum = (float)DHT11.humidity;

  // Verifica se houve alteração na temperatura
  if (current_hum != last_hum) {
    last_hum = current_hum;
    ledState = true;
  }
  if (current_temp != last_temp){
    last_temp = current_temp;
    ledState = true;
  }


  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(current_hum, 1);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(current_temp, 1);
  lcd.write(223);
  lcd.print("C");

  // Pisca o led
  if (ledState) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    ledState = false;
  }
}
