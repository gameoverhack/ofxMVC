//
//  BezierWarp.cpp
//  emptyExample
//
//  Created by game over on 7/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "BezierWarp.h"

GLfloat texpts [2][2][2] = {
    { {0, 0}, {1, 0} },	{ {0, 1}, {1, 1} }
};

//--------------------------------------------------------------
BezierWarp::BezierWarp(){
    currentCntrlX = -1;
    currentCntrlY = -1;
    numXPoints = 0;
    numYPoints = 0;
    width = 0;
    height = 0;
    warpX = 0;
    warpY = 0;
    warpWidth = 0;
    warpHeight = 0;
    bShowWarpGrid = false;
    bWarpPositionDiff = false;
}

//--------------------------------------------------------------
BezierWarp::~BezierWarp(){
    //fbo.destroy();
    cntrlPoints.clear();
}

//--------------------------------------------------------------
void BezierWarp::allocate(int w, int h){
	allocate(w, h, 5, 5, 50);
}

//--------------------------------------------------------------
void BezierWarp::allocate(int _w, int _h, int _numXPoints, int _numYPoints, float pixelsPerGridDivision){

    //disable arb textures (so we use texture 2d instead)
    ofDisableArbTex();

    if(_w == 0 || _h == 0 || _numXPoints == 0 || _numYPoints == 0){
        ofLogError() << "Cannot accept 0 as value for w, h numXPoints or numYPoints";
        return;
    }

    if(_w != width || _h != height){

        // the fbo texture needs to be a power of 2 because
        // we are using TEXTURE_2D instead of ARB textures
        // so we store requested width and height seperately

        width = _w;
        height = _h;

        fbo.allocate(ofNextPow2(width), ofNextPow2(height));
        ofLogVerbose() << "Allocating bezier fbo texture as: " << fbo.getWidth() << " x " << fbo.getHeight();
    }

    setWarpGrid(_numXPoints, _numYPoints);

    //set up texture map for bezier surface
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);

    setWarpGridResolution(pixelsPerGridDivision);
	ofEnableArbTex();
    //glShadeModel(GL_FLAT);

}

//--------------------------------------------------------------
void BezierWarp::begin(){
    fbo.begin();
    ofPushMatrix();
    ofScale(fbo.getWidth()/width, fbo.getHeight()/height, 1.0f);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

//--------------------------------------------------------------
void BezierWarp::end(){
    ofPopMatrix();
    fbo.end();
}

//--------------------------------------------------------------
void BezierWarp::draw(){
    draw(0, 0, width, height);
}

//--------------------------------------------------------------
void BezierWarp::draw(float x, float y){
    draw(x, y, width, height);
}

//--------------------------------------------------------------
void BezierWarp::draw(float x, float y, float w, float h){

    //if(!fbo.isAllocated()) return;

    // upload the bezier control points to the map surface
    // this can be done just once (or when control points change)
    // if there is only one bezier surface - but with multiple
    // it needs to be done every frame
    //glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, numXPoints, 0, 1, numXPoints * 3, numYPoints, &(cntrlPoints[0]));

    ofPushMatrix();

    ofTranslate(x, y);
    ofScale(w/width, h/height);

    ofTexture & fboTex = fbo.getTextureReference();

    fboTex.bind();
    {
        glEvalMesh2(GL_FILL, 0, gridDivX, 0, gridDivY);
    }
    fboTex.unbind();

    ofPopMatrix();

    if(bShowWarpGrid){
        if(bWarpPositionDiff){
            drawWarpGrid(warpX, warpY, warpWidth, warpHeight);
        }else{
            setWarpGridPosition(x, y, w, h);
        }
    }
}

//--------------------------------------------------------------
void BezierWarp::drawWarpGrid(float x, float y, float w, float h){

    ofPushStyle();
    ofPushMatrix();

    ofSetColor(255, 255, 255);
    ofTranslate(x, y);
    ofScale(w/width, h/height);

    glEvalMesh2(GL_LINE, 0, gridDivX, 0, gridDivY);

    for(int i = 0; i < numYPoints; i++){
        for(int j = 0; j < numXPoints; j++){
            ofFill();
            ofSetColor(255, 0, 0);
            ofCircle(cntrlPoints[(i*numXPoints+j)*3+0], cntrlPoints[(i*numXPoints+j)*3+1], 5);
            ofNoFill();
        }
    }

    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void BezierWarp::setWarpGrid(int _numXPoints, int _numYPoints, bool forceReset){

    if(_numXPoints != numXPoints || _numYPoints != numYPoints) forceReset = true;

    if(_numXPoints < 2 || _numYPoints < 2){
        ofLogError() << "Can't have less than 2 X or Y grid points";
        return;
    }

    if(forceReset){

        numXPoints = _numXPoints;
        numYPoints = _numYPoints;

        // calculate an even distribution of X and Y control points across fbo width and height
        cntrlPoints.resize(numXPoints * numYPoints * 3);
        for(int i = 0; i < numYPoints; i++){
            GLfloat x, y;
            y = (height / (numYPoints - 1)) * i;
            for(int j = 0; j < numXPoints; j++){
                x = (width / (numXPoints - 1)) * j;
                cntrlPoints[(i*numXPoints+j)*3+0] = x;
                cntrlPoints[(i*numXPoints+j)*3+1] = y;
                cntrlPoints[(i*numXPoints+j)*3+2] = 0;
                //cout << x << ", " << y << ", " << "0" << endl;
            }
        }
    }

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, numXPoints, 0, 1, numXPoints * 3, numYPoints, &(cntrlPoints[0]));
}

//--------------------------------------------------------------
void BezierWarp::setWarpGridPosition(float x, float y, float w, float h){
    warpX = x;
    warpY = y;
    warpWidth = w;
    warpHeight = h;
    bWarpPositionDiff = true;
}

//--------------------------------------------------------------
void BezierWarp::setWarpGridResolution(float pixelsPerGridDivision){
    setWarpGridResolution(ceil(fbo.getWidth() / pixelsPerGridDivision), ceil(fbo.getHeight() / pixelsPerGridDivision));
}

//--------------------------------------------------------------
void BezierWarp::setWarpGridResolution(int gridDivisionsX, int gridDivisionsY){
    // NB: at the moment this sets the resolution for all mapGrid
    // objects (since I'm not calling it every frame as it is expensive)
    // so if you try to set different resolutions
    // for different instances it won't work as expected

    gridDivX = gridDivisionsX;
    gridDivY = gridDivisionsY;
    glMapGrid2f(gridDivX, 0, 1, gridDivY, 0, 1);
}

//--------------------------------------------------------------
void BezierWarp::resetWarpGrid(){
    setWarpGrid(numXPoints, numYPoints, true);
}

//--------------------------------------------------------------
void BezierWarp::resetWarpGridPosition(){
    bWarpPositionDiff = false;
}

//--------------------------------------------------------------
float BezierWarp::getWidth(){
    return width;
}

//--------------------------------------------------------------
float BezierWarp::getHeight(){
    return height;
}

//--------------------------------------------------------------
int BezierWarp::getNumXPoints(){
    return numXPoints;
}

//--------------------------------------------------------------
int BezierWarp::getNumYPoints(){
    return numYPoints;
}

//--------------------------------------------------------------
int BezierWarp::getGridDivisionsX(){
    return gridDivX;
}

//--------------------------------------------------------------
int BezierWarp::getGridDivisionsY(){
    return gridDivY;
}

//--------------------------------------------------------------
void BezierWarp::toggleShowWarpGrid(){
    setShowWarpGrid(!getShowWarpGrid());
}

//--------------------------------------------------------------
void BezierWarp::setShowWarpGrid(bool b){
    bShowWarpGrid = b;
    if(bShowWarpGrid){
        ofRegisterMouseEvents(this);
    }else{
        ofUnregisterMouseEvents(this);
    }
}

//--------------------------------------------------------------
bool BezierWarp::getShowWarpGrid(){
    return bShowWarpGrid;
}

//--------------------------------------------------------------
ofFbo& BezierWarp::getFBO(){
    return fbo;
}

//--------------------------------------------------------------
ofTexture& BezierWarp::getTextureReference(){
    return fbo.getTextureReference();
}

//--------------------------------------------------------------
void BezierWarp::setControlPoints(vector<GLfloat> _cntrlPoints){
    cntrlPoints.clear();
    cntrlPoints = _cntrlPoints;
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, numXPoints, 0, 1, numXPoints * 3, numYPoints, &(cntrlPoints[0]));
}

//--------------------------------------------------------------
vector<GLfloat> BezierWarp::getControlPoints(){
    return cntrlPoints;
}

//--------------------------------------------------------------
vector<GLfloat>& BezierWarp::getControlPointsReference(){
    return cntrlPoints;
}

//--------------------------------------------------------------
void BezierWarp::mouseMoved(ofMouseEventArgs & m){

}

//--------------------------------------------------------------
void BezierWarp::mouseDragged(ofMouseEventArgs & m){

    if(!bShowWarpGrid) mouseReleased(m);

    float x = m.x;
    float y = m.y;

    if(bWarpPositionDiff){
        x = (m.x - warpX) * width/warpWidth;
        y = (m.y - warpY) * height/warpHeight;
    }

    if(currentCntrlX != -1 && currentCntrlY != -1){
        cntrlPoints[(currentCntrlX*numXPoints+currentCntrlY)*3+0] = x;
        cntrlPoints[(currentCntrlX*numXPoints+currentCntrlY)*3+1] = y;
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, numXPoints, 0, 1, numXPoints * 3, numYPoints, &(cntrlPoints[0]));
    }
}

//--------------------------------------------------------------
void BezierWarp::mousePressed(ofMouseEventArgs & m){

    if(!bShowWarpGrid) mouseReleased(m);

    float x = m.x;
    float y = m.y;

    if(bWarpPositionDiff){
        x = (m.x - warpX) * width/warpWidth;
        y = (m.y - warpY) * height/warpHeight;
    }

    float dist = 10.0f;

    for(int i = 0; i < numYPoints; i++){
        for(int j = 0; j < numXPoints; j++){
            if(x - cntrlPoints[(i*numXPoints+j)*3+0] >= -dist && x - cntrlPoints[(i*numXPoints+j)*3+0] <= dist &&
               y - cntrlPoints[(i*numXPoints+j)*3+1] >= -dist && y - cntrlPoints[(i*numXPoints+j)*3+1] <= dist){
                currentCntrlX = i;
                currentCntrlY = j;
            }
        }
    }
}

//--------------------------------------------------------------
void BezierWarp::mouseReleased(ofMouseEventArgs & m){
    currentCntrlX = -1;
    currentCntrlY = -1;
}
