#include "BinaryPreprocessor.h"


BinaryPreprocessor::BinaryPreprocessor()
{
    _parameters.setName("Pre-Processor");
    _parameters.add(_blurLevel.set("Blur Level", 0, 0, 30));
    _parameters.add(_threshold.set("Threshold", 128, 0, 255));
    _parameters.add(_invertThreshold.set("Invert Threshold", false));
    _parameters.add(_erodeIterations.set("Erode Iterations", 0, 0, 30));
    _parameters.add(_dilateIterations.set("Dilate Iterations", 0, 0, 30));
}


BinaryPreprocessor::~BinaryPreprocessor()
{
}


ofParameterGroup& BinaryPreprocessor::parameters()
{
    return _parameters;
}
