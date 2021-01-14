



#include "NewPing.h"

#define TRIGGER_PINL  A2  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINL     A3  // Arduino pin tied to echo pin on ping sensor.

#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIGGER_PINF  12  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINF     13  // Arduino pin tied to echo pin on ping sensor.

#define TRIGGER_PINR  5  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINR     4  // Arduino pin tied to echo pin on ping sensor.





int dir;


#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4



float P = 0.7 ;
float D = 0.5 ;
float I = 0.4 ;
float oldErrorP ;
float totalError ;
int offset = 5 ;

int wall_threshold = 13 ;
//int left_threshold = 10 ;
//int right_threshold = 10 ;
int front_threshold = 7 ;

boolean frontwall ;
boolean leftwall ;
boolean rightwall ;
boolean first_turn ;
boolean rightWallFollow ;
boolean leftWallFollow ;





int en1 = 8 ;
int en2 = 9 ;

int en3 = 11 ;
int en4 =  10;

int enA = 6 ;
int enB = 7 ;

int baseSpeed = 70 ;

int RMS ;
int LMS ;

//int LED = 13 ;
//int led1 = 8 ;
//int led2 = 9 ;



NewPing sonarLeft(TRIGGER_PINL, ECHO_PINL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PINR, ECHO_PINR, MAX_DISTANCE);
NewPing sonarFront(TRIGGER_PINF, ECHO_PINF, MAX_DISTANCE);

unsigned int pingSpeed = 30; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.


float oldLeftSensor, oldRightSensor, leftSensor, rightSensor, frontSensor, oldFrontSensor, lSensor, rSensor, fSensor;

//int TestNUM = 1  ;



void setup() {

  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.


  for (int i = 2; i <= 13; i++) //For Motor Shield
    pinMode(i, OUTPUT);



  first_turn = false ;
  rightWallFollow = false ;
  leftWallFollow = false ;
 

}

void loop() {


  //========================================START========================================//


  ReadSensors();

  walls();


  if ( first_turn == false ) {

    pid_start();

  }
  else if (leftWallFollow == true ) {

    PID(true) ;

  }
  else if (rightWallFollow == true ) {
    PID(false) ;
  }


  if (leftwall == true && rightwall == false && frontwall == true ) {

    // turnright();
    PID(false) ;

    if ( first_turn == false ) {

      //      right_threshold = right_threshold - offset ;
      //      left_threshold = left_threshold + offset ;


      first_turn = true ;
      rightWallFollow = true ;
      
//      digitalWrite(led2 , LOW );
    //  digitalWrite(led1 ,HIGH );
    }
  }
   if (leftwall == false && rightwall == true && frontwall == true ) {

    //  turnleft();
    PID(true) ;

    if ( first_turn == false ) {

      //      left_threshold = left_threshold - offset ;
      //      right_threshold = right_threshold + offset ;

      first_turn = true ;
      leftWallFollow = true ;
//      digitalWrite(LED , HIGH);
       
    }
  }
   if ( leftSensor == 0 || leftSensor > 100 && rightSensor == 0 || rightSensor > 100 && frontSensor == 0 || frontSensor > 100 ) {

    setDirection(STOP);
  }



  // read sensors & print the result to the serial monitor //


  Serial.print(" Left : ");
  Serial.print(leftSensor);
  Serial.print(" cm ");
  Serial.print(" Right : ");
  Serial.print(rightSensor);
  Serial.print(" cm ");
  Serial.print(" Front : ");
  Serial.print(frontSensor);
  Serial.print(" cm ");

  //measure error & print the result to the serial monitor


  Serial.print("error=");
  Serial.print(totalError);


}








//--------------------------------- direction control ---------------------------------//

void setDirection(int dir) {
  Serial.print("set Direction .. \n\n");
  if ( dir == FORWARD ) {
     Serial.print("GO FORWARD .. \n");
    digitalWrite(en1, LOW);   // Left wheel forward
    digitalWrite(en2, HIGH);
    digitalWrite(en3, LOW);  // Right wheel forward
    digitalWrite(en4, HIGH);
  }
  else if ( dir == LEFT ) {
    Serial.print("GO LEFT .. \n");
    digitalWrite(en1, HIGH);   // Left wheel forward
    digitalWrite(en2, LOW );
    digitalWrite(en3, LOW );  // Right wheel forward
    digitalWrite(en4, HIGH);
  }
  else if ( dir == RIGHT ) {
    Serial.print("GO RIGHT .. \n");
    digitalWrite(en1, LOW);   // Left wheel forward
    digitalWrite(en2, HIGH);
    digitalWrite(en3, HIGH);  // Right wheel forward
    digitalWrite(en4, LOW);
  }
  else if ( dir == STOP ) {
    Serial.print("STOP!!!!!!! .. \n");
    digitalWrite(en1, HIGH);   // Left wheel forward
    digitalWrite(en2, HIGH );
    digitalWrite(en3, HIGH );  // Right wheel forward
    digitalWrite(en4, HIGH);
  }
  else if ( dir == BACKWARD ) {
    Serial.print("GO BACKWARD .. \n");
    digitalWrite(en1, HIGH );   // Left wheel forward
    digitalWrite(en2, LOW );
    digitalWrite(en3, HIGH );  // Right wheel forward
    digitalWrite(en4, LOW );
  }
}
//---------------------------------------------------------------------------//


//--------------------------------- Sensors ---------------------------------//

void ReadSensors() {
Serial.print("Start Read Sensors .. \n");
  //leftSensor = sonarLeft.ping_median(TestNUM);     //accurate but slow
  //rightSensor = sonarRight.ping_median(TestNUM);     //accurate but slow
  //frontSensor = sonarFront.ping_median(TestNUM);     //accurate but slow

  //leftSensor = sonarLeft.convert_cm(leftSensor);
  //rightSensor = sonarRight.convert_cm(rightSensor);
  //frontSensor = sonarFront.convert_cm(frontSensor);

  lSensor = sonarLeft.ping_cm(); //ping in cm
  rSensor = sonarRight.ping_cm();
  fSensor = sonarFront.ping_cm();


  leftSensor = (lSensor + oldLeftSensor) / 2; //average distance between old & new readings to make the change smoother
  rightSensor = (rSensor + oldRightSensor) / 2;
  frontSensor = (fSensor + oldFrontSensor) / 2;


  oldLeftSensor = leftSensor; // save old readings for movment
  oldRightSensor = rightSensor;
  oldFrontSensor = frontSensor;

}

//---------------------------------------------------------------------------//


//--------------------------------- control ---------------------------------//

void pid_start() {
Serial.print("pid_start .. \n");
  float errorP = leftSensor - rightSensor ;
  float errorD = errorP - oldErrorP;
  float errorI = (2.0 / 3.0) * errorI + errorP ;

  totalError = P * errorP + D * errorD + I * errorI ;
  
  oldErrorP = errorP ;

  RMS = baseSpeed + totalError ;
  LMS = baseSpeed - totalError ;

  if (RMS < 0) {
    

    RMS = map(RMS , 0 , -255, 0, 255);

    analogWrite(enA , RMS);
    analogWrite(enB , LMS);
 Serial.print("GO RIGHT .. \n");
    setDirection(RIGHT);

  }
  else if (LMS < 0) {
    LMS = map(LMS , 0 , -255, 0, 255);


    analogWrite(enA , RMS);
    analogWrite(enB , LMS);
 Serial.print("GO LEFT .. \n");
    setDirection(LEFT);
  }
  else {

    analogWrite(enA , RMS);
    analogWrite(enB , LMS);
 Serial.print("GO FORWARD .. \n");
    setDirection(FORWARD);
  }

}


//----------------------------- wall follow  control -------------------------------//

void PID( boolean left ) {
 Serial.print("PID .. \n");
  if (left == true ) {
 Serial.print("left true .. \n");
    float errorP = leftSensor - rightSensor - offset ;
    float errorD = errorP - oldErrorP;
    float errorI = (2.0 / 3) * errorI + errorP ;


    totalError = P * errorP + D * errorD + I * errorI ;

    oldErrorP = errorP ;


    RMS = baseSpeed + totalError ;
    LMS = baseSpeed - totalError ;

    //  if(RMS < -255) RMS = -255; if(RMS > 255)RMS = 255 ;
    //  if(LMS < -255) LMS = -255;  if(LMS > 255)LMS = 255 ;


    if (RMS < 0) {

      RMS = map(RMS , 0 , -255, 0, 255);

      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO RIGHT .. \n");
      setDirection(RIGHT);

    }
    else if (LMS < 0) {
      LMS = map(LMS , 0 , -255, 0, 255);


      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO LEFT .. \n");
      setDirection(LEFT);
    }
    else {

      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO FORWARD .. \n");
      setDirection(FORWARD);
    }

  }
  else {

    float errorP = leftSensor - rightSensor + offset ;
    float errorD = errorP - oldErrorP;
    float errorI = (2.0 / 3) * errorI + errorP ;

    totalError = P * errorP + D * errorD + I * errorI ;

    oldErrorP = errorP ;


    RMS = baseSpeed + totalError ;
    LMS = baseSpeed - totalError ;

    //  if(RMS < -255) RMS = -255; if(RMS > 255)RMS = 255 ;
    //  if(LMS < -255) LMS = -255;  if(LMS > 255)LMS = 255 ;
Serial.print("Left false .. \n");

    if (RMS < 0) {

      RMS = map(RMS , 0 , -255, 0, 255);

      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO RIGHT .. \n");
      setDirection(RIGHT);

    }
    else if (LMS < 0) {
      LMS = map(LMS , 0 , -255, 0, 255);


      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO LEFT .. \n");
      setDirection(LEFT);
    }
    else {

      analogWrite(enA , RMS);
      analogWrite(enB , LMS);
Serial.print("GO FORWARD .. \n");
      setDirection(FORWARD);
    }

  }

}

//--------------------------- wall detection --------------------------------//

void walls() {
    Serial.print("** Check walls ** \n");


  if ( leftSensor < wall_threshold ) {
    leftwall = true ;
    Serial.print("Left wall .. \n");
  }
  else {
    leftwall = false ;
    Serial.print("Left no wall .. \n");
  }


  if ( rightSensor < wall_threshold ) {
    rightwall = true ;
    Serial.print("right wall .. \n");
  }
  else {
    rightwall = false ;
    Serial.print("right no wall .. \n");


  } if ( frontSensor < front_threshold ) {
    frontwall = true ;
    Serial.print("front wall .. \n");
  }
  else {
    frontwall = false ;
    Serial.print("front no wall .. \n");
  }

}



//---------------------------------------------------------------------------//

void turnright() {


  LMS = baseSpeed ;

  RMS = LMS * rightSensor / ( rightSensor + 11 ) ;

Serial.print("turn right .. \n");
}

//---------------------------------------------------------------------------//

void turnleft() {


  RMS = baseSpeed ;

  LMS = RMS * leftSensor / ( leftSensor + 11 ) ;
Serial.print("turn left .. \n");
}


//---------------------------------------------------------------------------//
