// Include the TimerOne Library from Paul Stoffregen
//#include "TimerOne.h"
#include <NewPing.h>

// for 3 Ultra Sensor
int trigPinL =A2;
int echoPinL = A3;

int trigPinF = 12;
int echoPinF = 13;

int trigPinR = 5;
int echoPinR = 4;



#define MAX_DISTANCE 100 


boolean frontwall ;
boolean leftwall ;
boolean rightwall ;

int wall_threshold = 13 ;
//int left_threshold = 10 ;
//int right_threshold = 10 ;
int front_threshold = 7 ;



// Constants for Interrupt Pins
// Change values if not using Arduino Uno

const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1


int in1 = 8; 
int in2 = 9; 
int in3 = 11; 
int in4 = 10; 

int ena = 6; 
int enb = 7; 





NewPing sonarLeft(trigPinL, echoPinL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(trigPinR, echoPinR, MAX_DISTANCE);
NewPing sonarFront(trigPinF, echoPinF, MAX_DISTANCE);




float lSensor, rSensor, fSensor;

// Integers for pulse counters
unsigned int counter1 = 0;
unsigned int counter2 = 0;

// Float for number of slots in encoder disk
float diskslots = 20;  // Change to match value of encoder disk

// Interrupt Service Routines

// Motor 1 pulse count ISR
void ISR_count1()  
{
  counter1++;  // increment Motor 1 counter value
} 

// Motor 2 pulse count ISR
void ISR_count2()  
{
  counter2++;  // increment Motor 2 counter value
} 

// TimerOne ISR
/*void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
  Serial.print("Motor Speed 1: "); 
  float rotation1 = (counter1 / diskslots) * 60.00;  // calculate RPM for Motor 1
  Serial.print(rotation1);  
  Serial.print(" RPM - "); 
  counter1 = 0;  //  reset counter to zero
  Serial.print("Motor Speed 2: "); 
  float rotation2 = (counter2 / diskslots) * 60.00;  // calculate RPM for Motor 2
  Serial.print(rotation2);  
  Serial.println(" RPM"); 
  counter2 = 0;  //  reset counter to zero
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer
}
*/

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
  analogWrite(ena, 255); 
  analogWrite(enb, 255);  
// forward
 /* digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3 , LOW);
  digitalWrite(in4, HIGH);*/
  
 /* Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  delay(1000); 
*/
}

void loop() {
//float durationL, distanceL;
 /* digitalWrite(trigPinL,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPinL, LOW);
  durationL=pulseIn(echoPinL, HIGH);
  distanceL =(durationL/2)/29.1;
  Serial.print("Left : ");
  Serial.print(distanceL);
  Serial.println("CM");
  delay(1500);
  Serial.println("************************");
  
  float durationR, distanceR;
  digitalWrite(trigPinR,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPinR, LOW);
  durationR=pulseIn(echoPinR, HIGH);
  distanceR =(durationR/2)/29.1;
  Serial.print("Right : ");
  Serial.print(distanceR);
  Serial.println("CM");
  delay(1500);
  Serial.println("************************");

  float durationF, distanceF;
  digitalWrite(trigPinF,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPinF, LOW);
  durationF=pulseIn(echoPinF, HIGH);
  distanceF =(durationF/2)/29.1;
  Serial.print("Forword : ");
  Serial.print(distanceF);
  Serial.println("CM");
  delay(1500);
  Serial.println("************************");*/

ReadSensors();
walls();


Serial.println("left is their wall!!!!:");  
Serial.println(leftwall);

Serial.println("right  is their is wall!!!!:");
Serial.println(rightwall);

Serial.println("forward their is  wall!!!!");
Serial.println(frontwall);
delay(2000);










  
}





void ReadSensors() {
lSensor = sonarLeft.ping_cm(); //ping in cm
rSensor = sonarRight.ping_cm();
fSensor = sonarFront.ping_cm();

/*Serial.println("left");
Serial.println(lSensor);

Serial.println("right");
Serial.println(rSensor);

Serial.println("forward");
Serial.println(fSensor);
delay(2000);
*/
  
}



void walls() {


  if ( lSensor < wall_threshold ) {
    leftwall = true ;
  }
  else {
    leftwall = false ;
  }


  if ( rSensor < wall_threshold ) {
    rightwall = true ;
  }
  else {
    rightwall = false ;


  } if ( fSensor < front_threshold ) {
    frontwall = true ;
  }
  else {
    frontwall = false ;
  }

}
