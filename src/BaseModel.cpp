//
//  BaseModel.cpp
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "BaseModel.h"
#include "SerializationExports.h"

//--------------------------------------------------------------
BaseModel::BaseModel() {
    ofxLogVerbose() << "Constructing BaseModel" << endl;
	gui = NULL;
    reset();
}

//--------------------------------------------------------------
BaseModel::~BaseModel() {
    ofxLogVerbose() << "Destroying BaseModel" << endl;
    reset();
}

//--------------------------------------------------------------
bool BaseModel::save(const string& filename, const ArchiveType& archiveType){
    return Serializer.saveClass(filename, (*this), archiveType);
}

//--------------------------------------------------------------
bool BaseModel::load(const string& filename, const ArchiveType& archiveType){
    return Serializer.loadClass(filename, (*this), archiveType);
}

//--------------------------------------------------------------
void BaseModel::reset(){
    //reset defaults
    padLength = 15;
    applicationName = applicationPath = "";
    removeAllProperties();
}
