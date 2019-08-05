#include "ofApp.h"

void ofApp::setup()
{
    ofBackground(80);
    
    // Initialize Kinect V2
    std::vector<ofxKinectV2::KinectDeviceInfo> deviceList = ofxKinectV2().getDeviceList();
  
    // Check to see if there is a KinectV2 attached.
    if (deviceList.size() > 0)
    {
        std::cout << "Success: Kinect V2 found" << std::endl;
    }
    else
    {
        std::cerr << "Failure: No Kinect V2 found" << std::endl;
        return;
    }
    
    // Reset min/max distances (as per ofxKinectV2).
    kinect.params.getFloat("minDistance").set(0);
    kinect.params.getFloat("maxDistance").set(12000);
    
    kinect.open(deviceList[0].serial);
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
    ofSetColor(ofColor::yellow);

    std::vector<ofPolyline> contours = depthProcessor.contourFinder().getPolylines();
  
    for (const auto& contour: contours)
    {
        //
        glm::vec3 center = contour.getCentroid2D();
        
        int depth = kinect.getRawDepthPixels().getColor(center.x, center.y).getBrightness();
        
        ofDrawBitmapStringHighlight("Depth: " + ofToString(depth), center);
        
        contour.draw();
        
        
    }
}
