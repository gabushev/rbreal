#include <Wire.h> 
#include <IRremote.h>
#define ADDR 128  //адрес нужного нам Slave
#define LED_PIN 13
#define BTN_PIN 9

int val = 0;
long fromSlaveCodes[2] = {0,0};

char fakeByte;

IRsend irsend;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW  
  Wire.begin(); //включить i2c
  Serial.begin(9600);
}
void loop() {
  val = digitalRead(BTN_PIN);
  digitalWrite(LED_PIN, val);
  if (val == HIGH)
  {
    for (int i = 0; i < 3; i++)
    {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(100);
    }
  }
  trySlave();
      Serial.print("0=");
    Serial.println(fromSlaveCodes[0]);
    Serial.print("1=");
    Serial.println(fromSlaveCodes[1]);
  if(fromSlaveCodes[0] > 0 || fromSlaveCodes[1] > 0) {

//    fromSlaveCodes[0] = 0; fromSlaveCodes[1] = 0;
  }
  
  delay(1000);
}

void trySlave(){
  for(int i = 0; i < (sizeof(fromSlaveCodes) / sizeof(fromSlaveCodes[0])); i++) {
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

