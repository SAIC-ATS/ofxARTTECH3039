//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofxCv.h"
#include "Circle.h"


class ConvexHull
{
public:
    struct Defect
    {
        /// \brief The index of the point in the original contour.
        std::size_t index = 0;

        /// \brief The position of the defect.
        glm::vec2 point;

        /// \brief The index of the defect start point in the original contour.
        ///
        /// This point corresponds to a point in the convex hull.
        std::size_t startIndex = 0;

        /// \brief The start point of the of the defect.
        glm::vec2 startPoint;

        /// \brief The mid point between startPoint and endPoint.
        glm::vec2 midPoint;

        /// \brief The index of the defect end point in the original contour.
        ///
        /// This point corresponds to a point in the convex hull.
        std::size_t endIndex = 0;

        /// \brief The end point of the defect.
        glm::vec2 endPoint;

        /// \brief The perpendicular intercept between.
        ///
        /// The vector describing the perpendicular
        glm::vec2 defectHullChordNormal;

        glm::vec2 defectHullChordNormalPoint;

        float hullChordLength = 0;

        float arcLength = 0;
        float arcLengthPost = 0;
        float arcLengthPre = 0;

        /// \brief The depth of the defect
        ///
        /// The normal distance from the hull to the defect.
        float depth = 0;

    };

    /// \brief Create a convex hull.
    ConvexHull(float minimumDefectDepth = 0);

    /// \brief Create a cotext hull.
    /// \param contour The contour to analyze.
    ConvexHull(const ofPolyline& contour, float minimumDefectDepth = 0);

    void setContour(const ofPolyline& contour, float minimumDefectDepth = 0);

    /// \brief Get the original contour.
    ofPolyline contour() const;

    /// \brief Create the convex hull from the contour.
    ofPolyline convexHull() const;

    /// \returns the indices of the contour that represent the convex hull.
    std::vector<std::size_t> convexHullIndices() const;

    /// \returns the indices of the contour that represent the convex hull defects.
    std::vector<Defect> convexHullDefects() const;

private:
    float _minimumDefectDepth = 0;

    /// \brief The original contour.
    ofPolyline _contour;

    /// \brief The indices of the contour that represent the convex hull.
    std::vector<std::size_t> _convexHullIndices;

    /// \brief The convex hull.
    mutable ofPolyline _convexHull;

    /// \brief the indices of the contour that represent the convex hull defects.
    std::vector<Defect> _convexHullDefects;

    /// \brief Custom C++ implementation that finds indices.
    static void convexityDefects(const cv::Mat& contourMat,
                                 const std::vector<int>& hullIndices,
                                 std::vector<cv::Vec4i>& convexityDefects,
                                 float minimumDefectDepth);

};


class CvUtils
{
public:
    /// \brief Calculate the radius of curvature for each point given its neighbors.
    ///
    /// For points along a straight line, an infinite radius of curvature is
    /// returned.
    ///
    /// If a circle cannot be be fit for a given set of points, the circle's
    /// radius is set to std::numeric_limits<float>::infinity().
    ///
    /// \param contour The contour to fit.
    /// \returns a set of radii of curvature for each point in the contour.
    static std::vector<Circle> fitCircles(const ofPolyline& contour);

    /// \brief Caclculate the radii of curvature for each point given its neighbors.
    ///
    /// For points along a straight line, an infinite radius of curvature is returns.
    ///
    /// \returns a set of radii of curvature for each point in the contour.
    static std::vector<float> getCurvatures(const ofPolyline& contour);

    /// \brief Smooth values in a vector of floats using a low pass filter.
    static std::vector<float> smooth(const std::vector<float>& data, float alpha = 0.90);

};





