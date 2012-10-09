// подключение библиотеки
#include <IRremote.h> 
#define LED_PIN 13
#define RECV_PIN 2 //вход ИК приемника

#define HEADSHOT 0xa90 //Пакет выстрела

IRrecv irrecv(RECV_PIN); 
decode_results results; 

unsigned long volatile IRBuffer;
long volatile IRCode;
unsigned long IRStartTime, IREndTime;

void setup() 
{ 
  pinMode(LED_PIN, OUTPUT);
  // последовательный порт 
  Serial.begin(9600); 
  // включить приемник 
  irrecv.enableIRIn(); 
  // прерывания для ИК
  IRStartTime=0;IREndTime=0;
  attachInterrupt(0, getIRCode, FALLING); 
} 
  
void loop() 
{ 
  // обработка кода нажатия
  if(IRCode>0)
  {
    Serial.println(IRCode);
    ir_go(IRCode);  
    IRCode=0;  
  } 
}

// получить код переданный с ИК пульта
void getIRCode()
{
  detachInterrupt(0);    // отключить прерывание 0
  if (irrecv.decode(&results))  
  { 
    if (results.value > 0 && results.value < 0xFFFFFFFF) 
    { 
      IRBuffer = results.value; 
      IREndTime=millis();
      if (IREndTime-IRStartTime>1000)
      {
        IRCode = IRBuffer;IRStartTime=IREndTime;
      }
      else
      {
        IRCode = 0;
      }
    } 
    irrecv.resume(); 
  } 
  attachInterrupt(0, getIRCode, CHANGE); 
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
