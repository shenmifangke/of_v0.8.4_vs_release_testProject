#pragma once

#include "ofMain.h"
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
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void render1();
    void render2();

    int renderMode;

    float	radialNoiseCursor,noiseStep;
    float drawX,drawY;

    float tempColor;
    ofFbo canvas;
    bool autoFade;
    int fadeSpeed;// in %
    int fadeSpeed2;//in alpha
    int curveSegment;
    float noiseAmount;
    //GUI
    ofxUICanvas *gui;
    float red,green,blue,alpha;
    //render
    bool stopRendering;
    //save img
    vector<ofImage>processedImages;
    string originalFileExtension;
    ofPixels pixels;
    ofImage img;

    //for render2
    float radiusRaise;
};
