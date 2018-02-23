#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "BinaryPreprocessor.h"


class DepthProcessor
{
public:
    DepthProcessor();
    
    void allocate(std::size_t width, std::size_t height);

    void setRawDepthPixels(const ofFloatPixels& rawDepthPixels);
    
    void setForegroundThreshold(float f);
    void setBackgroundThreshold(float f);
    
    float getForegroundThreshold() const;
    float getBackgroundThreshold() const;

    const ofPixels& getDepthPixels() const;
    
    void setDisplay(bool display);
    
    const ofxCv::ContourFinder& contourFinder() const;
    
    /// \brief Get the depth between 0-255.
    int getDepthAtPosition(std::size_t x, std::size_t y) const;

    virtual void setUseTexture(bool bUseTex) { };
    
    /// \returns true if an internal ofTexture is being used.
    virtual bool isUsingTexture() const { return true; }

private:
    void _setup(ofEventArgs& evt);
    void _update(ofEventArgs& evt);
    void _draw(ofEventArgs& evt);
    void _exit(ofEventArgs& evt);
    void _onKeyPressed(ofKeyEventArgs& evt);
    
    void _onMousePressed(ofMouseEventArgs& evt);
    void _onMouseDragged(ofMouseEventArgs& evt);
    void _onMouseReleased(ofMouseEventArgs& evt);

    void _foregroundThresholdChanged(float& f);
    void _backgroundThresholdChanged(float& f);

    ofxPanel _gui;
    
    bool _isNewRoi = true;
    ofRectangle _roi;
    glm::vec2 _roiStart;
    
    ofPixels _depthPixelsDisplay;
    ofTexture _depthTextureDisplay;
    
    ofPixels _depthPixelsThreshold;
    ofTexture _depthTextureThreshold;

    ofPixels _scaledDepthPixels;
    ofTexture _scaledDepthTexture;

    ofMesh _depthMesh;
    
    bool _display = true;
    
    ofParameterGroup _depthParameters;
    ofParameter<float> _foregroundThreshold;
    ofParameter<float> _backgroundThreshold;
    ofParameter<bool> _binarize;
    ofParameter<bool> _autoSetRange;
//    ofParameter<bool> _is3D;
    
    ofEasyCam _camera;

    
    BinaryPreprocessor _binaryPreprocessor;
    
//    ofParameterGroup _prerprocessParameters;
//    ofParameter<float> _blurLevel;
//    ofParameter<int> _threshold;
//    ofParameter<bool> _invertThreshold;
//    ofParameter<int> _erodeIterations;
//    ofParameter<int> _dilateIterations;

    ofxCv::ContourFinder _contourFinder;

    ofParameterGroup _contourParameters;
    ofParameter<float> _contourMinArea;
    ofParameter<float> _contourMaxArea;
    ofParameter<float> _contourThreshold;
    ofParameter<bool> _contourHoles;
    ofParameter<bool> _contourSimplify;
    
    ofEventListener _setupListener;
    ofEventListener _updateListener;
    ofEventListener _drawListener;
    ofEventListener _exitListener;
    ofEventListener _keyPressedListener;
    ofEventListener _mousePressedListener;
    ofEventListener _mouseDraggedListener;
    ofEventListener _mouseReleasedListener;
    
};
