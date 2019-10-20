#include "MyButton.h"

MyButton::MyButton()
{
    bRegisteredEvents = false;
}

MyButton::~MyButton()
{
    clear();
}
bool MyButton::testPixel(float _x, float _y )//采用移动进入rect区域再在鼠标移动时检测鼠标下的点的状态
{
    ofImage img = buttonUp;
    ofColor color;
    color = img.getColor((_x - x)/scale, (_y -y)/scale);
    if(!color.a)return false;
    else return true;
}
void MyButton::mouseMoved(ofMouseEventArgs & args)
{
    switch(buttonMode)
    {
    case 1://rect mode
    if(buttonArea.inside(args.x, args.y))
    {
        current_img = buttonMove;
    }
    else
    {
        current_img = buttonUp;
    }
        break;
    case 2://circle Mode
    if(inside(args.x-button_width/2, args.y-button_width/2))
    {
        current_img = buttonMove;
    }
    else
    {
        current_img = buttonUp;
    }
        break;
    case 3:
    if(buttonArea.inside(args.x, args.y)&& testPixel(args.x,args.y))
    {
        current_img = buttonMove;
    }
    else
    {
        current_img = buttonUp;
    }
        break;
    default:
        break;
    }
}
void MyButton::mouseDragged(ofMouseEventArgs & args) {}
void MyButton::mousePressed(ofMouseEventArgs & args)
{
//    if(ofInsidePoly(ofPoint(args.x, args.y),points))
//    {
//        color = ofColor::blue;
//    }
    switch(buttonMode)
    {
    case 1://rect mode
        if (buttonArea.inside(args.x, args.y))
        {
            current_img = buttonDown;
        }
        break;
    case 2:
    if(inside(args.x-button_width/2, args.y-button_width/2))
    {
        current_img = buttonDown;
    }
        break;
    case 3:
    if(buttonArea.inside(args.x, args.y)&& testPixel(args.x,args.y))
    {
        current_img = buttonDown;
    }
        break;
    default:
        break;
    }
}
void MyButton::mouseReleased(ofMouseEventArgs & args)
{
    //这里是鼠标释放 注意图案的变化不要有约束条件 有约束条件的鼠标的变化
    current_img = buttonUp;
    // if the mouse is pressed over the circle an event will be notified (broadcasted)
    // the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.

    switch(buttonMode)
    {
    case 1://rect mode
    if (buttonArea.inside(args.x, args.y))//在内部释放鼠标才是点击
    {
        //ofVec2f mousePos = ofVec2f(args.x, args.y);

        ofNotifyEvent(clickedInside, buttonMode, this);
        //ofNotifyEvent(clickedInsideGlobal, mousePos);
        //cout<<buttonMode<<endl;
    }
        break;
    case 2:
    if(inside(args.x-button_width/2, args.y-button_width/2))
    {
        ofNotifyEvent(clickedInside, buttonMode, this);
    }
        break;
    case 3:
    if(buttonArea.inside(args.x, args.y)&& testPixel(args.x,args.y))
    {
        ofNotifyEvent(clickedInside, buttonMode, this);
    }
        break;
    default:
        break;
    }
}
void MyButton::clear()
{
    if(bRegisteredEvents)
    {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        bRegisteredEvents = false;
    }
}
//this function checks if the passed arguments are inside the circle.must be circle
bool MyButton::inside(float _x, float _y )
{
    return (ofVec2f(_x, _y).distance(ofVec2f(x, y)) <button_width/2);//
}

void MyButton::draw()
{
    canvas.begin();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//这句是清空画布
    current_img.draw(0,0,buttonUp.getWidth(),buttonUp.getHeight());
    canvas.end();

    canvas.draw(x,y,button_width,button_height);
    //ofPushStyle();
//ofSetColor(color);
//ofCircle(x,y, radius);
//ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);//always do before you draw will be right  这里有个bug就是ofimage 多了会自动切换到平滑模式 所以使用offbo过度
//current_img.draw(x,y,button_width,button_height);
//    ofPopStyle();

//    ofPushStyle();
//    ofSetColor(color);
//    ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
//    ofBeginShape();
//    for(int n = 0; n<points.size(); n++)
//    {
//        ofVertex(points[n].x,points[n].y);
//    }
//    ofEndShape();
//    ofPopStyle();
}
void MyButton::setup(vector<ofImage> img,int x,int y,int buttonMode,float scale)//(int radius, int x, int y, ofColor color)
{
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->color = color;
    this->scale = scale;
    this->buttonMode = buttonMode;
    buttonUp = img[0];
    current_img = buttonUp;
    buttonMove = img[1];
    buttonDown = img[2];

    button_width = buttonUp.getWidth()*scale;
    button_height = buttonUp.getHeight()*scale;

    buttonArea.set(x,y,button_width,button_height);

    ofFbo::Settings s = ofFbo::Settings();
    s.width = buttonUp.getWidth();
    s.height = buttonUp.getHeight();
    s.internalformat = GL_RGBA;
    s.useDepth = false;
    // depthStencilAsTexture is not supported!
    s.depthStencilAsTexture = false;
    s.minFilter = GL_NEAREST;//GL_NEAREST
    s.maxFilter = GL_NEAREST;//GL_LINEAR
    canvas.allocate(s);


//    points.push_back(ofPoint(330,0));
//    points.push_back(ofPoint(300,60));
//    points.push_back(ofPoint(450,30));
//    points.push_back(ofPoint(550,100));
//    points.push_back(ofPoint(330,0));
//    ofSetHexColor(0xb5de10);
//    ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
//    ofBeginShape();
//    for(int n = 0; n<points.size(); n++)
//    {
//        ofVertex(points[n].x,points[n].y);
//    }
//    ofEndShape();
    if(!bRegisteredEvents)
    {
        ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
        bRegisteredEvents = true;
    }
}
