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
//Serial.println(fakeCode, HEX);
  
  delay(1000);
}

void trySlave(){
  Wire.requestFrom(ADDR, 4);
//  byte bufferByte = 0;
//  long bufferLong, codeCandidate = 0;
  
//  fakeCode = 0;
  
while(Wire.available())    // slave may send less than requested
  { 
    long fakeCode = Wire.read();    // receive a byte as character
    Serial.println(fakeCode, HEX);
  }
    //bufferLong = 0;
    //codeCandidate = 0;
    //for(int k=0; k<sizeof(fakeCode); k++) {
      //bufferByte = Wire.read();
      //bufferLong = bufferByte;
      //bufferLong = bufferLong << k*8;
      //Serial.println(bufferByte, BIN);
      //codeCandidate = bufferLong | codeCandidate;
    //}
    //if(codeCandidate > 0) {
      //fakeCode = codeCandidate;
    //}
  
}

