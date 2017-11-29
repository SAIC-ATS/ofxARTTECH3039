#include "ofApp.h"


void ofApp::setup()
{
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
}


void ofApp::update()
{
    kinect.update();
    if (kinect.isFrameNew())
    {
        depthProcessor.setRawDepthPixels(kinect.getRawDepthPixels());
    }
}


void ofApp::draw()
{
    std::vector<ofPolyline> contours = depthProcessor.contourFinder().getPolylines();
    
    ofSetColor(ofColor::yellow);
    for (const auto& contour: contours)
    {
        contour.draw();
    }
}
