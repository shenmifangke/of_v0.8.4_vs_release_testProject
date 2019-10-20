#pragma once

#include "ofMain.h"
#include "MyButton.h"
class testApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


    MyButton button1;
    MyButton button2;
    MyButton button3;
    MyButton button4;


    ofImage button1_1;
    ofImage button1_2;
    ofImage button1_3;
    ofImage button2_1;
    ofImage button2_2;
    ofImage button2_3;
    ofImage button3_1;
    ofImage button3_2;
    ofImage button3_3;
    ofImage button4_1;
    ofImage button4_2;
    ofImage button4_3;

    vector<ofImage> button1pack;
    vector<ofImage> button2pack;
    vector<ofImage> button3pack;
    vector<ofImage> button4pack;

    void ButtonPressed(int & e);
    void ButtonPressed2(int & e);

};
