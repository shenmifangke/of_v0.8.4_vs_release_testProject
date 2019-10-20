#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundAuto(false);ofBackground(0);
speed.resize(0);//从第二个点开始计算 也就是鼠标移动后的第一个点

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){





}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //清除自己画的图案
    if(key == 'c')
    {
        ofBackground(0);
        drawingLine.resize(0);
        speed.resize(0);
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
    drawingLine.addVertex(ofPoint(x,y));
    float distance = prePoint.distance(ofPoint(x,y));
    float temp = ofGetElapsedTimef();
    float time = temp-tempTime;
    tempTime = temp;

    speed.push_back(distance/time);
    ofSetHexColor(0xff0000);
    drawingLine.draw();

    //ofPoint testPoint = ofPoint(100,100);
    float angle = atan2(y - prePoint.y, x - prePoint.x);//角度弧度值
    //float angle = atan2(0 - testPoint.y, 0 - testPoint.x);//角度弧度值

float tempSpeed = distance/time/200;
if(tempSpeed<300&&tempSpeed>1){//防止拐角的地方过大 或者过小
        cout<<tempSpeed<<endl;
    ofPoint pR;//将一对极坐标转换为笛卡尔点坐标
    pR.x = tempSpeed*cos(angle - PI / 2);
    pR.y = tempSpeed*sin(angle - PI / 2);
//    pR = pR+testPoint;
    pR = pR+ofPoint(x,y);

    ofPoint pL;
    pL.x = tempSpeed*cos(angle + PI / 2);
    pL.y = tempSpeed*sin(angle + PI / 2);
//    pL = pL+testPoint;
    pL = pL+ofPoint(x,y);


//    ofCircle(pR,10);
//    ofCircle(pL,10);
//自己绘制图案
	ofSetHexColor(0xb5de10);
	ofBeginShape();
	ofVertex(pL);
	ofVertex(pR);
	ofVertex(prePoint);
//		ofVertex(10,10);
//		ofVertex(50,10);
//		ofVertex(50,100);
//		ofVertex(10,100);
	ofEndShape();
}
prePoint = ofPoint(x,y);

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
prePoint = ofPoint(x,y);
tempTime = ofGetElapsedTimef();
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
