int trigPinL =2;
int echoPinL = 3;

int trigPinF = 12;
int echoPinF = 13;

int trigPinR = 5;
int echoPinR = 4;

void setup() {
  Serial.begin(9600); 
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);

  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  
  // put your setup code here, to run once:

}

void loop() {
  float durationL, distanceL;
  digitalWrite(trigPinL,HIGH);
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
  Serial.println("************************");

}
