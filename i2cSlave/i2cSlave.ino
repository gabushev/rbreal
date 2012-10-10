#include <Wire.h> 
#define ADDR 128 //адрес устройства
#define LED_PIN 13

int volatile sentToMaster;

void setup() 
 { 
  pinMode(LED_PIN, OUTPUT);
  // последовательный порт 
  Serial.begin(9600); 
  
  Wire.begin(ADDR); //определим как ведомый с указанным адресом 
  Wire.onReceive(receive_handler); //ждем данных от мастера, функция возвращает int число байт данных
  Wire.onRequest(request_handler);
 } 
  
void loop()
{
  if(sentToMaster == 1) {
    Serial.println('1 ');
    sentToMaster = 0;
  }
}

void receive_handler(int numbytes)  
{
  for (int i=0;i<numbytes;i++) //читаем весь пакет  
  { 
    Serial.write(Wire.read()); // принимаем значение и передаем компьютеру 
  } 
  Serial.println(' '); //перенос строки 
}

void request_handler()  
{
  sentToMaster = 1;
  Wire.write("asdf");
}
