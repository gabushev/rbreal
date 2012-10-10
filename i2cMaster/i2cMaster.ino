#include <Wire.h> 
#include <IRremote.h>
#define ADDR 128  //адрес нужного нам Slave
#define LED_PIN 13
#define BTN_PIN 9

int val = 0;
char fromSlave = ' ';
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
  delay(300);
}

void trySlave(){
  Wire.requestFrom(ADDR,1, true);
    while(Wire.available()){
      fromSlave = Wire.read();
    }
  if (fromSlave == '!'){
    Serial.println("yes");
    fromSlave = ' ';
  }
}

