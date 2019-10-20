#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetBackgroundAuto(false);
ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);//always do before you draw will be right
ofDisableAntiAliasing();
ofDisableSmoothing();
    //button1.setup(100, ofRandom(ofGetWidth()-200)+100, ofRandom(ofGetHeight()-200)+100, ofColor::red);

    //button2.setup(100, ofRandom(ofGetWidth()-200)+100, ofRandom(ofGetHeight()-200)+100, ofColor::red);

//有些png是索引模式 读不出
button1_1.loadImage("button1_1.png");
button1_2.loadImage("button1_2.png");
button1_3.loadImage("button1_3.png");
button2_1.loadImage("button2_1.png");
button2_2.loadImage("button2_2.png");
button2_3.loadImage("button2_3.png");
button3_1.loadImage("button3_1.png");
button3_2.loadImage("button3_2.png");
button3_3.loadImage("button3_3.png");
button4_1.loadImage("button4_1.png");
button4_2.loadImage("button4_2.png");
button4_3.loadImage("button4_3.png");

button1pack.push_back(button1_1);
button1pack.push_back(button1_2);
button1pack.push_back(button1_3);
button2pack.push_back(button2_1);
button2pack.push_back(button2_2);
button2pack.push_back(button2_3);
button3pack.push_back(button3_1);
button3pack.push_back(button3_2);
button3pack.push_back(button3_3);
button4pack.push_back(button4_1);
button4pack.push_back(button4_2);
button4pack.push_back(button4_3);



button1.setup(button1pack,100,100,1,5);
ofAddListener(button1.clickedInside,this,&testApp::ButtonPressed);
button2.setup(button2pack,300,100,1,5);
ofAddListener(button2.clickedInside,this,&testApp::ButtonPressed2);
button3.setup(button3pack,500,100,2,5);
ofAddListener(button3.clickedInside,this,&testApp::ButtonPressed);
button4.setup(button4pack,200,350,3,4);
ofAddListener(button4.clickedInside,this,&testApp::ButtonPressed);

}
void testApp::ButtonPressed(int & e)
{

    cout<<"button in Mode: "<<e<<endl;
    switch(e)
    {
    case 1:
        cout<<"Rect Mode"<<endl;
        break;
    case 2:
        cout<<"Circle Mode"<<endl;
        break;
    case 3:
        cout<<"pixel Mode"<<endl;
        break;
    default:
        cout<<"No Mode"<<endl;
        break;
    }

}
void testApp::ButtonPressed2(int & e)
{

    cout<<"I'm Mode Two another button"<<endl;

}
//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{
    ofPushStyle();
    ofSetColor(10);
    ofDrawBitmapString("buttons with 3 Mode", 20, 20);
    ofPopStyle();
    button1.draw();
    button2.draw();
    button3.draw();
    button4.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
