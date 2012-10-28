// подключение библиотек
#include <PinChangeInt.h>
#include <IRremote.h> 
#include <Wire.h> 

#define NO_PORTB_PINCHANGES
#define NO_PORTC_PINCHANGES

#define ADDR 128 //адрес устройства
#define LED_PIN 13
#define RECV_PIN5 5 //вход ИК приемника
#define RECV_PIN6 6 //вход ИК приемника

#define HEADSHOT 0xa90 //Пакет выстрела

int receivePins[] = {RECV_PIN5, RECV_PIN6};
int receivePinsCount = sizeof(receivePins)/sizeof(receivePins[0]);

IRrecv IRreceivers[] = {IRrecv(receivePins[0]), IRrecv(receivePins[1])}; 
decode_results results[2]; 

long volatile IRCodes[2], i2cCodes[2];

void setup() 
{ 
  pinMode(LED_PIN, OUTPUT);
  // включить приемники
  for (int i=0;i<receivePinsCount;i++)
  {
    IRreceivers[i].enableIRIn();
    PCintPort::attachInterrupt(receivePins[i], getIRCode, FALLING); 
  } 

  Wire.begin(ADDR); //определим как ведомый с указанным адресом 
  Wire.onRequest(wireRequestHandler);
} 
  
void loop()
{
  for (int i=0;i<receivePinsCount;i++)
  {
    if(IRCodes[i] > 0)
    {
      i2cCodes[i] = IRCodes[i];
      IRCodes[i] = 0;
    }
  }
}

//получить индекс пина по номеру
int getPinIndex(int receivePin)
{
  for (int i=0;i<receivePinsCount;i++)
  {
    if(receivePins[i] == receivePin)
    {
      return i;
    }
  }
  return 0;
}

// получить код переданный с ИК пульта
void getIRCode()
{
  int pinIndex = getPinIndex(PCintPort::arduinoPin);
  PCintPort::detachInterrupt(receivePins[pinIndex]);  // отключить прерывание

  if (IRreceivers[pinIndex].decode(&results[pinIndex]))  
  { 
    if (results[pinIndex].value > 0 && results[pinIndex].value < 0xFFFFFFFF) 
    { 
      IRCodes[pinIndex] = results[pinIndex].value;
    } 
    IRreceivers[pinIndex].resume(); 
  } 
  PCintPort::attachInterrupt(receivePins[pinIndex], getIRCode, CHANGE); 
}

void wireRequestHandler()  
{
  for (int i=0;i<receivePinsCount;i++)
  {
    i2cWriteLong(i2cCodes[i]);
    if(i2cCodes[i] > 0)
    {
      i2cCodes[i] = 0;
    }
  } 
} 

void i2cWriteLong(long longToWrite) {
  byte buffer[4];
  
  int count = sizeof(longToWrite);
  
  for(int i = count; i > 0; i--)
  {
    buffer[i-1] = longToWrite & 0xFF;
    longToWrite >>= 8;
  }
    
  Wire.write(buffer, count);
}
