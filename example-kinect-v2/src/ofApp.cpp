#include "ofApp.h"

void ofApp::setup()
{
    // Initialize Kinect V2
    ofxKinectV2 tmp;
    vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
  
    // Check to see if there is a KinectV2 attached.
    if (deviceList.size() > 0) {
      std::cout<< "Success: Kinect V2 found";
    } else {
      std::cerr<< "Failure: No Kinect V2 found";
      return;
    }
  
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

