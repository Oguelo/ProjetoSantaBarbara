#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
// LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

float valor_calibracao = 21.34; // Fator de calibração

int contagem = 0;      // Variável de contagem
float soma_tensao = 0; // Variável para soma de tensão
float media = 0;       // Variável que calcula a media
float entrada_A0;      // Variável de leitura do pino A0
float tensao;
float t = millis();
float tempo;

void setup(void)
{
  Serial.begin(9600);
  // lcd.init();
  // lcd.begin(16,2);

  // lcd.backlight();
  // lcd.setCursor(0,0);
  // lcd.print("Estado:");
  if (sensor.begin())
  {
    Serial.print("ok");
    // lcd.setCursor(0,8);
    // lcd.print("Ok!");
  }
  else
  {
    Serial.println("Dispositivos não encontrado...");
    // lcd.setCursor(0,8);
    // lcd.print("Fail!");
    while (1)
      ;
  }
}

void loop(void)
{
  soma_tensao = 0; // Inicia soma_tensão em 0
  contagem = 0;
  uint16_t r, g, b, c, ColorTemp, Lux;
  sensor.getRawData(&r, &g, &b, &c);
  ColorTemp = sensor.calculateColorTemperature(r, g, b);
  Lux = sensor.calculateLux(r, g, b);
  tempo = (float)t / 1000.;
  if (c < 5000)
  {
    while (400 < r < 900) // temporario ate saber o intervalo
    {
      while (contagem < 10)
      {                                       // Executa enquanto contagem menor que 10
        tempo = millis();                     // Define o tempo em microssegundossrc/main.cpp
        entrada_A0 = analogRead(A0);          // Lê a entrada analógica
        tensao = (entrada_A0 * 5.0) / 1024.0; // Converte em tensão, o valor lido
        soma_tensao = (soma_tensao + tensao); // Soma a tensão anterior com a atual
        contagem++;                           // Soma 1 à variável de contagem
        delay(100);                           // Aguarda para próxima leitura
      }
      t = (float)tempo / 1000.;
      media = soma_tensao / 10;
      float valor_pH = -5.70 * media + valor_calibracao;
      //passo os resultados pro excell
      contagem = 0;
    }

  }else {
    Serial.print("O sensor RGB esta saturado")
  }
