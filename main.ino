/*
...:::  Bachelor Thesis Support :::...
  Author  : Jan Głos
  Date  : 25/03/18
*/

#define BENDING_PIN 0
#define LED_PIN 11
#define MIN_DIFF 10

int bending_old = 0;
int diff = 0;
int val_zero = 0;

int *v_zero = 0;
int *v_r = 0;
int *v_l = 0;
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


startUp(v_zero, v_l, v_r);
val_zero = *v_zero;
}

void loop(){

  int bending = analogRead(BENDING_PIN);

  if((bending - val_zero) < 0){
    diff = val_zero - bending;
  }else if((bending - val_zero) > 0){
    diff = 2*(bending - val_zero); //Different value for second site of sensor
  }else if((bending - val_zero) == 0){
    diff = -1;
  };


  if(0 < diff < 150){
    bending_old = diff;
    Serial.println("New value : ");
    Serial.println(diff);
    if(diff > MIN_DIFF){
      analogWrite(LED_PIN, diff);
    }else{
      analogWrite(LED_PIN, 0);
    }
  }else if(diff = -1){
    analogWrite(LED_PIN, 0);
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

void startUp(int *start_val, int *left_val, int *right_val){
  int read_val;
  Serial.println("Odczyt wartiści ? [Y/n]");
  int tmp = 0;
  do{
    tmp = Serial.read();
    delay(10);
  }while(tmp == 0)
  if(tmp == 0x59 || tmp == 0x79){
    Serial.println("Odczyt wartości zerowej [Y/n]");
    read_val = Serial.read();
    if(read_val == 0x59 || read_val == 0x79){
      *start_val = 0;
      do{
        *start_val = analogRead(BENDING_PIN);
        delay(10);
      }while(*start_val == 0);
      Serial.println("Odczytano :");
      Serial.println(*start_val);
    }else{
      Serial.println("BLAD !");
    }
    Serial.println("Odczyt wartości skrajnej lewej [Y/n]");
    read_val = Serial.read();
    if(read_val == 0x59 || read_val == 0x79){
      *left_val = 0;
      do{
        *left_val = analogRead(BENDING_PIN);
        delay(10);nflanty
      }while(*left_val == 0)
      Serial.println("Odczytano :");
      Serial.println(*left_val);
    }else{
      Serial.println("BLAD !");
    }
    Serial.println("Odczyt wartości skrajnej prawej [Y/n]");
    read_val = Serial.read();
    if(read_val == 0x59 || read_val == 0x79){
      *right_val = 0;
      do{
        *right_val = analogRead(BENDING_PIN);
        delay(10);
      }while(*right_val == 0)
      Serial.println("Odczytano :");
      Serial.println(*right_val);
    }else{
      Serial.println("BLAD !");
    }
  }else{

  }
}
