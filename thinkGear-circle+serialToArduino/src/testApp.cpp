#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &testApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &testApp::meditationListener);
	ofAddListener(thinkGear.blinkChangeEvent, this, &testApp::blinkListener);
    
    font.loadFont("font/DroidSansMono.ttf", 20);
	
    
    distAw = 0.0;
    prevAw = 0.0;
    currAw = 0.0;
    distMw = 0.0;
    prevMw = 0.0;
    currMw = 0.0;
    
    atChangeTime = 0.0;
    attention = 0.0;
    meChangeTime = 0.0;
    meditation = 0.0;
	
	//serial start
	ofSetVerticalSync(true);
	
	ofBackground(255);	
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
	mySerial.setup(0, baud); //open the first device
	
	
	bytesToSend[0] = ofMap(attention, 0, 100, 0, 254);
	bytesToSend[1] = 9;
	bytesToSend[2] = ofMap(blink, 0, 100, 0, 254);
//	bytesToSend[3] = 5;
	
//  bytesToSend[0] = 12;
//	bytesToSend[1] = 1;
//	bytesToSend[2] = 155;
	
	mySerial.flush();
    isInitialized = false;
	
	
}
//--------------------------------------------------------------
void testApp::update(){
    thinkGear.update();
    
    float cur = ofGetElapsedTimef();
    float progress = cur - atChangeTime;
    progress = ofClamp(progress, 0.0, 1.0);
    if (progress == 0.0) prevAw = currAw;
    currAw = ofxTweenLite::tween(prevAw, distAw, progress, OF_EASE_SINE_INOUT);
    
    progress = cur - meChangeTime;
    progress = ofClamp(progress, 0.0, 1.0);
    if (progress == 0.0) prevMw = currMw;
    currMw = ofxTweenLite::tween(prevMw, distMw, progress, OF_EASE_SINE_INOUT);
    
	//serial start
	if(mySerial.available() >0){ //something came in over serial port
        
        if(!isInitialized) isInitialized = true; //we heard from Arduino, we're initialized.
        
        char incoming = mySerial.readByte();
        cout << "message received: "<< incoming << endl;
        
        mySerial.flush(); //flush whatever messages were received, clean slate next frame
        
		mySerial.writeBytes(bytesToSend, NUM_MSG_BYTES); //send out current bytes to send
        numMsgSent++; //for our own count
	}
	
	//if(mySerial.available() >0){
//
//        bytesToSend[0] = 255;
//		bytesToSend[1] = ofMap(attention, 0, 100, 0, 254);
//		bytesToSend[2] = ofMap(blink, 0, 100, 0, 254);
//		
//		mySerial.writeBytes(bytesToSend, 3);
//		
//        cout << "bytesToSend[0] = " << bytesToSend[0] << endl;
//        cout << "bytesToSend[1] = " << bytesToSend[1] << endl;
//        cout << "bytesToSend[2] = " << bytesToSend[2] << endl;
//        cout << "------------------------"<<endl;
//        
//        mySerial.flush(); //give a little flush before we start anything
//	}
//	
//	if(ofGetElapsedTimeMillis() > sentMsgTime + 1000){
//		bSendSerialMessage = true;
//	}
}
//--------------------------------------------------------------
void testApp::draw()
{
	
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    ofPushStyle();
    ofSetColor(ofColor::black);
    string qStr = "";
    if (thinkGear.getSignalQuality() == 0.0)
    {
        qStr =
		"good";
//		mySerial.writeBytes(bytesToSend, NUM_MSG_BYTES);
//		bytesToSend[0] = ofMap(attention, 0, 100, 0, 254);
//		bytesToSend[1] = 9;
//		bytesToSend[2] = ofMap(blink, 0, 100, 0, 254);
//		bytesToSend[3] = 5;
    }
    else 
    {
        qStr = "poor (" + ofToString(thinkGear.getSignalQuality()) + ")";
    }
    font.drawString("signal quality = " + qStr, 10, 40);    
    ofPopStyle();
	
	//blink
	font.drawString("Blink", 10, (blink)); 
    
	//directions
	if (attention < 1 && meditation < 1) {
		font.drawString("CONCENTRATE ON DOT", ofGetWidth()/2, ofGetHeight()/2);
	}

	
    //attention bar
    ofPushMatrix();
    ofTranslate(0, 30);
    ofPushStyle();
    ofSetColor(ofColor::black);
    font.drawString("Attention", 10, ofGetHeight()/11 - 10);
    ofNoFill();
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, currAw);
//	ofRect(1, ofGetHeight()/11, ofGetWidth() - 2, 60);
    ofSetColor(ofColor::yellow, ofMap(currAw, 0.0, ofGetWidth(), 50, 255));
    ofFill(); 
	ofCircle(ofGetWidth()/2, ofGetHeight()/2, currAw);
//  ofRect(0, ofGetHeight()/11, currAw, 59);
	ofSetColor(ofColor::black, ofMap(currAw, 0.0, ofGetWidth(), 50, 255));
	font.drawString(ofToString(attention), 10, ofGetHeight()/11 + 40);

	
	mySerial.writeBytes(bytesToSend, NUM_MSG_BYTES);
	if (attention > 0 && attention < 100) {	
		bytesToSend[0] = ofMap(attention, 0, 100, 200, 254);
	}
	bytesToSend[1] = 9;
	if (blink > 0 && blink < 200) {
		bytesToSend[2] = ofMap(blink, 0, 100, 0, 254);
	}
	
//	if (attention > 0 && attention < 255) {
//		//mySerial.writeByte(ofMap(attention, 0, 250, 0, 255, true));
////		mySerial.writeByte(attention);
//		int controlVal = ofMap(attention, 0, 100, 255, 0, true);
//		sendToMotor(controlVal);
//	}
	
//    if (attention >= 30.0) {
//		serial.writeByte('H');
//		bSendSerialMessage = true;
//    }
//	else if (attention < 30.0) {
//		bSendSerialMessage = true;
//		serial.writeByte('L');
//	}
	
    ofPopStyle();
    ofPopMatrix();
    
	//key circle
	ofSetColor(ofColor::black);
//	ofTranslate(0, 30);
	ofCircle(ofGetWidth()/2, (ofGetHeight()/2)+30, 2);
	
	//meditation bar
    ofPushMatrix();
    ofTranslate(0, 30);
    ofPushStyle();
    ofSetColor(ofColor::black);
    font.drawString("Meditation", 10, (ofGetHeight()/3.5) - 10); 
    ofNoFill();
	ofCircle(ofGetWidth()/2, ofGetHeight()/2, currMw);
//  ofRect(1, (ofGetHeight()/3.5), ofGetWidth() - 2, 60);    
    ofSetColor(ofColor::cyan, ofMap(currMw, 0.0, ofGetWidth(), 50, 255));
    ofFill();
	ofCircle(ofGetWidth()/2, ofGetHeight()/2, currMw);
//	ofRect(0, (ofGetHeight()/3.5), currMw, 59);
	ofSetColor(ofColor::black, ofMap(currMw, 0.0, ofGetWidth(), 50, 255));
	font.drawString(ofToString(meditation), 10, (ofGetHeight()/3.5) + 40); 
	
//	if (meditation >= 20.0) {
//		serial.writeByte('H');
//		bSendSerialMessage = true;
//    }
//	else if (meditation < 20.0) {
//		bSendSerialMessage = true;
//		serial.writeByte('L');
//	}
	
    ofPopStyle();
    ofPopMatrix();
    
				
    ofSetWindowTitle("fps = " + ofToString(ofGetFrameRate()));
	
	//start serial
	if(!isInitialized)
        ofDrawBitmapString("PRESS 'S' TO SEND KICKOFF BYTE", 50, (ofGetHeight()/2)+200);
    else {
        //draw out current bytesToSend values:
        for(int i=0; i<NUM_MSG_BYTES; i++){
            string whichByte = "bytesToSend["+ofToString(i)+"] = ";
            ofDrawBitmapString(whichByte + ofToString((int)bytesToSend[i]), 50, (ofGetHeight()/2)+30+30*i);
            if     (i == 0) ofDrawBitmapString("(attention)", 225, (ofGetHeight()/2)+30+30*i);
            else if(i == 1) ofDrawBitmapString("(Motor pin)", 225, (ofGetHeight()/2)+30+30*i);
            else if(i == 2) ofDrawBitmapString("(blink speed)", 225, (ofGetHeight()/2)+30+30*i);
			//else if(i == 3) ofDrawBitmapString("(Servo pin)", 225, 30+30*i);
        }
        
        //just for debug
        //ofDrawBitmapString("num messages sent total: "+ ofToString(numMsgSent), 50, ofGetHeight()/2);
       // ofDrawBitmapString("press 'a','b','c','A','B','C', LEFT, or RIGHT arrows to update values", 50, (ofGetHeight()/2)+50);
    }
	
//	for(int i=0; i<NUM_MSG_BYTES; i++){
//        string whichByte = "bytesReceived["+ofToString(i)+"] = ";
//        font.drawString(whichByte+ofToString(bytesReceived[i]), 20, 30+30*i);
//    }
//    
//    //just for debug
//    font.drawString("num messages received total: "+ ofToString(numMsgRecvd), 20, 150);
//    
//    //if we haven't received anything yet
//    if(numMsgRecvd<1)
//        font.drawString("PRESS ANY KEY TO SEND KICK OFF BYTE", 20, 180);
	
	
	//serial.writeByte('H');
	//cout << 'writeByte' << endl;
	
//	if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
//		ofSetColor(0);
//	} else {
//		ofSetColor(220);
//	}
	
	//activate to see serial connection
//	string msg;
//	msg += "click to test serial:\n";
//	msg += "nBytes read " + ofToString(nBytesRead) + "\n";
//	msg += "nTimes read " + ofToString(nTimesRead) + "\n";
//	msg += "read: " + ofToString(bytesReadString) + "\n";
//	msg += "(at time " + ofToString(readTime, 3) + ")";
//	font.drawString(msg, ofGetWidth()/1.3, 100);
	//end serial

}


//--------------------------------------------------------------
//void testApp::sendToMotor(int val){
//	if(bSendSerialMessage == true){
//		cout << "SENDING TO MOTOR: " << val << endl;
//		mySerial.writeByte(val);
//		sentMsgTime = ofGetElapsedTimeMillis();
//		bSendSerialMessage = false;
//	}
//}

//--------------------------------------------------------------
void testApp::attentionListener(float &param){
    attention = param;
    distAw = ofMap(attention, 0.0, 100.0, 0, ofGetWidth());
    atChangeTime = ofGetElapsedTimef();
}
//--------------------------------------------------------------
void testApp::meditationListener(float &param){
    meditation = param;
    distMw = ofMap(meditation, 0.0, 100.0, 0, ofGetWidth());
    meChangeTime = ofGetElapsedTimef();    
}
//--------------------------------------------------------------
void testApp::blinkListener(float &param){
//    meditation = param;
//    distMw = ofMap(meditation, 0.0, 100.0, 0, ofGetWidth());
//    meChangeTime = ofGetElapsedTimef();   
	cout << "blink: "<<param<<endl;
	blink = param;
}
//--------------------------------------------------------------


void testApp::keyPressed(int key){

//	mySerial.writeByte('A');
//    cout << "sent kickoff byte!" << endl;
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//activate for mouse click serial test¨
	bSendSerialMessage = true;
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}