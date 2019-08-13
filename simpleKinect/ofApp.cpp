#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();

	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();

	ofSetWindowShape(640 * 2, 480 * 2);



}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	mesh = kinect.getDepthSource()->getMesh(true, ofxKinectForWindows2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);
}

//--------------------------------------------------------------
void ofApp::draw(){



	cam.begin();
	ofPushMatrix();
	ofScale(300,300,300);
		//setup some point cloud properties for kicks
		/*glPushAttrib(GL_POINT_BIT);
		glPointSize(5.0f);
		glEnable(GL_POINT_SMOOTH);

		ofPushStyle();*/
	ofSetColor(255,0,0);
		//bind kinect color camera texture and draw mesh from depth (which has texture coordinates)
		this->kinect.getColorSource()->getTextureReference().bind();

		//draw point cloud
		this->mesh.drawVertices();
		//if(mesh.getNumVertices()>0)cout<<mesh.getVerticesPointer()[12000].x<<endl;
		/*
		//draw triangles
		ofSetColor(255, 150);
		this->mesh.drawWireframe();
		
		//draw fills faded
		ofSetColor(255, 50);
		this->mesh.drawFaces();

		//unbind colour camera
		this->kinect.getColorSource()->getTextureReference().unbind();

		ofPopStyle();

		//clear the point cloud drawing attributes
		glPopAttrib();

		//draw the view cones of depth and colour cameras
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofSetColor(100, 200, 100);
		this->kinect.getDepthSource()->drawFrustum();
		ofSetColor(200, 100, 100);
		this->kinect.getColorSource()->drawFrustum();
		ofPopStyle();
		*/
	ofPopMatrix();
	cam.end();
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
