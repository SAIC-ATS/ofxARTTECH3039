#pragma once


#include "ofMain.h"
#include "ofxKinectV2.h"
#include "DepthProcessor.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    // Kinect V2.
    ofxKinectV2 kinect;
    DepthProcessor depthProcessor;

};
