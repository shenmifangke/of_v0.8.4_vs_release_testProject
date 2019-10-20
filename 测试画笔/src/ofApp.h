#pragma once

#include "ofMain.h"

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
		ofPolyline drawingLine;//鼠标绘制的线
		vector<float> speed;
		ofPoint prePoint;//上一个点
		ofPoint prePL;//上一个点的左侧法线
		ofPoint prePR;//上一个点的右侧法线
		float tempTime;
};
