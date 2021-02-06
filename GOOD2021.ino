// Include the TimerOne Library from Paul Stoffregen
//#include "TimerOne.h"
#include <NewPing.h>
//#include <ArduinoRobot.h>
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

int wall_threshold =12;

//int left_threshold = 10 ;
//int right_threshold = 10 ;
int front_threshold = 5;

// Constants for Interrupt Pins
// Change values if not using Arduino Uno

const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1


 

int in1 = 6; 
int in2 = 7; 
int in3 = 9; 
int in4 = 8;

int ena = 10; 
int enb = 11; 





NewPing sonarLeft(trigPinL, echoPinL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(trigPinR, echoPinR, MAX_DISTANCE);
NewPing sonarFront(trigPinF, echoPinF, MAX_DISTANCE);




float oldLeftSensor, oldRightSensor, leftSensor, rightSensor, frontSensor, oldFrontSensor, lSensor, rSensor, fSensor;


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
  analogWrite(ena,100); 
  analogWrite(enb,100); 
  
//setDirection(FORWARD);
 
 
  

}

void loop() {

//float durationL, distanceL;
//setDirection(FORWARD);

while(true){
delay(500);  
ReadSensors();
walls();

  if (frontwall == true ) {
    if (rightwall == true ){
      if (leftwall == true ){
       setDirection(STOP); 
      //stop();
      
      }else{
        setDirection(LEFT);      
      }
    }else{
      setDirection(RIGHT);
    }
    }
    else{
       setDirection(FORWARD);
   }






















   
 /* 

 // if (!frontwall || !leftwall || !rightwall){
ReadSensors();
walls();
/*Serial.println("frontwall");
    Serial.println(frontwall);

  Serial.println("leftwall");
  Serial.println(leftwall);

  Serial.println("rightwall");
  Serial.println(rightwall);
delay(500);
if (frontwall == false ){
  setDirection(FORWARD);
}
else if (leftwall == false ) {
 setDirection(LEFT);
}else if(rightwall == false){
  setDirection(RIGHT);
}else{
 setDirection(STOP);
}
*/


}
}



void ReadSensors() {

leftSensor = sonarLeft.ping_cm(); //ping in cm
rightSensor = sonarRight.ping_cm();
frontSensor = sonarFront.ping_cm();
Serial.print("left sensor : ");
Serial.println(leftSensor);
Serial.print("right sensor : ");
Serial.println(rightSensor);
Serial.print("front sensor : ");
Serial.println(frontSensor);

 oldLeftSensor = leftSensor; // save old readings for movment
 oldRightSensor = rightSensor;
 oldFrontSensor = frontSensor;
  
}



void walls() {


  if (  leftSensor < wall_threshold || leftSensor >= 1000 ) {
    leftwall = true ;
  }
  else {
    leftwall = false ;
  }


  if (  rightSensor < wall_threshold || rightSensor >=1000) {
    rightwall = true ;
  }
  else {
    rightwall = false ;


  } if (  frontSensor < front_threshold || frontSensor >=1000 ) {
    frontwall = true ;
  }
  else {
    frontwall = false ;
  }

}





void setDirection(int dir) {
 
  Serial.print("set Direction .. \n\n");
  if ( dir == FORWARD ) {
   Serial.print("GO FORWARD .. \n");
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(400);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);
  

  
  }
  else if ( dir == LEFT ) {
    Serial.print("GO LEFT .. \n");

  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);


    
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(400);
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

    Serial.print("GO RIGHT .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(400);
  
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);



  
  }
  else if ( dir == STOP ) {
    Serial.print("STOP!!!!!!! .. \n");
    digitalWrite(in1, HIGH);   // Left wheel forward
    digitalWrite(in2, HIGH );
    digitalWrite(in3, HIGH );  // Right wheel forward
    digitalWrite(in4, HIGH);
  }
  else if ( dir == BACKWARD ) {
  Serial.print("GO BACKWARD .. \n");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);


   delay(400);
  digitalWrite(in1, HIGH);   // Left wheel forward
  digitalWrite(in2, HIGH );
  digitalWrite(in3, HIGH );  // Right wheel forward
  digitalWrite(in4, HIGH);


  
  }


  
}
