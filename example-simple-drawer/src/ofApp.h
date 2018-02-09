#pragma once


#include "ofMain.h"
#include "ofxARTTECH3039.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;
  
    SimpleDrawTool drawTool;

    ofxPanel gui;

};
