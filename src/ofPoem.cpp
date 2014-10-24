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
    mPanelPositionAndSize = &((ofApp*)ofGetAppPtr())->mPanelPositionAndSize;
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
    drawWord(text[word_i]);
}

void ofPoem::advanceWord() {
    word_i = (word_i + 1) % text.size();
}

void ofPoem::backWord() {
    if (word_i != text.size() - 1) {
        word_i = (word_i - 1) % text.size();
    } else {
        word_i = 0;
    }
}

void ofPoem::drawWord(string word) {
    ofRectangle r;
    int i = 0;
    int len = ofUTF8::distance(word);
    int word_height = len*15 + 5;
    int char_width[len];
    int char_max = 0;
    int char_min = 0;
    // Find the biggest letter
    ofUTF8Ptr iter = ofUTF8::beginPtr(word);
    ofUTF8Ptr stop = ofUTF8::endPtr(word);
    while (iter != stop) {
        ofUniChar c = ofUTF8::getNext(iter);
        r = font->getStringBoundingBox(ofTextConverter::toUTF8(c), mPanelPositionAndSize->width/2 + mPanelPositionAndSize->x, mPanelPositionAndSize->y + i*17);
        char_width[i] = r.getWidth();
        if (char_width[i] < char_width[char_min]) {
            char_min = i;
        }
        if (char_width[i] > char_width[char_max]) {
            char_max = i;
        }
        ++i;
    }
    cout << char_width[char_min] << " " << char_width[char_max] << endl;
    
    // Draw the letter
    i = 0;
    iter = ofUTF8::beginPtr(word);
    while (iter != stop) {
        ofUniChar c = ofUTF8::getNext(iter);
        ofPushStyle();
        ofSetColor(0, 0, 0);
        //cout << ofTextConverter::toUTF8(c) << " " << r.getWidth() << endl ;
        // Left
        font->drawString(ofTextConverter::toUTF8(c), mPanelPositionAndSize->x + (char_width[char_max] - char_width[i])/2, mPanelPositionAndSize->y + (mPanelPositionAndSize->height - word_height)/2 + 14 + i*17);
        
        // Center
        font->drawString(ofTextConverter::toUTF8(c), mPanelPositionAndSize->width/2 + mPanelPositionAndSize->x - char_width[i]/2.0, mPanelPositionAndSize->y + (mPanelPositionAndSize->height - word_height)/2 + 10 + i*17);
        
        
        // Right
        font->drawString(ofTextConverter::toUTF8(c), mPanelPositionAndSize->x + mPanelPositionAndSize->width - char_width[char_max], mPanelPositionAndSize->y + (mPanelPositionAndSize->height - word_height)/2 + 14 + i*17);
        ofPopStyle();

        ++i;
    }
}