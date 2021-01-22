String serialData;  // collects characters 
int AnalogCh; 
char readchar;   // reading character
long steps=0;       // motor steps
long totalsteps=0;
long numb2=0;       // Rotation Left->1 , Right->2 or Stop->0
long Rotation=7; 
long Move=8;
long Motor=9; 
long Speed=400; 
// Define Motor connections 

void setup() {
#define dirPin Rotation   //defines Left or Right rotation
#define stepPin Motor  //applies pulses driving the motor
#define enablePin Move  // enables motor rotation  LOW-->rotate High-> no current

  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  Serial.begin(9600);
  while(Serial.read() >= 0) ;     //cleans memory on Serial
  digitalWrite(enablePin, HIGH);  // stop motor current
}


void loop() {

   if (Serial.available() > 0) {
   readchar=Serial.read();
   if (char(readchar) == 83) {   // ASCII S -- S500S
    ReadSteps();   }
   if (char(readchar) == 65) {   // ASCII A 
    ReadSpeed();   }
   if (char(readchar) == 68) {   // ASCII D
    ReadDir();  }
   serialData="";  }
  
  else if(Serial.available() == 0) {

  delay(100);                      }

    if(numb2>0){  
      readchar = Serial.read(); serialData += readchar; readchar = Serial.read(); serialData += readchar;  Rotation=serialData.toInt(); serialData=""; 
        if(numb2 == 1)            {
        digitalWrite(Rotation, LOW);}
            if(numb2 == 2)             {
            digitalWrite(Rotation, HIGH);}
                if(numb2<3){
                readchar = Serial.read(); serialData += readchar; readchar = Serial.read(); serialData += readchar;  Move=serialData.toInt(); serialData=""; 
                readchar = Serial.read(); serialData += readchar; readchar = Serial.read(); serialData += readchar;  Motor=serialData.toInt(); serialData="";
                setup();
                digitalWrite(Move, LOW);
                  while (steps<totalsteps)                                  { 
                  steps = steps +1 ;
                  digitalWrite(Motor, HIGH);   delayMicroseconds(Speed);
                  digitalWrite(Motor, LOW);    delayMicroseconds(Speed);      } 
                  steps=0; //Serial.println(numb2); 
                  numb2=0;
            digitalWrite(Move, HIGH); 
    }       }  
}


void ReadSteps() {
  totalsteps=0;
   while (Serial.available() > 0) {
    readchar = Serial.read();
    if (char(readchar) == 83) {
     totalsteps=serialData.toInt();
     readchar = 32; }   //ASCII code 56 is 8
    serialData += readchar; 
    readchar = 00;  }
     Serial.println(totalsteps);
}


void ReadSpeed() {
  Speed=400;
   while (Serial.available() > 0) {
    readchar = Serial.read();
    if (char(readchar) == 65) {
     Speed=serialData.toInt();
     readchar = 32; }   
    serialData += readchar; 
    readchar = 00;  }
     Serial.println(Speed);
}

void ReadDir() {
  numb2=0;
   
    readchar = Serial.read();
    serialData += readchar; 
    numb2=serialData.toInt();
    readchar = 00; 
    serialData="";
}
