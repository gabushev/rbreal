#include <Wire.h> 
#include <IRremote.h>
#define ADDR 128  //адрес нужного нам Slave
#define LED_PIN 13
#define BTN_PIN 9

int btn_val = 0;
long fromSlaveCodes[2] = {0,0};

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  Wire.begin(); //включить i2c
  Serial.begin(9600);
}
void loop() {
  btn_val = digitalRead(BTN_PIN);
  digitalWrite(LED_PIN, btn_val);
  if (btn_val == HIGH)
  {
    Wire.beginTransmission(ADDR);
    i2cWriteLong(0x67ab90);
    Wire.endTransmission();
    delay(100);
  }
  
  readFromSlave();

  for (int i = 0; i < (sizeof(fromSlaveCodes) / sizeof(fromSlaveCodes[0])); i++)
  {
    if(fromSlaveCodes[i] > 0)
    {
      Serial.print(i);
      Serial.print("=");
      Serial.println(fromSlaveCodes[i], HEX);
    }
  } 
  
  delay(100);
}

void readFromSlave(){
  for(int i = 0; i < (sizeof(fromSlaveCodes) / sizeof(fromSlaveCodes[0])); i++)
  {
    fromSlaveCodes[i] = i2cReadLong();
  }
}

long i2cReadLong(){
  long receivedLong = 0;
  byte buffer = 0;
  
  int count = sizeof(long);
  
  Wire.requestFrom(ADDR, count);
  while(Wire.available() < count);

  for(int i = 0; i < count; i++) {
    buffer = Wire.read();
    receivedLong <<= 8;
    receivedLong += buffer;
  }
  
  return receivedLong;
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
