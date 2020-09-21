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
int selfX, selfY, boxX, boxY;;
enum direct {WEST, SOUTH, EAST, NORTH};
direct facing;

float getDistance(int, int);

// Driving Functions
void driveStraight();
void turnLeft();
void powerOff();

// Indication functions
void startLights();
void printSelf();
void sendBox();

void setup(){

  Serial.begin(9600);
  
  pinMode(FLtrigPin, OUTPUT); 
  pinMode(FLechoPin, INPUT); 
  pinMode(FRtrigPin, OUTPUT);
  pinMode(FRechoPin, INPUT);
  pinMode(LefttrigPin, OUTPUT); 
  pinMode(LeftechoPin, INPUT); 
  pinMode(RighttrigPin, OUTPUT);
  pinMode(RightechoPin, INPUT);
  
 
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

    delay(9000);
    startLights();
    facing = WEST;
}

void loop(){
  
  int turns = 1;
  int tempX, tempY;
  bool done = false;
  
  delay(500);
  selfX = ((getDistance(FLtrigPin, FLechoPin) + getDistance(FRtrigPin, FRechoPin)) / 2) + 12;
  selfY = getDistance(RighttrigPin, RightechoPin) + 9;
  delay(2000);

  do{
    
    // Move Forward
    driveStraight();
    
    printSelf();
    delay(100);

    if( getDistance(LefttrigPin, LeftechoPin) <= 30){
      
      sendBox();
      //while(boxX <= 0 || boxY <= 0){
          //sendBox();
        //}
      
      tempX = boxX;
      tempY = boxY;
      //if(tempX >= 0 && tempY >= 0){
        Serial.print('b');Serial.print(tempX);Serial.print(',');Serial.print(tempY);
        Serial.println('!');
      //}
      
      while(getDistance(LefttrigPin, LeftechoPin) <= 30){
        printSelf();
        delay(100);
        analogWrite(LED, 150);
      }
      
      sendBox();
       //while(boxX <= 0 || boxY <= 0){
          //sendBox();
        //}
      
      if (facing == 0 || facing == 2){
        tempX = boxX;
      }
      else if (facing == 1 || facing == 3) {
        tempY = boxY;
      }
      //if(tempX >= 0 && tempY >= 0){
        Serial.print('b');Serial.print(tempX);Serial.print(',');Serial.print(tempY);
        Serial.println('!');
        analogWrite(LED, 0);
        //}
      }

    if(getDistance(FRtrigPin, FRechoPin) <= 17) {
      // Turn Left
      turnLeft();
      delay(2050);
      turns++;
    
     if (facing == 3) {    // Update Direction
        facing = (direct)(0);
       }
        else {facing = (direct)(facing + 1);}   
    }
    
  }while(turns<=4);

    // Move Forward
    driveStraight();

    while(!done){
     if( getDistance(LefttrigPin, LeftechoPin) <= 30){
    
        sendBox();
          //while(boxX <= 0 || boxY <= 0){
              //sendBox();
              //}
         tempX = boxX;
         tempY = boxY;
        //if(tempX >= 0 && tempY >= 0){
         Serial.print('b');Serial.print(tempX);Serial.print(',');Serial.print(tempY);
         Serial.println('!');
      //}
      
        while(getDistance(LefttrigPin, LeftechoPin) <= 30){
            printSelf();
            delay(800);
            analogWrite(LED, 150);
         }
      
        sendBox();
        tempX = boxX;
        Serial.print('b');Serial.print(tempX);Serial.print(',');Serial.print(tempY);
        Serial.println('!');
        analogWrite(LED, 0);
        done = true;
     }
    }

    // Power Off
    powerOff();

  printSelf();
  
  Serial.print("STOP");
  delay(1000);

  exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////

void startLights() {
 
  analogWrite(LED, 150);
  delay(1000);          // 3....
  analogWrite(LED, 0);
  analogWrite(LED, 150);
  delay(1000);          // 2....
  analogWrite(LED, 0);
  analogWrite(LED, 150);
  delay(1000);          // 1....
  analogWrite(LED, 0);
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

void printSelf() {

  if(facing == 0){
    selfX = (getDistance(FRtrigPin, FRechoPin)) + 12;
    selfY = getDistance(RighttrigPin, RightechoPin) + 9;
  }
  else if(facing == 1) {
    selfX = getDistance(RighttrigPin, RightechoPin) + 9;
    selfY = (150 - (getDistance(FRtrigPin, FRechoPin)) - 12);
  }
  else if(facing == 2) {
    selfX = (150 - ( + getDistance(FRtrigPin, FRechoPin)) - 12);
    selfY = (150 - getDistance(RighttrigPin, RightechoPin) - 9);
  }
  else if (facing == 3) {
    selfX = (150 - getDistance(RighttrigPin, RightechoPin) - 9);
    selfY = (getDistance(FRtrigPin, FRechoPin)) + 12;
  }
  //if(selfX >= 0 && selfX <= 150 && selfY >= 0 && selfY <= 150){
    Serial.print('s');Serial.print(selfX);Serial.print(','); Serial.print(selfY);
    Serial.print('!');
    Serial.println();
  //}
}

void sendBox(){

  if(facing == 0) {
    boxX = selfX;
    boxY = selfY + 10 + getDistance(LefttrigPin, LeftechoPin);
  }
  else if(facing == 1) {
    boxX = selfX + 10 + getDistance(LefttrigPin, LeftechoPin);
    boxY = selfY;
  }
  else if(facing == 2) {
    boxX = selfX;
    boxY = selfY - 10 - getDistance(LefttrigPin, LeftechoPin);
  }
  else if (facing == 3) {
    boxX = selfX - 10 - getDistance(LefttrigPin, LeftechoPin);
    boxY = selfY;
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
