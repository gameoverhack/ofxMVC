//
//  BaseWarp.h
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_BASEWARP
#define _H_BASEWARP

#include "ofFbo.h"
#include "ofGraphics.h"
#include "ofEvents.h"

class BaseWarp {

public:

    virtual ~BaseWarp();

    virtual void allocate(int w, int h);
    virtual void allocate(int w, int h, int numXPoints, int numYPoints, float pixelsPerGridDivision);

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void draw() = 0;
    virtual void draw(float x, float y) = 0;
    virtual void draw(float x, float y, float w, float h) = 0;

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;

    virtual ofFbo& getFBO() = 0;

    virtual ofTexture& getTextureReference() = 0;

//    virtual void mouseMoved(ofMouseEventArgs & e) = 0;
//    virtual void mouseDragged(ofMouseEventArgs & e) = 0;
//    virtual void mousePressed(ofMouseEventArgs & e) = 0;
//    virtual void mouseReleased(ofMouseEventArgs & e) = 0;

    virtual void setWarpGridPosition(float x, float y, float w, float h);
    virtual void setWarpGrid(int numXPoints, int numYPoints, bool forceReset = false);

    virtual void setWarpGridResolution(float pixelsPerGridDivision);
    virtual void setWarpGridResolution(int gridDivisionsX, int gridDivisionsY);

    virtual void resetWarpGrid();
    virtual void resetWarpGridPosition();

    virtual int getNumXPoints();
    virtual int getNumYPoints();

    virtual int getGridDivisionsX();
    virtual int getGridDivisionsY();

    virtual void toggleShowWarpGrid();
    virtual void setShowWarpGrid(bool b);
    virtual bool getShowWarpGrid();

    virtual void setControlPoints(vector<float> controlPoints);

    virtual vector<float> getControlPoints();
    virtual vector<float>& getControlPointsReference();

protected:

//    virtual void drawWarpGrid(float x, float y, float w, float h);

private:

};

#endif
