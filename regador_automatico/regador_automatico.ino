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
//CALIBRAÇÃO DO SENSOR
const int solo_seco = 1023;
const int solo_encharcado = 200;
//DEFINIÇÃO DE PINOS DE LEITURA E PINO DO RELÉ
// Define o pino do sensor de umidade do solo
const int sensorPin = A0;
// Define o pino do sensor que liga a bomba de água
const int relePin = 7;
// Define o percentual em que a bomba de água será ligada
//DEFINIÇÃO DE PARAMETROS PARA REGA
//umidade mínima
const int umidade_min = 60;
// Define o tempo em que a bomba de água ficará ligada (milisegundos)
const int tempo_rega = 5000;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(1);
  lcd.clear();

  // Configura o pino do relé como saída
  pinMode(relePin, OUTPUT);
}

void loop() {
  // Lê o valor do sensor de umidade do solo
  int sensorValue = analogRead(sensorPin);

  // Converte o valor do sensor em percentual
  int percentual = map(sensorValue, solo_encharcado, solo_seco, 100, 0);

  // Exibe o valor no LCD
  lcd.setCursor(1,0);
  lcd.print("Umidade: ");
  lcd.print(percentual);
  lcd.print("%             ");
  lcd.setCursor(1,1);
  lcd.print("                     ");

  // Exibe o valor no monitor serial

  Serial.print("Valor do sensor: ");
  Serial.print(sensorValue);
  Serial.print("-Umidade do solo: ");
  Serial.print(percentual);
  Serial.println("%");
  
  

  // Verifica se a umidade é menor que a umidade minima
  if (percentual <= umidade_min) {
    // Liga a bomba de água
    digitalWrite(relePin, HIGH);
    lcd.setCursor(1,0);
    lcd.print("Umidade: ");
    lcd.print(percentual);
    lcd.print("%                ");
    lcd.setCursor(1,1);
    lcd.print("Regando...          ");
    delay(tempo_rega);
  } else {
    // Desliga a bomba de água
    digitalWrite(relePin, LOW);
  }

  // Aguarda por um breve intervalo antes de fazer a próxima leitura
  delay(1000);
}
