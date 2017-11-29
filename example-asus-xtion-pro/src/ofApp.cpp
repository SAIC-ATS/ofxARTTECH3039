#include "ofApp.h"


void ofApp::setup()
{
    device.setup();
    
    if (depthStream.setup(device))
    {
        depthStream.setSize(640, 480);
        depthStream.setFps(30);
        depthStream.start();
    }
}


void ofApp::update()
{
    depthProcessor.setRawDepthPixels(depthStream.getPixelsRef());
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
