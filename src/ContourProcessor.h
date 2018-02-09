//#pragma once
//
//
//#include "ofMain.h"
//#include "ofxGui.h"
//#include "ofxCv.h"
//
//
//class DepthProcessor
//{
//public:
//    DepthProcessor();
//
//    template<typename DepthCamera>
//    void updateWithStream(DepthCamera& camera)
//    {
//        setRawDepthPixels(camera.getPixelsRef());
//    }
//
////    template<typename DepthCamera>
////    void updateMesh(DepthCamera& camera)
////    {
////
////    }
//
//    void allocate(std::size_t width, std::size_t height);
//
//    void setRawDepthPixels(const ofShortPixels& depthPixels);
//
//    void setForegroundThreshold(float f);
//    void setBackgroundThreshold(float f);
//
//    float getForegroundThreshold() const;
//    float getBackgroundThreshold() const;
//
//    const ofPixels& getDepthPixels() const;
//
//    void setDisplay(bool display);
//
//private:
//    void _setup(ofEventArgs& evt);
//    void _update(ofEventArgs& evt);
//    void _draw(ofEventArgs& evt);
//    void _onKeyPressed(ofKeyEventArgs& evt);
//
//    void _onMousePressed(ofMouseEventArgs& evt);
//    void _onMouseDragged(ofMouseEventArgs& evt);
//    void _onMouseReleased(ofMouseEventArgs& evt);
//
//    void _foregroundThresholdChanged(float& f);
//    void _backgroundThresholdChanged(float& f);
//
//    bool _isNewRoi = true;
//    ofRectangle _roi;
//    glm::vec2 _roiStart;
//
//    ofPixels _depthPixelsDisplay;
//    ofTexture _depthTextureDisplay;
//
//    ofPixels _depthPixelsThreshold;
//    ofTexture _depthTextureThreshold;
//
//    ofMesh _depthMesh;
//
//    bool _display = true;
//
//    ofxPanel _gui;
//    mutable ofxFloatSlider _foregroundThreshold;
//    mutable ofxFloatSlider _backgroundThreshold;
//
//    ofxToggle _invert;
//    ofxToggle _binarize;
//    ofxToggle _autoSetRange;
//    ofxToggle _colorize;
//    ofxToggle _is3D;
//    ofEasyCam _camera;
//
//    ofxCv::ContourFinder contourFinder;
//
//    ofxPanel _contour;
//    ofParameter<float> blurLevel;
//    ofParameter<int> threshold;
//    ofParameter<bool> invert;
//    ofParameter<int> erodeIterations;
//    ofParameter<int> dilateIterations;
//
//    ofParameter<float> contourMinArea;
//    ofParameter<float> contourMaxArea;
//    ofParameter<float> contourThreshold;
//    ofParameter<bool> contourHoles;
//    ofParameter<bool> contourSimplify;
//
//    ofEventListener _setupListener;
//    ofEventListener _updateListener;
//    ofEventListener _drawListener;
//    ofEventListener _keyPressedListener;
//    ofEventListener _mousePressedListener;
//    ofEventListener _mouseDraggedListener;
//    ofEventListener _mouseReleasedListener;
//
//};

