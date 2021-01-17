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

int wall_threshold = 12;
;
//int left_threshold = 10 ;
//int right_threshold = 10 ;
int front_threshold = 12 ;



// Constants for Interrupt Pins
// Change values if not using Arduino Uno

const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1


int in1 = 8; 
int in2 = 9; 

int in3 = 10; 
int in4 = 11; 

int ena = 6; 
int enb = 7; 





NewPing sonarLeft(trigPinL, echoPinL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(trigPinR, echoPinR, MAX_DISTANCE);
NewPing sonarFront(trigPinF, echoPinF, MAX_DISTANCE);




float oldLeftSensor, oldRightSensor, leftSensor, rightSensor, frontSensor, oldFrontSensor, lSensor, rSensor, fSensor;


/*// Integers for pulse counters
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
*/
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
  
//setDirection(FORWARD);
 
 
  

}

void loop() {
//float durationL, distanceL;

//




setDirection(FORWARD);


while(true){
 
setDirection(STOP);
delay(1000);
ReadSensors();
walls();



if (leftwall == false && rightwall == true && frontwall == true ) {
  setDirection(LEFT);
  Serial.println("turn left");
  
}
else if (leftwall == true && rightwall == false && frontwall == true){
   setDirection(RIGHT);
   Serial.println("turn right");
}
else if (leftwall == true && rightwall == true && frontwall == false){
   setDirection(FORWARD);
   Serial.println("turn forward");
}
else if (leftwall == true && rightwall == true && frontwall == true){
   setDirection(BACKWARD);
   Serial.println("turn backword");
}
}
//delay(2000);



}





void ReadSensors() {
lSensor = sonarLeft.ping_cm(); //ping in cm
rSensor = sonarRight.ping_cm();
fSensor = sonarFront.ping_cm();




 leftSensor = (lSensor + oldLeftSensor) / 2; //average distance between old & new readings to make the change smoother
 rightSensor = (rSensor + oldRightSensor) / 2;
 frontSensor = (fSensor + oldFrontSensor) / 2;


 oldLeftSensor = leftSensor; // save old readings for movment
 oldRightSensor = rightSensor;
 oldFrontSensor = frontSensor;

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


  if (  leftSensor < wall_threshold ) {
    leftwall = true ;
  }
  else {
    leftwall = false ;
  }


  if (  rightSensor < wall_threshold ) {
    rightwall = true ;
  }
  else {
    rightwall = false ;


  } if (  frontSensor < front_threshold ) {
    frontwall = true ;
  }
  else {
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
  }
  else if ( dir == LEFT ) {
    //Serial.print("GO LEFT .. \n");
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }
  else if ( dir == RIGHT ) {
    //Serial.print("GO RIGHT .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
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
  }
 /* delay(2000);
    //Serial.print("STOP!!!!!!! .. \n");
    digitalWrite(in1, HIGH);   // Left wheel forward
    digitalWrite(in2, HIGH );
    digitalWrite(in3, HIGH );  // Right wheel forward
    digitalWrite(in4, HIGH);
ReadSensors();
delay(5000);
*/

  
}
