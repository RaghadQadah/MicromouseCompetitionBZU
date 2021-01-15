//declaring all the pins and to which we need to connect to the Arduino

// for 3 Ultra Sensor
const int trig1 =A2;
const int echo1= A3;

const int trig2 = 12;
const int echo2 = 13;

const int trig3= 5;
const int echo3 = 4;

/*

int dir;


#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4



#define MAX_DISTANCE 100 


boolean frontwall =false;
boolean leftwall =true;
boolean rightwall =true;

int wall_threshold = 5
;
//int left_threshold = 10 ;
//int right_threshold = 10 ;
int front_threshold = 5 ;



// Constants for Interrupt Pins
// Change values if not using Arduino Uno

const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1



*/


const int in1 = 8; //left
const int in2 = 9; //left

const int in3 = 10; //right
const int in4 = 11; //right

const int ena = 6; 
const int enb = 7;  

int left_sat = 0;
int center_sat = 0;
int right_sat = 0;

long dur1;
int dist1;
long dur2;
int dist2,dist3,dist,dur;

// ultra is a function defined to calculate the distance between the robot and the wall in front
int ultra(int trig,int echo){
  digitalWrite(trig,LOW);
  delay(10);
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  dur=pulseIn(echo,HIGH);//pulseIN checks the duration of the pulse by calculating the time it takes to go from HIGH to LOW. 
  dist=dur*0.034/2;//calculating the distance between robot and wall based on the speed of sound.
  return dist;// returns distance
}

void StopMotor()// function defined to stop all motors by setting LOW as input.
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  delay(100);
}

void left(int PwmC = 100){ // function defined to turn left by making a point turn by turning motor on left side to LOW and on right to HIGH.
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena,PwmC);
  analogWrite(enb,PwmC);
}
void right(int PwmC = 100){// function defined to right left by making a point turn by turning motor on left side to HIGH and on right to LOW.
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena,PwmC);
  analogWrite(enb,PwmC);
}
void forward(int PwmC = 100){ // function for moving forward
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena,PwmC);
  analogWrite(enb,PwmC);
  //Serial.print(dist1);
}
void reverse(int PwmC = 100){ // function for moving forward
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena,PwmC);
  analogWrite(enb,PwmC);
  //Serial.print(dist1);
}

void setup(){
  Serial.begin(9600);
  pinMode(trig1,OUTPUT);// trigger and echo pins are set for the 3 ultrasonic sensors to send and recieve data.
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(trig3,OUTPUT);
  pinMode(echo3,INPUT);
  pinMode(in1,OUTPUT);// motors output is initialized. 
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT); //putyoursetupcodehere,torunonce:
}

bool Align(){
  int d1 = ultra(trig3,echo3),d2=ultra(trig1,echo1); // aligning the robot so that it moves in a straight line and corrects itself if found moving slanting
//  if((d1 > 40) || (d2 > 40))
//    return false;
  if(d1>d2)
  {  
    while(d1>d2)    //Turn Right untill Alignment 
    {
      right(100);
      d1 = ultra(trig3,echo3);
      d2=ultra(trig1,echo1);
    }
    return true;
  }
  else if(d2>d1)  // Turn left untill Alignmnet
  {
    while(d2>d1)
    {
      
      left(100);
      d1 = ultra(trig3,echo3);
      d2 = ultra(trig1,echo1);
    }
    return true;
  }
  return false;
  
}

void loop(){
  dist1 = ultra(trig1,echo1); // to check the distances between the robot and the wall
  dist2 = ultra(trig2,echo2);
  dist3 = ultra(trig3,echo3);
  Serial.print("\ndist1=");
  Serial.print(dist1);
  Serial.print("\ndist2=");
  Serial.print(dist2);
  Serial.print("\ndist3=");
  Serial.print(dist3);
  
  forward(150);
  
  if(((dist1<12)||(dist3<12))&&(dist2>20)){ //TurnRight
    StopMotor();
    right_sat=1;
  }
  
  else if(((dist1<12)||(dist3<12))&&(dist2<20)){ //TurnLeft
    StopMotor();
  left_sat=1;}
  
  if(left_sat==1){ // Given a delay after every turn so that it can align itself properly before moving ahead
    delay(500);
    left(100);
    delay(1550);
    left_sat=0;
    Align();
    StopMotor();
    Serial.println("Alignment Done");
  }
  else if(right_sat==1){
    delay(500);
    right(100);
    delay(1550);
    right_sat=0;
    Align();
    StopMotor();
    Serial.println("Alignment Done");
  }
}
