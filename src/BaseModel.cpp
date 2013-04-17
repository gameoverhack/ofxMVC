//
//  BaseModel.cpp
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "BaseModel.h"

//--------------------------------------------------------------
BaseModel::BaseModel() {
    //ofxLogNotice() << "Constructing BaseModel" << endl;
    padLength = 15;
}

//--------------------------------------------------------------
BaseModel::~BaseModel() {
    ofxLogNotice() << "Destroying BaseModel" << endl;
    reset();
}

/********************************************************
 * Load and save a model as TEXT, BIN or XML            *
 ********************************************************/

//--------------------------------------------------------------
bool BaseModel::save(string filename, ArchiveType archiveType){
    Serializer.saveClass(filename, (*this), archiveType);
}

//--------------------------------------------------------------
bool BaseModel::load(string filename, ArchiveType archiveType){
    Serializer.loadClass(filename, (*this), archiveType);
}

void BaseModel::reset(){
    // clean up memeory
    intProps.clear();
    floatProps.clear();
    stringProps.clear();
    boolProps.clear();
}

//--------------------------------------------------------------
bool BaseModel::addStateGroup(StateGroup stateGroup){
    map<string, StateGroup>::iterator it = stateGroups.find(stateGroup.name);
    if(it == stateGroups.end()){
        stateGroups[stateGroup.name] = stateGroup;
        return true;
    }else{
        ofLogError() << "State Group already exists: " << stateGroup.name << endl;
        return false;
    }
}

//--------------------------------------------------------------
bool BaseModel::removeStateGroup(string name){
    map<string, StateGroup>::iterator it = stateGroups.find(name);
    if(it != stateGroups.end()){
        stateGroups.erase(it);
        return true;
    }else{
        ofLogError() << "State Group doesn't exist: " << name << endl;
        return false;
    }
}

//--------------------------------------------------------------
StateGroup & BaseModel::getStateGroup(string name){
    map<string, StateGroup>::iterator it = stateGroups.find(name);
    if(it != stateGroups.end()){
        return it->second;
    }else{
        ofLogError() << "State Group doesn't exist: " << name << endl;
        StateGroup s;
        return s;
    }
}

//--------------------------------------------------------------
string BaseModel::getAllStatesAsString(){
    ostringstream os;
    for(map<string, StateGroup>::iterator it = stateGroups.begin(); it != stateGroups.end(); it++){
        os << it->second;
    }
    return os.str();
}

/********************************************************
 * Getters and setters for simple int, float, string	*
 * properties using boost::any and std::map. These		*
 * should NOT be used where efficiency really really	*
 * matters but still seem pretty quick!					*
 ********************************************************/

// overloaded property setters for PoD
//--------------------------------------------------------------
void BaseModel::setProperty(string property, int value){
    intProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, float value){
    floatProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, string value){
    stringProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, bool value){
    boolProps[property] = value;
}

// overloaded property setters for vector of PoD
//--------------------------------------------------------------
void BaseModel::setProperty(string property, vector<int> value){
    intVecProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, vector<float> value){
    floatVecProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, vector<string> value){
    stringVecProps[property] = value;
}

//--------------------------------------------------------------
void BaseModel::setProperty(string property, vector<bool> value){
    boolVecProps[property] = value;
}

//--------------------------------------------------------------
string BaseModel::getAllPropsAsString(){
    ostringstream os;
    for(map<string, int>::iterator it = intProps.begin(); it != intProps.end(); it++){
        string first = it->first;
        string second = ofToString(it->second);
        string type = "i";
        os << pad(first) << " = " << pad(second) << " (" << type << ")" << endl;
    }
    for(map<string, float>::iterator it = floatProps.begin(); it != floatProps.end(); it++){
        string first = it->first;
        string second = ofToString(it->second);
        string type = "f";
        os << pad(first) << " = " << pad(second) << " (" << type << ")" << endl;
    }
    for(map<string, string>::iterator it = stringProps.begin(); it != stringProps.end(); it++){
        string first = it->first;
        string second = ofToString(it->second);
        string type = "s";
        os << pad(first) << " = " << pad(second) << " (" << type << ")" << endl;
    }
    for(map<string, bool>::iterator it = boolProps.begin(); it != boolProps.end(); it++){
        string first = it->first;
        string second = it->second ? (string)"TRUE" : (string)"FALSE";
        string type = "b";
        os << pad(first) << " = " << pad(second) << " (" << type << ")" << endl;
    }
    
    int vecPrintSizeLimit = 4;
    
    for(map<string, vector<int> >::iterator it = intVecProps.begin(); it != intVecProps.end(); it++){
        string first = it->first;
        string second = "";
        bool dotted = false;
        for(int i = 0; i < it->second.size(); i++){
            if(i > floor(vecPrintSizeLimit/2.0) && i < it->second.size() - floor(vecPrintSizeLimit/2.0)){
                if(!dotted){
                    second += " ... ";
                    dotted = true;
                }
                continue;
            }else{
                if(i > 0) second += ", ";
                second += ofToString(it->second[i]);
            }
        }
        string type = "v(" + ofToString(it->second.size()) + ")<i>";
        os << pad(first) << " = " << second << " (" << type << ")" << endl;
    }
    for(map<string, vector<float> >::iterator it = floatVecProps.begin(); it != floatVecProps.end(); it++){
        string first = it->first;
        string second = "";
        bool dotted = false;
        for(int i = 0; i < it->second.size(); i++){
            if(i > floor(vecPrintSizeLimit/2.0) && i < it->second.size() - floor(vecPrintSizeLimit/2.0)){
                if(!dotted){
                    second += " ... ";
                    dotted = true;
                }
                continue;
            }else{
                if(i > 0) second += ", ";
                second += ofToString(it->second[i]);
            }
        }
        string type = "v(" + ofToString(it->second.size()) + ")<f>";
        os << pad(first) << " = " << second << " (" << type << ")" << endl;
    }
    for(map<string, vector<string> >::iterator it = stringVecProps.begin(); it != stringVecProps.end(); it++){
        string first = it->first;
        string second = "";
        bool dotted = false;
        for(int i = 0; i < it->second.size(); i++){
            if(i > floor(vecPrintSizeLimit/2.0) && i < it->second.size() - floor(vecPrintSizeLimit/2.0)){
                if(!dotted){
                    second += " ... ";
                    dotted = true;
                }
                continue;
            }else{
                if(i > 0) second += ", ";
                second += it->second[i];
            }
        }
        string type = "v(" + ofToString(it->second.size()) + ")<s>";
        os << pad(first) << " = " << second << " (" << type << ")" << endl;
    }
    for(map<string, vector<bool> >::iterator it = boolVecProps.begin(); it != boolVecProps.end(); it++){
        string first = it->first;
        string second = "";
        bool dotted = false;
        
        for(int i = 0; i < it->second.size(); i++){
            if(i > floor(vecPrintSizeLimit/2.0) && i < it->second.size() - floor(vecPrintSizeLimit/2.0)){
                if(!dotted){
                    second += " ... ";
                    dotted = true;
                }
                continue;
            }else{
                if(i > 0) second += ", ";
                second += it->second[i] ? (string)"TRUE" : (string)"FALSE";
            }
        }
        string type = "v(" + ofToString(it->second.size()) + ")<b>";
        os << pad(first) << " = " << second << " (" << type << ")" << endl;
    }
    return os.str();
}

//--------------------------------------------------------------
inline string BaseModel::pad(string & t_string) {
    
	// possibly a more elegant sprintf solution for this but can't work out how to
	// dynamically set sprintf(objWithWhiteSpace, "%" + ofToString(_padLength) + "s", objectName) ???
    
	string paddedString = t_string;
	int _padLength = 0;
	
	// check length and adjust overall pad if the objectName is longer than the current padLength
	if (t_string.size() > padLength && t_string.size() <= 25) padLength = t_string.size();
	
	_padLength = padLength - t_string.size();
	
	for (int i = 0; i < _padLength; i++) paddedString += " ";
	
	return paddedString;
}
