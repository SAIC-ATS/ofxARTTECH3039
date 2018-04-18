#pragma once


#include "ofxGui.h"
#include "ofxCv.h"


class BinaryPreprocessor: public ofBaseImage
{
public:
    BinaryPreprocessor();
    ~BinaryPreprocessor();

    template<typename PixelType>
    void update(const ofBaseHasPixels_<PixelType>& _pixels);

    template<typename PixelType>
    void update(const ofPixels_<PixelType>& _pixels);

    // ofBaseDraws
    using ofBaseDraws::draw;
    void draw(float x, float y, float w, float h) const override;
    
    // ofBaseHasTexture
    void setUseTexture(bool bUseTex) override;
    bool isUsingTexture() const override;
    const ofTexture& getTexture() const override;
    ofTexture& getTexture() override;

    // ofBaseHasPixels
    const ofPixels& getPixels() const override;
    ofPixels& getPixels() override;

    float getHeight() const override;
    float getWidth() const override;

    // Parameters
    ofParameterGroup& parameters();
    ofParameter<float> blurLevel;
    ofParameter<int> threshold;
    ofParameter<bool> invertThreshold;
    ofParameter<int> erodeIterations;
    ofParameter<int> dilateIterations;
    
private:
    ofParameterGroup _parameters;

    ofPixels _binaryPixels;
    
    bool _isUsingTexture = true;
    ofTexture _binaryTexture;
    
};


template<typename PixelType>
void BinaryPreprocessor::update(const ofBaseHasPixels_<PixelType>& pixels)
{
    update(pixels.getPixels());
}


template<typename PixelType>
void BinaryPreprocessor::update(const ofPixels_<PixelType>& pixels)
{
    if (pixels.getNumChannels() != 1)
        ofxCv::convertColor(pixels, _binaryPixels, CV_RGB2GRAY);
    else
        _binaryPixels = pixels;

    if (blurLevel > 0)
        ofxCv::blur(_binaryPixels, _binaryPixels, blurLevel);
    
    if (threshold > 0)
        ofxCv::threshold(_binaryPixels, _binaryPixels, threshold, invertThreshold);
    
    if (erodeIterations > 0)
        ofxCv::erode(_binaryPixels, _binaryPixels, erodeIterations);
    
    if (dilateIterations > 0)
        ofxCv::dilate(_binaryPixels, _binaryPixels, dilateIterations);
    
    if (_isUsingTexture)
        _binaryTexture.loadData(_binaryPixels);
}
