
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
int wall_threshold = 8;
int front_threshold = 20 ;
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1

int in1 = 8; 
int in2 = 9; 
int in3 = 10; 
int in4 = 11; 
int ena = 6; 
int enb = 7; 
int flag=1;
float leftSensor, rightSensor, frontSensor;
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

}



void loop() {
setDirection(FORWARD);
while(true){
delay(1000);
ReadSensors();
walls();

if (frontwall == true ) {
    Serial.println("front wall");
    if (rightwall == true ){
      Serial.println("right wall");
      if (leftwall == true ){
          Serial.println("left wall");
          while(rightwall == true && leftwall == true && frontwall == true  ){
            setDirection(BACKWARD);
            ReadSensors();
            walls();
          }
          
      }else{
        Serial.println("NO left wall");
        setDirection(LEFT);      
      }
    }else{
      Serial.println("NO right wall");
      setDirection(RIGHT);
    }  
}else{
    Serial.println("NO front wall");
    setDirection(FORWARD);

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
