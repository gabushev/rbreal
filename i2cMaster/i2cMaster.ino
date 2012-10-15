#include <Wire.h>  //подключаем библиотеку 
#define ADDR 128  //адрес нужного нам Slave
#define LED_PIN 13
#define BTN_PIN 9

int val = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW  
  Wire.begin(); //включить i2c
}
void loop() {
  val = digitalRead(BTN_PIN);
  digitalWrite(LED_PIN, val);
  if (val == HIGH)
  {
    Wire.beginTransmission(ADDR); //начало передачи, с адресом нужного нам Slave 
    Wire.write("Test, communication."); //отправляем данные для выбраного Slave 
    Wire.endTransmission();
    delay(1000);
  }
}

