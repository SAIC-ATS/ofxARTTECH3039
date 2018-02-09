//#include "DepthProcessor.h"
//
//
//DepthProcessor::DepthProcessor():
//    _setupListener(ofEvents().setup.newListener(this, &DepthProcessor::_setup)),
//    _updateListener(ofEvents().update.newListener(this, &DepthProcessor::_update)),
//    _drawListener(ofEvents().draw.newListener(this, &DepthProcessor::_draw)),
//    _keyPressedListener(ofEvents().keyPressed.newListener(this, &DepthProcessor::_onKeyPressed)),
//    _mousePressedListener(ofEvents().mousePressed.newListener(this, &DepthProcessor::_onMousePressed)),
//    _mouseDraggedListener(ofEvents().mouseDragged.newListener(this, &DepthProcessor::_onMouseDragged)),
//    _mouseReleasedListener(ofEvents().mouseReleased.newListener(this, &DepthProcessor::_onMouseReleased))
//{
//}
//
//
//void DepthProcessor::allocate(std::size_t width, std::size_t height)
//{
//    if (!_depthPixelsDisplay.isAllocated()
//    ||   _depthPixelsDisplay.getWidth() != width
//    ||   _depthPixelsDisplay.getHeight() != height)
//    {
//        _depthPixelsDisplay.allocate(width, height, OF_PIXELS_RGBA);
//        _depthPixelsThreshold.allocate(width, height, OF_PIXELS_GRAY);
//        _depthMesh.clear();
//        
//        _depthMesh.setMode(OF_PRIMITIVE_POINTS);
//        
//        _roi = ofRectangle(0, 0, width, height);
//        
//        for (std::size_t x = 0; x < width; x++)
//        {
//            for (std::size_t y = 0; y < height; y++)
//            {
//                _depthMesh.addColor(ofColor::white);
//                _depthMesh.addVertex({ x, y, 0 });
//            }
//        }
//    }
//}
//
//
//void DepthProcessor::setRawDepthPixels(const ofShortPixels& rawDepthPixels)
//{
//    uint16_t maxDepth = 0;
//    
//    allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight());
//
//    if (rawDepthPixels.getWidth() == 0 || rawDepthPixels.getHeight() == 0) return;
//    
//    int min = _invert ? 255 : 0;
//    int max = _invert ? 0 : 255;
//    
//    if (_isNewRoi)
//    {
//        _depthPixelsThreshold.set(min);
//        _isNewRoi = false;
//    }
//    
//    for (std::size_t x = 0; x < rawDepthPixels.getWidth(); x++)
//    {
//        for (std::size_t y = 0; y < rawDepthPixels.getHeight(); y++)
//        {
//            bool isInRoi = _roi.inside(x, y);
//            
//            uint16_t depth = rawDepthPixels[rawDepthPixels.getPixelIndex(x, y)];
//
//            maxDepth = std::max(maxDepth, depth);
//
//            std::size_t displayPixelsIndex = _depthPixelsDisplay.getPixelIndex(x, y);
//            std::size_t thresholdPixelsIndex = _depthPixelsThreshold.getPixelIndex(x, y);
//            
//            if (depth >= _foregroundThreshold && depth <= _backgroundThreshold)
//            {
//                int mappedValue = ofMap(depth, _foregroundThreshold, _backgroundThreshold, min, max);
//                
//                if (_binarize)
//                {
//                    _depthPixelsThreshold[thresholdPixelsIndex] = isInRoi ? max : min;
//                }
//                else
//                {
//                    _depthPixelsThreshold[thresholdPixelsIndex] = isInRoi ? mappedValue : min;
//                }
//                
//                if (_display)
//                {
//                    if (_colorize)
//                    {
//                        _depthPixelsDisplay.setColor(displayPixelsIndex, ofColor(isInRoi ? mappedValue : mappedValue / 2 ));
//                    }
//                    else
//                    {
//                        _depthPixelsDisplay.setColor(displayPixelsIndex, ofColor::fromHsb(mappedValue, isInRoi ? 255 : 100, isInRoi ? 255 : 100));
//                    }
//                }
//                
//            }
//            else
//            {
//                _depthPixelsThreshold[thresholdPixelsIndex] = min;
//                
//                if (_display)
//                    _depthPixelsDisplay.setColor(displayPixelsIndex, ofColor(min));
//            }
//        }
//    }
//
//    if (blurLevel > 0)
//        ofxCv::blur(_depthPixelsThreshold, _depthPixelsThreshold, blurLevel);
//
//    if (threshold > 0)
//        ofxCv::threshold(_depthPixelsThreshold, _depthPixelsThreshold, threshold, invert);
//
//    if (erodeIterations > 0)
//        ofxCv::erode(_depthPixelsThreshold, _depthPixelsThreshold, erodeIterations);
//    
//    if (dilateIterations > 0)
//        ofxCv::dilate(_depthPixelsThreshold, _depthPixelsThreshold, dilateIterations);
//    
//
//    contourFinder.setMinAreaRadius(contourMinArea);
//    contourFinder.setMaxAreaRadius(contourMaxArea);
//    contourFinder.setThreshold(contourThreshold);
//    contourFinder.setFindHoles(contourHoles);
//    contourFinder.setSimplify(contourSimplify);
//    contourFinder.findContours(_depthPixelsThreshold);
//
//    if (_display)
//        _depthTextureDisplay.loadData(_depthPixelsDisplay);
//    
//    _depthTextureThreshold.loadData(_depthPixelsThreshold);
//    
//    if (_autoSetRange && maxDepth > 0)
//    {
//        // Adjust our sliders.
//        _foregroundThreshold.setMax(maxDepth);
//        _backgroundThreshold.setMax(maxDepth);
//        _foregroundThreshold = 0;
//        _backgroundThreshold = maxDepth;
//        _autoSetRange = false;
//    }
//    
//
//}
//
//
//void DepthProcessor::setForegroundThreshold(float f)
//{
//    _foregroundThreshold = f;
//}
//
//
//void DepthProcessor::setBackgroundThreshold(float f)
//{
//    _backgroundThreshold = f;
//}
//
//
//float DepthProcessor::getForegroundThreshold() const
//{
//    return float(_foregroundThreshold);
//}
//
//
//float DepthProcessor::getBackgroundThreshold() const
//{
//    return float(_backgroundThreshold);
//}
//
//
//const ofPixels& DepthProcessor::getDepthPixels() const
//{
//    return _depthPixelsThreshold;
//}
//
//
//void DepthProcessor::setDisplay(bool display)
//{
//    _display = display;
//}
//
//
//void DepthProcessor::_setup(ofEventArgs& evt)
//{
//    uint16_t max = 4000;
//    
//    _foregroundThreshold.addListener(this, &DepthProcessor::_foregroundThresholdChanged);
//    _backgroundThreshold.addListener(this, &DepthProcessor::_backgroundThresholdChanged);
//
//    _gui.setup();
//    _gui.add(_autoSetRange.setup("expand range", true));
//    _gui.add(_foregroundThreshold.setup("front depth threshold", 0, 0, max));
//    _gui.add(_backgroundThreshold.setup("back depth threshold", max, 0, max));
//    _gui.add(_invert.setup("threshold invert", false));
//    _gui.add(_binarize.setup("threshold binarize", false));
//    _gui.add(_is3D.setup("Show 3D", false));
//    
//    
////    gui.setup();
//    _gui.add(blurLevel.set("Blur Level", 0, 0, 30));
//    _gui.add(threshold.set("Threshold", 128, 0, 255));
//    _gui.add(erodeIterations.set("Erode Iterations", 0, 0, 30));
//    _gui.add(dilateIterations.set("Dilate Iterations", 0, 0, 30));
//    _gui.add(invert.set("Invert", false));
//    
//    _gui.add(contourMinArea.set("Contour Min Area", 10, 1, 100));
//    _gui.add(contourMaxArea.set("Contour Max Area", 200, 1, 500));
//    _gui.add(contourThreshold.set("Contour Threshold", 128, 0, 255));
//    _gui.add(contourHoles.set("Contour Holes", false));
//    _gui.add(contourSimplify.set("Contour Simplify", true));
//
//}
//
//
//void DepthProcessor::_update(ofEventArgs& evt)
//{
//}
//
//
//void DepthProcessor::_draw(ofEventArgs& evt)
//{
//    if (!_display) return;
//    
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
//        if (_depthTextureDisplay.isAllocated())
//            _depthTextureDisplay.draw(0, 0);
//        
//        if (_depthTextureThreshold.isAllocated())
//        {
//            ofPushMatrix();
//            ofTranslate(_depthTextureDisplay.getWidth(), 0);
//            
//            _depthTextureThreshold.draw(0, 0);
//            
//            ofPushStyle();
//            ofSetLineWidth(3);
//            
//            for (auto i = 0; i < contourFinder.size(); ++i)
//            {
//                if (contourFinder.getHole(i))
//                {
//                    ofSetColor(ofColor::red);
//                }
//                else
//                {
//                    ofSetColor(ofColor::green);
//                }
//                
//                contourFinder.getPolyline(i).draw();
//                
//                auto label = contourFinder.getLabel(i);
//                
//                auto age = contourFinder.getTracker().getAge(label);
//                
//                ofDrawBitmapStringHighlight(ofToString(label) + " : " + ofToString(age), ofxCv::toOf(contourFinder.getCenter(i)));
//            }
//            
//            ofPopStyle();
//            ofPopMatrix();
//        }
//    }
//    
//    _gui.draw();
//}
//
//
//void DepthProcessor::_onKeyPressed(ofKeyEventArgs& evt)
//{
//    if (evt.key == 'd')
//    {
//        _display = !_display;
//        
//        if (_display)
//        {
//            _gui.maximizeAll();
//        }
//        else
//        {
//            _gui.minimizeAll();
//        }
//    }
//}
//
//
//void DepthProcessor::_onMousePressed(ofMouseEventArgs& evt)
//{
//    _roi = ofRectangle(evt.x, evt.y, 0, 0);
//    _roiStart = _roi.getPosition();
//    _isNewRoi = true;
//}
//
//
//void DepthProcessor::_onMouseDragged(ofMouseEventArgs& evt)
//{
//    _roi = ofRectangle(_roiStart, 0, 0);
//    _roi.growToInclude(evt.x, evt.y);
//    _isNewRoi = true;
//}
//
//
//void DepthProcessor::_onMouseReleased(ofMouseEventArgs& evt)
//{
//    _roi.growToInclude(evt.x, evt.y);
//    
//    if (_roi.getWidth() < 4 && _roi.getHeight() < 4)
//    {
//        _roi = ofRectangle(0, 0, _depthPixelsDisplay.getWidth(), _depthPixelsDisplay.getHeight());
//    }
//    else
//    {
//        _roi = _roi.getIntersection(ofRectangle(0, 0, _depthPixelsDisplay.getWidth(), _depthPixelsDisplay.getHeight()));
//    }
//    
//    _isNewRoi = true;
//}
//
//
//void DepthProcessor::_foregroundThresholdChanged(float& f)
//{
//    if (_backgroundThreshold - f < 75)
//        _backgroundThreshold = f + 75;
//}
//
//
//void DepthProcessor::_backgroundThresholdChanged(float& f)
//{
//    if (_foregroundThreshold - f > 75)
//        _foregroundThreshold = f - 75;
//}
//
