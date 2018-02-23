#include "ContourProcessor.h"


ContourProcessor::ContourProcessor()
{
    _contourParameters.setName("Detection");
    _contourParameters.add(minArea.set("Contour Min Area", 10, 1, 100));
    _contourParameters.add(maxArea.set("Contour Max Area", 200, 1, 500));
    _contourParameters.add(threshold.set("Contour Threshold", 128, 0, 255));
    _contourParameters.add(findHoles.set("Find Holes", true));
    _contourParameters.add(simplifyContours.set("Simplify Contours", true));

    _trackingParameters.setName("Tracking");
    _trackingParameters.add(trackPersistence.set("Track Persistence", 15, 1, 60));
    _trackingParameters.add(trackMaximumDistance.set("Track Maximum Distance", 64, 0, 128));

    _drawParameters.setName("Drawing");
    _drawParameters.add(drawContours.set("Contours", true));
    _drawParameters.add(drawLabels.set("Labels", true));
    _drawParameters.add(drawBoundingRect.set("Bounding Rect", false));
    _drawParameters.add(drawCenter.set("Center", true));
    _drawParameters.add(drawCentroid.set("Centroid", false));
    _drawParameters.add(drawAverage.set("Average", false));
    _drawParameters.add(drawMinAreaRect.set("Min. Area Rect", false));
    _drawParameters.add(drawEnclosingCircle.set("Enclosing Circle", false));
    _drawParameters.add(drawFitQuad.set("Fit Quad", false));

    _parameters.setName("Contour Processor");
    _parameters.add(_contourParameters);
    _parameters.add(_drawParameters);
    _parameters.add(_trackingParameters);
}


ContourProcessor::~ContourProcessor()
{
}


void ContourProcessor::draw() const
{
    ofPushStyle();
    ofSetLineWidth(3);

    for (const auto& contour: _trackedContours)
    {
        _drawContour(contour.second);
    }

    ofPopStyle();
}


ofxCv::ContourFinder& ContourProcessor::contourFinder()
{
    return _contourFinder;
}


ofParameterGroup& ContourProcessor::parameters()
{
    return _parameters;
}


void ContourProcessor::_drawContour(const Contour& contour) const
{
    float a = contour.trackStrength * 255;

    if (drawContours)
    {
        ofNoFill();

        if (contour.isHole)
            ofSetColor(255, 0, 0, a);
        else
            ofSetColor(0, 255, 0, a);

        contour.polyline.draw();
    }


    if (drawBoundingRect)
    {
        ofNoFill();
        ofSetColor(ofColor::cyan, a);
        ofDrawRectangle(contour.boundingRect);
    }

    if (drawCenter)
    {
        ofNoFill();
        ofSetColor(ofColor::yellow, a);
        ofDrawCircle(contour.center, 2);
    }

    if (drawCentroid)
    {
        ofNoFill();
        ofSetColor(ofColor::green, a);
        ofDrawCircle(contour.centroid, 3);
    }

    if (drawAverage)
    {
        ofNoFill();
        ofSetColor(ofColor::blue, a);
        ofDrawCircle(contour.average, 4);
    }

    if (drawMinAreaRect)
    {
        ofNoFill();
        ofSetColor(ofColor::chartreuse, a);
        ofRectangle rect(- contour.minAreaRect.width / 2,
                         - contour.minAreaRect.height / 2,
                         contour.minAreaRect.width,
                         contour.minAreaRect.height);
        ofPushMatrix();
        ofTranslate(contour.minAreaRect.center);
        ofRotateZDeg(contour.minAreaRect.angleDeg);
        ofDrawRectangle(rect);
        ofPopMatrix();
    }

    if (drawEnclosingCircle)
    {
        ofNoFill();
        ofSetColor(ofColor::gray, a);
        ofDrawCircle(contour.minEnclosingCircle.center(),
                     contour.minEnclosingCircle.radius());
    }

    if (drawFitQuad)
    {
        ofSetColor(ofColor::pink, a);
        contour.fitQuad.draw();
    }

    if (drawLabels)
    {
        ofDrawBitmapStringHighlight(ofToString(contour.label) + ": " + ofToString(contour.age),
                                    contour.center,
                                    ofColor(ofColor::black, a),
                                    ofColor(ofColor::white, a));
    }
}



