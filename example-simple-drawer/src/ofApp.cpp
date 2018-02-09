#include "ofApp.h"

void ofApp::setup()
{
    gui.setup();
    gui.add(drawTool.parameters());
}


void ofApp::draw()
{
    drawTool.draw();
    gui.draw();
    
    // Draw vertices as circles.
    ofNoFill();
    ofSetColor(ofColor::yellow);
    for (auto polyline: drawTool.polylines())
    {
        for (auto vertex: polyline)
        {
            ofDrawCircle(vertex, 10);
        }
    }
}
