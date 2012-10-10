// подключение библиотеки
#include <IRremote.h> 
#include <Wire.h> 
#define ADDR 128 //адрес устройства
#define LED_PIN 13
#define RECV_PIN 2 //вход ИК приемника

#define HEADSHOT 0xa90 //Пакет выстрела
#define WIREHEADSHOT '!' //Байт для передачи на мастер по i2c при попадании выстрела

IRrecv irrecv(RECV_PIN); 
decode_results results; 

unsigned long volatile IRBuffer;
long volatile IRCode;
unsigned long IRStartTime, IREndTime;

char volatile i2cBuffer;
int volatile sentToMaster;

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
  
  Wire.begin(ADDR); //определим как ведомый с указанным адресом 
  Wire.onRequest(request_handler);
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
      i2cBuffer = WIREHEADSHOT;
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

void request_handler()  
{
  Wire.write(i2cBuffer);
  i2cBuffer = '0';
}
