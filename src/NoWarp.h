//
//  NoWarp.h
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_NOWARP
#define _H_NOWARP

#include "BaseWarp.h"

class NoWarp : public BaseWarp {
    
public:
    
	NoWarp();
    ~NoWarp();
    
    void allocate(int w, int h);
    
    void begin();
    void end();
    
    void draw();
    void draw(float x, float y);
    void draw(float x, float y, float w, float h);
    
    float getWidth();
    float getHeight();
    
    ofFbo& getFBO();
    
    ofTexture& getTextureReference();
    
    void mouseMoved(ofMouseEventArgs & e);
    void mouseDragged(ofMouseEventArgs & e);
    void mousePressed(ofMouseEventArgs & e);
    void mouseReleased(ofMouseEventArgs & e);
    
protected:
	
    ofFbo fbo;
    
    float width;
    float height;
    
private:
	
};

#endif
