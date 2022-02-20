const int b0 = 2;
const int b1 = 3;
const int b2 = 4;
const int b3 = 5;
const int b4 = 6;
const int b5 = 7;
const int b6 = 8;
const int b7 = 9;

int led_Pins[] = {2,3,4,5};

void writeToB(unsigned char temp)
{
//Serial.println(temp);
if ((temp>>7) & 0x01){digitalWrite(b7,HIGH);}
if ((temp>>6) & 0x01){digitalWrite(b6,HIGH);}
if ((temp>>5) & 0x01){digitalWrite(b5,HIGH);}
if ((temp>>4) & 0x01){digitalWrite(b4,HIGH);}
if ((temp>>3) & 0x01){digitalWrite(b3,HIGH);}
if ((temp>>2) & 0x01){digitalWrite(b2,HIGH);}
if ((temp>>1) & 0x01){digitalWrite(b1,HIGH);}
if (temp & 0x01){digitalWrite(b0,HIGH);}
}




typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 1;
task tasks[tasksNum];
enum LED_STATES{start, s_one_p, s_one_r, s_two_p, s_two_r, s_three_p, s_three_r, s_four_p, s_four_r, s_five_p, s_five_r} LED_STATE;


int TickFct_LS(int state){
  
  switch(LED_STATE){    
    case start: 
        Serial.println("S0");
        LED_STATE =  s_one_p;      
        break;
    case s_one_p:
        Serial.println("S1_p");
        lightshow_one();      
        LED_STATE = s_two_p;      
        break;
    case s_two_p:
        Serial.println("S2_p");
        lightshow_two();      
        LED_STATE = s_three_p;
        break;
    case s_three_p:
      Serial.println("S3_p");
        lightshow_three();
        LED_STATE = s_four_p;
        break;
    case s_four_p:
      Serial.println("S4_p");
        lightshow_four();
        LED_STATE = s_five_p;
        break;
    case s_five_p:
      Serial.println("S5_p");
        lightshow_five();
        LED_STATE = s_one_p;
        break;
    }

    return state;
}



void setup() {
  unsigned char i = 0;
  tasks[i].state = start;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_LS;
  i++;
  pinMode(A0, INPUT);
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);
  pinMode(b5, OUTPUT);
  pinMode(b6, OUTPUT);
  pinMode(b7, OUTPUT);
  Serial.begin(9600);
}

void resetB()
{
digitalWrite(b7,LOW);
digitalWrite(b6,LOW);
digitalWrite(b5,LOW);
digitalWrite(b4,LOW);
digitalWrite(b3,LOW);
digitalWrite(b2,LOW);
digitalWrite(b1,LOW);
digitalWrite(b0,LOW);
}

int power(int base, int expo){
  int result = base;
  if(expo == 0) return 1;
  for(int i = 1; i < expo; i++){  
    result *= base;
  }
  return result;
}
void lightshow_one(){
  for(int i = 0; i < 8; i++){
    if(digitalRead(A0)) return;
    resetB();
    writeToB(power(2,i)); 
    delay(200);     
  }  
}

void lightshow_two(){
  for(int i = 7; i >= 0; i--){
    if(digitalRead(A0)) return;
    resetB();
    writeToB(power(2,i)); 
    delay(200);     
  }  
}

void lightshow_three(){  
  lightshow_one();
  lightshow_two();  
}

void lightshow_four(){
  for(int i = 2; i < 10; i++){
    resetB();
    if(digitalRead(A0)) return;
    digitalWrite(i, HIGH); digitalWrite(11-i, HIGH);
    delay(200);
  }  
}

void lightshow_five(){    
  for(int i = 0; i < (sizeof(led_Pins)/2);i++){
    if(digitalRead(A0)) return;
    int bitmask = 255;
    bitmask = ~(bitmask << i);
    writeToB(bitmask);     
    int count = 0;    
      while(count < 3){ //control blinking of individual LED 
      if(digitalRead(A0)) return;
      digitalWrite(led_Pins[i], HIGH);
      delay(200);
      digitalWrite(led_Pins[i], LOW);
      delay(200);
      ++count;    
      }  
  }
}


void loop() {
  resetB();
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  delay(500);
  
  
}
