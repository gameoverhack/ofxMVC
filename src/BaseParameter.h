//
//  BaseParameter.h
//
//  Created by gameover on 13/06/14.
//  Copyright (c) 2014 trace media. All rights reserved.
//

//#ifndef __H_BASEPARAMETER
//#define __H_BASEPARAMETER

#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "ofxLogger.h"

template<typename T>
class Parameter;

class BaseParameter{
    
public:
    
    BaseParameter(){};//cout << "Create BaseParameter" << endl;}
    virtual ~BaseParameter(){};//cout << "Destroy BaseParameter" << endl;}
    
    virtual string getName() const{return "";}
	virtual void setName(string name){}
	virtual string toString() const{}
	virtual void fromString(string str){}
	virtual string type() const{}
    
//    template<typename T>
//	Parameter<T> * cast(){
//		return static_cast<Parameter<T> *>(*this);
//	}
    
    template<typename T>
	Parameter<T> & cast(){
		return static_cast<Parameter<T> &>(*this);
	}
    
	template<typename T>
	const Parameter<T> & cast() const{
		return static_cast<const Parameter<T> &>(*this);
	}
    
	friend ostream& operator<<(ostream& os, const BaseParameter& p);
    friend ostream& operator<<(ostream& os, const BaseParameter* p);
    
    friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        // nothing to do here!
    }
    
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(BaseParameter)

ostream& operator<<(ostream& os, const BaseParameter* p){
	os << p->toString();
	return os;
};

ostream& operator<<(ostream& os, const BaseParameter& p){
	os << p.toString();
	return os;
};

//#endif
//
//#ifndef __H_PARAMETER
//#define __H_PARAMETER

template <typename T>
class Parameter : public BaseParameter{
    
public:
    
	Parameter():name(""), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){}
    
	Parameter(const string& _name):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){}
    
	Parameter(const string& _name, const T& _value):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
		set(_value);
	}
    
    Parameter(const string& _name, const T& _value, const T& _min, const T& _max):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
        setRange(_min, _max);
		set(_value);
	}
    
	Parameter(const string& _name, T& _value):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
		set(_value);
	}
    
    Parameter(const string& _name, T& _value, const T& _min, const T& _max):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
        setRange(_min, _max);
		set(_value);
	}
    
	Parameter(const string& _name, T* _value, bool bSetValue = false):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
		set(_value, bSetValue);
	}
    
    Parameter(const string& _name, T* _value, const T& _min, const T& _max, bool bSetValue = false):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
        setRange(_min, _max);
		set(_value, bSetValue);
	}
    
	Parameter(const string& _name, T** _value, bool bSetValue = false):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
		set(_value, bSetValue);
	}
    
    Parameter(const string& _name, T** _value, const T& _min, const T& _max, bool bSetValue = false):name(_name), value(new T), bUseEvents(true), bTrackChanges(true), min(new T), max(new T), bMinSet(false), bMaxSet(false), typeName(typeid(*this).name()){
        setRange(_min, _max);
		set(_value, bSetValue);
	}
    
	~Parameter(){
		delete value;
	}
    
    string type() const{
        return typeName;
    }
    
    void setUseEvents(bool b){
        bUseEvents = b;
    }
    
    void setTrackChanges(bool b){
        bTrackChanges = b;
    }
    
    void setName(const string& _name){
		name = _name;
	}
    
    string getName() const{
		return name;
	}
    
    string toString() const{
        return ofToString((*value), 6);
    }
    
    //    void fromString(string str){
    //        T v;
    //        stringstream sstr;
    //        sstr << str;
    //        sstr >> (*value);
    //        valueChanged();
    //    }
    
	void set(const T& _value){
		(*value) = _value;
        valueChanged();
	}
    
    void set(const string _name, const T& _value){
        name = _name;
		(*value) = _value;
        valueChanged();
	}
    
	void set(T& _reference){
		(*value) = _reference;
        valueChanged();
	}
    
    void set(const string _name, T& _reference){
        name = _name;
		(*value) = _reference;
        valueChanged();
	}
    
    // TODO: so this causes a problem when using with GUI -> can't track value changes :(
	void set(T* _reference, bool bSetValue = false){
		if(!bSetValue) *_reference = (*value);
		value = _reference;
        valueChanged();
	}
    
    void set(const string _name, T* _reference, bool bSetValue = false){
        name = _name;
		if(!bSetValue) *_reference = (*value);
		value = _reference;
        valueChanged();
	}
    
	void set(T** _pointer, bool bSetValue = false){
		if(bSetValue && *_pointer != NULL) (*value) = **_pointer;
		*_pointer = value;
        valueChanged();
	}
    
    void set(const string _name, T** _pointer, bool bSetValue = false){
        name = _name;
		if(bSetValue && *_pointer != NULL) (*value) = **_pointer;
		*_pointer = value;
        valueChanged();
	}
    
    void set(Parameter<T>* other){
        name = other->name;
		value = other->value;
        min = other->min;
        max = other->max;
        valueChanged();
	}
    
    void setRange(const T& _min, const T& _max){
        (*min) = _min;
        (*max) = _max;
        bMinSet = bMaxSet = true;
    }
    
    void setMin(const T& _min){
        (*min) = _min;
        bMinSet = true;
    }
    
    void setMax(const T& _max){
        (*max) = _max;
        bMaxSet = true;
    }
    
    T getMin(){
        return (*min);
    }
    
    T getMax(){
        return (*max);
    }
    
    bool isRangeSet(){
        return (bMinSet && bMaxSet);
    }
    
    T getValue() const{
		return (*value);
	}
    
	const T& getReference(){
		return (*value);
	}
    
    T* getPointer(){
    		return value;
    }
    
    void inline update(){
        if(bTrackChanges){
            if(lvalue != (*value)){
                valueChanged();
            }
        }
    }
    
	const T * operator->() const;
	operator const T & () const;
    
	Parameter<T> & operator=(const Parameter<T> & v);
	const T & operator=(const T & v);
    
	T operator++(int v);
	Parameter<T> & operator++();
    
	T operator--(int v);
	Parameter<T> & operator--();
    
	template<typename OtherType>
	Parameter<T> & operator+=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator-=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator*=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator/=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator%=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator&=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator|=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator^=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator<<=(const OtherType & v);
	template<typename OtherType>
	Parameter<T> & operator>>=(const OtherType & v);
    
    ofEvent<T> parameterEvent;
    
protected:
    
    virtual inline void valueChanged(){
        //if(min != max) (*value) = CLAMP((*value), (*min), (*max));
        if(bTrackChanges) lvalue = (*value);
        if(bUseEvents) ofNotifyEvent(parameterEvent, (*value), this);
    }
    
	string name;
	T * value;
    T lvalue;
    T * min;
    T * max;
    
    string typeName;
    
    bool bMinSet;
    bool bMaxSet;
    bool bUseEvents;
    bool bTrackChanges;
    
    friend class boost::serialization::access;

    template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
        
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseParameter);
        ar & BOOST_SERIALIZATION_NVP(name);
        ar & BOOST_SERIALIZATION_NVP((*value));
        ar & BOOST_SERIALIZATION_NVP((*min));
        ar & BOOST_SERIALIZATION_NVP((*max));
        ar & BOOST_SERIALIZATION_NVP(typeName);
        ar & BOOST_SERIALIZATION_NVP(bMinSet);
        ar & BOOST_SERIALIZATION_NVP(bMaxSet);
        ar & BOOST_SERIALIZATION_NVP(bUseEvents);
        ar & BOOST_SERIALIZATION_NVP(bTrackChanges);

        
	}

//	template<class Archive>
//    void save(Archive & ar, const unsigned int version) const{
//        
//        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseParameter);
//        ar & BOOST_SERIALIZATION_NVP(name);
//        ar & BOOST_SERIALIZATION_NVP((*value));
//        ar & BOOST_SERIALIZATION_NVP((*min));
//        ar & BOOST_SERIALIZATION_NVP((*max));
//        ar & BOOST_SERIALIZATION_NVP(bMinSet);
//        ar & BOOST_SERIALIZATION_NVP(bMaxSet);
//        ar & BOOST_SERIALIZATION_NVP(bUseEvents);
//        ar & BOOST_SERIALIZATION_NVP(bTrackChanges);
//
//	};
//    
//	template<class Archive>
//    void load(Archive & ar, const unsigned int version){
//        
//        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseParameter);
//        ar & BOOST_SERIALIZATION_NVP(name);
//        ar & BOOST_SERIALIZATION_NVP((*value));
//        ar & BOOST_SERIALIZATION_NVP((*min));
//        ar & BOOST_SERIALIZATION_NVP((*max));
//        ar & BOOST_SERIALIZATION_NVP(bMinSet);
//        ar & BOOST_SERIALIZATION_NVP(bMaxSet);
//        ar & BOOST_SERIALIZATION_NVP(bUseEvents);
//        ar & BOOST_SERIALIZATION_NVP(bTrackChanges);
//
//	};
//	
//    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
};

//BOOST_CLASS_VERSION(BaseParameter, 0)

template<typename T>
inline const T * Parameter<T>::operator->() const{
	return value;
}

template<typename T>
inline Parameter<T>::operator const T & () const{
	return (*value);
}

template<typename T>
Parameter<T> & Parameter<T>::operator=(const Parameter<T> & v){
	(*value) = v;
    valueChanged();
	return *this;
}

template<typename T>
inline const T & Parameter<T>::operator=(const T & v){
	(*value) = v;
    valueChanged();
	return (*value);
}

template<typename T>
T Parameter<T>::operator++(int v){
	T r = (*value);
	(*value)++;
    valueChanged();
	return r;
}

template<typename T>
Parameter<T> & Parameter<T>::operator++(){
	++(*value);
    valueChanged();
	return *this;
}

template<typename T>
T Parameter<T>::operator--(int v){
	T r = (*value);
	(*value)--;
    valueChanged();
	return r;
}

template<typename T>
Parameter<T> & Parameter<T>::operator--(){
	--(*value);
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator+=(const OtherType & v){
	(*value) += v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator-=(const OtherType & v){
	(*value) -= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator*=(const OtherType & v){
	(*value) *= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator/=(const OtherType & v){
	(*value) /= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator%=(const OtherType & v){
	(*value) %= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator&=(const OtherType & v){
	(*value) &= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator|=(const OtherType & v){
	(*value) |= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator^=(const OtherType & v){
	(*value) ^= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator<<=(const OtherType & v){
	(*value) <<= v;
    valueChanged();
	return *this;
}

template<typename T>
template<typename OtherType>
Parameter<T> & Parameter<T>::operator>>=(const OtherType & v){
	(*value) >>= v;
    valueChanged();
	return *this;
}

//#endif
