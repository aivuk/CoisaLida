//
//  ofPoem.cpp
//  CoisaLida
//
//  Created by walrus on 10/21/14.
//
//

#include "ofPoem.h"
#include "ofMain.h"
#include "ofApp.h"
#include <sstream>
#include <string>

void ofPoem::setup(std::string _text) {
    istringstream itext(_text);
    copy(istream_iterator<std::string>(itext),
         istream_iterator<string>(),
         back_inserter(text));
    
    // Get font from ofApp
    font = ((ofApp*)ofGetAppPtr())->helvetica;

}

void ofPoem::drawText() {
    char c;
    string s = text[2];
    cout << s;
    ofRectangle r;
    for(unsigned int i = 0; i < s.length(); i++) {
        int dx;
        c = s[i];
        r = font.getStringBoundingBox(string(&c), 400, 259 + i*14);
        dx = (14 - r.getWidth())/2;
        font.drawString(string(&c), 400, 259 + i*14);
    }
}