// подключение библиотек
#include <PinChangeInt.h>
#include <IRremote.h> 
#define LED_PIN 13
#define RECV_PIN5 5 //вход ИК приемника
#define RECV_PIN6 6 //вход ИК приемника

#define HEADSHOT 0xa90 //Пакет выстрела

int receivePins[] = {RECV_PIN5, RECV_PIN6};
IRrecv IRreceivers[] = {IRrecv(receivePins[0]), IRrecv(receivePins[1])}; 

decode_results results; 

unsigned long volatile IRBuffer, IRStartTime = 0, IREndTime = 0;
long volatile IRCode;
int volatile fromWhat;

void setup() 
{ 
  pinMode(LED_PIN, OUTPUT);
  // последовательный порт 
  Serial.begin(9600); 
  // включить приемники
  for (int i=0;i<sizeof(IRreceivers);i++)
  {
    IRreceivers[i].enableIRIn();
    PCintPort::attachInterrupt(receivePins[i], getIRCode, FALLING); 
  } 
} 
  
void loop() 
{ 
  // обработка кода нажатия
  if(IRCode>0)
  {
    Serial.println(IRCode);
    Serial.println(fromWhat);
    ir_go(IRCode);  
    fromWhat = 0;
    IRCode=0;  
  } 
}

//получить индекс пина по номеру
int getPinIndex(int receivePin)
{
  int receivePinsCount = sizeof(receivePins)/sizeof(receivePins[0]);
  
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
        fromWhat = PCintPort::arduinoPin;
        IRCode = IRBuffer;IRStartTime=IREndTime;
      }
      else
      {
        IRCode = 0;
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
