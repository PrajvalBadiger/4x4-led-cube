//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
byte dataRow = 0x0F;
byte dataCol1 = 0x6F;
byte dataCol2 = 0xF6;
int t = 300;
void setup() {
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

//diag();
//delay(t);
blinkAll();
delay(t);
}

void diag(){
dataRow = 0x0F;
dataCol1 = 0x6F;
 dataCol2 = 0xF6;
 for(int i =0;i<10;i++){
 out();
dataCol2 = ((dataCol2 & 0x01) ? 0x80 : 0x00) | (dataCol2 >> 1);
 dataCol1 = ((dataCol1 & 0x80) ? 0x01 : 0x00) | (dataCol1 << 1);
}
}

void ColRotate() {
  dataRow = 0x0F;
  dataCol1 = 0xEF;
  dataCol2 = 0xFF;
  for (int i = 0; i < 4; i++) {
    out();
    dataCol1 = ((dataCol1 & 0x80) ? 0x01 : 0x00) | (dataCol1 << 1);

  }
  for (int j = 0; j < 3; j++)
    dataCol1 = ((dataCol1 & 0x80) ? 0x01 : 0x00) | (dataCol1 << 1);
  out();

  dataCol1 = 0xFF;
  dataCol2 = 0x7F;
  out();

  for (int k = 0; k < 4; k++)
    dataCol2 = ((dataCol2 & 0x80) ? 0x01 : 0x00) | (dataCol2 << 1);
  out();

  for (int i = 0; i < 3; i++) {

    dataCol2 = ((dataCol2 & 0x01) ? 0x80 : 0x00) | (dataCol2 >> 1);
    Serial.println(dataCol2);
    out();
  }

  for (int i = 0; i < 4; i++)
    dataCol2 = ((dataCol2 & 0x01) ? 0x80 : 0x00) | (dataCol2 >> 1);
  out();
  dataCol1 = 0xFE;
  dataCol2 = 0xFF;
  out();

  for (int k = 0; k < 2; k++) {
    dataCol1 = ((dataCol1 & 0x80) ? 0x01 : 0x00) | (dataCol1 << 1);
    out();
  }
  dataCol1 = 0xFF;
  dataCol2 = 0xBF;

  out();
  dataCol2 = ((dataCol2 & 0x01) ? 0x80 : 0x00) | (dataCol2 >> 1);
  out();
  delay(20);
}
void DownUp() {
  dataRow = 0x08;
  dataCol1 = 0xEE;
  dataCol2 = 0xEE;
  for (int f = 0; f < 4; f++) {
    for (int i = 0; i < 4; i++) {
      out();
      dataRow = dataRow >> 1;
      if (dataRow == 0x00) dataRow = 0x01;
    }
    for (int i = 4; i > 0; i--) {
      out();
      dataRow = dataRow << 1;
    }
    dataCol1 = ((dataCol1 & 0x80) ? 0x01 : 0x00) | (dataCol1 << 1);
    dataCol2 = ((dataCol2 & 0x80) ? 0x01 : 0x00) | (dataCol2 << 1);

  }

}


void blinkAll() {
  for (int i = 0; i < 4; i++) {
    dataRow = 0x0F;
    dataCol1 = 0x00;
    dataCol2 = 0x00;
    out();
    delay(100);
    dataRow = 0x0F;
    dataCol1 = 0xFF;
    dataCol2 = 0xFF;
    out();
    delay(100);
  }
}
void out() {

  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, dataRow );
  shiftOut(dataPin, clockPin, dataCol1);
  shiftOut(dataPin, clockPin, dataCol2);
  digitalWrite(latchPin, 1);
  delay(t);
}














// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
