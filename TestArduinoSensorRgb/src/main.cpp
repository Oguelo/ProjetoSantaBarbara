#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <TimeLib.h>
// LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_TCS34725 sensorCor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

float valor_calibracao = 21.34; // Fator de calibração
extern volatile unsigned long zero_millis;
int contagem = 0;
int LABEL = 1;
float soma_tensao = 0; // Variável para soma de tensão
float media = 0;       // Variável que calcula a media
float entrada_A0;      // Variável de leitura do pino A0
float tensao;
time_t tempoInicial = 0;
time_t tempoFinal;
time_t t;
time_t tempoMedio;
String valor_str = "<";


void setup(void)
{
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL, tempo_seg, valor_ph");
  // lcd.init();
  // lcd.begin(16,2);

  // lcd.backlight();
  // lcd.setCursor(0,0);
  // lcd.print("Estado:");
  if (sensorCor.begin())
  {
    Serial.print("");
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
  sensorCor.getRawData(&r, &g, &b, &c);
  ColorTemp = sensorCor.calculateColorTemperature(r, g, b);
  Lux = sensorCor.calculateLux(r, g, b);
  int rosa = 5000;
  int lilas = 200;


  if (c < 5000)
  {
   
    while (!( lilas < r && r < rosa)) // temporario ate saber o intervalo
    {

      Serial.print("Vermelho : "); Serial.print(r); Serial.print(" ");
      while (contagem < 10)
      {                                       // Executa enquanto contagem menor que 10
        t = minute(t);                                     
        entrada_A0 = analogRead(A0);          // Lê a entrada analógica
        tensao = (entrada_A0 * 5.0) / 1024.0; // Converte em tensão, o valor lido
        soma_tensao = (soma_tensao + tensao); // Soma a tensão anterior com a atual
        contagem++;                           // Soma 1 à variável de contagem
        delay(100);                           // Aguarda para próxima leitura
      }
      tempoMedio = minute(tempoMedio) - t;
      tempoFinal = tempoMedio;

      media = soma_tensao / 10;
      float valor_ph = -5.70 * media + valor_calibracao;
      Serial.print(tempoFinal);
      Serial.print(";");
      Serial.println(valor_ph);
      tempoInicial = tempoInicial + tempoMedio;
      sensorCor.getRawData(&r, &g, &b, &c);
   
      // valor_str.concat(tempo_seg);
      //  valor_str.concat(",");
      //valor_str.concat(valor_ph);
    
      
      // passo os resultados pro excell
      contagem = 0;
      delay(5000);
     
    }
    Serial.print("Analise Concluida");
    delay(3000);
    noInterrupts();
  }
  
  else
  {
    Serial.print("O sensor RGB esta saturado");
  }
}
