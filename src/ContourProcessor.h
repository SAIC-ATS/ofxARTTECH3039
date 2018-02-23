#pragma once


#include "ofxGui.h"
#include "ofxCv.h"
#include "Circle.h"

class Contour
{
public:
    struct RotatedRectangle
    {
      glm::vec2 center;
      float width = 0;
      float height = 0;
      float angleDeg = 0;
    };


    unsigned int label = 0;

    unsigned int age = 0;
    unsigned int lastSeen = 0;

    /// \brief A value between 0 and 1.
    ///
    /// 1 indicates fully tracked.
    /// 0 indicates fully lost.
    /// Values between 0 and 1 indicate an active search for a known target.
    float trackStrength = 0;

    ofPolyline polyline;
    bool isHole = false;

    ofRectangle boundingRect;
    glm::vec2 center;
    glm::vec2 centroid;
    glm::vec2 average;
    double area = 0;
    double arcLength = 0;
    RotatedRectangle minAreaRect;
    Circle minEnclosingCircle;
    RotatedRectangle fitEllipse;
    ofPolyline fitQuad;
    glm::vec2 velocity;


};


class ContourProcessor
{
public:
    ContourProcessor();
    ~ContourProcessor();

    template<typename PixelType>
    void update(const ofBaseHasPixels_<PixelType>& _pixels);

    template<typename PixelType>
    void update(const ofPixels_<PixelType>& _pixels);

    void draw() const;

    ofxCv::ContourFinder& contourFinder();

    ofParameterGroup& parameters();

    ofParameter<float> minArea;
    ofParameter<float> maxArea;
    ofParameter<float> threshold;
    ofParameter<bool> findHoles;
    ofParameter<bool> simplifyContours;

    ofParameter<bool> drawContours;
    ofParameter<bool> drawLabels;
    ofParameter<bool> drawBoundingRect;
    ofParameter<bool> drawCenter;
    ofParameter<bool> drawCentroid;
    ofParameter<bool> drawAverage;
    ofParameter<bool> drawMinAreaRect;
    ofParameter<bool> drawEnclosingCircle;
    ofParameter<bool> drawFitQuad;

    ofParameter<int> trackPersistence;
    ofParameter<float> trackMaximumDistance;

    ofEvent<const Contour> onContourAdded;
    ofEvent<const Contour> onContourUpdated;
    ofEvent<const Contour> onContourRemoved;

private:
    ofParameterGroup _contourParameters;
    ofParameterGroup _drawParameters;
    ofParameterGroup _trackingParameters;
    ofParameterGroup _parameters;

    std::map<unsigned int, Contour> _trackedContours;

    mutable ofxCv::ContourFinder _contourFinder;

    void _drawContour(const Contour& contour) const;
};


template<typename PixelType>
void ContourProcessor::update(const ofBaseHasPixels_<PixelType>& pixels)
{
    update(pixels.getPixels());
}


template<typename PixelType>
void ContourProcessor::update(const ofPixels_<PixelType>& pixels)
{
    _contourFinder.setMinAreaRadius(minArea);
    _contourFinder.setMaxAreaRadius(maxArea);
    _contourFinder.setThreshold(threshold);
    _contourFinder.setFindHoles(findHoles);
    _contourFinder.setSimplify(simplifyContours);

    _contourFinder.getTracker().setPersistence(trackPersistence);
    _contourFinder.getTracker().setMaximumDistance(trackMaximumDistance);

    _contourFinder.findContours(pixels);

    auto& tracker = _contourFinder.getTracker();

    const auto& currentLabels = tracker.getCurrentLabels();
    const auto& newLabels = tracker.getNewLabels();
    const auto& deadLabels = tracker.getDeadLabels();

    for (auto label: deadLabels)
    {
        std::map<unsigned int, Contour>::iterator trackedIter = _trackedContours.find(label);
        if (trackedIter != _trackedContours.end())
        {
            if (!tracker.existsCurrent(label))
            {
                ofNotifyEvent(onContourRemoved, trackedIter->second);
                _trackedContours.erase(trackedIter);
            }
            else
            {
                trackedIter->second.age = tracker.getAge(label);
                trackedIter->second.lastSeen = tracker.getLastSeen(label);
                trackedIter->second.trackStrength = ofClamp((1 - trackedIter->second.lastSeen / float(trackPersistence)), 0, 1);
                ofNotifyEvent(onContourUpdated, trackedIter->second);
            }
        }
    }

    for (std::size_t i = 0; i < _contourFinder.size(); ++i)
    {
        auto label = _contourFinder.getLabel(i);

        if (!tracker.existsCurrent(label))
        {
            std::cout << "Skipping because label is not tracked." <<std::endl;
            continue;
        }

        Contour contour;

        contour.label = label;
        contour.age = tracker.getAge(label);
        contour.lastSeen = tracker.getLastSeen(label);
        contour.trackStrength = 1;

        contour.polyline = _contourFinder.getPolyline(i);
        contour.isHole = _contourFinder.getHole(i);

        contour.boundingRect = ofxCv::toOf(_contourFinder.getBoundingRect(i));
        contour.center = ofxCv::toOf(_contourFinder.getCenter(i));
        contour.centroid = ofxCv::toOf(_contourFinder.getCentroid(i));
        contour.average = ofxCv::toOf(_contourFinder.getAverage(i));

        contour.area = _contourFinder.getContourArea(i);
        contour.arcLength = _contourFinder.getArcLength(i);

        auto minAreaRect = _contourFinder.getMinAreaRect(i);
        contour.minAreaRect.angleDeg = minAreaRect.angle;
        contour.minAreaRect.center = ofxCv::toOf(minAreaRect.center);
        contour.minAreaRect.width = minAreaRect.size.width;
        contour.minAreaRect.height = minAreaRect.size.height;

        float radius = 0;
        auto minEnclosingCircle = _contourFinder.getMinEnclosingCircle(i, radius);
        contour.minEnclosingCircle = Circle(ofxCv::toOf(minEnclosingCircle), radius);

        auto fitEllipse = _contourFinder.getFitEllipse(i);
        contour.fitEllipse.angleDeg = fitEllipse.angle;
        contour.fitEllipse.center = ofxCv::toOf(fitEllipse.center);
        contour.fitEllipse.width = fitEllipse.size.width;
        contour.fitEllipse.height = fitEllipse.size.height;

        contour.fitQuad = ofxCv::toOf(_contourFinder.getFitQuad(i));
        contour.velocity = ofxCv::toOf(_contourFinder.getVelocity(i));

        auto trackedIter = _trackedContours.find(label);
        if (trackedIter != _trackedContours.end())
        {
            trackedIter->second = contour;
            ofNotifyEvent(onContourUpdated, trackedIter->second);
        }
        else
        {
            _trackedContours[label] = contour;
            ofNotifyEvent(onContourAdded, trackedIter->second);
        }
    }
}
