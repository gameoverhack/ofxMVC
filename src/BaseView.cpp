//
//  BaseView.cpp
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "BaseView.h"

//--------------------------------------------------------------
BaseView::BaseView(){
    ofxLogVerbose() << "Creating BaseView" << endl;
}

//--------------------------------------------------------------
BaseView::~BaseView(){
    ofxLogVerbose() << "Destroying BaseView" << endl;
}

//--------------------------------------------------------------
void BaseView::setup(float _width, float _height, ViewOption _viewOptions, string _viewName){
    
    bIsSetup = false;
    
    viewOptions = _viewOptions;
    width = displayWidth = _width;
    height = displayHeight = _height;
    viewName = _viewName;
    bNoWarp = false;
    
    if(getIsOption(VIEW_USE_WINDOW)){
        
    }
    
    if(getIsOption(VIEW_USE_BEZIERWARP) && getIsOption(VIEW_USE_MATRIXWARP)){
        ofxLogError() << "Can only use one warp type at a time" << endl;
        return;
    }
#ifdef USE_FENSTER
    if(getIsOption(VIEW_USE_WINDOW)){
        
        ofxLogVerbose() << "Allocating View window mode: ofFenster" << endl;
        
        window = ofxFensterManager::get()->createFenster(0, 0, width, height);
        window->addListener(this);
        window->setBackgroundColor(0,0,0);
        window->setWindowTitle(viewName);
    }
#endif
    if(getIsOption(VIEW_USE_BEZIERWARP)){
        
        ofxLogVerbose() << "Allocating View warp mode: ofxBezierWarp" << endl;
        
        warp = new BezierWarp();
        warp->allocate(width, height);
        
    }
    
    if(getIsOption(VIEW_USE_MATRIXWARP)){
        ofxLogError() << "Allocating View warp mode: ofxMatrixWarp - NOT IMPLEMENTED" << endl;
        assert(false);
    }
    
    if(!getIsOption(VIEW_USE_FBO) && 
       !getIsOption(VIEW_USE_BEZIERWARP) && 
       !getIsOption(VIEW_USE_MATRIXWARP)){
        
        ofxLogWarning() << "No render mode allocated" << endl;
        bNoWarp = true;
        
    }
    
    if(getIsOption(VIEW_USE_FBO) && 
       !getIsOption(VIEW_USE_BEZIERWARP) && 
       !getIsOption(VIEW_USE_MATRIXWARP)){
        
        ofxLogVerbose() << "Allocating View warp mode: ofxNoWarp" << endl;
        
        warp = new NoWarp();
        warp->allocate(width, height);
    }
    
    bIsSetup = true;
    
}

//--------------------------------------------------------------
void BaseView::draw(){
    draw(0, 0, width, height);
}

//--------------------------------------------------------------
void BaseView::draw(float x, float y){
    draw(x, y, width, height);
}

//--------------------------------------------------------------
void BaseView::draw(float x, float y, float w, float h){
    if(!bNoWarp) warp->draw(x, y, w * displayWidth/width, h * displayHeight/height);
}

//--------------------------------------------------------------
void BaseView::begin(){
    if(!bNoWarp) warp->begin();
}

//--------------------------------------------------------------
void BaseView::end(){
    if(!bNoWarp) warp->end();
}

void BaseView::setPosition(float x, float y, float w, float h){
    if(getIsOption(VIEW_USE_WINDOW)){
#ifdef USE_FENSTER
        window->setWindowShape(w, h);
        window->setWindowPosition(x, y);
#endif
        displayWidth = w;
        displayHeight = h;
    }else{
        ofxLogError() << "Not a window view" << endl;
    }
    if(getIsOption(VIEW_USE_BEZIERWARP)){
        warp->setWarpGridPosition(0, 0, w, h);
    }
}

//--------------------------------------------------------------
float BaseView::getWidth(){
    return width;
}

//--------------------------------------------------------------
float BaseView::getHeight(){
    return height;
}

//--------------------------------------------------------------
string BaseView::getName(){
    return viewName;
}
#ifdef USE_FENSTER
//--------------------------------------------------------------
ofxFenster& BaseView::getWindow(){
    return (*window);
}
#endif
//--------------------------------------------------------------
ofFbo& BaseView::getFBO(){
    return warp->getFBO();
}

//--------------------------------------------------------------
ofTexture& BaseView::getTextureReference(){
    return warp->getTextureReference();
}

//--------------------------------------------------------------
bool BaseView::getIsSetup(){
    return bIsSetup;
}

//--------------------------------------------------------------
bool BaseView::getIsOption(ViewOption viewOption){
    if((viewOptions & viewOption) == viewOption){
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
void BaseView::windowResized(int w, int h){
    displayWidth = w;
    displayHeight = h;
    if(getIsOption(VIEW_USE_BEZIERWARP)){
        warp->setWarpGridPosition(0, 0, w, h);
    }
}