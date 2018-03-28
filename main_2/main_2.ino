/*
...:::  Bachelor Thesis Support :::...
  Author  : Jan Głos
  Date  : 25/03/18
*/

#define BENDING_PIN 0
#define LED_PIN 11
#define MIN_DIFF 7
#define MIN_DIFF_BLINDING 20
#define MIN_VALUE 10
#define MAX_VALUE 99
#define SAMPLE_LEN 6

#define INCREMENTING
#define AVERANGE
//#define READING
//#define TRUE
//#define BLINDING
//#define BLINDING_PANIC
//#define TREND
//#define TREND_TRUE
#define TREND_TRUE_WAIT

int bending_old = 0;
int val_zero = 0;

void lightsUp(){
  for(int i = MIN_VALUE; i < MAX_VALUE + 10; i ++){
    analogWrite(LED_PIN, i);
    delay(19);
  }
}

void lightsDown(){
  for(int i = MAX_VALUE + 10; i > MIN_VALUE; i --){
    analogWrite(LED_PIN, i);
    delay(10);
  }
}

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
#ifdef READING
  while(1){
    val_zero = analogRead(BENDING_PIN);
    Serial.println("Initial Value :");
    Serial.println(val_zero);
    delay(200);
  }
#endif
#ifdef TRUE
  while(1){
    val_zero = analogRead(BENDING_PIN);
    Serial.println("Initial Value :");
    Serial.println(val_zero);
    delay(20);
    analogWrite(LED_PIN, (val_zero - 640));
  }
#endif
#ifdef BLINDING
while(1){
  for(int i = MIN_VALUE; i < MAX_VALUE; i ++){
    analogWrite(LED_PIN, i);
    delay(10);
  }
  for(int i = MAX_VALUE; i > MIN_VALUE; i --){
    analogWrite(LED_PIN, i);
    delay(10);
  }
}
#endif
#ifdef BLINDING_PANIC
  blindingPanic();
#endif
#ifdef TREND
bool zero = false;
while(1){
  if(((analogRead(BENDING_PIN) - val_zero) > MIN_DIFF)){
    for(int i = MIN_VALUE; i < MAX_VALUE + 10; i ++){
      analogWrite(LED_PIN, i);
      delay(19);
    }
    zero = false;
  }else if(((analogRead(BENDING_PIN) - val_zero) < MIN_DIFF) && zero){
    for(int i = MAX_VALUE + 10; i > MIN_VALUE; i --){
      analogWrite(LED_PIN, i);
      delay(9);
    }
    zero = true;
  }
}
#endif

  int sample_tab[SAMPLE_LEN] = {0};
  int bending = 0, diff = 0, diff_avr = 0;

#ifdef TREND_TRUE
bool noLight = true, light = false;
analogWrite(LED_PIN, MIN_VALUE);
while(1){
  bending = analogRead(BENDING_PIN);
  delay(1);

  if((bending - val_zero) < 0){             //Value from first side of sensor
    diff = val_zero - bending;
  }else if((bending - val_zero) > 0){       //Value from second side of sensor
    diff = 2*(bending - val_zero);          //Quantity becouse of smaller value
  }else if((bending - val_zero) == 0){      //Equal - no motion from sensor
    diff = 0;
  };

  if((diff > MIN_DIFF) && noLight){

    lightsUp();
    light = true;
    noLight = false;
  }else if((diff <= MIN_DIFF) && light){
    lightsDown();
    noLight = true;
    light = false;
  }else{
    if(noLight){
      analogWrite(LED_PIN, MIN_VALUE);
    }
  }
}
#endif

#ifdef TREND_TRUE_WAIT
bool noLight = true, light = false;
analogWrite(LED_PIN, MIN_VALUE);
while(1){
  bending = analogRead(BENDING_PIN);
  delay(1);

  if((bending - val_zero) < 0){             //Value from first side of sensor
    diff = val_zero - bending;
  }else if((bending - val_zero) > 0){       //Value from second side of sensor
    diff = 2*(bending - val_zero);          //Quantity becouse of smaller value
  }else if((bending - val_zero) == 0){      //Equal - no motion from sensor
    diff = 0;
  };

  if((diff > MIN_DIFF) && noLight){

    lightsUp();
    while(diff > MIN_DIFF){};
    lightsDown();
    noLight = true;
    
  }else{
    if(noLight){
      analogWrite(LED_PIN, MIN_VALUE);
    }
  }
}
#endif

#ifdef AVERANGE
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
#else
bending = analogRead(BENDING_PIN);
delay(1);

if((bending - val_zero) < 0){             //Value from first side of sensor
  diff = (val_zero - bending);
}else if((bending - val_zero) > 0){       //Value from second side of sensor
  diff = 2*(bending - val_zero);          //Quantity becouse of smaller value
}else if((bending - val_zero) == 0){      //Equal - no motion from sensor
  diff = 0;
};
diff_avr = diff;
#endif

#ifdef INCREMENTING
  if(0 < diff_avr < MAX_VALUE){              //Normal working value
    incrementFromTo(bending_old, (MIN_VALUE + diff_avr), 10);
    bending_old = MIN_VALUE + diff_avr;
  }else if(diff_avr = 0){                   //Zero value - for no motion
    incrementFromTo(bending_old, MIN_VALUE, 10);
    bending_old = MIN_VALUE;
  }else{                                    //Too high value >99
    incrementFromTo(bending_old, (MAX_VALUE + MIN_VALUE), 10);
    bending_old = MAX_VALUE + MIN_VALUE;
  };
#else
  if(0 < diff_avr < MAX_VALUE){              //Normal working value
    analogWrite(LED_PIN, (MIN_VALUE + diff_avr));
  }else if(diff_avr = 0){                   //Zero value - for no motion
    analogWrite(LED_PIN, MIN_VALUE);
  }else{                                    //Too high value >99
    analogWrite(LED_PIN, (MAX_VALUE + MIN_VALUE));
  };
#endif

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
    if((max - min) < 5){
          while (min <= max) {
      min ++;
      analogWrite(LED_PIN, min);
      delay(time);
    }
    }

  }else if(max < min){
    if((min - max) < 5){
          while (max <= min) {
      max ++;
      analogWrite(LED_PIN, max);
      delay(time);
    }
    }

  }else{
    analogWrite(LED_PIN, max);
  }
}

void blinding(int increment){
  if(increment > 20){
    for(int i = MIN_VALUE; i < MAX_VALUE; i ++){
      analogWrite(LED_PIN, i);
      delay(1);
    }
    for(int i = MAX_VALUE; i > MIN_VALUE; i --){
      analogWrite(LED_PIN, i);
      delay(1);
    }
  }else{
    for(int i = MIN_VALUE; i < MAX_VALUE; i ++){
      analogWrite(LED_PIN, i);
      delay(12);
    }
    for(int i = MAX_VALUE; i > MIN_VALUE; i --){
      analogWrite(LED_PIN, i);
      delay(8);
    }
  }
}

void panic(int loop){
  for(int k = 0; k < loop; k ++){
    for(int i = MIN_VALUE; i < MAX_VALUE + 10; i ++){
      analogWrite(LED_PIN, i);
      delay(1);
    }
    for(int i = MAX_VALUE + 10; i > MIN_VALUE; i --){
      analogWrite(LED_PIN, i);
      delay(1);
    }
  }
}

bool panicQ(){
  if((analogRead(BENDING_PIN) - val_zero) > MIN_DIFF_BLINDING){
    return 1;
  }else{
    return 0;
  }
}
//(analogRead(BENDING_PIN) - val_zero) < MIN_DIFF_BLINDING
void blindingPanic(){
  while(1){
    for(int i = MIN_VALUE; i < MAX_VALUE; i ++){
      analogWrite(LED_PIN, i);
      delay(19);
      if(panicQ()){
        panic(2);
      }
    }
    for(int i = MAX_VALUE; i > MIN_VALUE; i --){
      analogWrite(LED_PIN, i);
      delay(8);
      if(panicQ()){
        panic(3);
      }
    }
  }
}
