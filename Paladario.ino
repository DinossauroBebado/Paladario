//-----Projeto:32:00                                                                 ---------
//-----Titulo:Paladario                                                              ---------
//-----Autor:@dinossauro.bebado/@DinossauroBbad1                                     ---------
//-----Objetivo :Ter controle de sensores, luzes e equipamentos do paladario         ---------
//-----atraves da internet                                                           ---------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//-------------------Wi-fi Config--------------------------#
const char *ssid     = "Torta";
const char *password = "3141592653589";
//-------------------NTP Config----------------------------#
const long utcOffsetInSeconds = -10800;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};
//-------------------Wi-fi Config--------------------------#
int hora_ligar = 10; 
int hora_desligar = 22;
//-------------------LCD Config----------------------------#
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
// INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(endereco, colunas, linhas);
//-------------------Definição de pinos--------------------#
const int bomba = D3 ; 
const int luz = D4 ; 
//-------------------Declaração de variaveis---------------#
int hora;
int minutos;
int segundo;
// Define o cliente NTP para ter o tempo
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(9600);
  
  lcd.init(); // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight(); // LIGA A ILUMINAÇÃO DO DISPLAY
  lcd.clear(); // LIMPA O DISPLAY
  lcd.print("Paladario Iniciado");
  lcd.setCursor(0, 1); // POSICIONA O CURSOR NA PRIMEIRA COLUNA DA LINHA 2
  
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  lcd.println("Paladario Conectado");
  timeClient.begin();
  //---------------
  pinMode(luz,OUTPUT);
  pinMode(bomba,OUTPUT);
  digitalWrite(bomba,LOW );//mantem a entrada da bomba de agua ligada o tempo tempo
  //--------------- 
  delay(1000);
  lcd.clear();
}

void loop() {
  //----------------------Codigo para as luzes desligadas 
  timeClient.update();
  hora = timeClient.getHours();
  minutos = timeClient.getMinutes();
  segundo = timeClient.getSeconds();
  lcdDisplay(hora,minutos,segundo,hora_ligar,hora_desligar,false);
  Serial.println(hora);
  Serial.println("Paladario desligado");
  //----------------------Codigo para as luzes ligadas 
  while(hora>=hora_ligar && hora<hora_desligar)
  {
  Serial.println("Paladario ligado");
  Serial.println(timeClient.getHours());
  digitalWrite(luz,LOW);
  timeClient.update();
  hora = timeClient.getHours();
  minutos = timeClient.getMinutes();
  segundo = timeClient.getSeconds();
  lcdDisplay(hora,minutos,segundo,hora_ligar,hora_desligar,true);
  delay(1000);
  }
  
  digitalWrite(luz,HIGH);
  delay(1000);
}


void lcdDisplay(int horas,int minutos,int segundos,int  hora_ligar,int hora_desligar, bool light)
{ //monstra a hora e quando o timer liga e desliga
  lcd.clear(); 
  lcd.setCursor(0,0);//linha de cima 
  lcd.print("On:");
  lcd.print(hora_ligar); 
  lcd.print("--->");
  lcd.print("Off:");
  lcd.print(hora_desligar); 
  lcd.setCursor(0,1); //linha debaixo 
  lcd.print("-----");
  lcd.print(horas);
  lcd.print(":");
  lcd.print(minutos);
  lcd.print(":");
  lcd.print(segundos);
  lcd.print("-----");
  if(light){lcd.backlight();//liga ou desliga as luzes do LCD 
  }
  else{lcd.noBacklight();
    }
  }
