#include "ofApp.h"

void ofApp::setup()
{
    // Initialize Kinect V2
    vector <ofxKinectV2::KinectDeviceInfo> deviceList = ofxKinectV2().getDeviceList();
  
    // Check to see if there is a KinectV2 attached.
    if (deviceList.size() > 0) {
        std::cout<< "Success: Kinect V2 found" << std::endl;
    } else {
        std::cerr<< "Failure: No Kinect V2 found" << std::endl;
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
    std::vector<ofPolyline> contours = depthProcessor.contourFinder().getPolylines();
  
    ofSetColor(ofColor::yellow);

    for (const auto& contour: contours)
    {
        contour.draw();
    }
}
