

 
int motorIn1 = 8;
int motorIn2 = 9;
int motorEnA = 6;
 
int encoder = 0;
 
volatile unsigned int counter;
int rpm;
 

void setup() {
  
  Serial.begin(9600); 
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnA, OUTPUT);
  pinMode(encoder, INPUT);
 
  digitalWrite(encoder, HIGH);
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnA, 100);
 
  attachInterrupt(0,countpulse,RISING);
 
}
 
void countpulse(){
        counter++;
}
 
void loop() {
  static uint32_t previousMillis;
   
  if (millis() - previousMillis >= 1000) {
            rpm = (counter/20)*60;          
            counter = 0;
            previousMillis += 1000;
           
            
  }
 
 
  Serial.print("Speed: ");
 Serial.print(rpm);
  Serial.println(" rps");
  delay(1000);
}
