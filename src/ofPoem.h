//
//  ofPoem.h
//  CoisaLida
//
//  Created by Edgar Z. Alvarenga on 10/21/14.
//
//

#ifndef CoisaLida_ofPoem_h
#define CoisaLida_ofPoem_h

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"

class ofPoem {
    
public:
    enum FrameType{
        KINECT,
        VIDEO
    };
    
    void update();
    void setup(std::string text);
    void draw(int screen);
    void drawText();
    void drawWord(string word);
    void advanceWord();
    void backWord();
    
    ofColor color;
    vector<string> text;
    FrameType script;
    string videoInfo;
    ofxTrueTypeFontUC *font;
    unsigned long long wordTime;
    int word_i;
    ofRectangle *mPanelPositionAndSize;
};


#endif
