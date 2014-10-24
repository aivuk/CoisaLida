#include "ofApp.h"
#include "ofxSyphon.h"
#include "ofxTrueTypeFontUC.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
    ofTrueTypeFont::setGlobalDpi(72);
    
    
    screenSetup(); //screen and some OF setups
    kinectSetup(); //kinetic setup
    guiSetup(); //GUI Setup
    
    
    mPanelPositionAndSize = ofRectangle(37,259, 215, 168);
    mCanvas.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.setColor(ofColor(0));
    panelsMask.loadImage("SP_Urban_MASK_025.png");
    panelsMask.crop(mPanelPositionAndSize.x, mPanelPositionAndSize.y, mPanelPositionAndSize.width, mPanelPositionAndSize.height);
    fiespMask.loadImage("SP_Urban_MASK_025.png");

	helvetica.loadFont("HelveticaNeueLTStd-Hv.otf", 14);

    player.loadMovie("aviao2.mp4");
   // player.play();

    tempFbo.allocate(mCanvas.width, mCanvas.height);
    
    // Create poems
    string poem = "O ESSENCIAL É SABER VER SABER VER SEM ESTAR A PENSAR SABER VER QUANDO SE VÊ E NEM PENSAR QUANDO SE VÊ NEM VER QUANDO SE PENSA";
    ofPoem newPoem;
    newPoem.setup(poem);
    poems.push_back(newPoem);
    cout << poems[0].text[0];
}

//--------------------------------------------------------------
void ofApp::update() {

    ofEnableAlphaBlending();
	ofBackground(255, 255, 255);
    kinectUpdate();
    
    if (running) {
        poems[0].update();
        player.update();
    }

}


//--------------------------------------------------------------
void ofApp::draw() {
    
    ofBackground(0);
    ofSetColor(255);
    
    mCanvas.draw(400,14);
    mPanels.draw(400,259);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10,10, ofColor(255,0,255), ofColor(255,255,0));
    
    fiespMask.draw(0,0);
    //mPanels.draw(mPanelPositionAndSize.x, mPanelPositionAndSize.y);
    
    /*ofPushStyle();
    ofSetColor(255,0,0);
    ofRect(mPanelPositionAndSize.x, mPanelPositionAndSize.y, mPanelPositionAndSize.width, mPanelPositionAndSize.height);
    ofPopStyle();*/
    
    if (bDebugMode)
        debugMode();

    //player.draw(37, 259);
    
    /*
    tempFbo.begin();
        ofBackground(0);
        player.draw(0, 0, mCanvas.getWidth(), mCanvas.getHeight());
    tempFbo.end();
    tempFbo.readToPixels(mCanvas.getPixelsRef());

    mCanvas.reloadTexture();
    toPanels(mCanvas, mPanels); */
    
    poems[0].drawText();

    syphonServer.publishScreen(); //syphon screen

}

void ofApp::drawText() {
    
}

///DEBUG-MODE

void ofApp::debugMode(){

    ofPushMatrix();

        ofTranslate(400,400);
        ofScale(300.0/kinect.width,200.0/kinect.height);

        grayImage.draw(0,0);

            ofPushMatrix();
                ofTranslate(kinect.width/2,kinect.height/2);
                ofPushStyle();
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofSetColor(255,0,0,100);
                ofFill();
                ofRect(0,0, kinect.width, kinect.height);

                ofPopStyle();
            ofPopMatrix();
        contourFinder.draw();

    ofPushStyle();

    ofPopStyle();

    ofPopMatrix();

    
    //loop through all blobs detected and draw the centroid and lables

    RectTracker& tracker = contourFinder.getTracker();
    
    for(int i=0; i < contourFinder.size(); i++){
        unsigned int label = contourFinder.getLabel(i);

        if(tracker.existsPrevious(label)) {

            ofPoint center = toOf(contourFinder.getCenter(i));
            ofPushStyle();
            ofSetColor(255,0,0);
            ofFill();
            ofPushMatrix();
                ofTranslate(400,400);
                ofScale(300.0/kinect.width,200.0/kinect.height);
                ofEllipse(center.x,center.y,10,10);
                string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                ofDrawBitmapString(msg,center.x,center.y);
                ofVec2f velocity = toOf(contourFinder.getVelocity(i));
                ofPushMatrix();
                    ofTranslate(center.x, center.y);
                    ofScale(10, 10);
                    ofLine(0, 0, velocity.x, velocity.y);
                    ofPopMatrix();
                ofPopMatrix();
            ofPopMatrix();
            ofPopStyle();
        }

    }

    // draw instructions
    ofPushStyle();

    ofSetColor(255, 255, 255);
    stringstream reportStream;

    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
        << "motor / led / accel controls are not currently supported" << endl << endl;
    }

   reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
    << "set near threshold " << nearThreshold << " (press: + -)" << endl
    << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.getContours().size()
    << ", fps: " << ofGetFrameRate() << endl
    << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;


    ofDrawBitmapString(reportStream.str(), 20, 652);
    ofPopStyle();
    gui.draw();

    
    ofFbo tempFbo;
    tempFbo.allocate(mCanvas.width, mCanvas.height);
    tempFbo.begin();
        ofBackground(0);
        //ofSetColor(20,20,200);
        //ofFill();
        //ofRect(ofGetMouseX(),ofGetMouseY(),100,100);
        grayImage.draw(0,0,mCanvas.width,mCanvas.height);
    tempFbo.end();
    tempFbo.readToPixels(mCanvas.getPixelsRef());

    mCanvas.reloadTexture();
    toPanels(mCanvas, mPanels);


}

void ofApp::toPanels(ofImage &canvas, ofImage &panels){
    if((canvas.getWidth() < panels.getWidth()) || (canvas.getHeight() < panels.getHeight()))
        return;
    
    for(int y=0; y<panels.getHeight(); y++){
        int leftOffset=0, rightOffset=0;
        int gapSize = max(0, (int)((0.0-74.0)/mPanels.getHeight()*y+72.0));
        
        for(int x=0; x<=panels.getWidth()/2; x++){
            // left
            if(panelsMask.getColor(panels.width/2-x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2-x, y, canvas.getColor(canvas.getWidth()/2-x+(leftOffset*gapSize), y));
            }
            else{
                leftOffset = 1;
            }
            
            // right
            if(panelsMask.getColor(panels.width/2+x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2+x, y, canvas.getColor(canvas.getWidth()/2+x-(rightOffset*gapSize), y));
            }
            else{
                rightOffset = 1;
            }
        }
    }
    panels.reloadTexture();
}


//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

void ofApp::screenSetup(){

    syphonServer.setName("kinectTracking");

    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

}

void ofApp::kinectUpdate(){

    kinect.update();

    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {

        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);


        // update the cv images
        grayImage.flagImageChanged();

        contourFinder.setMinAreaRadius(minBlobSize);
        contourFinder.setMaxAreaRadius(maxBlobSize);
        contourFinder.findContours(grayImage);

    }

}
void ofApp::kinectSetup(){

    // enable depth.video image calibration
    kinect.setRegistration(true);

    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)

    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }

    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    ofSetFrameRate(60);
    // zero the tilt on startup
    kinect.setCameraTiltAngle(0);
    nearThreshold = 255;

    //blob tracking system parameter
    contourFinder.getTracker().setPersistence(10);
    contourFinder.getTracker().setMaximumDistance(128);
    
}



void ofApp::guiSetup(){



    gui.setup("Settings", "settings.xml", 310,100);

    gui.add(enableMouse.set("Mouse DEBUG",true));
    parametersKinect.setName("Kinect");


    parametersKinect.add(farThreshold.set("Far Threshold", 0,0, 255 ));
    parametersKinect.add(numMaxBlobs.set("Num Max Blos",10,0,15));
    parametersKinect.add(maxBlobSize.set("max Blob Size",0,0,500));
    parametersKinect.add(minBlobSize.set("min Blob Size",0,0,500));

    parametersKinect.add(offsetX.set("Offset X", 0,-200, 200 ));
    parametersKinect.add(offsetY.set("Offset Y", 0,-200, 200 ));



    gui.add(parametersKinect);



    gui.minimizeAll();

    gui.loadFromFile("settings.xml");

}



//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {

        case ' ':
            running = !running;
            break;

        case 'd':
            bDebugMode = !bDebugMode;
            break;
   

	}



}


void ofApp::mouseMoved(int x, int y){


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}
