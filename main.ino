/*
...:::  Bachelor Thesis Support :::...
  Author  : Jan GÅ‚os
  Date  : 25/03/18
*/

#define BENDING_PIN 0
#define LED_PIN 13
#define DIFFERENCE 50

int bending_old = 0;
int diff = 0;
int val_zero = 0;

void setup(){

  pinMode(LED_PIN, OUTPUT);
analogWrite(LED_PIN, 0);
    Serial.begin(9600);
  while (!Serial) {
  }
analogWrite(LED_PIN, 150);
delay(1000);
val_zero = analogRead(BENDING_PIN);
Serial.println("Initial Value :");
Serial.println(val_zero);
analogWrite(LED_PIN, 0);
}

void loop(){

  int bending = analogRead(BENDING_PIN);

  if((bending - DIFFERENCE) < val_zero){
    diff = val_zero - bending;
  }else if(val_zero < (bending + DIFFERENCE)){
    diff = bending - val_zero;
  }else{
    analogWrite(LED_PIN, bending_old);
  };


  if(0 < diff < 150){
    analogWrite(LED_PIN, diff);
    bending_old = diff;
    Serial.println("New value : ");
    Serial.println(diff);
  }else{
    analogWrite(LED_PIN, bending_old);
    Serial.println("too high difference, old value");
  };


  /*
  if (bending < 150){
    analogWrite(LED_PIN, bending/4);
  }else if (bending >= 150){
    analogWrite(LED_PIN, 150);
  }
  Serial.println(bending);
  bending_old = bending;
  */
  delay(100);

}