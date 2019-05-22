//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
byte dataRow=0x08;
byte dataCol1=0xEE;
byte dataCol2=0xEE;
byte Mydata1,Mydata2;
int t = 100;
void setup() {
  // put your setup code here, to run once:
 pinMode(latchPin, OUTPUT);
  Serial.begin(9600);
  // put your main code here, to run repeatedly:
    
}

void loop() {
  dataRow = 0x08;
for(int i=0;i<4;i++){
digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin,dataRow ); 
    Serial.print(digitalRead(dataPin));  
    shiftOut(dataPin, clockPin, dataCol1);
    Serial.print(digitalRead(dataPin)); 
      shiftOut(dataPin, clockPin,dataCol2);
      Serial.print(digitalRead(dataPin)); 
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(t);
dataRow= dataRow>>1;
if(dataRow==0x00) dataRow = 0x01;
}
for(int i=4;i>0;i--){
digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin,dataRow ); 
    Serial.print(digitalRead(dataPin));  
    shiftOut(dataPin, clockPin, dataCol1);
    Serial.print(digitalRead(dataPin)); 
      shiftOut(dataPin, clockPin,dataCol2);
      Serial.print(digitalRead(dataPin)); 
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(t);
dataRow= dataRow<<1;
//if(dataRow==0x10) dataRow = 0x01;
}  
dataCol1 = ((dataCol1 & 0x80)?0x01:0x00) | (dataCol1 << 1);
dataCol2 = ((dataCol2 & 0x80)?0x01:0x00) | (dataCol2 << 1);
}
// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
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
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
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
