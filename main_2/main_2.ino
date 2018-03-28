/*
...:::  Bachelor Thesis Support :::...
  Author  : Jan Głos
  Date  : 25/03/18
*/

#define BENDING_PIN 0
#define LED_PIN 11
#define MIN_DIFF 10
#define MIN_VALUE 15
#define MAX_VALUE 99
#define SAMPLE_LEN 6

int bending_old = 0;
int val_zero = 0;

void setup(){

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 100);
  Serial.begin(9600);
  while (!Serial) {};
  val_zero = analogRead(BENDING_PIN);
  Serial.println("Initial Value :");
  Serial.println(val_zero);
  analogWrite(LED_PIN, 0);
}

void loop(){
  int sample_tab[SAMPLE_LEN] = {0};
  int bending = 0, diff = 0, diff_avr = 0;

  for(int i = 0; i < SAMPLE_LEN; i ++){
    bending = analogRead(BENDING_PIN);
    delay(1);

    if((bending - val_zero) < 0){             //Value from first side of sensor
      diff = val_zero - bending;
    }else if((bending - val_zero) > 0){       //Value from second side of sensor
      diff = 2*(bending - val_zero);          //Quantity becouse of smaller value
    }else if((bending - val_zero) == 0){      //Equal - no motion from sensor
      diff = 0;
    };

    sample_tab[i] = diff;
  };

  diff = 0;

  for(int i = 0; i < SAMPLE_LEN; i ++){
    diff += sample_tab[i];
  }

  diff_avr = diff/SAMPLE_LEN;

  if(0 < diff_avr < MAX_VALUE){              //Normal working value
    //analogWrite(LED_PIN, (MIN_VALUE + diff_avr));
    incrementFromTo(bedning_old, (MIN_VALUE + diff_avr), 3);
    bending_old = MIN_VALUE + diff_avr;
  }else if(diff_avr = 0){                   //Zero value - for no motion
    //analogWrite(LED_PIN, MIN_VALUE);
    incrementFromTo(bedning_old, MIN_VALUE, 3);
    bending_old = MIN_VALUE;
  }else{                                    //Too high value >99
    //analogWrite(LED_PIN, (MAX_VALUE + MIN_VALUE));
    incrementFromTo(bending_old, (MAX_VALUE + MIN_VALUE), 3);
    bending_old = MAX_VALUE + MIN_VALUE;
  };
}

void startUp(int *start_val, int *left_val, int *right_val){
  int read_val;
  Serial.println("Odczyt wartiści ? [Y/n]");
  int tmp = 0;
  do{
    tmp = Serial.read();
    delay(10);
  }while(tmp == 0);
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
        delay(10);
      }while(*left_val == 0);
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
      }while(*right_val == 0);
      Serial.println("Odczytano :");
      Serial.println(*right_val);
    }else{
      Serial.println("BLAD !");
    }
  }else{

  }
}

void incrementFromTo(int min, int max, int time){
  if(max > min){
    while (min <= max) {
      min ++;
      analogWrite(LED_PIN, min);
      delay(time);
    }
  }else if(max < min){
    while (max <= min) {
      max ++;
      analogWrite(LED_PIN, max);
      delay(time);
    }
  }else{
    analogWrite(LED_PIN, max);
  }
}
