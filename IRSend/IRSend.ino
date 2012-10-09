#include <IRremote.h>

int LED_PIN = 13;
int BTN_PIN = 9;
int val = 0;

IRsend irsend;

// the setup routine runs once when you press reset:
void setup()
{                
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void loop()
{
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
}

