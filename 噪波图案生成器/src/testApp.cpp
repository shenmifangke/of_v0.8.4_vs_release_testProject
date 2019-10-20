#include "testApp.h"

//--------------------------------------------------------------
//如果保存有底色 那是渐隐叠加造成 可以把叠加关了 清空再重新渲染
void testApp::setup()
{
    ofSetWindowTitle("noise example");
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetCircleResolution(256);
    //ofSetBackgroundAuto(false);
    ofSetFrameRate(60);
    //ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    autoFade = true;
    fadeSpeed = 5;
    fadeSpeed2 = 1;
    radialNoiseCursor= 0.0;
    noiseStep = 0.002;
    curveSegment = 5;
    noiseAmount = 500.0;

    drawX = drawY = 0.0;
    //for render2
    radiusRaise = 10.0;
    //render
    stopRendering = false;
    renderMode = 1;

    tempColor = 0.0;
    canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    img.allocate(ofGetWidth(),ofGetHeight(),OF_IMAGE_COLOR_ALPHA);//OF_IMAGE_COLOR_ALPHA
    /////////////////////////GUI
    gui = new ofxUISuperCanvas("SuperWindow", OFX_UI_FONT_MEDIUM);
    gui->setColorBack(ofColor(100,30,100,200));
    gui->setDrawBack(true);
    //gui->addWidgetDown(new ofxUILabel("CUSTOM WIDGETS", OFX_UI_FONT_LARGE));
    red = 0;
    green = 0;
    blue = 0;
    alpha = 255;

    gui->addSpacer(100, 2);
    gui->addFPSSlider("FPS SLIDER", 200, 30, 200);
    gui->addLabel("curves control", OFX_UI_FONT_MEDIUM);
    gui->addSlider("noiseStep",0,0.01,noiseStep,300,20);

    gui->addLabel("auto fade out", OFX_UI_FONT_MEDIUM);
    gui->addToggle("autoFadeOut", true, 44, 44);
    gui->addSlider("fadeSpeed",1,10,fadeSpeed,300,20);
    gui->addSlider("curveSegment",1,100,curveSegment,300,20);
    gui->addSlider("noiseAmount",1,1000,noiseAmount,300,20);

    gui->addLabel("render control", OFX_UI_FONT_MEDIUM);
	gui->addButton("clear", false, 22, 22);
	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);//改成往右排
	gui->addToggle("beRender", stopRendering, 44, 44);
    gui->addButton("save",false, 44, 44);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN); //改成往下排
    //select witch to render
    gui->addSpacer();
    vector<string> hnames; hnames.push_back("mode1"); hnames.push_back("mode2"); hnames.push_back("mode3");
	gui->addLabel("RENDER MODE", OFX_UI_FONT_MEDIUM);
	gui->addRadio("HR", hnames, OFX_UI_ORIENTATION_HORIZONTAL);

// UI color set
//    gui->addLabel("window properties control", OFX_UI_FONT_MEDIUM);
//    gui->addSlider("red",0,255,red,200,20);
//    gui->addSlider("green",0,255,green,200,20);
//    gui->addSlider("blue",0,255,blue,200,20);
//    gui->addSlider("alpha",0,255,alpha,200,20);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::update()
{

}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();

    if(name == "red")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        red = slider->getValue();
    }
    else if(name == "blue")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        blue = slider->getValue();
    }
    else if(name == "green")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        green = slider->getValue();
    }
    else if(name == "alpha")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        alpha = slider->getValue();
    }
    else if(name == "noiseStep")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        noiseStep = slider->getValue()/100;
    }
    else if(name == "fadeSpeed")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float temp = ofMap(slider->getValue(),0,1,0.1,10);
        if(temp>=1)
        {
            fadeSpeed = int(temp);
            fadeSpeed2 = 1;
        }
        else
        {
            fadeSpeed = 1;
            fadeSpeed2 = 10 - int(temp *10);
        }

        //cout<<fadeSpeed<<" "<<fadeSpeed2<<endl;
    }
    else if(name == "curveSegment")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        curveSegment = int(slider->getValue()*100+1);
    }
    else if(name == "noiseAmount")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        noiseAmount = ofMap(slider->getValue(),0,1,0,1000);
    }
    else if(name == "autoFadeOut")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue())
        {
            autoFade = true;
        }
        else
        {
            autoFade = false;
        }
    }
    else if(name == "save")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue()){
        cout<<"now save image"<<endl;


        ofFbo tempFBO;
        tempFBO.allocate(canvas.getWidth(),canvas.getHeight(),GL_RGBA);
        tempFBO.begin();
        canvas.draw(0,0);
        tempFBO.end();

        tempFBO.readToPixels(pixels);//pixels 不支持GL_RGBA32F_ARB 所以重新拿fbo绘制
        img.setFromPixels(pixels);

        processedImages.clear();
        processedImages.push_back(img);
        if (processedImages.size()==0)
        {
            //User is trying to save without anything to output - bail
            return;
        }

        //
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower(originalFileExtension), "Save your file");
        if (saveFileResult.bSuccess)
        {
            processedImages[0].saveImage(saveFileResult.filePath);
        }
        }
    }
    else if(name == "beRender")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue())
        {
            stopRendering = true;
        }
        else
        {
            stopRendering = false;
        }
    }
    else if(name == "clear")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue())
        {
            canvas.begin();
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            canvas.end();
        }
    }
    else if(name == "mode1")
    {
        renderMode = 1;
        cout<<"Mode switch to "<<renderMode<<endl;
    }    else if(name == "mode2")
    {
        renderMode = 2;
        cout<<"Mode switch to "<<renderMode<<endl;
    }    else if(name == "mode3")
    {
        renderMode = 3;
        cout<<"Mode switch to "<<renderMode<<endl;
    }



    //一开始的alpha为255所以*255后很大超出了255就变为0了 所以在此纠正 其他三张颜色一开始为0所以没有限制
    if(alpha>1)alpha =1;
    //设置颜色
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(red*255,green*255,blue*255,alpha*255));
}
//--------------------------------------------------------------
void testApp::exit()
{
    delete gui;
}
//--------------------------------------------------------------
void testApp::draw()
{
    if(stopRendering){
ofxUIButton *button = (ofxUIButton*)(gui->getWidget("autoFadeOut"));
button->setValue(false);
autoFade = false;
    }
    else {
        switch(renderMode)
        {
            case 1:render1();break;
            case 2:render2();break;
            case 3:render1();break;
            default:break;
        }


    }

    canvas.draw(0,0);

}
void testApp::render2()
{
	float centerX = ofGetWidth()/2;
	float centerY = ofGetHeight()/2;
	canvas.begin();


	ofPushStyle();
	ofSetLineWidth(2);
	ofPushMatrix();
	ofTranslate(centerX,centerY,0);
	ofSetHexColor(0x00ff00);
	ofNoFill();
    ofColor c = ofColor::fromHsb( tempColor, 255, 200 );
    ofSetColor(c,230);
    ofSetLineWidth(1);
    if(tempColor<255)tempColor+=0.03;
    else tempColor = 0;

	ofBeginShape();
	ofCurveVertex(0,0 - 100);
float angle = 0;
radialNoiseCursor +=noiseStep;
    for( int i=0; i<=curveSegment; i++ )
    {
        drawY = noiseAmount*ofNoise(radialNoiseCursor +20*i);
        angle = i*(360.0/curveSegment)/180.0*PI;
        ofCurveVertex((drawY+radiusRaise)*cos(angle),(drawY+radiusRaise)*sin(angle));
    }
    radiusRaise +=0.1;
    ofCurveVertex(0,0 + 100);
    ofEndShape();
	ofPopMatrix();
	ofPopStyle();

	canvas.end();
}
void testApp::render1()
{
    canvas.begin();
    //auto fade control
    if(autoFade && ofGetFrameNum()%fadeSpeed == 0)
    {
        ofPushStyle();
        ofSetColor(0,0,0,fadeSpeed2);
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofPopStyle();
    }
    ofPushStyle();
    ofPushMatrix();
    ofColor c = ofColor::fromHsb( tempColor, 255, 200 );
    ofSetColor(c,230);
    ofSetLineWidth(1);
    if(tempColor<255)tempColor+=0.1;
    else tempColor = 0;
    ofTranslate(0,ofGetHeight()/2);
    ofNoFill();
    ofBeginShape();
    ofCurveVertex(-10,0);
    radialNoiseCursor +=noiseStep;
    for( int i=0; i<=curveSegment; i++ )
    {
        drawY = noiseAmount*ofSignedNoise(radialNoiseCursor +20*i);//just to separate each other  乘数过小会产生规律性
        float xx = i * ofGetWidth() / curveSegment;
        float yy = drawY;
        ofCurveVertex(xx,yy);
    }
    ofCurveVertex(ofGetWidth()+10,0);
    ofEndShape();


    ofPopMatrix();
    ofPopStyle();
    canvas.end();
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
//if(button == 0){ofEnableAntiAliasing();cout<<"ofEnableAntiAliasing"<<endl;}
//if(button == 2){ofDisableAntiAliasing();cout<<"ofDisableAntiAliasing"<<endl;}
//if(button == 0){ofEnableSmoothing();cout<<"ofEnableSmoothing"<<endl;}
//if(button == 2){ofDisableSmoothing();cout<<"ofDisableSmoothing"<<endl;}
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
