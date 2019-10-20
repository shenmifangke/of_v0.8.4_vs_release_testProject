#include "testApp.h"
//http://ljxz-cdnres.mingchaoonline.com/map/huihun_041/x_x.jpg?v=38436
//数字自增加方式
//自动行检测功能
//stop refresh
//00x 非纵列坐标
//http://app100701490.imgcache.qzoneapp.com/app100701490/mjclient/resource/00/004/0000400114.jpg?v=62417356
//http://app100701490.imgcache.qzoneapp.com/app100701490/mjclient/resource/00/201/0020100005.jpg?v=420155556
//http://mhxjcdn.jieyou.com/q_0917/res/map/10001/map10_4.jpg?v=C012//xy颠倒
//http://app100660362.imgcache.qzoneapp.com/app100660362/old/scene/200002/tile/200002_2_3.jpg//重复数字 1为开始 下面开始数字为1变更
//检测的test注意也要同步调整  startNum 0和1之间调整
//http://jzsc1.res.ts100.com/rxcdn32/data/map/1005/nyx_01_01.jpg?1964195419
//产生01 和 00之类的问题
//http://app100678328.imgcache.qzoneapp.com/app100678328/beta/assets/mapAssets/scene/100/front/pic10_6.jpg?ver=20130926
void testApp::setup()
{
    //ofSetLogLevel(OF_LOG_VERBOSE);//输出所有操作信息
    //ofSetBackgroundAuto(false);
    loading=false;
    hitEnd = false;
    ofRegisterURLNotification(this);
    //404控制与暂停
    errorDetect = 0;

    //如果低于这个值 像素画超过了会折回
    //有最大像素量 大概比7000*7000大 错误后会返回unable to get fibitmap from ofPixels
    //也可以大于7000 貌似计算的是总像素

    startNum = 0;

    oneLinePicNum = 28;//for mode 2
    yOffset = startNum;
    picNumX = startNum;//

    picNumY =yOffset;//if not be 0 may wrong
    cutOffset = yOffset * imageWidth;//0 must to see if not be 0 may wrong
    testURLNum = 9002;//begin search Num
    searchingMode =1;//0 for url test,1 for x_x ,2 for xxx


    //text read
    // this is our buffer to stroe the text data
    ofBuffer buffer = ofBufferFromFile("url.txt");
    if(buffer.size())
    {

        // we now keep grabbing the next line
        // until we reach the end of the file
        while(buffer.isLastLine() == false)
        {

            // move on to the next line
            string line = buffer.getNextLine();

            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false)
            {
                urlInfo.push_back(line);
            }

            // print out the line
            cout << line << endl;

        }

    }
    //needToChange = "http://smys-cdnres.me4399.com/resource/img/map/";
    needToChange = urlInfo[0];
    needToChange2 = "http://app100701490.imgcache.qzoneapp.com/app100701490/mjclient/resource/00/201/00201";

    //donotNeedChange = ".jpg?551040";
    donotNeedChange = urlInfo[1];
    donotNeedChange2 = ".jpg?v=415113620";
    WIDTH = ofToInt(urlInfo[3]);//9000;//8100
    HEIGHT = ofToInt(urlInfo[4]);//6000
    imageWidth = ofToInt(urlInfo[2]);//300 240
    bigImg.allocate(WIDTH,HEIGHT,OF_IMAGE_COLOR_ALPHA);
    displayImg.allocate(WIDTH,HEIGHT);
    /////////////////////////GUI
    gui = new ofxUISuperCanvas("Window", OFX_UI_FONT_MEDIUM);

    //gui->addWidgetDown(new ofxUILabel("CUSTOM WIDGETS", OFX_UI_FONT_LARGE));

    gui->addLabel("start", OFX_UI_FONT_MEDIUM);
    gui->addToggle("startButton", false, 44, 44);
    gui->addSpacer(100, 2);
    //text input main num
    gui->addWidgetDown(new ofxUILabel("TEXT INPUT", OFX_UI_FONT_MEDIUM));

    gui->addWidgetDown(new ofxUITextInput(300, "input", "0000", OFX_UI_FONT_LARGE));

    ofxUITextInput* textinput = (ofxUITextInput *) gui->getWidget("input");
    textinput->setAutoClear(false);
    //HORIZONTAL RADIO
    vector<string> hnames;
    hnames.push_back("0");
    hnames.push_back("1");
    hnames.push_back("2");
    gui->addLabel("MODE RADIO", OFX_UI_FONT_MEDIUM);
    gui->addRadio("HR", hnames, OFX_UI_ORIENTATION_HORIZONTAL);

    //text input offset num
    gui->addWidgetDown(new ofxUILabel("Input Offset Y", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUITextInput(300, "inputOffset", "00", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUILabel("fix X,Y should not hit Offset Y", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUITextInput(300, "inputOffset_X", "00", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUITextInput(300, "inputOffset_Y", "00", OFX_UI_FONT_LARGE));


    ofxUITextInput* textinput2 = (ofxUITextInput *) gui->getWidget("inputOffset");
    textinput2->setAutoClear(false);


    //GUI
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    //gui->loadSettings("GUI/guiSettings.xml");//会造成直接初始化 自带问题
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
//cout<<"00000000000000000000000000000"<<endl;
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    //cout << name << endl;
    if(name == "startButton")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        //cout << "value: " << button->getValue() << endl;
        if(button->getValue())
        {
            img.clear();
            doNext();
            loading =true;
            hitEnd = false;
        }
        else
        {
            loading=false;
            hitEnd = true;
            cout<<"Searching is stopped"<<endl;
        }
    }
    if(name == "input")
    {
        ofxUITextInput *input = (ofxUITextInput *) e.widget;
        processingNum = ofToString(input->getTextString());
        testURLNum = ofToInt(input->getTextString());
        cout<<testURLNum<<endl;
        cout<<(input->getTextString())<<endl;
    }
    if(name == "inputOffset")
    {
        ofxUITextInput *input = (ofxUITextInput *) e.widget;
        yOffset = ofToInt(input->getTextString());

        picNumY =yOffset;//if not be 0 may wrong
        int temp = imageWidth;
        cutOffset = yOffset * temp;//0 must to see if not be 0 may wrong

    }
    if(name == "inputOffset_X")
    {
        ofxUITextInput *input = (ofxUITextInput *) e.widget;
        int xOffset = ofToInt(input->getTextString());

        picNumX =xOffset;
        loading=false;
        hitEnd = true;
        errorDetect = 0;
        img.clear();

    }
    if(name == "inputOffset_Y")
    {
        ofxUITextInput *input = (ofxUITextInput *) e.widget;
        int yOffset = ofToInt(input->getTextString());

        picNumY =yOffset;
        loading=false;
        hitEnd = true;
        errorDetect = 0;
        img.clear();

    }
    if(name == "0" ||name == "1"||name == "2")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        //cout << name << "\t value: " << toggle->getValue() << endl;
        //string tempStr = toggle->getValue();

        loading=false;
        hitEnd = true;
        picNumY =yOffset;
        picNumX = startNum;
        img.clear();
        errorDetect = 0;
        displayImg.begin();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//这两句是清空画布
        displayImg.end();
        if(name=="2")searchingMode = 2;
        if(name=="1")searchingMode = 1;
        if(name=="0")searchingMode = 0;
        bigImg.clear();//清理图片
        bigImg.allocate(WIDTH,HEIGHT,OF_IMAGE_COLOR_ALPHA);
        cout<<"mode has been reset to  : "<<name<<endl;

    }

}
//--------------------------------------------------------------
void testApp::urlResponse(ofHttpResponse & response)
{
    if(hitEnd)return;
    if(response.status==200)
    {
        img.loadImage(response.data);
        loading=false;
        errorDetect=0;
        if(searchingMode == 0){cout<<"find"<<endl;hitEnd = true;}
    }
    else
    {

        switch(searchingMode)
        {
        case 0:
            testURLNum ++;
            cout<<"test URL num is: ";
            cout<<ofToString(testURLNum)<<endl;
            loading = true;
            doNext();
            break;
        case 1:
            cout << response.status << " " << response.error << endl;
            cout << "trying to get next line" <<endl;
            picNumY++;
            picNumX = startNum;

            loading = true;
            errorDetect ++;
            //cout<<"error detect = "+ofToString(errorDetect)<<endl;
            if(errorDetect > 1 && response.status!=-1)
            {
                loading=false;
                cout<<"finish!"<<endl;
            }
            else doNext();
            break;

        case 2:
            cout << response.status << " " << response.error << endl;
            cout << "trying to get next pic" <<endl;
            picNumX ++;
            loading = true;
            errorDetect = 2;
            //cout<<"error detect = "+ofToString(errorDetect)<<endl;
            if(errorDetect > 1 && response.status!=-1)
            {
                loading=false;
                cout<<"finish!"<<endl;
            }
            else doNext();
            break;
        default :
            break;
        }

    }
}

//--------------------------------------------------------------
void testApp::update()
{
//    if(isStop)
//    {
//        loading=false;
//        hitEnd = true;
//    }
//    else
//    {
//        loading=true;
//        hitEnd = false;
//    }
}

//--------------------------------------------------------------
void testApp::draw()
{

    if(loading)
        ofDrawBitmapString("loading...", 10, ofGetHeight()+20);

    if(img.bAllocated() && !hitEnd)
    {

        ofSetColor(255);//without this  will be dark

        displayImg.begin();
        ofPushMatrix();
        //height and with change main set the block in other place
        ofTranslate(picNumX*imageWidth - imageWidth,picNumY*imageWidth);
        img.draw(0, 0 -cutOffset,img.getWidth(),img.getHeight());
        ofPushStyle();

        if(picNumX%10==0||(picNumY+1)%10==0){
        ofSetHexColor(0xff0000);
        }
        else if(picNumX%5==0||(picNumY+1)%5==0){
        ofSetHexColor(0xffbbbb);
        }else {
        ofSetHexColor(0xffffff);
        }
        ofRect(0,0 - cutOffset,25,imageWidth);//小了会看不到
        ofRect(0,0 - cutOffset,imageWidth,25);
        ofPopStyle();
        ofPopMatrix();
        displayImg.end();

        doNext();
    }
    displayImg.draw(0,0,WIDTH/10,HEIGHT/10);//to display more
    ofPushStyle();
    ofSetColor(0);
    ofDrawBitmapString("hit spacebar to load image from web", 10, ofGetHeight()/2-50);
    ofDrawBitmapString("hit s key to save image", 10, ofGetHeight()/2-25);
    ofDrawBitmapString("hit e key to stop", 10, ofGetHeight()/2);
    ofDrawBitmapString("hit r key to restart(need hit e first)", 10, ofGetHeight()/2+25);

    switch(searchingMode)
    {
    case 1:
        ofDrawBitmapString("now downloading : "+needToChange+processingNum, 10, ofGetHeight()/2+50);
        break;
    case 2:
        ofDrawBitmapString("now downloading : "+needToChange2, 10, ofGetHeight()/2+50);
        break;
    default:
        break;
    }
    ofDrawBitmapString("test num is " + ofToString(testURLNum), 10, ofGetHeight()/2+75);

    ofPopStyle();
}
//
void testApp::doNext()
{
    //这里处理的实际上是上一次的图片
    int oldX = picNumX;
    int oldY = picNumY;

    switch(searchingMode)
    {
    case 1:
        if(picNumX == startNum && picNumY == startNum)
        {
            picNumX = startNum;
        }
        else if(picNumX == startNum)
        {
            picNumX = WIDTH/imageWidth-1;//8000/imageWidth
            picNumY --;
        }
        else picNumX -- ;
        break;
    case 2:
        if(picNumX == startNum)
        {
            picNumX = oneLinePicNum-1;//8000/imageWidth
            picNumY --;
        }
        else
            picNumX -- ;
        break;
    default:
        break;

    }
    cout<<"begindraw"<<endl;
    //cout<<picNumX<<endl;
    //cout<<picNumY<<endl;
    if(img.isAllocated())
    {
        int largeEdge;//the place will be decided by the large edge because pic isn't has pixels in small edge
        //if(img.height > img.width)largeEdge = img.height;
        //else largeEdge=img.width;
        largeEdge = imageWidth;

        for (int y = 0; y < img.getHeight(); y++)
        {
            for (int x = 0; x < img.getWidth(); x++)
            {
                bigImg.setColor(x+largeEdge*picNumX, y+largeEdge*picNumY - cutOffset,img.getColor(x,y));
            }
        }

    }
    cout<<"enddraw"<<endl;
    //处理完后 还原数字
    picNumX = oldX;
    picNumY = oldY;


    switch(searchingMode)
    {
    case 0:
        cout<<"Now testing"<<endl;//37
        //ofLoadURLAsync("http://smys-cdnres.me4399.com/resource/img/map/"+ofToString(testURLNum)+"/0_0.jpg?551040");
        //ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/map"+ofToString(picNumX)+"_"+ofToString(picNumY)+donotNeedChange);
        //ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/nyx_"+"_"+ofToString(picNumX)+"_"+ofToString(picNumY)+donotNeedChange);
        //not finish ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/nyx_"+ofToString(picNumY,2,'0')+"_"+ofToString(picNumX,2,'0')+donotNeedChange);
        ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/front/pic"+ofToString(picNumX)+"_"+ofToString(picNumY)+donotNeedChange);
        //cout<<(needToChange+ofToString(testURLNum)+"/nyx_"+ofToString(picNumX,2,'0')+"_"+ofToString(picNumY,2,'0')+donotNeedChange)<<endl;
        break;
    case 1:
        //x_x
        //可能会x y颠倒
        //ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/nyx_"+ofToString(picNumY,2,'0')+"_"+ofToString(picNumX,2,'0')+donotNeedChange);
        ofLoadURLAsync(needToChange+ofToString(testURLNum)+"/front/pic"+ofToString(picNumX)+"_"+ofToString(picNumY)+donotNeedChange);
        //xxx 00286
        //picNum++;
        picNumX ++;
        if(picNumX ==WIDTH/imageWidth)
        {
            //picNumY++;
            picNumX = startNum;
            cout<<"image is too wide"<<endl;
            hitEnd = true;
        }
        if(picNumY ==HEIGHT/imageWidth && !cutOffset)hitEnd = true;//高度过高 并且不是偏移拼接的时候才是停止 默认如果有拼接数值的话就不会超过了

        cout<<"picNumX = " + ofToString(picNumX)<<endl;
        cout<<"picNumY = " + ofToString(picNumY)<<endl;
        break;

    case 2:
    {
        //如果在case中声明 必须用大括号包裹
        string temp;
        temp = needToChange2+ofToString(picNumY*oneLinePicNum+picNumX,5,'0')+donotNeedChange;
        ofLoadURLAsync(temp);
        cout<<temp<<endl;
    }

    picNumX ++;
    if(picNumX == oneLinePicNum)
    {
        picNumX = startNum;
        picNumY ++;
    }
    if((imageWidth*picNumY + imageWidth)>HEIGHT)
    {
        hitEnd = true;
        cout<<"mission break because hit the end of ofImage!!!";
    }
    break;
    default:
        break;
    }
    img.clear();
    loading=false;
}
//--------------------------------------------------------------
void testApp::exit()
{
    ofUnregisterURLNotification(this);
    //gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(key == ' ')
    {
        img.clear();
        doNext();
        loading =true;
    }
    if (key == 's')
    {
        // bigImg.grabScreen(0,0,1000,1000);

        processedImages.clear();
        processedImages.push_back(bigImg);
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

        //Store the processed image

    }
    if (key == 'e')
    {
        loading=false;
        hitEnd = true;
        cout<<"Searching is stopped"<<endl;
    }
    if (key == 'r')
    {
        loading=false;
        hitEnd = false;
        picNumY =startNum;
        picNumX = startNum;
        img.clear();
        errorDetect = 0;
        displayImg.begin();
        //glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//这两句是清空画布
        displayImg.end();
        doNext();
        cout<<"Searching has be refreshed"<<endl;
    }
    if(key == 't')
    {
        cout<<"test begin!"<<endl;
        doNext();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
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
    displayImg.draw(0,0);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
