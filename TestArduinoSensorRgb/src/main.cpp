#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <TimeLib.h>
#include <ArduinoSTL.h>
#include <list>

// LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_TCS34725 sensorCor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

float valor_calibracao = 21.34; // Fator de calibração
extern volatile unsigned long zero_millis;
int contagem = 0;
int LABEL = 1;
std::list<String> listaDados;
float entrada_A0; // Variável de leitura do pino A0
float tensao;
byte valorTela = 0;
String dadosJuntos;
int menu;

void exibirDados(String dadosJuntos)
{
  Serial.println(dadosJuntos);
}
void limparMonitorSerial() {
  Serial.flush(); // Descarta todos os dados na saída serial
  Serial.write(27); // Envia o caractere de escape
  Serial.print("[2J"); // Envia a sequência de escape para limpar a tela
  Serial.write(27); // Envia o caractere de escape
  Serial.print("[H"); // Envia a sequência de escape para posicionar o cursor no início da tela
}

String medicao()
{
  String dadosTempo;
  int segundos = 0;
  int minutos = 0;
  float soma_tensao = 0; // Variável para soma de tensão
  float media = 0;
  float valor_ph = 0;
  unsigned long tempo1;
  unsigned long tempo2;
  unsigned long tempoFinal = 0;
  soma_tensao = 0; // Inicia soma_tensão em 0
  int contagem = 0;
  uint16_t r, g, b, c, ColorTemp, Lux;
  sensorCor.getRawData(&r, &g, &b, &c);
  ColorTemp = sensorCor.calculateColorTemperature(r, g, b);
  Lux = sensorCor.calculateLux(r, g, b);
  int rosa = 5000;
  int lilas = 200;
  if (c < 5000)
  {
    while (!(lilas < r && r < rosa)) // temporario ate saber o intervalo
    {
      tempo1 = millis();
      while (contagem < 10) // Executa enquanto contagem menor que 10
      {
        entrada_A0 = analogRead(A0);          // Lê a entrada analógica
        tensao = (entrada_A0 * 5.0) / 1024.0; // Converte em tensão, o valor lido
        soma_tensao = (soma_tensao + tensao); // Soma a tensão anterior com a atual
        contagem++;                           // Soma 1 à variável de contagem
        delay(100);
        // Aguarda para próxima leitura
      }
      tempo2 = millis();
      // tempoFinal = tempoMedio - tempoInicial;
      media = soma_tensao / 10;
      valor_ph = -5.70 * media + valor_calibracao;

      // tempoInicial = tempoInicial + tempoMedio;
      tempoFinal = ((tempo2 - tempo1) + tempoFinal);
      minutos = ((tempoFinal / 1000) / 60);
      segundos = ((tempoFinal / 1000) % 60);
      if (segundos < 10)
      {
        dadosJuntos = minutos + ':' + '0' + segundos + ';' + valor_ph;
      }
      else
      {
        dadosJuntos = minutos + ':' + segundos + ';' + valor_ph;
      }

      listaDados.push_back(dadosJuntos);
      return dadosJuntos;
      contagem = 0;
      delay(5000);
    }
    Serial.println("Analise Concluida, coloração alcançada");
    Serial.println("----------------------------------Estes são os dados Coletados:----------------------------------");
    for (auto elemento : listaDados)
    {
      Serial.println(elemento);
    }

    delay(3000);
    noInterrupts();
  }

  else
  {
    Serial.print("O sensor RGB esta saturado");
  }
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL, tempo_seg, valor_ph");
  if (sensorCor.begin())
  {
    Serial.print("");
    Serial.println("Bem vindo ao medidor de PH e coloração");
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

void loop()
{
  byte valorTela = 0;
  Serial.println("Escolha a opção:");
  Serial.println("1 - Medir ph da solução");
  Serial.println("2 - Exibir dados Coletados ");
  Serial.println("3 - Encerrar Programa");

  if (Serial.available() > 0)
  {
    valorTela = Serial.read();

    if ((valorTela != '1') && (valorTela != '2') && (valorTela != '3'))
    {
      Serial.print("O numero digitado é incorreto:");
      Serial.println((char)valorTela);
      Serial.print("Digite Novamente:");
      valorTela = Serial.read();
    }
    else
    {
      if (valorTela == '1')
      {
        listaPHT = medicao();
        exibirDados(listaPHT);
      }
      else if (valorTela == '2')
      {
        limparMonitorSerial();
        Serial.println("----------------------------------Estes são os dados Coletados:----------------------------------");
        for (auto elemento : listaDados)
        {
          Serial.println(elemento);
        }
      }

      else if (valorTela == '3')
      {
        Serial.print("Programa Finalizado!");
        Serial.end();
        while (true)
        {
        } // loop infinito para manter o programa encerrado
      }
    }
  }
}
