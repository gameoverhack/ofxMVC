//
//  NoWarp.cpp
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "NoWarp.h"

//--------------------------------------------------------------
NoWarp::NoWarp(){
    
}

//--------------------------------------------------------------
NoWarp::~NoWarp(){
    
}

//--------------------------------------------------------------
void NoWarp::allocate(int w, int h){
    fbo.allocate(w, h);
}

//--------------------------------------------------------------
void NoWarp::begin(){
    fbo.begin();
    glPushMatrix();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

//--------------------------------------------------------------
void NoWarp::end(){
    glPopMatrix();
    fbo.end();
}

//--------------------------------------------------------------
void NoWarp::draw(){
    draw(0, 0, width, height);
}

//--------------------------------------------------------------
void NoWarp::draw(float x, float y){
    draw(x, y, width, height);
}

//--------------------------------------------------------------
void NoWarp::draw(float x, float y, float w, float h){
    fbo.draw(x, y, w, h);
}

//--------------------------------------------------------------
float NoWarp::getWidth(){
    return width;
}

//--------------------------------------------------------------
float NoWarp::getHeight(){
    return height;
}

//--------------------------------------------------------------
ofFbo& NoWarp::getFBO(){
    return fbo;
}

//--------------------------------------------------------------
ofTexture& NoWarp::getTextureReference(){
    return fbo.getTextureReference();
}

//--------------------------------------------------------------
void NoWarp::mouseMoved(ofMouseEventArgs & e){
    
}

//--------------------------------------------------------------
void NoWarp::mouseDragged(ofMouseEventArgs & e){
    
}

//--------------------------------------------------------------
void NoWarp::mousePressed(ofMouseEventArgs & e){
    
}

//--------------------------------------------------------------
void NoWarp::mouseReleased(ofMouseEventArgs & e){
    
}