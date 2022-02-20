#include <Servo.h>


const int b0 = 2;
const int b1 = 3;
const int b2 = 4;
const int b3 = 5;
const int b4 = 6;
const int b5 = 7;
const int b6 = 8;
const int b7 = 9;
const int servoPin = 10;
Servo myServo;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 2;
task tasks[tasksNum];
void resetA(){
  
  digitalWrite(A0, LOW);  
  digitalWrite(A1, LOW);  
  digitalWrite(A2, LOW);  
  digitalWrite(A3, LOW);  
  digitalWrite(A4, LOW);
  
}
enum Lock_States{start, lock_r, lock_p, unlock_p, unlock_r} L_state;

enum light_States{initial};

int TickFct_LGS(int state){

  switch(state){
    case start:
      
      if(map(analogRead(A5), 0, 1023, 0, 255) < 127) digitalWrite(b0, HIGH); //if voltage < 2.5 Volts
      else digitalWrite(b0, LOW);
      state = start;
      break; 
  } 

    return state;
}
int TickFct_LS(int state){
 switch(state){
  case start:
    myServo.write(180);
    state = lock_r;
    break;
  case lock_r:
    state = (digitalRead(A0) && digitalRead(A3) && !digitalRead(A1) && !digitalRead(A2) && !digitalRead(A4)) ? unlock_p : lock_r;    
    break;
  case unlock_p:
    myServo.write(0);
    state = (digitalRead(A0) && digitalRead(A3) && !digitalRead(A1) && !digitalRead(A2) && !digitalRead(A4)) ? unlock_p : unlock_r;
    break;
  case unlock_r:
    state = (digitalRead(A0) && digitalRead(A3) && !digitalRead(A1) && !digitalRead(A2) && !digitalRead(A4)) ? lock_p : unlock_r;
    break;
  case lock_p:
    state = (digitalRead(A0) && digitalRead(A3) && !digitalRead(A1) && !digitalRead(A2) && !digitalRead(A4)) ? lock_p : start;
    break;
 }
  return state;
}

void setup(){
  unsigned char i = 0;
  tasks[i].state = start;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_LS;
  i++;  
  tasks[i].state = initial;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_LGS;
  myServo.attach(servoPin);
  myServo.write(180);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT);
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);
  pinMode(b5, OUTPUT);
  pinMode(b6, OUTPUT);
  pinMode(b7, OUTPUT);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  
}

void writeToB(unsigned long temp)
{

Serial.println(temp);

if ((temp>>7) & 0x01){digitalWrite(b7,HIGH);}
else digitalWrite(b7,LOW);
if ((temp>>6) & 0x01){digitalWrite(b6,HIGH);}
else digitalWrite(b6,LOW);
if ((temp>>5) & 0x01){digitalWrite(b5,HIGH);}
else digitalWrite(b5,LOW);
if ((temp>>4) & 0x01){digitalWrite(b4,HIGH);}
else digitalWrite(b4,LOW);
if ((temp>>3) & 0x01){digitalWrite(b3,HIGH);}
else digitalWrite(b3,LOW);
if ((temp>>2) & 0x01){digitalWrite(b2,HIGH);}
else digitalWrite(b2,LOW);
if ((temp>>1) & 0x01){digitalWrite(b1,HIGH);}
else digitalWrite(b1,LOW);
if (temp & 0x01){digitalWrite(b0,HIGH);}
else digitalWrite(b0,LOW);
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


void loop(){

//  myServo.attach(servoPin);
//  myServo.write(map(voltage, 0, 1023, 0, 180));
  resetA();
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  delay(25);
  
}
