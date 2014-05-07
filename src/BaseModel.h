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

//#define USE_OPENFRAMEWORKS_TYPES 1

#define USE_OFXUI 1

#ifdef USE_OFXUI
#include "ofxUI.h"

enum ofxMVCPropertyType{
    OFX_MVC_TYPE_INT = 0,
    OFX_MVC_TYPE_FLOAT,
    OFX_MVC_TYPE_STRING,
    OFX_MVC_TYPE_BOOL,
    OFX_MVC_TYPE_VINT = 0,
    OFX_MVC_TYPE_VFLOAT,
    OFX_MVC_TYPE_VSTRING,
    OFX_MVC_TYPE_VBOOL
#ifdef USE_OPENFRAMEWORKS_TYPES
    ,
    OFX_MVC_TYPE_POINT,
    OFX_MVC_TYPE_RECTANGLE,
    OFX_MVC_TYPE_COLOR,
    OFX_MVC_TYPE_VPOINT,
    OFX_MVC_TYPE_VRECTANGLE,
    OFX_MVC_TYPE_VCOLOR
#endif
};

typedef struct{
    string name;
    ofxUIWidgetType wtype;
    ofxMVCPropertyType ptype;
    double min;
    double max;
} UIType;
#endif

#ifdef USE_OPENFRAMEWORKS_TYPES
#include "ofTypes.h"
#endif

#include "ofxLogger.h"
#include "SerializationUtils.h"
#include "States.h"
#include <libproc.h>
#import <ifaddrs.h>
#import <arpa/inet.h>

class BaseModel {
    
public:
    
    BaseModel();
    virtual ~BaseModel();
	
    bool save(string filname, ArchiveType archiveType);
    bool load(string filname, ArchiveType archiveType);
    void reset();
    
    // state getter/setters
    bool addStateGroup(StateGroup stateGroup);
    bool removeStateGroup(string name);
    
    StateGroup & getStateGroup(string name);
    
    string getAllStatesAsString();
    
    // generic property getter/setters for PoD
	void setProperty(string property, int value, bool bGui = false, int gMin = 0, int gMax = 0);
    void setProperty(string property, float value, bool bGui = false, float gMin = 0, float gMax = 0);
    void setProperty(string property, string value, bool bGui = false);
    void setProperty(string property, bool value, bool bGui = false);
#ifdef USE_OPENFRAMEWORKS_TYPES
	void setProperty(string property, ofPoint);
    void setProperty(string property, ofRectangle);
#endif
    
    // generic property getter/setters for vector of PoD
	void setProperty(string property, vector<int> value);
    void setProperty(string property, vector<float> value);
    void setProperty(string property, vector<string> value);
    void setProperty(string property, vector<bool> value);
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    void setProperty(string property, vector<ofPoint> value);
    void setProperty(string property, vector<ofRectangle> value);
#endif
    
    // get any ANY property in a map using templates
    // no cast necessary: use T var = appModel->getProperty<T>("varName")
    template<typename T>
    T getProperty(string property){
        T t; // using this to overload the template isn't pretty but waaay easier than speciliazation
        return getProperty(property, t);
    };
    
    template<typename T>
    T& getPropertyReference(string property){
        T t; // using this to overload the template isn't pretty but waaay easier than speciliazation
        return getProperty(property, t);
    };
    
    template<typename T>
    bool hasProperty(string property){
        T t; // using this to overload the template isn't pretty but waaay easier than speciliazation
        return hasProperty(property, t);
    };
    
    template<typename T>
    void removeProperty(string property){
        T t; // using this to overload the template isn't pretty but waaay easier than speciliazation
        removeProperty(property, t);
    };
    
    void removeAllProperties();
    
    // template overloads for PoD getProperty
    int getProperty(string property, int & value){
        assert(intProps.count(property) != 0);
        return intProps[property];
    };
    float getProperty(string property, float & value){
        assert(floatProps.count(property) != 0);
        return floatProps[property];
    };
    string getProperty(string property, string & value){
        assert(stringProps.count(property) != 0);
        return stringProps[property];
    };
    bool getProperty(string property, bool & value){
        assert(boolProps.count(property) != 0);
        return boolProps[property];
    };
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    ofPoint getProperty(string property, ofPoint & value){
        assert(ofPointProps.count(property) != 0);
        return ofPointProps[property];
    };
    ofRectangle getProperty(string property, ofRectangle & value){
        assert(ofRectangleProps.count(property) != 0);
        return ofRectangleProps[property];
    };
#endif
    
    // template overloads for vector of PoD getProperty
    vector<int>& getProperty(string property, vector<int> & value){
        assert(intVecProps.count(property) != 0);
        return intVecProps[property];
    };
    vector<float>& getProperty(string property, vector<float> & value){
        assert(floatVecProps.count(property) != 0);
        return floatVecProps[property];
    };
    vector<string>& getProperty(string property, vector<string> & value){
        assert(stringVecProps.count(property) != 0);
        return stringVecProps[property];
    };
    vector<bool>& getProperty(string property, vector<bool> & value){
        assert(boolVecProps.count(property) != 0);
        return boolVecProps[property];
    };
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    vector<ofPoint>& getProperty(string property, vector<ofPoint> & value){
        assert(ofPointVecProps.count(property) != 0);
        return ofPointVecProps[property];
    };
    vector<ofRectangle>& getProperty(string property, vector<ofRectangle> & value){
        assert(ofRectangleVecProps.count(property) != 0);
        return ofRectangleVecProps[property];
    };
#endif
    
    bool hasProperty(string property, int & value){
        if(intProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, float & value){
        if(floatProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, string & value){
        if(stringProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, bool & value){
        if(boolProps.count(property) != 0) return true;
        return false;
    };
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    bool hasProperty(string property, ofPoint & value){
        if(ofPointProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, ofRectangle & value){
        if(ofRectangleProps.count(property) != 0) return true;
        return false;
    };
#endif
    
    // template overloads for vector of PoD getProperty
    bool hasProperty(string property, vector<int> & value){
        if(intVecProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, vector<float> & value){
        if(floatVecProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, vector<string> & value){
        if(stringVecProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, vector<bool> & value){
        if(boolVecProps.count(property) != 0) return true;
        return false;
    };
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    bool hasProperty(string property, vector<ofPoint> & value){
        if(ofPointVecProps.count(property) != 0) return true;
        return false;
    };
    bool hasProperty(string property, vector<ofRectangle> & value){
        if(ofRectangleVecProps.count(property) != 0) return true;
        return false;
    };
#endif
    
    // generic property removers for PoD
	void removeProperty(string property, int value);
    void removeProperty(string property, float value);
    void removeProperty(string property, string value);
    void removeProperty(string property, bool value);
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    void removeProperty(string property, ofPoint value);
    void removeProperty(string property, ofRectangle value);
#endif
    
    // generic property removers for vector of PoD
	void removeProperty(string property, vector<int> value);
    void removeProperty(string property, vector<float> value);
    void removeProperty(string property, vector<string> value);
    void removeProperty(string property, vector<bool> value);
    
#ifdef USE_OPENFRAMEWORKS_TYPES
    void removeProperty(string property, vector<ofPoint> value);
    void removeProperty(string property, vector<ofRectangle> value);
#endif
    
    void toggleProperty(string property);
    
    friend ostream& operator<< (ostream &os, BaseModel &bm);
    
    string getAllPropsAsString();
    
    string getApplicationPath();
    string getApplicationName();
    string getIPAddress();
    
    bool operator==(const BaseModel& other) {
        return (intProps == other.intProps &&
                floatProps == other.floatProps &&
                stringProps == other.stringProps &&
                boolProps == other.boolProps &&
                intVecProps == other.intVecProps &&
                floatVecProps == other.floatVecProps &&
                stringVecProps == other.stringVecProps &&
                boolVecProps == other.boolVecProps
#ifdef USE_OPENFRAMEWORKS_TYPES
                &&
                ofPointProps == other.ofPointProps &&
                ofRectangleProps == other.ofRectangleProps &&
                ofPointVecProps == other.ofPointVecProps &&
                ofRectangleVecProps == other.ofRectangleVecProps
#endif
                );
    }
    
    bool operator!=(const BaseModel& other) {
        return !(*this == other);
    }
    
    friend void swap(BaseModel& first, BaseModel& second){
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        cout << "Swapping base model" << endl;
        
        swap(first.applicationName, second.applicationName);
        swap(first.applicationPath, second.applicationPath);
        
        swap(first.intProps, second.intProps);
        swap(first.floatProps, second.floatProps);
        swap(first.stringProps, second.stringProps);
        swap(first.boolProps, second.boolProps);
        
        swap(first.intVecProps, second.intVecProps);
        swap(first.floatVecProps, second.floatVecProps);
        swap(first.stringVecProps, second.stringVecProps);
        swap(first.boolVecProps, second.boolVecProps);
        

#ifdef USE_OPENFRAMEWORKS_TYPES
        swap(first.ofPointProps, second.ofPointProps);
        swap(first.ofRectangleProps, second.ofRectangleProps);
        swap(first.ofPointVecProps, second.ofPointVecProps);
        swap(first.ofRectangleVecProps, second.ofRectangleVecProps);
#endif
        
#ifdef USE_OFXUI
        swap(first.gui, second.gui);
        swap(first.guitypes, second.guitypes);
        swap(first.gLabel, second.gLabel);
#endif
    }
    
    BaseModel& operator=(BaseModel other) {
        cout << "Assigning base model" << endl;
        swap(*this, other);
        return *this;
    }
    
//    BaseModel( const BaseModel& other ){
//        cout << "Copying base model" << endl;
//        
//        applicationName = other.applicationName;
//        applicationPath = other.applicationPath;
//        
//        intProps = other.intProps;
//        floatProps = other.floatProps;
//        stringProps = other.stringProps;
//        boolProps = other.boolProps;
//        
//        intVecProps = other.intVecProps;
//        floatVecProps = other.floatVecProps;
//        stringVecProps = other.stringVecProps;
//        boolVecProps = other.boolVecProps;
//        
//#ifdef USE_OPENFRAMEWORKS_TYPES
////        ofPointProps = other.ofPointProps;
////        ofRectangleProps = other.ofRectangleProps;
////        ofPointVecProps = other.ofPointVecProps;
////        ofRectangleVecProps = other.ofRectangleVecProps;
//#endif
//        
//#ifdef USE_OFXUI
////        gui = other.gui;
////        guitypes = other.guitypes;
////        gLabel = other.gLabel;
//#endif
//    }
    
#ifdef USE_OFXUI
    void setupGui(string label = "BaseModel", float x = 0.0f, float y = 0.0f, float w = 200.0f, float h = 0.0f);
    void setGuiPosition(ofPoint p);
    void setGuiPosition(float x, float y);
    void setGuiLabel(string label);
    ofxUIRectangle* getGuiRect();
    ofxUIScrollableCanvas * getGui();
#endif
    
protected:
    
#ifdef USE_OFXUI
    ofxUIScrollableCanvas * gui;
    vector<UIType> guitypes;
    string gLabel;
    void addGuiElement(UIType t);
#endif
    
    
    // state storage
    map<string, StateGroup> stateGroups;
    
    // generic property helper methods
    inline string pad(string & objectName);
	int padLength;
    
    // generic property storage
    map<string, int> intProps;
    map<string, float> floatProps;
    map<string, string> stringProps;
    map<string, bool> boolProps;
    
    map<string, vector<int> > intVecProps;
    map<string, vector<float> > floatVecProps;
    map<string, vector<string> > stringVecProps;
    map<string, vector<bool> > boolVecProps;

#ifdef USE_OPENFRAMEWORKS_TYPES
    map<string, ofPoint> ofPointProps;
    map<string, ofRectangle> ofRectangleProps;
    map<string, vector<ofPoint> > ofPointVecProps;
    map<string, vector<ofRectangle> > ofRectangleVecProps;
#endif
    
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & BOOST_SERIALIZATION_NVP(intProps);
        ar & BOOST_SERIALIZATION_NVP(floatProps);
        ar & BOOST_SERIALIZATION_NVP(stringProps);
        ar & BOOST_SERIALIZATION_NVP(boolProps);
        ar & BOOST_SERIALIZATION_NVP(intVecProps);
        ar & BOOST_SERIALIZATION_NVP(floatVecProps);
        ar & BOOST_SERIALIZATION_NVP(stringVecProps);
        ar & BOOST_SERIALIZATION_NVP(boolVecProps);
#ifdef USE_OPENFRAMEWORKS_TYPES
        ar & BOOST_SERIALIZATION_NVP(ofPointProps);
        ar & BOOST_SERIALIZATION_NVP(ofRectangleProps);
        ar & BOOST_SERIALIZATION_NVP(ofPointVecProps);
        ar & BOOST_SERIALIZATION_NVP(ofRectangleVecProps);
#endif
#ifdef USE_OFXUI
        ar & BOOST_SERIALIZATION_NVP(guitypes);
#endif
	}
    
    string applicationName;
    string applicationPath;
    
private:
    
};

inline ostream& operator<<(ostream& os, BaseModel &bm){
	os << bm.getAllPropsAsString();
	return os;
};

#ifdef USE_OFXUI
namespace boost {
    namespace serialization {
        
        template<class Archive>
        void serialize(Archive & ar, UIType & u, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(u.name);
            ar & BOOST_SERIALIZATION_NVP(u.wtype);
            ar & BOOST_SERIALIZATION_NVP(u.ptype);
            ar & BOOST_SERIALIZATION_NVP(u.min);
            ar & BOOST_SERIALIZATION_NVP(u.max);
        };
        
    };
};
#endif

#ifdef USE_OPENFRAMEWORKS_TYPES
namespace boost {
    namespace serialization {
        
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
#endif
    
//typedef Singleton<BaseModel> BaseModelSingleton;					// Global declaration

//static BaseModel * baseModel	= BaseModelSingleton::Instance();

#endif
