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
#include <tr1/unordered_map>

enum FrameType {
    WORD,
    KINECT,
    VIDEO,
    STOP
};


struct poemScript {
    int word_idx;
    FrameType frame;
};

class ofPoem {
    
public:
    void update();
    void setup(std::string text);
    void draw();
    void drawText();
    void drawWord(string word);
    void drawVideo();
    void advanceWord();
    void backWord();
    void addScript(int i, FrameType f, string video_info = NULL);
    
    ofColor color;
    vector<string> text;
    string videoInfo;
    ofxTrueTypeFontUC *font;
    unsigned long long wordTime;
    int word_i;
    FrameType frame = WORD;
    
    bool played = false;
    ofImage *mCanvas, *mPanels;
    ofRectangle *mPanelPositionAndSize;
    tr1::unordered_map<int, FrameType> script;
    tr1::unordered_map<int, ofVideoPlayer> scriptVideo;
    ofFbo tempFbo;
};


#endif
