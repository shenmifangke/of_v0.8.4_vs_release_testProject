#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	device_ = new ofxKinect2::Device();
	device_->setup();

	if(depth_.setup(*device_))
	{
		depth_.open();
	}

	if (color_.setup(*device_))
	{
		color_.open();
	}

	if (ir_.setup(*device_))
	{
		ir_.open();
	}
	if (body_.setup(*device_))
	{
		body_.open();
	}

}

//--------------------------------------------------------------
void ofApp::update(){
	device_->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	color_.draw(0,0,ofGetWidth(),ofGetHeight());
	depth_.draw(ofGetWidth()- 512, ofGetHeight() - 424);
	ir_.draw(ofGetWidth() - 512, ofGetHeight() - 848);
	body_.draw();
	//myBody.drawBody();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit()
{
	color_.close();
	depth_.close();
	ir_.close();
	body_.close();
	device_->exit();
	delete device_;
	device_ = NULL;
}