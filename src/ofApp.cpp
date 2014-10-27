#include "ofApp.h"
#include "ofxSyphon.h"
#include "ofxTrueTypeFontUC.h"
#include <tr1/unordered_map>

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
    float vmax = 0;
    ofTrueTypeFont::setGlobalDpi(72);
    
    
    screenSetup(); //screen and some OF setups
    kinectSetup(); //kinetic setup
    guiSetup(); //GUI Setup
    
    
    wordColor = ofColor(colorRed, colorGreen, colorBlue);
    mPanelPositionAndSize = ofRectangle(37,259, 215, 168);
    mCanvas.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mCanvas.setColor(ofColor(0));
    mPanels.setColor(ofColor(0));
    panelsMask.loadImage("SP_Urban_MASK_025.png");
    panelsMask.crop(mPanelPositionAndSize.x, mPanelPositionAndSize.y, mPanelPositionAndSize.width, mPanelPositionAndSize.height);
    fiespMask.loadImage("SP_Urban_MASK_025.png");
    
	helvetica.loadFont("HelveticaNeueLTStd-Hv.otf", 14);

    player.loadMovie("aviao2.mp4");
    
    // Events listeners for time parameters
    
    wordTimeMin.addListener(this, &ofApp::updateWordTimeMin);
    wordTimeMax.addListener(this, &ofApp::updateWordTimeMax);
    kinectTimeMin.addListener(this, &ofApp::updateKinectTimeMin);
    kinectTimeMax.addListener(this, &ofApp::updateKinectTimeMax);
    colorRed.addListener(this, &ofApp::updateColorRed);
    colorGreen.addListener(this, &ofApp::updateColorGreen);
    colorBlue.addListener(this, &ofApp::updateColorBlue);

    // Create poems
    
    ofPoem poem1, poem2, poem3, poem4, poem5, poem6;
    
    // Poem 1: Alberto Caeiro (F. Pessoa)
    string poemText = "O ESSENCIAL É SABER VER SABER VER SEM ESTAR A PENSAR SABER VER QUANDO SE VÊ E NEM PENSAR QUANDO SE VÊ NEM VER QUANDO SE PENSA";
    poem1.setup(poemText);
    poem1.addScript(4, VIDEO, "poema-1/s-sombrinha caindo2.mov");
    poem1.addScript(6, VIDEO, "poema-1/s-coisas caindo_b.mov");
    poem1.addScript(15, VIDEO, "poema-1/s-Luz piscando.mov");
    poem1.addScript(18, VIDEO, "poema-1/s-retalhos caindo4.mov");
    poem1.addScript(26, KINECT);
    poems.push_back(poem1);
    
    // Poem 2: Joãozito Pereira apud Paul Virilio
    poemText = "A VELOCIDADE É O CEGAMENTO QUANTO MAIS VELOCIDADE MAIS A PAISAGEM VIRA PASSAGEM";
    poem2.addScript(4, VIDEO, "poema-2/s-SOLDA SPARK.mov");
    poem2.addScript(7, VIDEO, "poema-2/s-Tesla Coil_continuo5.mov");
    poem2.addScript(12, KINECT);
    poem2.setup(poemText);
    poems.push_back(poem2);

    // Poem 3: Pierre Clastres
    poemText = "ALGUMA COISA EXISTE NA AUSÊNCIA PERCEBER É SUBTRAIR";
    poem3.setup(poemText);
    poem3.addScript(2, VIDEO, "poema-3/s-TV liga desliga3.mov");
    poem3.addScript(4, KINECT);
    poem3.addScript(7, VIDEO, "poema-3/s-Luz piscando.mov");
    poems.push_back(poem3);
    
    // Poem 4: Camila Nuñez
    poemText = "É COMO SE FICASSE UM VÃO ENTRE O DIA E A NOITE ENTRE SONHO E VIGÍLIA É POR É POR VIR É POR IR É POR ALGO QUE ESCAPA QUE ESVAI ALI BEM NA SUA FRENTE A OLHO NU SE DESPEDINDO";
    poem4.setup(poemText);
    
    poems.push_back(poem4);
    
    // Poem 5: Oscar Wilde
    poemText = "TODOS NÓS ESTAMOS NA SARJETA MAS ALGUNS DE NÓS OLHAM PARA AS ESTRELAS";
    poem5.setup(poemText);
    
    poems.push_back(poem5);

    // Poem 6: Clarice Lispector (Paixao Segundo GH)
    poemText = "NÃO COMPREENDO O QUE VI E NEM MESMO SEI SE VI JÁ QUE MEUS OLHOS TERMINARAM NÃO SE DIFERENCIANDO DA COISA VISTA";
    poem6.setup(poemText);
    
    poems.push_back(poem6);

    
    // Start poem 1
    poems[0].start();
}

void ofApp::updateWordTimeMin(float &time) {
    wordTimeMin = time;
}

void ofApp::updateWordTimeMax(float &time) {
    wordTimeMax = time;
}

void ofApp::updateKinectTimeMin(float &time) {
    kinectTimeMin = time;
}

void ofApp::updateKinectTimeMax(float &time) {
    kinectTimeMax = time;
}

void ofApp::updateColorRed(int &r) {
    colorRed = r;
    wordColor.set(colorRed, colorGreen, colorBlue);
}

void ofApp::updateColorGreen(int &g) {
    colorGreen = g;
    wordColor.set(colorRed, colorGreen, colorBlue);
}

void ofApp::updateColorBlue(int &b) {
    colorBlue = b;
    wordColor.set(colorRed, colorGreen, colorBlue);
}

//--------------------------------------------------------------
void ofApp::update() {

    ofEnableAlphaBlending();
	ofBackground(255, 255, 255);
    kinectUpdate();
    
    if (running) {
        if (poems[runningPoem].frame == STOP) {
            advancePoem();
        }
        poems[runningPoem].update();
    }

}

void ofApp::advancePoem() {
    runningPoem = (runningPoem + 1) % poems.size();
    
    poems[runningPoem].word_i = 0;
    poems[runningPoem].start();
}

void ofApp::backPoem() {
    runningPoem = (runningPoem - 1);
    if (runningPoem < 0) {
        runningPoem = poems.size() - 1;
    }
    
    poems[runningPoem].word_i = 0;
    poems[runningPoem].start();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofBackground(0);
    ofSetColor(255);
    
    mCanvas.draw(400,14);
    mPanels.draw(400,259);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10,10, ofColor(255,0,255), ofColor(255,255,0));
    
    fiespMask.draw(0,0);
    mPanels.draw(mPanelPositionAndSize.x, mPanelPositionAndSize.y);
    
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
    
    poems[runningPoem].draw();

    syphonServer.publishScreen(); //syphon screen

}

void ofApp::kinectVelocities() {
    velocityAverage = 0;
    int countoursNum = contourFinder.size();
    for(int i=0; i < countoursNum; i++){
        ofVec2f velocity = toOf(contourFinder.getVelocity(i));
        velocityAverage += velocity.length();
    }
    velocityAverage /= countoursNum;
}

///DEBUG-MODE

void ofApp  ::debugMode(){
    
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

    /*
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
    toPanels(mCanvas, mPanels);*/


}

void ofApp::drawKinect() {
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

void ofApp::drawToPanels(ofFbo toDraw, bool connect) {
    if (connect) {
        toDraw.readToPixels(mCanvas.getPixelsRef());
        mCanvas.reloadTexture();
        toPanels(mCanvas, mPanels);
    } else {
        toDraw.readToPixels(mPanels.getPixelsRef());
        mPanels.reloadTexture();
    }
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

        kinectVelocities();
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

    // Kinect parameters
    
    gui.add(enableMouse.set("Mouse DEBUG",true));
    parametersKinect.setName("Kinect");
    parametersKinect.add(farThreshold.set("Far Threshold", 0,0, 255 ));
    parametersKinect.add(numMaxBlobs.set("Num Max Blos",10,0,15));
    parametersKinect.add(maxBlobSize.set("max Blob Size",0,0,500));
    parametersKinect.add(minBlobSize.set("min Blob Size",0,0,500));
    parametersKinect.add(offsetX.set("Offset X", 0,-200, 200 ));
    parametersKinect.add(offsetY.set("Offset Y", 0,-200, 200 ));
    gui.add(parametersKinect);

    // Time Parameters
    
    parametersTime.setName("Time");
    parametersTime.add(wordTimeMin.set("Word time min", 500, 500, 5000));
    parametersTime.add(wordTimeMax.set("Word time max", 1000, 500, 5000));
    parametersTime.add(kinectTimeMin.set("Kinect time min", 500, 500, 5000));
    parametersTime.add(kinectTimeMax.set("Kinect time max", 1000, 500, 5000));
    gui.add(parametersTime);
    
    // Visual Parameters
    
    gui.minimizeAll();
    parametersVisual.setName("Visual");
    parametersVisual.add(colorRed.set("Red", 255, 0, 255));
    parametersVisual.add(colorGreen.set("Gree", 255, 0, 255));
    parametersVisual.add(colorBlue.set("Blue", 255, 0, 255));
    gui.add(parametersVisual);

    gui.loadFromFile("settings.xml");

}



//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {

        case ' ':
            running = !running;
            break;

        case 's':
            gui.saveToFile("settings.xml");
            break;
            
        case 'd':
            bDebugMode = !bDebugMode;
            break;
   
        case 'n':
            running = false;
            poems[runningPoem].backWord();
            break;
        case 'm':
            running = false;
            poems[runningPoem].advanceWord();
            break;
            
        case 'k':
            advancePoem();
            break;
        case 'l':
            backPoem();
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
