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
    tempFbo.allocate(mPanelPositionAndSize->width, mPanelPositionAndSize->height);
    word_i = 0;
}

void ofPoem::update() {
    unsigned long long now = ofGetElapsedTimeMillis();
    if (script.count(word_i) > 0 && !played) {
        frame = script[word_i];
        played = false;
    }
    
    switch (frame) {
        case WORD:
            if (now - wordTime > 500) {
                wordTime = now;
                advanceWord();
                played = false;
            }
            break;
        case VIDEO:
            if (scriptVideo[word_i].getIsMovieDone()) {
                scriptVideo[word_i].stop();
                scriptVideo[word_i].setFrame(0);
                frame = WORD;
                wordTime = now;
                played = true;
                return;
            }
            
            if (!scriptVideo[word_i].isPlaying()) {
                scriptVideo[word_i].setLoopState(OF_LOOP_NONE);
                scriptVideo[word_i].play();
                scriptVideo[word_i].update();
            } else {
                scriptVideo[word_i].update();
            }
            break;
        case KINECT:
            break;
        case STOP:
            break;
    }
}

void ofPoem::drawText() {
    drawWord(text[word_i]);
}

void ofPoem::drawVideo() {    
    tempFbo.begin();
    ofBackground(0);
    scriptVideo[word_i].draw(0, 0, mPanelPositionAndSize->getWidth(), mPanelPositionAndSize->getHeight());
    tempFbo.end();
    ((ofApp*)ofGetAppPtr())->drawToPanels(tempFbo, true);
}

void ofPoem::advanceWord() {
    word_i = (word_i + 1) % text.size();
    if (word_i == 0) {
        frame = STOP;
    }
}

void ofPoem::backWord() {
    if (word_i != text.size() - 1) {
        word_i = (word_i - 1) % text.size();
    } else {
        word_i = 0;
    }
}

void ofPoem::addScript(int i, FrameType f, string video_info) {
    script.insert(std::make_pair<int, FrameType>(i, f));
    
    if (f == VIDEO) {
        ofVideoPlayer video;
        video.loadMovie(video_info);
        scriptVideo.insert(std::make_pair<int, ofVideoPlayer>(i, video));
    }
}

void ofPoem::draw() {
    switch (frame) {
        case WORD:
            drawWord(text[word_i]);
            break;
        case VIDEO:
            drawVideo();
            break;
        case KINECT:
            break;
        case STOP:
            break;
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

    tempFbo.begin();
    ofBackground(0);
    
    
    while (iter != stop) {
        ofUniChar c = ofUTF8::getNext(iter);
        r = font->getStringBoundingBox(ofTextConverter::toUTF8(c), mPanelPositionAndSize->width/2, i*17);
        char_width[i] = r.getWidth();
        if (char_width[i] < char_width[char_min]) {
            char_min = i;
        }
        if (char_width[i] > char_width[char_max]) {
            char_max = i;
        }
        ++i;
    }
    
    // Draw the letter
    i = 0;
    iter = ofUTF8::beginPtr(word);
    while (iter != stop) {
        ofUniChar c = ofUTF8::getNext(iter);
        ofPushStyle();
        ofSetColor(255);
        //cout << ofTextConverter::toUTF8(c) << " " << r.getWidth() << endl ;

        // Left
        font->drawString(ofTextConverter::toUTF8(c), 0, (mPanelPositionAndSize->height - word_height)/2 + 14 + i*17);
        
        // Center
        font->drawString(ofTextConverter::toUTF8(c), mPanelPositionAndSize->width/2 - char_width[i]/2.0, (mPanelPositionAndSize->height - word_height)/2 + 10 + i*17);
        
        // Right
        font->drawString(ofTextConverter::toUTF8(c), mPanelPositionAndSize->width - char_width[char_max], (mPanelPositionAndSize->height - word_height)/2 + 14 + i*17);
        ofPopStyle();

        ++i;
    }
    tempFbo.end();
    ((ofApp*)ofGetAppPtr())->drawToPanels(tempFbo);

}