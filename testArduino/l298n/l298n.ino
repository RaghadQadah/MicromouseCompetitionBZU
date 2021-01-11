int in1 = 8; 
int in2 = 9; 
int in3 = 11; 
int in4 = 10; 
int ena = 6; 
int enb = 7; 

void setup() {
  
  Serial.begin(9600); 
 // pinMode(ena, OUTPUT); 
  pinMode(enb, OUTPUT); 

  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
 analogWrite(ena, 255); 
 analogWrite(enb, 255);
 delay(1000); 
}

void loop() {
  

  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000); 
  // forward
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2000); 
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000); 
  

}
