// подключение библиотек
#include <PinChangeInt.h>
#include <IRremote.h> 
#include <Wire.h> 
#define ADDR 128 //адрес устройства
#define LED_PIN 13
#define RECV_PIN5 5 //вход ИК приемника
#define RECV_PIN6 6 //вход ИК приемника

#define HEADSHOT 0xa90 //Пакет выстрела

int receivePins[] = {RECV_PIN5, RECV_PIN6};
IRrecv IRreceivers[] = {IRrecv(receivePins[0]), IRrecv(receivePins[1])}; 
int receivePinsCount = sizeof(receivePins)/sizeof(receivePins[0]);

decode_results results; 

unsigned long volatile IRBuffer, IRStartTime = 0, IREndTime = 0;
long volatile IRCodes[2];

char volatile i2cBuffer;
int volatile sentToMaster;

void setup() 
 { 
  pinMode(LED_PIN, OUTPUT);
  // последовательный порт 
  Serial.begin(9600); 
  
  // включить приемники
  for (int i=0;i<receivePinsCount;i++)
  {
    IRreceivers[i].enableIRIn();
    PCintPort::attachInterrupt(receivePins[i], getIRCode, FALLING); 
  } 
  
  Wire.begin(ADDR); //определим как ведомый с указанным адресом 
  Wire.onRequest(wireRequestHandler);
  
  Serial.print("hack the planet");
 } 
  
void loop()
{
  if(IRCodes[0] > 0) {
    Serial.print("0=");
    Serial.println(IRCodes[0]);
  }
  if(IRCodes[1] > 0) {
    Serial.print("1=");
    Serial.println(IRCodes[1]);
  }
}

//получить индекс пина по номеру
int getPinIndex(int receivePin)
{
  for (int i=0;i<receivePinsCount;i++)
  {
    if(receivePins[i] == receivePin) {
      return i;
    }
  }
  return 0;
}

// получить код переданный с ИК пульта
void getIRCode()
{
  PCintPort::detachInterrupt(PCintPort::arduinoPin);  // отключить прерывание
  int pinIndex = getPinIndex(PCintPort::arduinoPin);
  if (IRreceivers[pinIndex].decode(&results))  
  { 
    if (results.value > 0 && results.value < 0xFFFFFFFF) 
    { 
      IRBuffer = results.value; 
      IREndTime=millis();
      if (IREndTime-IRStartTime>1000)
      {
        IRCodes[pinIndex] = IRBuffer;
        IRStartTime=IREndTime;
      }
      else
      {
        IRCodes[pinIndex] = 0;
      }
    } 
    IRreceivers[pinIndex].resume(); 
  } 
  PCintPort::attachInterrupt(PCintPort::arduinoPin, getIRCode, CHANGE); 
}

void wireRequestHandler()  
{
//  i2cWriteLong(0x9ac);
  for (int i=0;i<receivePinsCount;i++)
  {
    i2cWriteLong(IRCodes[i]);
    if(IRCodes[i]>0) {
      IRCodes[i] = 0;
    }
  } 
} 

void i2cWriteLong(long myLong) {
  byte myBuffer[4];
  
  int count = sizeof(myLong);
  
  for(int i = count; i > 0; i--) {
    myBuffer[i-1] = myLong & 0xFF;
    myLong >>= 8;
  }
    
  Wire.write(myBuffer, count);
}
