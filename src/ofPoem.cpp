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
#include "ofUTF8.h"
#include "ofUnicode.h"
#include "ofTextConverter.h"
#include <sstream>
#include <string>

void ofPoem::setup(std::string _text) {
    istringstream itext(_text);
    copy(istream_iterator<std::string>(itext),
         istream_iterator<string>(),
         back_inserter(text));
    
    // Get font from ofApp
    font = &((ofApp*)ofGetAppPtr())->helvetica;
    word_i = 0;

}

void ofPoem::update() {
    unsigned long long now = ofGetElapsedTimeMillis();
    if (now - wordTime > 500) {
        wordTime = now;
        word_i = (word_i + 1) % text.size();
    }
}

void ofPoem::drawText() {
    drawWord(text[word_i], 400, 259);
}

void ofPoem::drawWord(string word, int x, int y) {
    ofUTF8Ptr iter = ofUTF8::beginPtr(word);
    ofUTF8Ptr stop = ofUTF8::endPtr(word);
    ofRectangle r;
    int dx;
    int i = 0;
    while (iter != stop) {
        ofUniChar c = ofUTF8::getNext(iter);
        
        r = font->getStringBoundingBox(ofTextConverter::toUTF8(c), 400, 259 + i*20);
        dx = (14 - r.getWidth())/2;
        font->drawString(ofTextConverter::toUTF8(c), x + dx, y + i*20);
        ++i;
    }
}