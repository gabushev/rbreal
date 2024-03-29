// подключение библиотек
#include <PinChangeInt.h>
#include <IRremote.h> 
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
} 
  
void loop() 
{ 
  for (int i=0;i<receivePinsCount;i++)
  {
    if(IRCodes[i]>0) {
      Serial.print(receivePins[i]);
      Serial.print('=');
      Serial.println(IRCodes[i]);
      IRCodes[i] = 0;
    }
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

void ir_go(long kod)
{
  switch(kod)
  {
    case HEADSHOT :    // попадание выстрела
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      Serial.println("bang!");
      break;
    default:
      Serial.println("unknown code");  
      break;
  }
}
