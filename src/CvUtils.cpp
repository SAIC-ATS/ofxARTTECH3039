//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "CvUtils.h"


ConvexHull::ConvexHull(float minimumDefectDepth): _minimumDefectDepth(minimumDefectDepth)
{
}


ConvexHull::ConvexHull(const ofPolyline& contour, float minimumDefectDepth)
{
    setContour(contour, minimumDefectDepth);
}


void ConvexHull::setContour(const ofPolyline& contour, float minimumDefectDepth)
{
    _minimumDefectDepth = minimumDefectDepth;

    _contour = contour;

    std::vector<cv::Point2i> contour2i;

    cv::Mat(ofxCv::toCv(_contour)).copyTo(contour2i);

    cv::Mat contourMat(contour2i);

    // By passing std::vector<int> we get indices.
    std::vector<int> hullIndices;
    cv::convexHull(contourMat, hullIndices);

    if (hullIndices.size() > 0 && contour.size() > 0)
    {
        std::vector<cv::Vec4i> convexityDefects;

        // TODO: has a bug
        // cv::convexityDefects(contourMat, hullIndices, convexityDefects);

        // Our local, drop-in, inefficient (?) hack.
        ConvexHull::convexityDefects(contourMat, hullIndices, convexityDefects, _minimumDefectDepth);

        _convexHullDefects.clear();

        for (auto defect: convexityDefects)
        {
            Defect _defect;
            _defect.index = defect[2];
            auto defectPoint = contour2i[_defect.index];
            _defect.point = glm::vec2(defectPoint.x, defectPoint.y);

            _defect.startIndex = defect[0];
            auto startPoint = contour2i[_defect.startIndex];
            _defect.startPoint = glm::vec2(startPoint.x, startPoint.y);

            _defect.endIndex = defect[1];
            auto endPoint = contour2i[_defect.endIndex];
            _defect.endPoint = glm::vec2(endPoint.x, endPoint.y);

            auto diff = _defect.endPoint - _defect.startPoint;
            std::swap(diff.x, diff.y);
            diff.x *= -1;
            _defect.defectHullChordNormal = glm::normalize(diff);

            _defect.midPoint = (_defect.startPoint + _defect.endPoint) / 2.;


            cv::Mat pre = contourMat.rowRange(_defect.startIndex, _defect.index);
            cv::Mat post = contourMat.rowRange(_defect.index, std::max(contourMat.total(),
                                                                       _defect.endIndex));

            _defect.arcLengthPre = cv::arcLength(pre, false);
            _defect.arcLengthPost = cv::arcLength(post, false);

            _defect.arcLength = _defect.arcLengthPre + _defect.arcLengthPost;

            _defect.hullChordLength = glm::distance(_defect.startPoint, _defect.endPoint);

//            cv::Vec4f preLine;
//            cv::fitLine(pre, preLine, CV_DIST_L2, 0, 0.01, 0.01);
//
//            cv::Vec4f postLine;
//            cv::fitLine(post, postLine, CV_DIST_L2, 0, 0.01, 0.01);
//
//            _defect.arcPre = glm::normalize(glm::vec2(preLine[0], preLine[1]));
//            _defect.arcPost = glm::normalize(glm::vec2(postLine[0], postLine[1]));

            _defect.depth = defect[3] / 256.0;

            _defect.defectHullChordNormalPoint = _defect.point + (_defect.defectHullChordNormal * _defect.depth);

            _convexHullDefects.push_back(_defect);
        }
    }

    _convexHullIndices.clear();

    for (auto index: hullIndices)
    {
        if (index > -1)
        {
            _convexHullIndices.push_back(index);
        }
        else
        {
            ofLogWarning("ConvexHull::setContour") << "Found negative convex hull index, skipping.";
        }
    }
}


ofPolyline ConvexHull::contour() const
{
    return _contour;
}


ofPolyline ConvexHull::convexHull() const
{
    if (_convexHullIndices.size() != 0 && _convexHull.size() != _convexHullIndices.size())
    {
        _convexHull.clear();
        _convexHull.setClosed(true);

        for (auto index: _convexHullIndices)
        {
            const auto& point = _contour[index];
            _convexHull.addVertex(point);
        }
    }

    return _convexHull;
}


std::vector<std::size_t> ConvexHull::convexHullIndices() const
{
    return _convexHullIndices;
}


std::vector<ConvexHull::Defect> ConvexHull::convexHullDefects() const
{
    return _convexHullDefects;
}


void ConvexHull::convexityDefects(const cv::Mat& _contourMat,
                                  const std::vector<int>& _hullIndices,
                                  std::vector<cv::Vec4i>& convexityDefects,
                                  float minimumDefectDepth)
{
    // Make a sorted version of the hull indices.
    auto hullIndices = _hullIndices;
    std::sort(hullIndices.begin(), hullIndices.end());

    CvMat contourMat = _contourMat;
    CvMat hullMat = cvMat(1, hullIndices.size(), CV_32SC1, reinterpret_cast<void*>(&hullIndices[0]));
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* defects = cvConvexityDefects(&contourMat, &hullMat, storage);

    for (auto i = 0; i < defects->total; ++i)
    {
        CvConvexityDefect* cur = reinterpret_cast<CvConvexityDefect*>(cvGetSeqElem(defects, i));

        if (cur->depth < minimumDefectDepth)
        {
            continue;
        }

        // Find the two convex hull indices.
        int startIndex = -1;
        int endIndex = -1;

        for (int hullIndex: hullIndices)
        {
            auto point = _contourMat.at<cv::Point2i>(hullIndex);

            if (startIndex < 0 || endIndex < 0)
            {
                if (startIndex < 0 &&
                    cur->start->x == point.x &&
                    cur->start->y == point.y)
                {
                    startIndex = hullIndex;
                }

                if (endIndex < 0 &&
                    cur->end->x == point.x &&
                    cur->end->y == point.y)
                {
                    endIndex = hullIndex;
                }
            }
            else
            {
                break;
            }
        }

        // Find the defec index by searching between the start and end hull indices.
        int defectIndex = -1;

        for (int index = startIndex; index <= std::max(_contourMat.total(), std::size_t(endIndex)); ++index)
        {
            auto point = _contourMat.at<cv::Point2i>(index);

            if (cur->depth_point->x == point.x &&
                cur->depth_point->y == point.y)
            {
                defectIndex = index;
                break;
            }
        }

        if (startIndex >= 0 && endIndex >= 0 && defectIndex >= 0)
        {
            cv::Vec4i defect;
            defect[0] = startIndex;
            defect[1] = endIndex;
            defect[2] = defectIndex;
            defect[3] = cur->depth * 256.0; // Convert it, to match C++ api.
            convexityDefects.push_back(defect);
        }
        else
        {
            ofLogError("ConvexHull::convexityDefects") << "Invalid index.";
        }
    }

    cvReleaseMemStorage(&storage);
}


std::vector<Circle> CvUtils::fitCircles(const ofPolyline& contour)
{
    std::vector<Circle> circles;

    for (auto i = 0; i < contour.size(); ++i)
    {
        // If we have less than three points, we can't calculate curvature.
        // Also deal with end conditions if the contour is not closed.
        if (contour.size() < 3 || (!contour.isClosed() && (i == 0 || i == contour.size())))
        {
            circles.push_back(Circle(std::numeric_limits<float>::infinity()));
        }
        else
        {
            auto nextI = (i + 1) % contour.size();
            auto prevI = (i == 0) ? (contour.size() - 1) : (i - 1);

            glm::vec2 pt1(contour[prevI].x, contour[prevI].y);
            glm::vec2 pt2(contour[i].x, contour[i].y);
            glm::vec2 pt3(contour[nextI].x, contour[nextI].y);

            Circle c;

            if (Circle::fromPoints(pt1, pt2, pt3, c))
            {
                circles.push_back(c);
            }
            else
            {
                circles.push_back(Circle(std::numeric_limits<float>::infinity()));
            }
        }
    }

    return circles;
}


std::vector<float> CvUtils::getCurvatures(const ofPolyline& contour)
{
    auto circles = fitCircles(contour);

    std::vector<float> curvatures;

    for (auto i = 0; i < circles.size(); ++i)
    {
        const auto& circle = circles[i];

        float sign = 1;

        if (std::isfinite(circle.radius()))
        {
            const auto& position = circle.center();
            const auto& point = glm::vec2(contour[i].x, contour[i].y);
            sign = glm::normalize(glm::cross(contour[i], glm::vec3(position, 0))).z;
        }

        curvatures.push_back(sign / circle.radius());

    }

    return curvatures;
}


std::vector<float> CvUtils::smooth(const std::vector<float>& data, float alpha)
{
    std::vector<float> smoothed;

    float lastValue = 0;

    for (auto value: data)
    {
        auto v = alpha * lastValue + (1 - alpha) * value;
        smoothed.push_back(v);
        lastValue = v;
    }

    return smoothed;
}

