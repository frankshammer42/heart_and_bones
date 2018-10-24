#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	hb_sound.load("sound/trimmed.wav");
	hb_sound.setVolume(0.75f);
	overall_switch_counter = 0;
	int num = 1500;
	incoming_hb = false;
	p.assign(num, heart_particle());
	//currentMode = PARTICLE_MODE_ATTRACT;
	currentModeStr = "IDDDDDDDLEEEEEEEEEEE"; 
	resetParticles();
	mySerial.setup("/dev/cu.usbmodem1421", 115200);


}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the forth demo 
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].change_status(incoming_hb);		
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
	int myByte = 0;
	myByte = mySerial.readByte();
	if ( myByte == OF_SERIAL_NO_DATA ){
          //printf("no data yet\n");
	}
	else if ( myByte == OF_SERIAL_ERROR ){
  		printf("an error occurred\n");
	}
	else{
        printf("Data is %d\n", myByte);
		if (myByte == 1) {
			incoming_hb = true;
		}
		else if (myByte == 0){
			incoming_hb = false;
		}
	}

	if (incoming_hb){
		currentModeStr = "Got Heart Beat Transferring!!"; 
		if (overall_switch_counter > 95){
			hb_sound.play();
			hb_sound.setSpeed(0.9);
			overall_switch_counter = 0;
		}
	}
	else{
		currentModeStr = "Listeing Heartbeats"; 
	}
	
	//if (myByte == 250){
		//currentModeStr = "Listeing Heartbeats"; 
	//}
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].change_status(incoming_hb);
		p[i].update();
	}
	overall_switch_counter++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	ofSetColor(100);	
	ofDrawBitmapString(currentModeStr, 150, ofGetHeight() - 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1'){
		incoming_hb = false;
	}
	if( key == '2'){
		incoming_hb = true;
	}
	if( key == ' ' ){
		resetParticles();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

