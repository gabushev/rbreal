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

int receivePins[] = {RECV_PIN5, RECV_PIN6};
int receivePinsCount = sizeof(receivePins)/sizeof(receivePins[0]);

IRrecv IRreceivers[] = {IRrecv(receivePins[0]), IRrecv(receivePins[1])};
IRsend irsend;
decode_results results[2];

long volatile IRInBuffer[2], IROutBuffer, i2cInBuffer, i2cOutBuffer[2];

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  // включить приемники
  for (int i = 0; i < receivePinsCount; i++)
  {
    IRreceivers[i].enableIRIn();
    PCintPort::attachInterrupt(receivePins[i], getIRCode, FALLING);
  }

  Wire.begin(ADDR); //определим как ведомый с указанным адресом
  Wire.onReceive(wireReceiveHandler);
  Wire.onRequest(wireRequestHandler);
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < receivePinsCount; i++)
  {
    if(IRInBuffer[i] > 0)
    {
      i2cOutBuffer[i] = IRInBuffer[i];
      IRInBuffer[i] = 0;
    }
    if(i2cInBuffer > 0)
    {
      IROutBuffer = i2cInBuffer;
      i2cInBuffer = 0;
      irsend.sendSony(IROutBuffer, 24); //есть ли смысл в промежуточной переменной?
      IROutBuffer = 0;
      // включить приемники
      for (int i = 0; i < receivePinsCount; i++)
      {
        IRreceivers[i].enableIRIn();
      }
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
      IRInBuffer[pinIndex] = results[pinIndex].value;
    }
    IRreceivers[pinIndex].resume();
  }
  PCintPort::attachInterrupt(receivePins[pinIndex], getIRCode, CHANGE);
}

void wireRequestHandler()
{
  for (int i = 0; i < receivePinsCount; i++)
  {
    i2cWriteLong(i2cOutBuffer[i]);
    if(i2cOutBuffer[i] > 0)
    {
      i2cOutBuffer[i] = 0;
    }
  }
}

void wireReceiveHandler(int bytesCount)
{
  long receivedLong = 0;
  byte buffer = 0;

  for (int i = 0; i < bytesCount; i++) //читаем весь пакет
  {
    buffer = Wire.read();
    receivedLong <<= 8;
    receivedLong += buffer;
  }

  i2cInBuffer = receivedLong;
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
