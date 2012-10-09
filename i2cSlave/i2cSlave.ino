#include <Wire.h> 
#define ADDR 128 //адрес устройства
#define LED_PIN 13

void setup() 
 { 
  pinMode(LED_PIN, OUTPUT);
  // последовательный порт 
  Serial.begin(9600); 
  
  Wire.begin(ADDR); //определим как ведомый с указанным адресом 
  Wire.onReceive(receive_handler); //ждем данных от мастера, функция возвращает int число байт данных
 } 
  
void loop(){}

void receive_handler(int numbytes)  
{
  for (int i=0;i<numbytes;i++) //читаем весь пакет  
  { 
    Serial.print(Wire.read()); // принимаем значение и передаем компьютеру 
  } 
  Serial.println(' '); //перенос строки 
}
