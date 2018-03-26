/*
...:::  Bachelor Thesis Support :::...
  Author  : Jan Głos
  Date  : 25/03/18
*/

#define BENDING_PIN 0
#define LED_PIN 13

int bending_old = 0;
int diff = 0;
int val_zero = 0;

void setup(){

  pinMode(LED_PIN, OUTPUT);
analogWrite(LED_PIN, 100);
    Serial.begin(9600);
  while (!Serial) {
  }
val_zero = analogRead(BENDING_PIN);
Serial.println("Initial Value :");
Serial.println(val_zero);
analogWrite(LED_PIN, 0);
}

void loop(){

  int bending = analogRead(BENDING_PIN);

  if((bending - val_zero) < 0){
    diff = val_zero - bending;
  }else if((bending - val_zero) > 0){
    diff = bending - val_zero;
  }else if((bending - val_zero) == 0){
    diff = -1;
    //analogWrite(LED_PIN, bending_old);
  };


  if(0 < diff < 150){
    analogWrite(LED_PIN, diff);
    bending_old = diff;
    Serial.println("New value : ");
    Serial.println(diff);
  }else if(diff = -1){
    analogWrite(LED_PIN, bending_old);
    Serial.println("No difference, old value");
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
  delay(10);

}
