#include "TimerOne.h"

int trigPinL =A2;
int echoPinL = A3;
int trigPinF = 12;
int echoPinF = 13;
int trigPinR = 5;
int echoPinR = 4;
int dir;
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define MAX_DISTANCE 100 
boolean frontwall ;
boolean leftwall ;
boolean rightwall ;
int wall_threshold = 10;
int front_threshold = 10 ;
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1
int x=0;
int y=0;
int in1 = 8; 
int in2 = 9; 
int in3 = 10; 
int in4 = 11; 
int ena = 6; 
int enb = 7; 
int flag=1;
// Integers for pulse counters
unsigned int counter = 0;
float leftSensor, rightSensor, frontSensor;
// Float for number of slots in encoder disk
float diskslots = 20;  // Change to match value of encoder disk
const byte MOTOR = 2;  // Motor 1 Interrupt Pin - INT 0
float disMotor=0;
float diWheels=3.3;
void ISR_count()  
{
  counter++;  // increment Motor 1 counter value
}

void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
  //Serial.print("Motor Speed : "); 
  float rotation = (counter / diskslots) * 60.00;  // calculate RPM for Motor 
 // Serial.print(rotation);  
  //Serial.print(" RPM - "); 
  counter = 0;  //  reset counter to zero
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer
}

void setup() {
 Serial.begin(9600); 
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(ena, OUTPUT); 
  pinMode(enb, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  analogWrite(ena, 150); 
  analogWrite(enb, 150); 
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR), ISR_count, RISING);  // Increase counter 1 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  delay(1000); 
}



void loop() {
setDirection(FORWARD);

while(true){

delay(1000);
ReadSensors();
walls();

if (frontwall == true ) {
    Serial.println("front wall");
    delay(1500);
    if (rightwall == true ){
      Serial.println("right wall");
          delay(1500);

      if (leftwall == true ){
          Serial.println("left wall");
              delay(1500);

          while(rightwall == true && leftwall == true && frontwall == true  ){
            setDirection(BACKWARD);
            disMotor=(2* 3.1415926536*diWheels *counter)/1000;
            counter=0;
                delay(1500);

            Serial.print("Distance"); 
            Serial.println(disMotor);
            y-=disMotor;
            Serial.print("************* y :");
            Serial.println(y);
            ReadSensors();
            walls();
          }
          
      }else{
        Serial.println("NO left wall");
            delay(1500);

        setDirection(LEFT); 
        disMotor=(2* 3.1415926536*diWheels *counter)/100;
        counter=0;
            delay(1500);

        Serial.print("Distance"); 
        Serial.println(disMotor);  
        x-=disMotor;
        Serial.print("************* x :");
        Serial.println(x);
      }
    }else{
          delay(1500);

      Serial.println("NO right wall");
      setDirection(RIGHT);
      disMotor=(2* 3.1415926536*diWheels *counter)/100;
      counter=0;
          delay(1500);

      Serial.print("Distance"); 
      Serial.println(disMotor);
      x+=disMotor;
      Serial.print("************* x :");
      Serial.println(x);
    }  
}else{
    Serial.println("NO front wall");
        delay(1500);
    
    setDirection(FORWARD);
    disMotor=(2* 3.1415926536*diWheels *counter)/100;
    counter=0;
        delay(1500);

    Serial.print("Distance"); 
    Serial.println(disMotor);
    y+=disMotor;
    Serial.print("************* y :");
    Serial.println(y);


}}
}





void ReadSensors() {

long duration;
  digitalWrite(trigPinL, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPinL, LOW);
  duration = pulseIn(echoPinL, HIGH);
  leftSensor = (duration/2) / 29.1;
  digitalWrite(trigPinF, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPinF, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPinF, LOW);
  duration = pulseIn(echoPinF, HIGH);
  frontSensor = (duration/2) / 29.1;
  digitalWrite(trigPinR, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPinR, LOW);
  duration = pulseIn(echoPinR, HIGH);
  rightSensor = (duration/2) / 29.1; 
}



void walls() {


  if (  leftSensor < wall_threshold || leftSensor >= 1000 ) {
    Serial.print("left sensor :");
    Serial.println(leftSensor);
    leftwall = true ;
  }
  else {
    Serial.print("left sensor :");
    Serial.println(leftSensor);
    leftwall = false ;
  }
  if (  rightSensor < wall_threshold || rightSensor >= 1000) {
    Serial.print("right sensor :");
    Serial.println(rightSensor);
    rightwall = true ;
  }
  else {
    Serial.print("right sensor :");
    Serial.println(rightSensor);
    rightwall = false ;
  } if (  frontSensor < front_threshold || frontSensor >= 1000 ) {
    Serial.print("front sensor :");
    Serial.println(frontSensor);
    frontwall = true ;
  }
  else {
    Serial.print("front sensor :");
    Serial.println(frontSensor);
    frontwall = false ;
  }
}





void setDirection(int dir) {
   Serial.print("set Direction .. \n\n");
  if ( dir == FORWARD ) {
   //  Serial.print("GO FORWARD .. \n");
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);
  

  
  }
  else if ( dir == LEFT ) {
    //Serial.print("GO LEFT .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(500);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH); 
  
  }
  else if ( dir == RIGHT ) {
   digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);
    //Serial.print("GO RIGHT .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(500);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);
  }
  else if ( dir == STOP ) {
    //Serial.print("STOP!!!!!!! .. \n");
    digitalWrite(in1, HIGH);   // Left wheel forward
    digitalWrite(in2, HIGH );
    digitalWrite(in3, HIGH );  // Right wheel forward
    digitalWrite(in4, HIGH);
  }
  else if ( dir == BACKWARD ) {
  //  Serial.print("GO BACKWARD .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);


   delay(500);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);
  }
}
