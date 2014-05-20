//
//  BaseView.h
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//
#ifndef _H_BASEVIEW
#define _H_BASEVIEW

#include "ofMain.h"
#include "BaseModel.h"
#include "NoWarp.h"
#include "BezierWarp.h"
#ifdef USE_FENSTER
#include "ofxFensterManager.h"
#endif
enum ViewOption{
    VIEW_USE_WINDOW     = 0x01,        // 1   - adds date to the log
    VIEW_USE_FBO        = 0x02,        // 2   - adds time to the log
    VIEW_USE_BEZIERWARP = 0x04,        // 4   - adds the class & function names to the log
    VIEW_USE_MATRIXWARP = 0x08,        // 8   - adds the log level type to the log
};
#ifdef USE_FENSTER
class BaseView : public ofxFensterListener {
#else
class BaseView {
#endif
public:

    BaseView();
    virtual ~BaseView();

    void setup(float width, float height, ViewOption viewOptions, string viewName);
    virtual void update() = 0;

    void draw();
    void draw(float x, float y);
    void draw(float x, float y, float w, float h);

    void begin();
    void end();

    void setPosition(float x, float y, float w, float h);

    float getWidth();
    float getHeight();
    string getName();
#ifdef USE_FENSTER
    ofxFenster&     getWindow();
#endif
    ofFbo&          getFBO();
    ofTexture&      getTextureReference();

    template<class T>
    T& getWarp(){
        return *(static_cast<T*>(warp));
    }

    void windowResized(int w, int h);

    bool getIsSetup();

protected:

    inline bool     getIsOption(ViewOption viewOption);

    ViewOption      viewOptions;
#ifdef USE_FENSTER
    ofxFenster*     window;
#endif
    BaseWarp*       warp;

    float           width, displayWidth;
    float           height, displayHeight;

    string          viewName;

    bool            bIsSetup;
    bool            bNoWarp;

private:

};

#endif
