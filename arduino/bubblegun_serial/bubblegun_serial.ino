 #include <Servo.h>
Servo myservo;

int pos = 0;
int motorPin=9;

int bytesReceived[3]; //incoming bytes

void setup() {
  pinMode(motorPin, OUTPUT); //motor on 9

  myservo.attach(5); //servo on 5
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() >= 3) { //we have received THREE BYTES (at least)

    for (int i=0; i< 3; i++){ //parse out all three bytes
      bytesReceived[i] = Serial.read();
    }

    int motorStrength  = bytesReceived[0]; //use the first byte to determine which LED    
    int whichMotor = bytesReceived[1]; //use the second byte to determine numBlinks
    int blinkStrength     = bytesReceived[2]; //use the third to control blink speed (delay)
    sendPower(motorStrength, whichMotor, blinkStrength); //pass that in to the blink function
  }
}


void sendPower(int motorStrength, int whichMotor, int blinkStrength){ //blinkLED function
 for(int i=0; i < numTimes; i++){
  analogWrite(whichMotor, motorStrength);
 }

  if (bytesReceived[2] > 0) {
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(10);                       // waits 15ms for the servo to reach the position 
    } 
    for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(5);                       // waits 15ms for the servo to reach the position 
    } 
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(10);                       // waits 15ms for the servo to reach the position 
    } 
  }
  delay(100); //put 1000 in between so we can see when 1 message starts and ends

  //AT THE END OF THIS FUNCTION, I AM REQUESTING THE NEXT UPDATE FROM OF:
  Serial.write('N');
}

