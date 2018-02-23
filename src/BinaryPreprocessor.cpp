#include "BinaryPreprocessor.h"


BinaryPreprocessor::BinaryPreprocessor()
{
    _parameters.setName("Binary Preprocessor");
    _parameters.add(blurLevel.set("Blur Level", 0, 0, 30));
    _parameters.add(threshold.set("Threshold", 128, 0, 255));
    _parameters.add(invertThreshold.set("Invert Threshold", false));
    _parameters.add(erodeIterations.set("Erode Iterations", 0, 0, 30));
    _parameters.add(dilateIterations.set("Dilate Iterations", 0, 0, 30));
}


BinaryPreprocessor::~BinaryPreprocessor()
{
}


ofParameterGroup& BinaryPreprocessor::parameters()
{
    return _parameters;
}


void BinaryPreprocessor::draw(float x, float y, float w, float h) const
{
    if (_isUsingTexture && _binaryTexture.isAllocated())
        _binaryTexture.draw(x, y, w, h);
    
}


void BinaryPreprocessor::setUseTexture(bool bUseTex)
{
    _isUsingTexture = bUseTex;
    
    if (!_isUsingTexture)
        _binaryTexture.clear();
}


bool BinaryPreprocessor::isUsingTexture() const
{
    return true;
}


const ofTexture& BinaryPreprocessor::getTexture() const
{
    return _binaryTexture;
}


ofTexture& BinaryPreprocessor::getTexture()
{
    return _binaryTexture;
}

const ofPixels& BinaryPreprocessor::getPixels() const
{
    return _binaryPixels;
}


ofPixels& BinaryPreprocessor::getPixels()
{
    return _binaryPixels;
}


float BinaryPreprocessor::getHeight() const
{
    return _binaryPixels.getHeight();
}


float BinaryPreprocessor::getWidth() const
{
    return _binaryPixels.getWidth();
}
