#include <Wire.h> 
#include <IRremote.h>
#define ADDR 128  //адрес нужного нам Slave
#define LED_PIN 13
#define BTN_PIN 9

int val = 0;
long fromSlaveCodes[2] = {0,0};
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
  if(fromSlaveCodes[0] > 0 || fromSlaveCodes[1] > 0) {
    Serial.print("0=");
    Serial.println(fromSlaveCodes[0]);
    Serial.print("1=");
    Serial.println(fromSlaveCodes[1]);
    fromSlaveCodes[0] = 0; fromSlaveCodes[1] = 0;
  }
  
  delay(1000);
}

void trySlave(){
  Wire.requestFrom(ADDR, 8);
  int currentLongFromSlave = 0;
  byte bufferByte = 0;
  long bufferLong, codeCandidate = 0;
  while(Wire.available()){
    fromSlaveCodes[currentLongFromSlave] = 0;
    bufferLong = 0;
    codeCandidate = 0;
    for(int k=0; k<4; k++) {
      bufferByte = Wire.read();
      Serial.println(bufferByte, BIN);
      bufferLong = bufferByte;
      bufferLong = bufferLong << k*8;
      codeCandidate = bufferLong | codeCandidate;
    }
    if(codeCandidate > 0) {
      fromSlaveCodes[currentLongFromSlave] = codeCandidate;
    }
    currentLongFromSlave++;
  }
}

