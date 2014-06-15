//
//  BaseWarp.cpp
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "BaseWarp.h"

//--------------------------------------------------------------
BaseWarp::~BaseWarp(){

}

//--------------------------------------------------------------
void BaseWarp::allocate(int w, int h){
    ofLogWarning() << "BaseWarp does not implement allocate with width and height";
}

//--------------------------------------------------------------
void BaseWarp::allocate(int w, int h, int numXPoints, int numYPoints, float pixelsPerGridDivision){
    ofLogWarning() << "BaseWarp does not implement allocate with numXPoints, numYPoints and pixelsPerGridDivision";
}

//--------------------------------------------------------------
void BaseWarp::setWarpGridPosition(float x, float y, float w, float h){
    ofLogWarning() << "BaseWarp does not implement setWarpGridPosition";
}

//--------------------------------------------------------------
void BaseWarp::setWarpGrid(int numXPoints, int numYPoints, bool forceReset){
    ofLogWarning() << "BaseWarp does not implement setWarpGrip";
}

//--------------------------------------------------------------
void BaseWarp::setWarpGridResolution(float pixelsPerGridDivision){
    ofLogWarning() << "BaseWarp does not implement setWarpGridResolution";
}

//--------------------------------------------------------------
void BaseWarp::setWarpGridResolution(int gridDivisionsX, int gridDivisionsY){
    ofLogWarning() << "BaseWarp does not implement setWarpGridResolution";
}

//--------------------------------------------------------------
void BaseWarp::resetWarpGrid(){
    ofLogWarning() << "BaseWarp does not implement resetWarpGrid";
}

//--------------------------------------------------------------
void BaseWarp::resetWarpGridPosition(){
    ofLogWarning() << "BaseWarp does not implement resetWarpGridPosition";
}

//--------------------------------------------------------------
int BaseWarp::getNumXPoints(){
    ofLogWarning() << "BaseWarp does not implement getNumXPoints";
    return 0;
}

//--------------------------------------------------------------
int BaseWarp::getNumYPoints(){
    ofLogWarning() << "BaseWarp does not implement getNumYPoints";
    return 0;
}

//--------------------------------------------------------------
int BaseWarp::getGridDivisionsX(){
    ofLogWarning() << "BaseWarp does not implement getGridDivisionsX";
    return 0;
}

//--------------------------------------------------------------
int BaseWarp::getGridDivisionsY(){
    ofLogWarning() << "BaseWarp does not implement getGridDivisionsY";
    return 0;
}

//--------------------------------------------------------------
void BaseWarp::toggleShowWarpGrid(){
    ofLogWarning() << "BaseWarp does not implement toggleShowWarpGrid";
}

//--------------------------------------------------------------
void BaseWarp::setShowWarpGrid(bool b){
    ofLogWarning() << "BaseWarp does not implement setShowWarpGrid";
}

//--------------------------------------------------------------
bool BaseWarp::getShowWarpGrid(){
    ofLogWarning() << "BaseWarp does not implement getShowWarpGrid";
    return false;
}

//--------------------------------------------------------------
void BaseWarp::setControlPoints(vector<float> controlPoints){
    ofLogWarning() << "BaseWarp does not implement setControlPoints";
}

//--------------------------------------------------------------
vector<float> BaseWarp::getControlPoints(){
    ofLogWarning() << "BaseWarp does not implement getControlPoints";
    vector<float> v;
    return v;
}

//--------------------------------------------------------------
vector<float>& BaseWarp::getControlPointsReference(){
    ofLogWarning() << "BaseWarp does not implement getControlPointsReference";
    vector<float> v;
    return v;
}

//--------------------------------------------------------------
//void BaseWarp::drawWarpGrid(float x, float y, float w, float h){
//    ofLogWarning() << "BaseWarp does not implement drawWarpGrid";
//}
