 
char inByte;         // incoming serial byte
int bytesReceived[3];
int count;
 
void setup() {
  // start serial port at 115200 bps:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
}
 
void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) { //we have received a request for messages
  
    // get incoming byte, in the future can use this for different commands
    inByte = Serial.read();
//    if(inByte==255){
//     count = 0;
//      bytesReceived[count]=inByte;
//      count++;
//    }else{
      bytesReceived[count] = inByte;
      count++;
 //   } 
     
     for(int i=0;i<3;i++){
           Serial.println(bytesReceived[0]);   
     }
  }
 
}
