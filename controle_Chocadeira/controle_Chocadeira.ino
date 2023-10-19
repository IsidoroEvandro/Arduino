//Autor: Isidoro Evandro Schut
//Projeto: Chocadeira de Ovos
//
//
//Para que funcione corretamente eh necessario instalar as bibliotecas:
//Adafruit-Sensor: https://github.com/adafruit/Adafruit_Sensor/tree/master
//DHT-sensor: https://www.arduino.cc/reference/en/libraries/dht-sensor-library/

/*
Bibliotecas
*/

#include "DHT.h"

/*
Variaveis e constantes
*/

#define DHTPIN 11         //pino usado sensor
#define DHTTYPE DHT22     //tipo do sensor

float tmin = 36.5;        //temperatura minima
float tmax = 38;          //temperatura maxima

/*
Enderecamento hardware
*/

const int LEDPIN = 8;     //led vermelho indicacao
const int PINRESIS = 2;   //led azul resistecia
const int PINCOOLER = 4;  //led amarelo injecao de umidade e calor
const int PINVENTI = 7;   //led verde retirada de umidade e calor

/*
Objetos
*/
DHT dht(DHTPIN, DHTTYPE); //objeto do sensor

/*
Inicializacao
*/

void setup() {
  Serial.begin(9600);
  Serial.print("DHTTYPE: ");
  Serial.println(DHTTYPE);
  Serial.println("Type,\tumidae (%),\ttemperatura (C)\ttempo (br)");
  dht.begin();

  pinMode(LEDPIN, OUTPUT);
  pinMode(PINRESIS, OUTPUT);
  pinMode(PINCOOLER, OUTPUT);
  pinMode(PINVENTI, OUTPUT);
}

/*
Leituras 
*/

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Sem resposta do sensor!"));
    return;
  }

  leitura(h, t, f);
  checkTemp(t);
  checkHumidity(h);
  delay(2000);
}

/*
Funcao de leitura
*/

void leitura(float h, float t, float f) {
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("DHTTYPE: ");
  Serial.print(DHTTYPE);
  Serial.print(F("   Umidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatua: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Sensacao Termica: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

/*
Funcoes de controle
*/

void checkTemp(float t) {
  t = dht.readTemperature();
  if (t >= tmin && t<=tmax) {
    digitalWrite(LEDPIN, HIGH);
    Serial.println("Dentro do esperado");
    digitalWrite(PINRESIS, LOW);
    digitalWrite(PINCOOLER, LOW);
    digitalWrite(PINVENTI, LOW);
  } else if (t <= tmin) {
    Serial.println("Temperatura baixa!");
    digitalWrite(PINRESIS, HIGH);
    digitalWrite(PINCOOLER, HIGH);
    digitalWrite(PINVENTI, LOW);
  } /*else if (t <= tmax) {
    Serial.println("Temperatura ideal!");
    
  } */else if(t >= tmax) {
    Serial.println("Temperatura alta!");
    digitalWrite(PINVENTI, HIGH);
    digitalWrite(PINRESIS, LOW);
    digitalWrite(PINCOOLER, HIGH);
    /*delay(3000);
    digitalWrite(PINVENTI, LOW);
    digitalWrite(PINCOOLER, LOW); */
  }
  else {
    Serial.println("Fora do esperado");
  }
}

void checkHumidity(float h) {
  //logica da umidade
}
