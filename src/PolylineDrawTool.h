//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMain.h"


class PolylineDrawTool
{
public:
    PolylineDrawTool();
    virtual ~PolylineDrawTool();
    
    void draw() const;
    
    bool isDrawing() const;
    
    std::vector<ofPolyline> polylines() const;

    ofParameterGroup& parameters();

    ofEvent<ofPolyline> onPolyline;
    
private:
    void _mouseDragged(ofMouseEventArgs& args);
    void _mousePressed(ofMouseEventArgs& args);
    void _mouseReleased(ofMouseEventArgs& args);

    void _addVertex(ofMouseEventArgs& args);
    
    ofParameterGroup _parameters;
    ofParameter<bool> _enable;
    ofParameter<bool> _drawMultiple;
    ofParameter<bool> _close;
    ofParameter<bool> _curveTo;
    ofParameter<void> _clearAll;
    ofParameter<void> _clearLast;
    ofParameter<bool> _drawVertices;


    ofEventListener _mousePressedListener;
    ofEventListener _mouseReleasedListener;
    ofEventListener _mouseDraggedListener;

    ofEventListener _clearAllListener;
    ofEventListener _clearLastListener;

    std::vector<ofPolyline> _polylines;

    bool _isDrawing = false;
};

