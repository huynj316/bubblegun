#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	//serial start
	ofSetVerticalSync(true);
	
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
	mySerial.setup(0, baud); //open the first device
	
	bytesToSend[1] = 115;
	bytesToSend[2] = 32;
	
	mySerial.flush();
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (bSendSerialMessage){
		
		// (1) write the letter "a" to serial:
		mySerial.writeByte('a');
		
		bSendSerialMessage = false;
	}
	//serial start
	if(mySerial.available() >0){
		
		bytesToSend[0] = 255;

		//		bytesToSend[1] = ofMap(attention, 0, 100, 0, 254);
		//		bytesToSend[2] = ofMap(blink, 0, 100, 0, 254);
		
		mySerial.writeBytes(bytesToSend, 3);
		
		cout << "bytesToSend[0] = " << bytesToSend[0] << endl;
		cout << "bytesToSend[1] = " << bytesToSend[1] << endl;
		cout << "bytesToSend[2] = " << bytesToSend[2] << endl;
		cout << "------------------------"<<endl;
		
		mySerial.flush(); //give a little flush before we start anything
	}
		

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    ofSetColor(255);
	
	if (isInitialized = true) {
		bSendSerialMessage = true;	
	}
	else {
		bSendSerialMessage = false;
	}


	//draw out our received data
    for(int i=0; i<NUM_MSG_BYTES; i++){
        string whichByte = "bytesToSend["+ofToString(i)+"] = ";
        ofDrawBitmapString(whichByte+ofToString(bytesToSend[i]), 50, 30+30*i);
    }
    
    //just for debug
    ofDrawBitmapString("num messages sent total: "+ ofToString(numMsgSent), 50, 150);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}