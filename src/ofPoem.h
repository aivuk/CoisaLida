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
    ofColor color;
    vector<string> text;
    FrameType script;
    string videoInfo;
    ofTrueTypeFont font;

};


#endif
