#include "DepthProcessor.h"


DepthProcessor::DepthProcessor():
    _setupListener(ofEvents().setup.newListener(this, &DepthProcessor::_setup)),
    _updateListener(ofEvents().update.newListener(this, &DepthProcessor::_update)),
    _drawListener(ofEvents().draw.newListener(this, &DepthProcessor::_draw)),
    _exitListener(ofEvents().exit.newListener(this, &DepthProcessor::_exit)),
    _keyPressedListener(ofEvents().keyPressed.newListener(this, &DepthProcessor::_onKeyPressed)),
    _mousePressedListener(ofEvents().mousePressed.newListener(this, &DepthProcessor::_onMousePressed)),
    _mouseDraggedListener(ofEvents().mouseDragged.newListener(this, &DepthProcessor::_onMouseDragged)),
    _mouseReleasedListener(ofEvents().mouseReleased.newListener(this, &DepthProcessor::_onMouseReleased))
{
}


void DepthProcessor::allocate(std::size_t width, std::size_t height)
{
    if (!_depthPixelsDisplay.isAllocated()
    ||   _depthPixelsDisplay.getWidth() != width
    ||   _depthPixelsDisplay.getHeight() != height)
    {
        _depthPixelsDisplay.allocate(width, height, OF_PIXELS_RGBA);
        _depthPixelsThreshold.allocate(width, height, OF_PIXELS_GRAY);
        _scaledDepthPixels.allocate(width, height, OF_PIXELS_GRAY);
        _depthMesh.clear();
        
        _depthMesh.setMode(OF_PRIMITIVE_POINTS);
        
        _roi = ofRectangle(0, 0, width, height);
        
        for (std::size_t x = 0; x < width; x++)
        {
            for (std::size_t y = 0; y < height; y++)
            {
                _depthMesh.addColor(ofColor::white);
                _depthMesh.addVertex({ x, y, 0 });
            }
        }
    }
}

void DepthProcessor::setRawDepthPixels(const ofFloatPixels& rawDepthPixels)
{
    uint16_t maxDepth = 0;
    
    allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight());

    if (rawDepthPixels.getWidth() == 0 || rawDepthPixels.getHeight() == 0) return;
    
    int min = 0;
    int max = 255;
    
    if (_isNewRoi)
    {
        _depthPixelsThreshold.set(min);
        _isNewRoi = false;
    }
    
    for (std::size_t x = 0; x < rawDepthPixels.getWidth(); x++)
    {
        for (std::size_t y = 0; y < rawDepthPixels.getHeight(); y++)
        {
            bool isInRoi = _roi.inside(x, y);
            
            uint16_t depth = rawDepthPixels[rawDepthPixels.getPixelIndex(x, y)];

            int mappedDepth = ofMap(depth, _foregroundThreshold, _backgroundThreshold, min, max);

            _scaledDepthPixels[_scaledDepthPixels.getPixelIndex(x, y)] = mappedDepth;
            
            maxDepth = std::max(maxDepth, depth);

            std::size_t displayPixelsIndex = _depthPixelsDisplay.getPixelIndex(x, y);
            std::size_t thresholdPixelsIndex = _depthPixelsThreshold.getPixelIndex(x, y);
            
            if (depth >= _foregroundThreshold && depth <= _backgroundThreshold)
            {
                _depthPixelsThreshold[thresholdPixelsIndex] = isInRoi ? mappedDepth : min;
                
                if (_display)
                    _depthPixelsDisplay.setColor(displayPixelsIndex, ofColor::fromHsb(mappedDepth, isInRoi ? 255 : 100, isInRoi ? 255 : 100));
            }
            else
            {
                _depthPixelsThreshold[thresholdPixelsIndex] = min;
                
                if (_display)
                    _depthPixelsDisplay.setColor(displayPixelsIndex, ofColor(min));
            }
        }
    }

    if (_blurLevel > 0)
        ofxCv::blur(_depthPixelsThreshold, _depthPixelsThreshold, _blurLevel);

    if (_threshold > 0)
        ofxCv::threshold(_depthPixelsThreshold, _depthPixelsThreshold, _threshold, _invertThreshold);

    if (_erodeIterations > 0)
        ofxCv::erode(_depthPixelsThreshold, _depthPixelsThreshold, _erodeIterations);

    if (_dilateIterations > 0)
        ofxCv::dilate(_depthPixelsThreshold, _depthPixelsThreshold, _dilateIterations);

    _contourFinder.setMinAreaRadius(_contourMinArea);
    _contourFinder.setMaxAreaRadius(_contourMaxArea);
    _contourFinder.setThreshold(_contourThreshold);
    _contourFinder.setFindHoles(_contourHoles);
    _contourFinder.setSimplify(_contourSimplify);
    
    _contourFinder.findContours(_depthPixelsThreshold);
    
    if (_display)
        _depthTextureDisplay.loadData(_depthPixelsDisplay);
    
    _depthTextureThreshold.loadData(_depthPixelsThreshold);
    _scaledDepthTexture.loadData(_scaledDepthPixels);
    
    if (_autoSetRange && maxDepth > 0)
    {
        // Adjust our sliders.
        _foregroundThreshold.setMax(maxDepth);
        _backgroundThreshold.setMax(maxDepth);
        _foregroundThreshold = 0;
        _backgroundThreshold = maxDepth;
        _autoSetRange = false;
    }
}


void DepthProcessor::setForegroundThreshold(float f)
{
    _foregroundThreshold = f;
}


void DepthProcessor::setBackgroundThreshold(float f)
{
    _backgroundThreshold = f;
}


float DepthProcessor::getForegroundThreshold() const
{
    return float(_foregroundThreshold);
}


float DepthProcessor::getBackgroundThreshold() const
{
    return float(_backgroundThreshold);
}


const ofPixels& DepthProcessor::getDepthPixels() const
{
    return _depthPixelsThreshold;
}


void DepthProcessor::setDisplay(bool display)
{
    _display = display;
}


void DepthProcessor::_setup(ofEventArgs& evt)
{
    uint16_t max = 4000;
    
    _foregroundThreshold.addListener(this, &DepthProcessor::_foregroundThresholdChanged);
    _backgroundThreshold.addListener(this, &DepthProcessor::_backgroundThresholdChanged);

    _depthParameters.setName("Depth Processor");
    _depthParameters.add(_autoSetRange.set("Auto Range", true));
    _depthParameters.add(_foregroundThreshold.set("Near Threshold", 0, 0, max));
    _depthParameters.add(_backgroundThreshold.set("Far Threshold", max, 0, max));
    // _depthParameters.add(_is3D.set("Show 3D", false));
    
    _prerprocessParameters.setName("Pre-Processor");
    _prerprocessParameters.add(_blurLevel.set("Blur Level", 0, 0, 30));
    _prerprocessParameters.add(_threshold.set("Threshold", 128, 0, 255));
    _prerprocessParameters.add(_invertThreshold.set("Invert Threshold", false));
    _prerprocessParameters.add(_erodeIterations.set("Erode Iterations", 0, 0, 30));
    _prerprocessParameters.add(_dilateIterations.set("Dilate Iterations", 0, 0, 30));

    _contourParameters.setName("Contour Processor");
    _contourParameters.add(_contourMinArea.set("Contour Min Area", 10, 1, 100));
    _contourParameters.add(_contourMaxArea.set("Contour Max Area", 200, 1, 500));
    _contourParameters.add(_contourThreshold.set("Contour Threshold", 128, 0, 255));
    _contourParameters.add(_contourHoles.set("Find Holes", false));
    _contourParameters.add(_contourSimplify.set("Simplify Contours", true));

    _gui.setup();
    _gui.add(_depthParameters);
    _gui.add(_prerprocessParameters);
    _gui.add(_contourParameters);
    
    _gui.loadFromFile("DepthProcessor.xml");
}


void DepthProcessor::_update(ofEventArgs& evt)
{
}


void DepthProcessor::_draw(ofEventArgs& evt)
{
    ofSetColor(255);
    
    if (!_display) return;
    
//    if (_is3D)
//    {
//        _camera.begin();
//
//        glPointSize(3);
//        ofPushMatrix();
//        // the projected points are 'upside down' and 'backwards'
//        ofScale(1, -1, -1);
//        ofTranslate(0, 0, -1000); // center the points a bit
//        ofEnableDepthTest();
//        _depthMesh.drawVertices();
//        ofDisableDepthTest();
//        ofPopMatrix();
//
//        _camera.end();
//    }
//    else
//    {
        if (_depthTextureDisplay.isAllocated())
            _depthTextureDisplay.draw(0, 0);

        if (_scaledDepthTexture.isAllocated())
            _scaledDepthTexture.draw(0, _depthTextureDisplay.getHeight());
            
    
        if (_depthTextureThreshold.isAllocated())
        {
            ofPushMatrix();
            ofTranslate(_depthTextureDisplay.getWidth(), 0);
            
            
            
            _depthTextureThreshold.draw(0, 0);
            
            ofPushStyle();
            ofSetLineWidth(3);
            
            for (std::size_t i = 0; i < _contourFinder.size(); ++i)
            {
                if (_contourFinder.getHole(i))
                {
                    ofSetColor(ofColor::red);
                }
                else
                {
                    ofSetColor(ofColor::green);
                }

                _contourFinder.getPolyline(i).draw();

                auto label = _contourFinder.getLabel(i);

                auto age = _contourFinder.getTracker().getAge(label);

                ofDrawBitmapStringHighlight(ofToString(label) + " : " + ofToString(age), ofxCv::toOf(_contourFinder.getCenter(i)));
            }
            
            ofPopStyle();
            ofPopMatrix();
        }
//    }
    
    _gui.draw();
}


void DepthProcessor::_onKeyPressed(ofKeyEventArgs& evt)
{
    if (evt.key == 'd')
    {
        _display = !_display;
        
        if (_display)
        {
            _gui.maximizeAll();
        }
        else
        {
            _gui.minimizeAll();
        }
    }
}


void DepthProcessor::_onMousePressed(ofMouseEventArgs& evt)
{
    _roi = ofRectangle(evt.x, evt.y, 0, 0);
    _roiStart = _roi.getPosition();
    _isNewRoi = true;
}


void DepthProcessor::_onMouseDragged(ofMouseEventArgs& evt)
{
    _roi = ofRectangle(_roiStart, 0, 0);
    _roi.growToInclude(evt.x, evt.y);
    _isNewRoi = true;
}


void DepthProcessor::_onMouseReleased(ofMouseEventArgs& evt)
{
    _roi.growToInclude(evt.x, evt.y);
    
    if (_roi.getWidth() < 4 && _roi.getHeight() < 4)
    {
        _roi = ofRectangle(0, 0, _depthPixelsDisplay.getWidth(), _depthPixelsDisplay.getHeight());
    }
    else
    {
        _roi = _roi.getIntersection(ofRectangle(0, 0, _depthPixelsDisplay.getWidth(), _depthPixelsDisplay.getHeight()));
    }
    
    _isNewRoi = true;
}


void DepthProcessor::_foregroundThresholdChanged(float& f)
{
    if (_backgroundThreshold - f < 75)
        _backgroundThreshold = f + 75;
}


void DepthProcessor::_backgroundThresholdChanged(float& f)
{
    if (_foregroundThreshold - f > 75)
        _foregroundThreshold = f - 75;
}


const ofxCv::ContourFinder& DepthProcessor::contourFinder() const
{
    return _contourFinder;
}


int DepthProcessor::getDepthAtPosition(std::size_t x, std::size_t y) const
{
    if (_scaledDepthPixels.isAllocated())
    {
        std::size_t xx = ofClamp(x, 0, _scaledDepthPixels.getWidth() - 1);
        std::size_t yy = ofClamp(y, 0, _scaledDepthPixels.getHeight() - 1);
        return _scaledDepthPixels[_scaledDepthPixels.getPixelIndex(xx, yy)];
    }
    return -1;
}

void DepthProcessor::_exit(ofEventArgs& evt) {
    _gui.saveToFile("DepthProcessor.xml");
}
