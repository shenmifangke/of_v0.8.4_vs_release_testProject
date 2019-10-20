#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
class ofApp : public ofBaseApp{

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
ofImage img;
ofMesh mesh;
ofMesh lineMesh;
vector<ofPoint> temp;
vector<ofPoint> allPoints;
        string asciiCharacters;
        ofTrueTypeFont  font;


        ofFbo fboBind;
        ofImage videoBindimg;
        ofPixels pixels;

list<ofPoint> lastMesh;//笔画的最后一小块 用来覆盖尾部

vector<ofPoint> tailMeshPoint;//用于保存拖尾的点
ofMesh tailMesh;

//绘画时跟随
    float lastTime;//上一次的时间 用于平滑时间 防止突变
    float lastSpeed;//用于记录的上一次的速度 作为平滑速度
    float tempLastSpeed;//记录lastSpeed的临时变量 /是没有反转的速度
    //绘制时参数
    ofPolyline drawingLine;//记录鼠标绘制的线
    ofPolyline drawingLineSpeed;//记录绘制速度数据 更改x用于记录点到点的时间 y用于记录速度 都是0位置是空的
ofFbo drawingLineFbo;//正在绘制的线条fbo
    //记录速度相关的量
    ofPoint prePoint;//上一个点
    ofPoint prePL;//上一个点的左侧法线
    ofPoint prePR;//上一个点的右侧法线
const float yourSpeedDrawing = 0.3;
const float yourBeginSpeed = 0.;
const float yourMaxStroke = 14.;
float designInitTime;//设计模式下初始化时间


    void _readXML();//读取xml
    void _drawXML();//读取xml
    //写入xml相关
    ofxXmlSettings XML;
    //参考线内容解析
    vector<ofPolyline> initLineArray;//参考线组 包含一幅画所有的笔触
    vector<ofPolyline> initSpeedArray;//包含所有参考线的速度组数据 这里从vector<vector<float> >修改 为x与s关系曲线 便于计算不同位置速度权重
    bool isFinishFFT;//是否完成当前画笔参考的fft计算
    int pointCount;
    bool isDesign;//是否是设计
    bool isStart;//是否开始选择
bool isUsingStrokeSpeed;//切换是否记录画笔的速度
ofFbo lineFbo;//用于绘制参考线
};
