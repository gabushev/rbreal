void setup() {
  Serial.begin(9600);
  long myL = 0xa90;
  byte myB = 0;
  long myLbuf = 0;
  long myLrecv = 0;
  
  Serial.println(myL, BIN);
  Serial.println(' ');
  
  myLbuf = 0;
  
  myB = myL & 0xFF;
  myL = myL >> 8;
  
  myLbuf = myB;
  myLbuf = myLbuf << 0;
  myLrecv = myLbuf | myLrecv;
  Serial.println(myB, BIN);
  Serial.println(myLrecv, BIN);
  Serial.println(myL, BIN);
  Serial.println(' ');

  myLbuf = 0;
  
  myB = myL & 0xFF;
  myL = myL >> 8;
  
  myLbuf = myB;
  myLbuf = myLbuf << 8;
  myLrecv = myLbuf | myLrecv;
  Serial.println(myB, BIN);
  Serial.println(myLrecv, BIN);
  Serial.println(myL, BIN);
  Serial.println(' ');
}

void loop() {}
