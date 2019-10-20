#pragma once

#include "ofMain.h"
#include "ofURLFileLoader.h"
#include "ofxUI.h"
class testApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
void guiEvent(ofxUIEventArgs &e);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void urlResponse(ofHttpResponse & response);

    ofImage img;
    bool loading;
    void doNext();
    bool hasNext;
    bool hitEnd;
    int errorDetect;//自动行检测功能
    int picNumX;
    int picNumY;
    int yOffset;//for Insufficient height
    int testURLNum;
    int imageWidth;

    int oneLinePicNum;//一行中区块个数 用于分析第二个
    string needToChange;
    string needToChange2;

    string donotNeedChange;
    string donotNeedChange2;
    ofFbo displayImg;
    ofImage bigImg;

    int searchingMode;
    int WIDTH;
    int HEIGHT;
    int cutOffset;//当高度大小不合适时候 将进行分割（以宽度手动调整）
    //save img
    vector<ofImage>processedImages;
    string originalFileExtension;
    //GUI
	ofxUICanvas *gui;
string processingNum;
//text read
vector <string> urlInfo;
int startNum;//开始的数值 有的是0 有的是 1开始计数

};
