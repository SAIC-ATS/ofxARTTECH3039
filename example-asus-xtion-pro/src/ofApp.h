#pragma once


#include "ofMain.h"
#include "ofxNI2.h"
#include "DepthProcessor.h"


class ofApp: public ofBaseApp
{
public:
	void setup() override;
	void update() override;
    void draw() override;

    // The OpenNI device.
    ofxNI2::Device device;
    ofxNI2::DepthStream depthStream;

    // The Depth Processor
    DepthProcessor depthProcessor;
    
    
};
