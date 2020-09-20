const int LED = A5;

const int FLenable = A1;
const int FRenable = A0;
const int RLenable = A15;
const int RRenable = A14;

// Front Left Motor
const int FLmotorPin1  = 2;  // Pin 14 of L293
const int FLmotorPin2  = 3;  // Pin 10 of L293
// Front Right Motor
const int FRmotorPin3  = 4; // Pin  7 of L293
const int FRmotorPin4  = 5;  // Pin  2 of L293
// Rear Left Motor
const int RLmotorPin3 = 7;
const int RLmotorPin4 = 6;
// Rear Right Motor
const int RRmotorPin1 = 9;
const int RRmotorPin2 = 8;

// UltraSonic Sensors
const int FLtrigPin = 53; 
const int FLechoPin = 51;
const int FRtrigPin = 12; 
const int FRechoPin = 11;
const int LefttrigPin = 33; 
const int LeftechoPin = 31;
const int RighttrigPin = 26; 
const int RightechoPin = 24;

float duration, distance;
int selfX, selfY;
enum direct {WEST, SOUTH, EAST, NORTH};
bool inProgress = false;

float getDistance(int, int);

//Driving Functions
void driveStraight();
void turnLeft();
void powerOff();

void printSelf(int);
void sendBox(int);

void setup(){
  
  pinMode(FLtrigPin, OUTPUT); 
  pinMode(FLechoPin, INPUT); 
  pinMode(FRtrigPin, OUTPUT);
  pinMode(FRechoPin, INPUT);
  pinMode(LefttrigPin, OUTPUT); 
  pinMode(LeftechoPin, INPUT); 
  pinMode(RighttrigPin, OUTPUT);
  pinMode(RightechoPin, INPUT);
  Serial.begin(9600);
 
    //Set pins as outputs
    pinMode(LED, OUTPUT);
    pinMode(FLenable, OUTPUT);
    pinMode(FRenable, OUTPUT);
    pinMode(RLenable, OUTPUT);
    pinMode(RRenable, OUTPUT);
    pinMode(FLmotorPin1, OUTPUT);   // FL FWD
    pinMode(FLmotorPin2, OUTPUT);
    pinMode(FRmotorPin3, OUTPUT);   // FR FWD
    pinMode(FRmotorPin4, OUTPUT);
    pinMode(RLmotorPin3, OUTPUT);   // RL FWD
    pinMode(RLmotorPin4, OUTPUT);
    pinMode(RRmotorPin1, OUTPUT);   // RR FWD
    pinMode(RRmotorPin2, OUTPUT);
    
    //enable
    analogWrite(FLenable, 255);
    analogWrite(FRenable, 255);
    analogWrite(RLenable, 255);
    analogWrite(RRenable, 255);
}

void loop(){

  // Start up blink
  Serial.print("Scanning will commence in 3...");
  analogWrite(LED, 150);
  delay(1000);
  analogWrite(LED, 0);
  Serial.print("2...");
  analogWrite(LED, 150);
  delay(1000);
  analogWrite(LED, 0);
  Serial.print("1...");
  analogWrite(LED, 150);
  delay(1000);
  analogWrite(LED, 0);

  inProgress = true;
  
  int turns = 1;
  direct facing = WEST;
  delay(500);
  selfX = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
  selfY = getDistance(RighttrigPin, RightechoPin) + 9;
  Serial.print("Starting Point: ");Serial.print(selfX);Serial.print(", "); Serial.println(selfY);
  //Serial.print("Left Sensor: ");Serial.print(getDistance(FLtrigPin, FLechoPin));
  //Serial.print(" Right Sensor: ");Serial.println(getDistance(FRtrigPin, FRechoPin));
  Serial.println("Facing: ");Serial.println(facing);
  delay(2000);

  do{
    
    // Move Forward
    driveStraight();
    
    printSelf(facing);
    delay(100);

    if( getDistance(LefttrigPin, LeftechoPin) <= 30){
      sendBox(facing);
      while(getDistance(LefttrigPin, LeftechoPin) <= 30){
        //Serial.print("...Scanning Box...");
        analogWrite(LED, 150);
      }
      //Serial.println("...Done Scanning...");
      sendBox(facing);
      analogWrite(LED, 0);
    }

    if( (getDistance(FLtrigPin, FLechoPin) <= 15 ) || (getDistance(FRtrigPin, FRechoPin) <= 15)) {
      // Turn Left
    turnLeft();
    delay(1900);
    turns++;
    
    if (facing == 3) {    // Update Direction
      facing = (direct)(0);
    }
    else {facing = (direct)(facing + 1);}
    Serial.print("Now facing: ");Serial.println(facing);   
    }
    
  }while(turns<=4);

    // Move Forward
    driveStraight();
    delay(2000);

    // Power Off
    powerOff();

  printSelf(0);
  inProgress = false;
  delay(1000);

  exit(0);
}

void driveStraight() {

   digitalWrite(FLmotorPin1, HIGH);
   digitalWrite(FLmotorPin2, LOW);
   digitalWrite(FRmotorPin3, HIGH);
   digitalWrite(FRmotorPin4, LOW);
   digitalWrite(RLmotorPin3, HIGH);
   digitalWrite(RLmotorPin4, LOW);
   digitalWrite(RRmotorPin1, HIGH);
   digitalWrite(RRmotorPin2, LOW);
  
}

void turnLeft() {
    
    digitalWrite(FLmotorPin1, LOW);
    digitalWrite(FLmotorPin2, HIGH);
    digitalWrite(FRmotorPin3, HIGH);
    digitalWrite(FRmotorPin4, LOW);
    digitalWrite(RLmotorPin3, LOW);
    digitalWrite(RLmotorPin4, HIGH);
    digitalWrite(RRmotorPin1, HIGH);
    digitalWrite(RRmotorPin2, LOW);
  
}

void powerOff() {

    digitalWrite(FLmotorPin1, LOW);
    digitalWrite(FLmotorPin2, LOW);
    digitalWrite(FRmotorPin3, LOW);
    digitalWrite(FRmotorPin4, LOW);
    digitalWrite(RLmotorPin3, LOW);
    digitalWrite(RLmotorPin4, LOW);
    digitalWrite(RRmotorPin1, LOW);
    digitalWrite(RRmotorPin2, LOW);
}

void printSelf(int fac) {

  if(fac == 0){
    selfX = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
    selfY = getDistance(RighttrigPin, RightechoPin) + 9;
  }
  else if(fac == 1) {
    selfX = getDistance(RighttrigPin, RightechoPin) + 9;
    selfY = (150 - ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) - 12);
  }
  else if(fac == 2) {
    selfX = (150 - ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) - 12);
    selfY = (150 - getDistance(RighttrigPin, RightechoPin) - 9);
  }
  else if (fac == 3) {
    selfX = (150 - getDistance(RighttrigPin, RightechoPin) - 9);
    selfY = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
  }
  if(selfX >= 0 && selfX <= 150 && selfY >= 0 && selfY <= 150){
    Serial.write('p');Serial.write(selfX);Serial.write(','); Serial.write(selfY);
    Serial.write('/');
  }
}

void sendBox(int fac){

  int boxX, boxY;

  if(fac == 0){
    boxX = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
    boxY = getDistance(RighttrigPin, RightechoPin) + 19 + getDistance(LefttrigPin, LeftechoPin);
  }
  else if(fac == 1) {
    boxX = getDistance(RighttrigPin, RightechoPin) + 19 + getDistance(LefttrigPin, LeftechoPin);
    boxY = (150 - ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) - 12);
  }
  else if(fac == 2) {
    boxX = (150 - ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) - 12);
    boxY = (150 - getDistance(RighttrigPin, RightechoPin) - 19 - getDistance(LefttrigPin, LeftechoPin));
  }
  else if (fac == 3) {
    boxX = (150 - getDistance(RighttrigPin, RightechoPin) - 19 - getDistance(LefttrigPin, LeftechoPin));
    boxY = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
  }
  
  if(boxX >= 0 && boxY >= 0){
  Serial.write('b');Serial.write(boxX);Serial.write(',');Serial.write(boxY);
  Serial.write('/');
  }
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  return distance;
  
}
