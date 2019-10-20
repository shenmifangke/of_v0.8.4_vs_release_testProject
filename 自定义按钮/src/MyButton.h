#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "ofMain.h"
class MyButton
{
public:
    MyButton();
    virtual ~MyButton();
    //ofEvent<ofVec2f> clickedInside;
    ofEvent<int> clickedInside;//这里消息类型可以随便改
    void draw();
    void clear();
    bool inside(float _x, float _y );
    bool testPixel(float _x, float _y );

    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);

    ofColor color;
    int radius, x, y;
    int buttonMode;
    float scale;
    int button_width,button_height;
    void setup(vector<ofImage> img,int x,int y,int buttonMode,float scale=1.0);//(int radius, int x, int y, ofColor color);
ofImage buttonUp;
ofImage buttonMove;
ofImage buttonDown;
ofRectangle buttonArea;
ofImage current_img;
    vector<ofPoint> points;


ofFbo canvas;


protected:
    bool bRegisteredEvents;
private:
};

#endif // MYBUTTON_H
