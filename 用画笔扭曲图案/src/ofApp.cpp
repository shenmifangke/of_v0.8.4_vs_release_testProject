#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    fboBind.allocate(700,40,GL_RGBA);
    drawingLineFbo.allocate(ofGetWidth(),ofGetHeight());
    img.loadImage("test.png");
    int height = 2;
    int width =50;
//这里是上面晃动的mesh
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x<width; x++)
        {
            mesh.addVertex(ofPoint(x*40,y*40,0)); // make a new vertex
            //mesh.addColor(ofColor(x*10,y*10,0));  // add a color at that vertex
            mesh.addTexCoord(ofPoint(x*40,y*40)); // make a new vertex
        }
    }
// now it's important to make sure that each vertex is correctly connected with the
// other vertices around it. This is done using indices, which you can set up like so:
    for (int y = 0; y<height-1; y++)
    {
        for (int x=0; x<width-1; x++)
        {
            mesh.addIndex(x+y*width);       // 0
            mesh.addIndex((x+1)+y*width);     // 1
            mesh.addIndex(x+(y+1)*width);     // 10

            mesh.addIndex((x+1)+y*width);     // 1
            mesh.addIndex((x+1)+(y+1)*width);   // 11
            mesh.addIndex(x+(y+1)*width);     // 10

        }
    }
    allPoints = mesh.getVertices();//所有顶点



//这里是拖尾mesh初始化信息
//构建初始化块
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x<2; x++)
        {
            tailMesh.addVertex(ofPoint(y*10,x*40)); // make a new vertex
            tailMesh.addTexCoord(ofPoint(y*10,x*40)); // make a new vertex
        }
    }

    int lineLength = 30;//长度
    int lineWidth = 2;//宽度
    for (int y = 0; y<lineLength-1; y++)
    {
        for (int x=0; x<lineWidth-1; x++)
        {
            tailMesh.addIndex(x+y*lineWidth);       // 0
            tailMesh.addIndex((x+1)+y*lineWidth);     // 1
            tailMesh.addIndex(x+(y+1)*lineWidth);     // 10

            tailMesh.addIndex((x+1)+y*lineWidth);     // 1
            tailMesh.addIndex((x+1)+(y+1)*lineWidth);   // 11
            tailMesh.addIndex(x+(y+1)*lineWidth);     // 10
        }
    }




    isUsingStrokeSpeed = true;
    temp.resize(allPoints.size());//临时控制点
    for(int i; i<allPoints.size(); i++)
    {
        temp[i]=allPoints[i]+ofPoint(-25,-25);
    }



    lineFbo.allocate(ofGetWidth(),ofGetHeight());
    _readXML();//读取xml信息
    _drawXML();//xml绘制

}

//--------------------------------------------------------------
void ofApp::update()
{
    //旋转点
    for(int i; i<allPoints.size(); i++)
    {
        allPoints[i].x = temp[i].x+200*(ofSignedNoise(ofGetElapsedTimef()+i/20)/5+1);
        allPoints[i].y = temp[i].y+200*(ofSignedNoise(ofGetElapsedTimef()+i%20)/5+1);
        mesh.setVertex(i,ofVec3f(allPoints[i].x,allPoints[i].y));
    }




    //更新拖尾粒子点
    for(int i; i<tailMeshPoint.size(); i++)
    {
        tailMesh.setVertex(i,tailMeshPoint[i]);
    }



}

//--------------------------------------------------------------
void ofApp::draw()
{

    fboBind.begin();
    ofPushStyle();
    ofSetHexColor(0x00ff00);
    ofRect(0,0,ofGetElapsedTimef()*50,40);
    ofPopStyle();
    fboBind.end();


    fboBind.getTextureReference().bind();//注意这里和img绑定不一样
    mesh.draw();
    lineMesh.draw();
    //tailMesh.draw();
    fboBind.getTextureReference().unbind();

img.bind();
tailMesh.draw();
img.unbind();

    ofPushStyle();
    ofSetHexColor(0xff0000);
    glPointSize(5);
    mesh.drawVertices();
    lineMesh.drawVertices();
    tailMesh.drawVertices();
//lineFbo.draw(0,0);


    ofPopStyle();

ofPushStyle();
drawingLineFbo.draw(0,0);
ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if(button == 0)
    {
        if(1)
        {
            //mouseDragged 间隔并不平滑 这里是用来平滑的
            if((ofGetElapsedTimef() - lastTime)>0.01)
            {
                int timeBetweenPoint = (ofGetElapsedTimef() - lastTime)*10000.;//只用用来记录点与点之间时间的临时变量
                if(timeBetweenPoint>9999)timeBetweenPoint=9999;//最大只为9999
                //边界处理 减1是为了防止出现100% 情况 造成保存数据多一位
                if(x>ofGetWidth()-1)x=ofGetWidth() - 1;
                else if(x<1)x = 1;
                if(y>ofGetHeight()-1)y=ofGetHeight() -1;
                else if(y<1)y = 1;

//绘画顶点坐标记录
                //删除过多的顶点//new
                tailMeshPoint.resize(60);//是drawingLine点数的两倍
                if(drawingLine.getVertices().size()>30)
                {
                    for(int i = 0; i<drawingLine.getVertices().size()-1; i++)
                    {
                        drawingLine.getVertices()[i]=drawingLine.getVertices()[i+1];
                    }
                    drawingLine.getVertices()[drawingLine.getVertices().size()-1] = ofPoint(x,y);//最后一个点赋予最新值
                }
                else drawingLine.addVertex(ofPoint(x,y));//加入新顶点
//cout<<drawingLine.getVertices().size()<<endl;
                ofPoint pR;
                ofPoint pL;
                float tempSpeed;
                float distance;
                float time;

                drawingLineFbo.begin();
                //ofClear(0,0,0,0);//new




                ofPushStyle();

//new
//for(int i = 0;i<drawingLine.getVertices().size()-;i++){
                if(isUsingStrokeSpeed)
                {
                    //计算速度
                    distance = prePoint.distance(ofPoint(x,y));

                    //if(lastTime<0.001)lastTime = 0.001;//阈值 去掉过小的 这里产生个问题 计算time的时候小误差都会算进去
                    time = ofGetElapsedTimef()-lastTime;
                    lastTime = ofGetElapsedTimef();//time计算过后 成为上一次的时长lastTime = time

                    float angle = atan2(y - prePoint.y, x - prePoint.x);//角度弧度值
                    tempSpeed = distance/time/200.;

                    //让第一次的速度变化 如果只选用了前两个点算了速度难以控制 所以使前5笔不受到计算影响 只按照普通平滑 这样开始能产生锐利的笔画
                    if(drawingLineSpeed.size()<5)
                    {
                        tempSpeed = (lastSpeed+tempSpeed)/2.;//与速度变量平滑  这句可以去掉
                        tempSpeed = (distance/time/200.);
                    }
                    else
                    {
                        tempSpeed = (lastSpeed+tempSpeed)/2.;//与速度变量平滑  这句可以去掉
                        //这里使用的方式就是让每次比上次增加的速度只增加固定的值 这样就能起到平滑的作用 缺点是一开始的速度是从0开始的 所以慢慢上升造成的问题就是开始会有一段时粗的
                        if((tempSpeed - lastSpeed)<-yourSpeedDrawing)tempSpeed = lastSpeed-yourSpeedDrawing;
                        else if((tempSpeed - lastSpeed)>yourSpeedDrawing)tempSpeed = lastSpeed+yourSpeedDrawing;
                    }




                    //防止拐角的地方过大 或者过小
                    if(tempSpeed>10.)tempSpeed= 10.;
                    if(tempSpeed<0.5)tempSpeed= 0.5;

                    tempLastSpeed = tempSpeed;//记录的是没有反转的速度

                    tempSpeed = yourMaxStroke/tempLastSpeed;//反转 速度与粗细关系



                    //将一对极坐标转换为笛卡尔点坐标

                    pR.x = tempSpeed*cos(angle - PI / 2);
                    pR.y = tempSpeed*sin(angle - PI / 2);
                    pR = pR+ofPoint(x,y);

                    pL.x = tempSpeed*cos(angle + PI / 2);
                    pL.y = tempSpeed*sin(angle + PI / 2);
                    pL = pL+ofPoint(x,y);

                    //连接上一个点的两侧 和这一次点的两侧 绘制地区
                    //ofPushStyle();
//                    ofSetHexColor(0xffffff);
ofSetColor(255,255,255,40);
                    ofBeginShape();
                    ofVertex(pL);
                    ofVertex(pR);
                    ofVertex(prePR);
                    ofVertex(prePL);
                    ofEndShape();
                    ofPopStyle();
//这种覆盖最后一格的方法要舍弃 因为透明度覆盖的时候会把前面如果有交错的也会覆盖 然后前面看起来就断了
//所以直接使用绘制全部的方法 用tailMeshPoint保存所有点
                    for(int i = 0; i<tailMeshPoint.size()-2; i+=2)
                    {
                        tailMeshPoint[i]=tailMeshPoint[i+2];
                        tailMeshPoint[i+1]=tailMeshPoint[i+3];
                    }
                    tailMeshPoint[58]= pL;
                    tailMeshPoint[59]= pR;//最后两点


//绘制线
/*
                        ofPushStyle();
                        //ofSetHexColor(0xff0000
//glEnable(GL_BLEND);
//glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_COLOR);
//glBlendEquation(GL_FUNC_ADD);//透明度覆盖模式
                        ofSetColor(255,0,0);
                        ofBeginShape();

                    for(int i = 0; i<tailMeshPoint.size(); i++)
                    {
                    ofVertex(tailMeshPoint[i]);
                    }
                        ofEndShape();
                        ofPopStyle();
*/


                }

                drawingLineFbo.end();

                if(isUsingStrokeSpeed)
                {
                    //刷新之前的测点
                    prePL = pL;
                    prePR = pR;
                    prePoint = ofPoint(x,y);//刷新上一次的点
                    lastSpeed = tempLastSpeed;//记录上一次的速度
                    drawingLineSpeed.addVertex(ofPoint(timeBetweenPoint,tempSpeed));//加入新顶点 放到后面进行记录 因为 lastSpeed 需要计算后才能得到
                }

            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if(button == 0)
    {
        if(1)
        {


            if(isUsingStrokeSpeed)
            {
                prePoint = ofPoint(x,y);
                prePL = prePoint;
                prePR = prePoint;
                tempLastSpeed = 0.;//ofGetElapsedTimef();
                lastTime = ofGetElapsedTimef();
                lastSpeed = yourBeginSpeed;//开头尖一点 默认0
            }
            //记录第一次的值
            drawingLine.addVertex(ofPoint(x,y));//加入新顶点
            designInitTime = ofGetElapsedTimef();//一笔的初始时间
            if(isUsingStrokeSpeed)drawingLineSpeed.addVertex(ofPoint(0.,0.));//第一次速度是0 时间也是0
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
void ofApp::_readXML()
{
    if( XML.loadFile("test.xml") )
    {
        cout<<"test.xml loaded!"<<endl;
    }
    else
    {
        cout<<"unable to load test.xml check data/ folder"<<endl;
    }


    vector<string> initxArray;//初始化的时候从xml中读入的原始未分割数据
    vector<string> inityArray;
    vector<string> initsArray;
    initLineArray.resize(0);
    initSpeedArray.resize(0);
    initxArray.resize(0);
    inityArray.resize(0);
    initsArray.resize(0);

    XML.pushTag("ALL");//进入根层级
//    cout<<XML.getValue("Name","noValue")<<endl;//后面一个参数是没有找到name的默认值 也决定name标签里的类型
//    cout<<XML.getValue("ScoreToEnable",-1)<<endl;
//    cout<<XML.getValue("Description","noValue")<<endl;
//    cout<<XML.getValue("DifficulyDescription",-1)<<endl;
//    cout<<XML.getValue("IsUsingStrokeSpeed",-1)<<endl;
    int numDragTags = XML.getNumTags("LineDraw");//查找LineDraw中LineDraw数量 这个不管后面多少冒号都只返回此时根目录的数量
    pointCount = 0;//总个数

    if(numDragTags <= 0)cout<<"xml error,no point found"<<endl;
    if(numDragTags > 0)
    {
        //we push into the last LineDraw tag this temporarirly treats the tag as the document root.
        XML.pushTag("LineDraw", numDragTags-1);

        //计算<LineDraw>节点数量
        int numPtTags = XML.getNumTags("PT");

        if(numPtTags > 0)
        {
            //int totalToRead = MIN(numPtTags, NUM_PTS);//限制读取数量
            int totalToRead = numPtTags;

            for(int i = 0; i < totalToRead; i++)//遍历所有linedraw节点
            {
                //the last argument of getValue can be used to specify
                //which tag out of multiple tags you are refering to.
                string x = XML.getValue("PT:X", "", i);//("LineDraw:S",0.,i);
                string y = XML.getValue("PT:Y", "", i);
                string s = XML.getValue("PT:S", "", i);

                //空数组会造成下面计算崩溃
                if(x != "")initxArray.push_back(x);
                if(y != "")inityArray.push_back(y);
                if(s != "")initsArray.push_back(s);

                pointCount++;
            }
        }
        XML.popTag();//回到all节点里
    }
    XML.popTag();//回到最外面


    //解析字符串到组中
    for(int i = 0; i<pointCount; i++) //遍历所有的点
    {
        ofPolyline initLine;//单独一笔参考线
        ofPolyline initSpeed;//单独一笔参考速度数据 与x的曲线

        for(int j = 0; j<initxArray[i].length(); j+=4) //遍历字符串x y
        {
            //转换字符串到百分比到具体值
            initLine.addVertex(ofVec2f(ofToFloat(initxArray[i].substr(j,4))/10000.*ofGetWidth(),
                                       ofToFloat(inityArray[i].substr(j,4))/10000.*ofGetHeight()
                                      ));
            if(initsArray.size()>0)initSpeed.addVertex(ofVec2f(ofToFloat(initxArray[i].substr(j,4))/10000.*ofGetWidth(),
                        ofToFloat(initsArray[i].substr(j,4))/100.
                                                                  ));
        }
        initLineArray.push_back(initLine);//总笔画组
        if(initsArray.size()>0)initSpeedArray.push_back(initSpeed);//总笔画速度组的组
        else isUsingStrokeSpeed = false;//如果没有读取到速度变量 就强制false
    }
}
void ofApp::_drawXML()
{
    //缓冲画布 载入绘制原始所有画笔图案
    lineFbo.begin();
    ofPushStyle();

    bool firstTime = true;


    //绘制遍历所有笔画
    for(int pointNum = 0; pointNum<pointCount; pointNum++)
    {
        if(initSpeedArray.size()>0)//存在速度变量的话就进行计算
        {
            ofPoint initPrePR = initLineArray[pointNum][0];//初始化计算法线计算点
            ofPoint initPrePL = initLineArray[pointNum][0];
            for(int i = 1; i<initSpeedArray[pointNum].size(); i++) //从第二个点开始
            {
                float angle = atan2(initLineArray[pointNum][i].y - initLineArray[pointNum][i-1].y,initLineArray[pointNum][i].x - initLineArray[pointNum][i-1].x);//角度弧度值

                float tempSpeed = initSpeedArray[pointNum][i].y;//注意速度从第2个点算起 注意x是x y是s


                //将一对极坐标转换为笛卡尔点坐标
                ofPoint pR;
                pR.x = tempSpeed*cos(angle - PI / 2);
                pR.y = tempSpeed*sin(angle - PI / 2);
                pR = pR+ofPoint(initLineArray[pointNum][i].x,initLineArray[pointNum][i].y);

                ofPoint pL;
                pL.x = tempSpeed*cos(angle + PI / 2);
                pL.y = tempSpeed*sin(angle + PI / 2);
                pL = pL+ofPoint(initLineArray[pointNum][i].x,initLineArray[pointNum][i].y);


                //连接上一个点的两侧 和上一次点的两侧
                ofSetHexColor(0xff0000);
                ofBeginShape();
                ofVertex(pL);
                ofVertex(pR);
                ofVertex(initPrePR);
                ofVertex(initPrePL);
                ofEndShape();



                if(pointNum==0)//第一笔
                {
                    int lintLength = initSpeedArray[pointNum].size();//线长度
                    int lineWidth = 0;//线宽度
//加入开始的两点
                    if(firstTime)
                    {
                        lineMesh.addVertex(initPrePL);//这个是需要画的点位置
                        lineMesh.addTexCoord(ofPoint(0,0));//这个是绑定图片显示的位置
                        lineMesh.addVertex(initPrePR);
                        lineMesh.addTexCoord(ofPoint(0,0));

                        firstTime = false;
                    }

                    lineMesh.addVertex(pL);
                    lineMesh.addTexCoord(ofPoint(40*i,0));
                    lineMesh.addVertex(pR);
                    lineMesh.addTexCoord(ofPoint(40*i,40));

                    lineMesh.addIndex(i*2-2);       // 0 绑定点顺序
                    lineMesh.addIndex(i*2-1);     // 1
                    lineMesh.addIndex(i*2);     // 2

                    lineMesh.addIndex(i*2-1);     // 1
                    lineMesh.addIndex(i*2+1);   // 3
                    lineMesh.addIndex(i*2);     // 2

                }




                initPrePR=pR;
                initPrePL=pL;

            }
//cout<<lineMesh.getNumVertices()<<endl;
//cout<<lineMesh.getNumTexCoords()<<endl;

        }
        //绘制中间线
        if(!isUsingStrokeSpeed)
        {
            ofSetHexColor(0xff0000);
            //绘制有粗细笔画本身
            initLineArray[pointNum].draw();
        }
    }
    ofPopStyle();
    lineFbo.end();
}
