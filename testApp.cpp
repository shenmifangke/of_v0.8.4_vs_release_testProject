#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
	kinect.initSensor();
	kinect.initColorStream();
	kinect.initIRStream();
	kinect.initDepthStream();
	kinect.initBodyIndexStream();
	kinect.initSkeletonStream();

	//simple start
	kinect.start();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;
	img.allocate(1000,1000,OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(0, 255);
	
	img.setFromPixels(kinect.getBodyIndexPixelsRef());
	img.update();
	img.draw(0,0);
	//kinect.drawSkeleton(0);
	//kinect.draw(0,0);

	//kinect v2 outputs depth/ir resolution of 512x424
	//kinect.drawDepth(0, ofGetHeight() - 424);
	//kinect.drawBodyIndex(512, ofGetHeight() - 424);
	//kinect.drawAllSkeletons(ofVec2f(640,480));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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