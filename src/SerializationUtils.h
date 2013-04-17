/*
 *  SerializationUtils.h
 *
 *  Created by gameover on 20/06/12.
 *  Copyright 2012 Matthew Gingold. All rights reserved.
 *
 */

#ifndef _H_SERIALIZATIONUTILS
#define	_H_SERIALIZATIONUTILS

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "ofMain.h"

#include "ofxLogger.h"

enum ArchiveType{
    ARCHIVE_TEXT,
    ARCHIVE_BINARY,
    ARCHIVE_XML
};

class _Serializer {
public:
    template <class vectorType>
    bool loadVector(string filePath, vector< vectorType > * vec, ArchiveType archiveType) {
        ofxLogNotice() << "Loading serial data: " << filePath << endl;
        std::ifstream ifs(ofToDataPath(filePath).c_str());
        if(ifs.fail()){
            ofxLogError() << "Could not load vector: " << filePath << endl;
            abort(); // Could be a bit over zealous
        }
        boost::archive::text_iarchive ia(ifs);
        ia >> (*vec);
        return true;
    }
    
    template <class vectorType>
    bool saveVector(string filePath, vector< vectorType > * vec, ArchiveType archiveType) {
        ofxLogNotice() << "Saving serial data: " << filePath << endl;
        std::ofstream ofs(ofToDataPath(filePath).c_str());
        if(ofs.fail()){
            ofxLogError() << "Could not save vector: " << filePath << endl;
            abort(); // Could be a bit over zealous
        }
        boost::archive::text_oarchive to(ofs);
        to << (*vec);
        return true;
    }
    
    template <class C>
    bool loadClass(string filePath, C &someClass, ArchiveType archiveType) {
        ofxLogNotice() << "Loading class data: " << filePath << endl;
        std::ifstream ifs(ofToDataPath(filePath).c_str());
        if(ifs.fail()){
            ofxLogError() << "Could not open file stream for loading: " << filePath << endl;
            return false;
        }
        try {
            ofxLogNotice() << "Loading from...";
            switch (archiveType) {
                case ARCHIVE_TEXT:
                {
                    ofxLogNotice() << "ARCHIVE_TEXT" << endl;
                    boost::archive::text_iarchive ia(ifs);
                    ia >> BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
                case ARCHIVE_BINARY:
                {
                    ofxLogNotice() << "ARCHIVE_BINARY" << endl;
                    boost::archive::binary_iarchive ia(ifs);
                    ia >> BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
                case ARCHIVE_XML:
                {
                    ofxLogNotice() << "ARCHIVE_XML" << endl;
                    boost::archive::xml_iarchive ia(ifs);
                    ia >> BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
            }
            
            return true;
        } catch (boost::archive::archive_exception e) {
            ofxLogError() << "Error unserializing class from file: " << filePath << endl;;
            return false;
        }
        
    }
    
    template <class C>
    bool saveClass(string filePath, C &someClass, ArchiveType archiveType) {
        ofxLogNotice() << "Saving class data: " << filePath << endl;
        std::ofstream ofs(ofToDataPath(filePath).c_str());
        if(ofs.fail()){
            ofxLogError() << "Could not open file stream for saving: " << filePath << endl;
            return false;
        }
        try {
            ofxLogNotice() << "Saving to...";
            switch (archiveType) {
                case ARCHIVE_TEXT:
                {
                    ofxLogNotice() << "ARCHIVE_TEXT" << endl;
                    boost::archive::text_oarchive oa(ofs);
                    oa << BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
                case ARCHIVE_BINARY:
                {
                    ofxLogNotice() << "ARCHIVE_BINARY" << endl;
                    boost::archive::binary_oarchive oa(ofs);
                    oa << BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
                case ARCHIVE_XML:
                {
                    ofxLogNotice() << "ARCHIVE_XML" << endl;
                    boost::archive::xml_oarchive oa(ofs);
                    oa << BOOST_SERIALIZATION_NVP(someClass);
                    break;
                }
            }
            return true;
        } catch (std::exception e) {
            ofxLogError() << "Error serializing class from file: " << filePath << endl;;
            return false;
        }
    }

};

static _Serializer Serializer;

#endif
