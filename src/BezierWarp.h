//
//  BezierWarp.h
//  DeepListen
//
//  Created by game over on 17/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_BEZIERWARP
#define _H_BEZIERWARP

#include "ofxBezierWarp.h"
#include "BaseWarp.h"


class BezierWarp : public BaseWarp {
    
public:    

	BezierWarp(){
        warp = new ofxBezierWarp();
    };
    ~BezierWarp(){
        delete warp;
    };
    
    void allocate(int w, int h){
        warp->allocate(w, h);
    };
    void allocate(int w, int h, int numXPoints, int numYPoints, float pixelsPerGridDivision){
        warp->allocate(w, h, numXPoints, numYPoints, pixelsPerGridDivision);
    };
    
    void begin(){
        warp->begin();
    };
    void end(){
        warp->end();
    };
    
    void draw(){
        warp->draw();
    };
    void draw(float x, float y){
        warp->draw(x, y);
    };
    void draw(float x, float y, float w, float h){
        warp->draw(x, y, w, h);
    };
    
    void setWarpGrid(int numXPoints, int numYPoints, bool forceReset = false){
        warp->setWarpGrid(numXPoints, numYPoints, forceReset);
    };
    void setWarpGridPosition(float x, float y, float w, float h){
        warp->setWarpGridPosition(x, y, w, h);
    };
    
    void setWarpGridResolution(float pixelsPerGridDivision){
        warp->setWarpGridResolution(pixelsPerGridDivision);
    };
    void setWarpGridResolution(int gridDivisionsX, int gridDivisionsY){
        warp->setWarpGridResolution(gridDivisionsX, gridDivisionsY);
    };
    
    void resetWarpGrid(){
        warp->resetWarpGrid();
    };
    void resetWarpGridPosition(){
        warp->resetWarpGridPosition();
    };
    
    float getWidth(){
        return warp->getWidth();
    };
    float getHeight(){
        return warp->getHeight();
    };
    
    int getNumXPoints(){
        return warp->getNumXPoints();
    };
    int getNumYPoints(){
        return warp->getNumYPoints();
    };
    
    int getGridDivisionsX(){
        return warp->getGridDivisionsX();
    };
    int getGridDivisionsY(){
        return warp->getGridDivisionsY();
    };
    
    void toggleShowWarpGrid(){
        warp->toggleShowWarpGrid();
    };
    void setShowWarpGrid(bool b){
        warp->setShowWarpGrid(b);
    };
    bool getShowWarpGrid(){
        return warp->getShowWarpGrid();
    };
    
    void toggleDoWarp(){
        warp->toggleDoWarp();
    }
    
    void setDoWarp(bool b){
        warp->setDoWarp(b);
    }
    
    bool getDoWarp(){
        return warp->getDoWarp();
    }
    
    ofFbo& getFBO(){
        return warp->getFBO();
    };
    
    ofTexture& getTextureReference(){
        return warp->getTextureReference();
    };
    
    void setControlPoints(vector<GLfloat> controlPoints){
        warp->setControlPoints(controlPoints);
    };
    
    vector<GLfloat> getControlPoints(){
        return warp->getControlPoints();
    };
    vector<GLfloat>& getControlPointsReference(){
        return warp->getControlPointsReference();
    };
    
private:
    
    ofxBezierWarp* warp;
    
};

    

#endif
