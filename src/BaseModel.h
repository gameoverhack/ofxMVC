//
//  BaseModel.h
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

// Singleton definition
#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_
#include "assert.h"
#include <cstdlib>
template <class T>
class Singleton{
public:
    static T* Instance() {
        if(!m_pInstance) m_pInstance = new T;
        assert(m_pInstance !=NULL);
        return m_pInstance;
    };
protected:
    Singleton();
    ~Singleton();
private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_pInstance;
};
template <class T> T* Singleton<T>::m_pInstance=NULL;
#endif

#ifndef _H_BASEMODEL
#define _H_BASEMODEL

#include "BaseGui.h"
#include "BaseParameter.h"
#include "ofxLogger.h"
#include "SerializationUtils.h"
#include "States.h"

#ifdef TARGET_OSX
#include <libproc.h>
#import <ifaddrs.h>
#import <arpa/inet.h>
#endif

using namespace std;
using namespace tr1;

class BaseModel {

public:

    BaseModel();
    virtual ~BaseModel();

    bool save(const string& filname, const ArchiveType& archiveType);
    bool load(const string& filname, const ArchiveType& archiveType);
    void reset();

    template<typename T>
    void setProperty(const string& name, T* value){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            Parameter<T>* parameter = new Parameter<T>(name, value);
            parameter->setUseEvents(false);
            parameter->setTrackChanges(false);
            parameters.insert(pair<string, BaseParameter*>(name, parameter));
            orderedParaVec.push_back(parameter);
            orderedParaMap.insert(pair<string, int>(name, orderedParaVec.size() - 1));
        }else{
            Parameter<T>* parameter = (Parameter<T>*)it->second;
            parameter->set(value);
        }
        
    }
    
    template<typename T>
    void setProperty(const string& name, const T& value){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            Parameter<T>* parameter = new Parameter<T>(name, value);
            parameter->setUseEvents(false);
            parameter->setTrackChanges(false);
            parameters.insert(pair<string, BaseParameter*>(name, parameter));
            orderedParaVec.push_back(parameter);
            orderedParaMap.insert(pair<string, int>(name, orderedParaVec.size() - 1));
        }else{
            Parameter<T>* parameter = (Parameter<T>*)it->second;
            parameter->set(value);
        }
        
    }
    
    template<typename T>
    inline void setProperty(const string& name, const T& value, const T& min, const T& max){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            Parameter<T>* parameter = new Parameter<T>(name, value, min, max);
            parameter->setUseEvents(false);
            parameter->setTrackChanges(false);
            parameters.insert(pair<string, BaseParameter*>(name, parameter));
            orderedParaVec.push_back(parameter);
            orderedParaMap.insert(pair<string, int>(name, orderedParaVec.size() - 1));
        }else{
            Parameter<T>* parameter = (Parameter<T>*)it->second;
            parameter->set(value);
        }
        
    }
    
    template<typename T>
    inline Parameter<T>* getProperty(const string& name){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            cout << "No Property: " << name << endl;
            assert(false);
        }else{
            return (Parameter<T>*)it->second;
        }
        
    }
    
    template<typename T>
    inline const T& getPropertyReference(const string& name){
        return getProperty<T>(name)->getReference();
    }
    
    inline bool addStateGroup(const StateGroup& stateGroup){
        unordered_map<string, StateGroup>::iterator it = stateGroups.find(stateGroup.name);
        if(it == stateGroups.end()){
            stateGroups[stateGroup.name] = stateGroup;
            return true;
        }else{
            ofLogError() << "State Group already exists: " << stateGroup.name << endl;
            return false;
        }
    }

    
    inline bool removeStateGroup(const string& name){
        unordered_map<string, StateGroup>::iterator it = stateGroups.find(name);
        if(it != stateGroups.end()){
            stateGroups.erase(it);
            return true;
        }else{
            ofLogError() << "State Group doesn't exist: " << name << endl;
            return false;
        }
    }

    
    inline StateGroup & getStateGroup(const string& name){
        unordered_map<string, StateGroup>::iterator it = stateGroups.find(name);
        if(it != stateGroups.end()){
            return it->second;
        }else{
            ofLogError() << "State Group doesn't exist: " << name << endl;
            StateGroup s;
            return s;
        }
    }

    
    inline void removeProperty(const string& name){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            cout << "No Property: " << name << endl;
            assert(false);
        }else{
            
            unordered_map<string, int>::iterator itt = orderedParaMap.find(name);
            
            orderedParaVec.erase(orderedParaVec.begin() + itt->second);
            orderedParaMap.erase(itt);
            
            delete it->second;
            parameters.erase(it);
        }
    }

    
    inline void removeAllProperties(){
        
        for(unordered_map<string, BaseParameter*>::iterator it = parameters.begin(); it != parameters.end(); ++it){
            delete it->second;
        }
        
        parameters.clear();
        orderedParaVec.clear();
        orderedParaMap.clear();
        
    }

    
    inline bool hasProperty(const string& name){
        return (parameters.find(name) != parameters.end());
    }

    
    inline void toggleProperty(const string& name){
        
        unordered_map<string, BaseParameter*>::iterator it = parameters.find(name);
        
        if(it == parameters.end()){
            cout << "No Property: " << name << endl;
            assert(false);
        }else{
            
            Parameter<bool>* parameter = (Parameter<bool>*)it->second;
            parameter->set(!parameter->getReference()); // this is dangerous as there is no way to check type!!!
            
        }
        
    }

    inline string getAllStatesAsString(){
        ostringstream os;
        for(unordered_map<string, StateGroup>::iterator it = stateGroups.begin(); it != stateGroups.end(); it++){
            os << it->second;
        }
        return os.str();
    }

    
    inline string getAllPropsAsString(){
        ostringstream os;
        
        for(int i = 0; i < orderedParaVec.size(); i++){
            BaseParameter* parameter = (BaseParameter*)orderedParaVec[i];
            os << parameter->getName() << ": " << parameter->toString() << endl;
        }
        
//        for(unordered_map<string, BaseParameter*>::iterator it = parameters.begin(); it != parameters.end(); ++it){
//            BaseParameter* parameter = (BaseParameter*)it->second;
//            os << it->first << ": " << parameter->toString() << endl;
//        }
        return os.str();
    }
    
    friend ostream& operator<< (ostream &os, BaseModel &bm);

#ifdef TARGET_OSX

    inline string getApplicationName(){
        if(applicationName == ""){
            getApplicationPath();
        }
        return applicationName;
    }

    inline string getApplicationPath(){
        // from http://stackoverflow.com/questions/799679/programatically-retrieving-the-absolute-path-of-an-os-x-command-line-app/1024933#1024933
        if(applicationPath == ""){
            int ret;
            pid_t pid;
            char pathbuf[1024];
            pid = getpid();
            ret = proc_pidpath (pid, pathbuf, sizeof(pathbuf));
            if(ret <= 0){
                ofLogError() << "PID " << pid << " proc_pidpath(): " << strerror(errno);
            }else{
                ofLogVerbose() << "proc " << pid << " path: " << pathbuf;
            }
            applicationPath = string(pathbuf);
            vector<string> pathParts = ofSplitString(applicationPath, "/");
            applicationName = pathParts[pathParts.size() - 1];
        }
        
        return applicationPath;
    }
    
    inline string getIPAddress(){
        
        // TODO: currently working for wifi but does it work for ethernet etc?
        
        struct ifaddrs *interfaces = NULL;
        struct ifaddrs *temp_addr = NULL;
        string wifiAddress = "";
        string cellAddress = "";
        
        // retrieve the current interfaces - returns 0 on success
        if(!getifaddrs(&interfaces)) {
            // Loop through linked list of interfaces
            temp_addr = interfaces;
            while(temp_addr != NULL) {
                sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
                if(sa_type == AF_INET || sa_type == AF_INET6) {
                    string name = temp_addr->ifa_name;
                    string addr = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr); // pdp_ip0
                    cout << "NAME: " << name << " ADDR: " << addr << endl;
                    if(name.find("en") != string::npos) {
                        
                        // Interface is the wifi connection on the iPhone
                        wifiAddress = addr;
                        
                    } else
                        
                        if(name == "pdp_ip0") {
                            // Interface is the cell connection on the iPhone
                            cellAddress = addr;
                        }
                }
                temp_addr = temp_addr->ifa_next;
            }
            // Free memory
            freeifaddrs(interfaces);
        }
        string addr = wifiAddress != "" ? wifiAddress : cellAddress;
        return addr != "" ? addr : "0.0.0.0";
    }
    
#endif

//    bool operator==(const BaseModel& other) {
//        return (parameters == other.parameters && stateGroups == other.stateGroups);
//    }
//
//    bool operator!=(const BaseModel& other) {
//        return !(*this == other);
//    }

    friend void swap(BaseModel& first, BaseModel& second){
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        cout << "Swapping base model" << endl;

        swap(first.applicationName, second.applicationName);
        swap(first.applicationPath, second.applicationPath);
        swap(first.stateGroups, second.stateGroups);
    }

    BaseModel& operator=(BaseModel other) {
        cout << "Assigning base model" << endl;
        swap(*this, other);
        return *this;
    }

protected:

    inline string pad(string & t_string) {
        
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
    
    // parameter storage
    unordered_map<string, BaseParameter*> parameters;
    unordered_map<string, int> orderedParaMap;
    vector<BaseParameter*> orderedParaVec;
    
    // state storage
    unordered_map<string, StateGroup> stateGroups;

	int padLength;

    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
        ar & BOOST_SERIALIZATION_NVP(parameters);
        ar & BOOST_SERIALIZATION_NVP(orderedParaVec);
        ar & BOOST_SERIALIZATION_NVP(orderedParaMap);
	}

    string applicationName;
    string applicationPath;

private:

};

BOOST_CLASS_VERSION(BaseModel, 4)

inline ostream& operator<<(ostream& os, BaseModel &bm){
	os << bm.getAllPropsAsString();
	return os;
};

namespace boost {
    namespace serialization {

		template<class Archive>
        void serialize(Archive & ar, ofColor & c, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(c.r);
            ar & BOOST_SERIALIZATION_NVP(c.g);
            ar & BOOST_SERIALIZATION_NVP(c.b);
			ar & BOOST_SERIALIZATION_NVP(c.a);
        };

        template<class Archive>
        void serialize(Archive & ar, ofPoint & p, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(p.x);
            ar & BOOST_SERIALIZATION_NVP(p.y);
            ar & BOOST_SERIALIZATION_NVP(p.z);
        };

        template<class Archive>
        void serialize(Archive & ar, ofRectangle & r, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(r.x);
            ar & BOOST_SERIALIZATION_NVP(r.y);
            ar & BOOST_SERIALIZATION_NVP(r.height);
            ar & BOOST_SERIALIZATION_NVP(r.width);
        };
    };
};

//typedef Singleton<BaseModel> BaseModelSingleton;					// Global declaration

//static BaseModel * baseModel	= BaseModelSingleton::Instance();

#endif
