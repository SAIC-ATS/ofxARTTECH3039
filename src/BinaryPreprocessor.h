#pragma once


#include "ofxGui.h"
#include "ofxCv.h"


class BinaryPreprocessor
{
public:
    BinaryPreprocessor();
    ~BinaryPreprocessor();

    template<typename PixelType>
    ofPixels_<PixelType> process(const ofPixels_<PixelType>& _pixels);
    
    ofParameterGroup& parameters();
    
private:
    ofParameterGroup _parameters;
    ofParameter<float> _blurLevel;
    ofParameter<int> _threshold;
    ofParameter<bool> _invertThreshold;
    ofParameter<int> _erodeIterations;
    ofParameter<int> _dilateIterations;

};


template<typename PixelType>
ofPixels_<PixelType> BinaryPreprocessor::process(const ofPixels_<PixelType>& _pixels)
{
    ofPixels_<PixelType> pixels = _pixels;
    
    if (_blurLevel > 0)
        ofxCv::blur(pixels, pixels, _blurLevel);
    
    if (_threshold > 0)
        ofxCv::threshold(pixels, pixels, _threshold, _invertThreshold);
    
    if (_erodeIterations > 0)
        ofxCv::erode(pixels, pixels, _erodeIterations);
    
    if (_dilateIterations > 0)
        ofxCv::dilate(pixels, pixels, _dilateIterations);
    
    return pixels;
}
