
// Parameters
const int drive_distance = 22;   // cm
const int motor_power = 200;      // 0-255
const int motor_offset = 5;       // Diff. when driving straight
const int wheel_d = 66.1;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 384;   // (4 pairs N-S) * (48:1 gearbox) * (2 falling/rising edges) = 384

// Pins
const int enc_l_pin = 2;          // Motor A
const int enc_r_pin = 3;          // Motor B
const int pwma_pin = 6;
const int ain1_pin = 8;
const int ain2_pin = 9;
const int pwmb_pin = 7;
const int bin1_pin = 10;
const int bin2_pin = 11;

//sensor 
int trigPinL =A2;
int echoPinL = A3;
int trigPinF = 12;
int echoPinF = 13;
int trigPinR = 5;
int echoPinR = 4;
//
boolean wallFront ;
boolean wallLeft ;
boolean wallRight ;
int wall_threshold = 10;
float leftSensor, rightSensor, frontSensor;
// Globals
volatile unsigned long enc_l = 0;
volatile unsigned long enc_r = 0;

void setup() {

  // Debug
  Serial.begin(9600);

  // Set up pins
  pinMode(enc_l_pin, INPUT_PULLUP);
  pinMode(enc_r_pin, INPUT_PULLUP);
  pinMode(pwma_pin, OUTPUT);
  pinMode(ain1_pin, OUTPUT);
  pinMode(ain2_pin, OUTPUT);
  pinMode(pwmb_pin, OUTPUT);
  pinMode(bin1_pin, OUTPUT);
  pinMode(bin2_pin, OUTPUT);
  //sensor
 pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  // Set up interrupts
  attachInterrupt(digitalPinToInterrupt(enc_l_pin), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_r_pin), countRight, CHANGE);

  // Drive straight
  //delay(1000);
//  enableMotors(true);

}

void loop() {
 
  driveStraight(drive_distance, motor_power);

}

void driveStraight(float dist, int power) {

  unsigned long num_ticks_l;
  unsigned long num_ticks_r;

  // Set initial motor power
  int power_l = motor_power;
  int power_r = motor_power;

  // Used to determine which way to turn to adjust
  unsigned long diff_l;
  unsigned long diff_r;

  // Reset encoder counts
  enc_l = 0;
  enc_r = 0;

  // Remember previous encoder counts
  unsigned long enc_l_prev = enc_l;
  unsigned long enc_r_prev = enc_r;

  // Calculate target number of ticks
  float num_rev = (dist * 10) / wheel_c;  // Convert to mm
  unsigned long target_count = num_rev * counts_per_rev;
  
  // Debug
  Serial.print("Driving for ");
  Serial.print(dist);
  Serial.print(" cm (");
  Serial.print(target_count);
  Serial.print(" ticks) at ");
  Serial.print(power);
  Serial.println(" motor power");

  // Drive until one of the encoders reaches desired count
  while ( (enc_l < target_count) && (enc_r < target_count) ) {
    ReadSensors();
    walls();
     if(!wallLeft){
    digitalWrite(bin1_pin, LOW); //left 90 degree
    digitalWrite(bin2_pin, HIGH);
    digitalWrite(ain1_pin, HIGH);
    digitalWrite(ain2_pin, LOW);
    analogWrite(pwma_pin, 150);
  analogWrite(pwmb_pin, 150);
  Serial.println("left 90 degree ");
    delay(100);
    brake();
    //delay(10000);
  }
    else if(!wallFront){
    // Sample number of encoder ticks
    num_ticks_l = enc_l;
    num_ticks_r = enc_r;

    // Print out current number of ticks
    Serial.print(num_ticks_l);
    Serial.print("\t");
    Serial.println(num_ticks_r);

    // Drive
    drive(power_l, power_r);

    // Number of ticks counted since last time
    diff_l = num_ticks_l - enc_l_prev;
    diff_r = num_ticks_r - enc_r_prev;

    // Store current tick counter for next time
    enc_l_prev = num_ticks_l;
    enc_r_prev = num_ticks_r;

    // If left is faster, slow it down and speed up right
    if ( diff_l > diff_r ) {
      power_l -= motor_offset;
      power_r += motor_offset;
    }

    // If right is faster, slow it down and speed up left
    if ( diff_l < diff_r ) {
      power_l += motor_offset;
      power_r -= motor_offset;
    }

    // Brief pause to let motors respond
   // delay(20);
  }
  else if (!wallRight){
    digitalWrite(ain1_pin, LOW); //right 90 degree
    digitalWrite(ain2_pin, HIGH);
    digitalWrite(bin1_pin, HIGH);
    digitalWrite(bin2_pin, LOW);
    analogWrite(pwma_pin, 150);
  analogWrite(pwmb_pin, 150);
    Serial.println("right 90 degree ");
    delay(100);
    brake();
  }
    
  }

  // Brake
  brake();
}
void drive(int power_a, int power_b) {

  // Constrain power to between -255 and 255
  power_a = constrain(power_a, -255, 255);
  power_b = constrain(power_b, -255, 255);

  // Left motor direction
  if ( power_a < 0 ) {
    digitalWrite(ain1_pin, LOW);
    digitalWrite(ain2_pin, HIGH);
  } else {
    digitalWrite(ain1_pin, HIGH);
    digitalWrite(ain2_pin, LOW);
  }

  // Right motor direction
  if ( power_b < 0 ) {
    digitalWrite(bin1_pin, LOW);
    digitalWrite(bin2_pin, HIGH);
  } else {
    digitalWrite(bin1_pin, HIGH);
    digitalWrite(bin2_pin, LOW);
  }

  // Set speed
  analogWrite(pwma_pin, abs(power_a));
  analogWrite(pwmb_pin, abs(power_b));
}

void brake() {
  digitalWrite(ain1_pin, LOW);
  digitalWrite(ain2_pin, LOW);
  digitalWrite(bin1_pin, LOW);
  digitalWrite(bin2_pin, LOW);
  analogWrite(pwma_pin, 0);
  analogWrite(pwmb_pin, 0);
}

void countLeft() {
  enc_l++;
}

void countRight() {
  enc_r++;
}
//*****************************************************************************************************************************************
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
//*****************************************************************************************************************************************
void walls() {
 // leftSensor-=2;
 // rightSensor-=2;
 // frontSensor-=2;
  if (  leftSensor < wall_threshold || (leftSensor >= 1000 && leftSensor < 1900) ) {
    Serial.print("left sensor :");
    Serial.println(leftSensor);
  Serial.println("wallLeft = true");
    wallLeft = true ;
  }
  else {
    Serial.print("left sensor :");
    Serial.println(leftSensor);
  Serial.println("wallLeft = false ;");
    wallLeft = false ;
  }
  if (  rightSensor < wall_threshold || (rightSensor >= 1000 && rightSensor <= 1900)) {
    Serial.print("right sensor :");
    Serial.println(rightSensor);
  Serial.println(" wallRight = true ;");
    wallRight = true ;
  }
  else {
    Serial.print("right sensor :");
    Serial.println(rightSensor);
    wallRight = false ;
  Serial.println("wallRight = false ;");
  } if (  frontSensor < wall_threshold || (frontSensor >= 1000 && frontSensor <1900) ) {
    Serial.print("front sensor :");
    Serial.println(frontSensor);
    wallFront = true ;
  Serial.println(" wallFront = true ;");
  }
  else {
    Serial.print("front sensor :");
    Serial.println(frontSensor);
    wallFront = false ;
  Serial.println("wallFront = false ;");
  }
}
