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

    float angle = atan2(y - prePoint.y, x - prePoint.x);//角度弧度值

float tempSpeed = distance/time/200;

//防止拐角的地方过大 或者过小
if(tempSpeed<300&&tempSpeed>1){
    cout<<tempSpeed<<endl;

    //将一对极坐标转换为笛卡尔点坐标
    ofPoint pR;
    pR.x = tempSpeed*cos(angle - PI / 2);
    pR.y = tempSpeed*sin(angle - PI / 2);
    pR = pR+ofPoint(x,y);

    ofPoint pL;
    pL.x = tempSpeed*cos(angle + PI / 2);
    pL.y = tempSpeed*sin(angle + PI / 2);
    pL = pL+ofPoint(x,y);


    //连接上一个点的两侧 和这一次点的两侧
	ofSetHexColor(0xb5de10);
	ofBeginShape();
	ofVertex(pL);
	ofVertex(pR);
	ofVertex(prePR);
	ofVertex(prePL);
	ofEndShape();

    //刷新之前的测点
	prePL = pL;
	prePR = pR;
}
prePoint = ofPoint(x,y);//刷新上一次的点

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
prePoint = ofPoint(x,y);
prePL = prePoint;
prePR = prePoint;
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
