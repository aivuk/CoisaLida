#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofPoem.h"
#include "ofxTrueTypeFontUC.h"

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	void drawText();
    void drawWord(string s, int char_size, int x, int y);
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void mouseMoved(int x, int y);
    
    void screenSetup();
    void kinectSetup();
    void kinectUpdate();
    void debugMode();
    void drawToPanels(ofFbo toDraw, bool connect = false);
    void toPanels(ofImage &canvas, ofImage &panels);
    
    void guiSetup();
    
    //gui Change events
    void kinectUpdateAreaW(int& kinectWidth);
    void kinectUpdateAreaH(int& kinectHeight);
    
    
    // blob Offset detais
    float applyOffsetX(float _x);
    float applyOffsetY(float _y);


    
	ofxKinect kinect;
    ofxSyphonServer syphonServer;
    ofxCv::ContourFinder contourFinder;


	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    //GUI
    ofxPanel gui; 

    ofParameterGroup parametersKinect;
    

    ofParameter<bool> enableMouse;
    ofParameter<int> farThreshold = 143;
    ofParameter<float> offsetX;
    ofParameter<float> offsetY;
    ofParameter<int> numMaxBlobs = 15;
    ofParameter<int> minBlobSize = 22;
    ofParameter<int> maxBlobSize = 354;

    ofxTrueTypeFontUC helvetica;
    bool running = true;
    ofVideoPlayer player;
    vector<ofPoem> poems;
    int runningPoem = 0;
    
    //options variables
    bool bDebugMode = true;
	int nearThreshold;
	
    ofFbo tempFbo;
    ofImage mCanvas, mPanels, panelsMask;
    ofImage fiespMask;
    ofRectangle mPanelPositionAndSize;


};
