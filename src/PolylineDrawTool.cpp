//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "PolylineDrawTool.h"


PolylineDrawTool::PolylineDrawTool():
    _mousePressedListener(ofEvents().mousePressed.newListener(this, &PolylineDrawTool::_mousePressed)),
    _mouseReleasedListener(ofEvents().mouseReleased.newListener(this, &PolylineDrawTool::_mouseReleased)),
    _mouseDraggedListener(ofEvents().mouseDragged.newListener(this, &PolylineDrawTool::_mouseDragged))
{
    _parameters.setName("Simple Draw Tool");
    _parameters.add(_enable.set("Enable", true));
    _parameters.add(_drawMultiple.set("Allow Multiple", false));
    _parameters.add(_close.set("Close Polyline", true));
    _parameters.add(_curveTo.set("Use Curve To", false));
    _parameters.add(_clearAll.set("Clear All Polylines"));
    _parameters.add(_clearLast.set("Clear Last Polyline"));
    _parameters.add(_drawVertices.set("Draw Vertices", true));
    
    _clearAllListener = _clearAll.newListener([&](){this->_polylines.clear();});
    _clearLastListener = _clearLast.newListener([&](){
        if (!_polylines.empty()) this->_polylines.pop_back();
    });
}


PolylineDrawTool::~PolylineDrawTool()
{
}


void PolylineDrawTool::draw() const
{
    for (std::size_t i = 0; i < _polylines.size(); ++i)
    {
        if (_isDrawing && (i == _polylines.size() - 1))
        {
            ofSetColor(255);
        }
        else
        {
            ofSetColor(127);
        }

        _polylines[i].draw();
        
        if (_drawVertices)
            for (auto vertex: _polylines[i]) ofDrawCircle(vertex, 5);
    }
}


bool PolylineDrawTool::isDrawing() const
{
    return _isDrawing;
}


std::vector<ofPolyline> PolylineDrawTool::polylines() const
{
    return _polylines;
}


ofParameterGroup& PolylineDrawTool::parameters()
{
    return _parameters;
}


void PolylineDrawTool::_mouseDragged(ofMouseEventArgs& args)
{
    if (!_enable) return;

    _addVertex(args);
}


void PolylineDrawTool::_mousePressed(ofMouseEventArgs& args)
{
    if (!_enable) return;
    
    _isDrawing = true;
    
    if (!_drawMultiple)
        _polylines.clear();
    
    _polylines.push_back(ofPolyline());
    
    _addVertex(args);
}

void PolylineDrawTool::_mouseReleased(ofMouseEventArgs& args)
{
    if (!_enable) return;

    _isDrawing = false;
    
    _addVertex(args);
    
    if (_close)
    {
        _polylines.back().close();
    }
    
    
    ofNotifyEvent(onPolyline, _polylines.back(), this);
}

void PolylineDrawTool::_addVertex(ofMouseEventArgs& args)
{
    if (_curveTo)
        _polylines.back().curveTo(args.x, args.y);
    else
        _polylines.back().addVertex(args.x, args.y);
}
