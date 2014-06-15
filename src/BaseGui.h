//
//  BaseGui.h
//
//  Created by gameover on 13/06/14.
//  Copyright (c) 2014 trace media. All rights reserved.
//

#ifndef __H_BASEGUI
#define __H_BASEGUI

#include "ofMain.h"
#include "BaseParameter.h"

class BaseWidget;
class Label;
class Text;
class CheckBox;
class Button;
class ListButton;
class Toggle;
class Slider;
class IntSlider;
class FloatSlider;
class DoubleSlider;
class Canvas;
class ImageTexture;
class Matrix;
class List;
class Gui;

static bool kGuiMouseDirty = false;
static BaseWidget* kGuiActiveWidget = NULL;
static map<string, BaseWidget*> kGuiWidgetMap;
static ofPoint kGuiMousePosition = ofPoint(-1, -1);
static ofPoint kGuiMouseOrigin = ofPoint(-1, -1);
static ofRectangle kGuiDisplayBounds = ofRectangle(0, 0 ,0, 0);
static ofTrueTypeFont kGuiFont;

class BaseWidget{
    
public:
    
    BaseWidget():
    bPixelsDirty(false),
    bValueChanged(false),
    cBackground(ofColor(32, 32, 32, 255)),
    cOutline(ofColor(164, 164, 164, 255)),
    bDrawBackground(false),
    bDrawBorder(true),
    borderSize(2),
    parent(NULL),
    label(NULL),
    bScroll(false),
    bMove(false),
    renderBounds(ofRectangle(0,0,0,0)),
    displayBounds(ofRectangle(0,0,0,0)),
    relativeBounds(ofRectangle(0,0,0,0)),
    layoutBounds(ofRectangle(0,0,0,0)),
    labelBounds(ofRectangle(0,0,0,0)),
    labelDisplayBounds(ofRectangle(0,0,0,0)),
    layoutDisplayBounds(ofRectangle(0,0,0,0)),
    borderBounds(ofRectangle(0,0,0,0)),
    displayScissorBounds(ofRectangle(0,0,0,0)),
    layoutDisplayScissorBounds(ofRectangle(0,0,0,0)),
    widgetsBounds(ofRectangle(0,0,0,0)),
    resizeMode(RESIZEMODE_FIT),
    bUseFBO(false),
    bVisible(true),
    bOnScreen(true)
    {}
    
    enum ResizeMode{
        RESIZEMODE_GROW,
        RESIZEMODE_FIT,
        RESIZEMODE_ABS
    };
    
    virtual ~BaseWidget(){}
    
    virtual void setup(){
        setBounds(0, 0, 1, 1);
    }
    
    virtual void setup(float w, float h){
        setBounds(0, 0, w, h);
    }
    
    virtual void setup(float x, float y, float w, float h){
        setBounds(x, y, w, h);
    }
    
    //virtual void setLabel(string _label){};
    
    virtual void update(){
        
        if(!bVisible || !bOnScreen) return;
        
        if(!isChild() && (inside(kGuiMousePosition.x, kGuiMousePosition.y) || kGuiActiveWidget == this)) checkMouse(this);
        
        for(int i = 0; i < widgets.size(); i++){
            if(inside(kGuiMousePosition.x, kGuiMousePosition.y) || kGuiActiveWidget == widgets[i]) checkMouse(widgets[i]);
            widgets[i]->update();
        }
        
    }
    
    virtual inline void draw(){
        
        if(!bVisible || !bOnScreen) return;
        
        if(bPixelsDirty){
            
            render();
            
        }else{
            for(int i = 0; i < widgets.size(); i++){
                widgets[i]->draw();
            }
            
        }
        
        if(hasChildren()){
            
            scissorBegin(layoutDisplayScissorBounds.x, layoutDisplayScissorBounds.y, layoutDisplayScissorBounds.width, layoutDisplayScissorBounds.height);
            
            BaseWidget::drawWidgets();
            ofSetColor(255, 255, 255, 255);
            
            scissorEnd();
            
            glPushMatrix();
            
            ofTranslate(displayBounds.x, displayBounds.y);
            
            scissorBegin(displayScissorBounds.x, displayScissorBounds.y, displayScissorBounds.width, displayScissorBounds.height);
            
            fbo.draw(0, 0);
            
            scissorEnd();
            
            scissorBegin(layoutDisplayScissorBounds.x, layoutDisplayScissorBounds.y, layoutDisplayScissorBounds.width, layoutDisplayScissorBounds.height);
            
            wfbo.draw(0, 0);
            
            scissorEnd();
            
            glPopMatrix();
        }
        
    }
    
    virtual inline void drawWidgets(){
        
        if(!bVisible || !bOnScreen) return;
        
        for(int i = 0; i < widgets.size(); i++){
            if(!widgets[i]->hasChildren()) widgets[i]->drawWidgets();
        }
        
        if(!hasChildren()){
            
            if(bValueChanged){
                
                if(!bWidgetDirty){
                    
                    parent->wfbo.begin();
                    
                    scissorBegin(relativeBounds.x + parent->borderBounds.x + parent->layoutBounds.x, relativeBounds.y + parent->borderBounds.y + parent->layoutBounds.y, relativeBounds.width, relativeBounds.height);
                    ofClear(0, 0, 0, 0);
                    scissorEnd();
                    
                    parent->wfbo.end();
                    bWidgetDirty = true;
                }
                
                glPushMatrix();
                
                ofTranslate(displayBounds.x, displayBounds.y);
                drawWidget();
                
                glPopMatrix();
                
                bValueChanged = false;
                
                
            }else{
                
                if(bWidgetDirty){
                    
                    parent->wfbo.begin();
                    
                    scissorBegin(relativeBounds.x + parent->borderBounds.x + parent->layoutBounds.x, relativeBounds.y + parent->borderBounds.y + parent->layoutBounds.y, relativeBounds.width, relativeBounds.height);
                    
                    ofClear(0, 0, 0, 0);
                    
                    ofTranslate(parent->layoutBounds.x, parent->layoutBounds.y);
                    ofTranslate(relativeBounds.x, relativeBounds.y);
                    
                    drawWidget();
                    
                    scissorEnd();
                    parent->wfbo.end();
                    
                    bWidgetDirty = false;
                    
                }
                
            }
            
        }
        
    }
    
    virtual inline void _render(){
        
        glPushMatrix();
        if(bDrawBackground) drawBackground();
        if(bDrawBorder) drawBorder();
        
        ofTranslate(borderBounds.x, borderBounds.y);
        
        glPushMatrix();
        ofTranslate(labelBounds.x, labelBounds.y);
        drawLabel();
        glPopMatrix();
        
        scissorBegin(layoutBounds.x, layoutBounds.y, layoutBounds.width, layoutBounds.height);
        ofTranslate(layoutBounds.x, layoutBounds.y);
        
        for(int i = 0; i < widgets.size(); i++){
            widgets[i]->render();
        }
        
        scissorEnd();
        
        glPopMatrix();
    }
    
    virtual inline void render(){
        
        if(bUseFBO){
            cout << "Render with FBO rl: " << relativeBounds << " ds: " << displayBounds << " re: " << renderBounds << " ll: " << layoutBounds << endl;
            
            fbo.begin();
            
            scissorBegin(0, 0, relativeBounds.width, relativeBounds.height);
            ofClear(0, 0, 0, 0);
            
            _render();
            scissorEnd();
            
            
            fbo.end();
            
        }else{
            
            glPushMatrix();
            
            ofTranslate(relativeBounds.x, relativeBounds.y);
            _render();
            
            glPopMatrix();
        }
        
        bPixelsDirty = false;
        
    }
    
    virtual void setResizeMode(ResizeMode mode){
        resizeMode = mode;
    }
    
    virtual void add(BaseWidget* widget){
        
        widget->setParent(this);
        widgets.push_back(widget);
        
        switch(resizeMode){
            case RESIZEMODE_GROW:
            {
                
                layoutBounds.width = MAX(layoutBounds.width, widget->relativeBounds.x + widget->relativeBounds.width + borderSize);
                layoutBounds.height = MAX(layoutBounds.height, widget->relativeBounds.y + widget->relativeBounds.height + borderSize);
                
            }
                break;
            case RESIZEMODE_FIT:
            {
                // TODO: do it!
                
            }
                break;
            case RESIZEMODE_ABS:
            {
                layoutBounds.width = MIN(relativeBounds.width, widget->relativeBounds.x + widget->relativeBounds.width + borderSize);
                layoutBounds.height = MIN(relativeBounds.height, widget->relativeBounds.y + widget->relativeBounds.height + borderSize);
                
            }
                break;
                
        }
        
        widgetsBounds.width = MAX(widgetsBounds.width, widget->relativeBounds.x + widget->relativeBounds.width + borderSize);
        widgetsBounds.height = MAX(widgetsBounds.height, widget->relativeBounds.y + widget->relativeBounds.height + borderSize);
        
        bUseFBO = true;
        
        recalculate();
        
    }
    
    virtual void setVisible(bool b){
        bVisible = b;
    }
    
    virtual bool getVisible(){
        return bVisible;
    }
    
    virtual void toggleVisible(){
        bVisible ^= true;
    }
    
    virtual void setPosition(float x, float y){
        relativeBounds.x = x;
        relativeBounds.y = y;
        recalculate();
    }
    
    virtual void setSize(float w, float h){
        relativeBounds.width = w;
        relativeBounds.height = h;
        recalculate();
    }
    
    virtual void setBounds(float x, float y, float w, float h){
        relativeBounds.x = x;
        relativeBounds.y = y;
        relativeBounds.width = w;
        relativeBounds.height = h;
        recalculate();
    }
    
    virtual ofRectangle& getBounds(){
        return relativeBounds;
    }
    
    virtual float getWidth(){
        return relativeBounds.width;
    }
    
    virtual float getHeight(){
        return relativeBounds.height;
    }
    
    virtual float getLayoutWidth(){
        return widgetsBounds.width;
    }
    
    virtual float getLayoutHeight(){
        return widgetsBounds.height;
    }
    
    virtual inline bool inside(float x, float y){
        return displayBounds.inside(x, y);
    }
    
    virtual void setBackgroundColor(int r, int g, int b, int a = 127){
        cBackground = ofColor(r, g, b, a);
    }
    
    virtual void setOutlineColor(int r, int g, int b, int a = 127){
        cOutline = ofColor(r, g, b, a);
    }
    
    virtual void setBorder(float b){
        borderSize = b;
        recalculate();
    }
    
    vector<BaseWidget*>& getWidgets(){
        return widgets;
    }
    
    friend class Label;
    friend class Text;
    friend class CheckBox;
    friend class Button;
    friend class ListButton;
    friend class Toggle;
    friend class Slider;
    friend class IntSlider;
    friend class FloatSlider;
    friend class DoubleSlider;
    friend class Canvas;
    friend class ImageTexture;
    friend class Matrix;
    friend class List;
    friend class Gui;
    
    void dirty(){
        recalculate();
        bPixelsDirty = true;
        
    }
    
protected:
    
    virtual inline void layout(){
        labelBounds = ofRectangle(0,0,0,0);
        layoutBounds = relativeBounds;
    }
    
    virtual inline void recalculate(){
        
        layout();
        
        switch(resizeMode){
            case RESIZEMODE_GROW:
            {
                
                relativeBounds.width = MAX(relativeBounds.width, layoutBounds.x + layoutBounds.width);
                relativeBounds.height = MAX(relativeBounds.height, layoutBounds.y + layoutBounds.height);
                
                if(bUseFBO){
                    renderBounds.width = relativeBounds.width;
                    renderBounds.height = relativeBounds.height;
                }
                
            }
                break;
            case RESIZEMODE_FIT:
            {
                
                // nothing yet
                
            }
                break;
            case RESIZEMODE_ABS:
            {
                if(bUseFBO){
                    
                    for(int i = 0; i < widgets.size(); i++){
                        renderBounds.width = MAX(relativeBounds.width, widgets[i]->relativeBounds.x + widgets[i]->relativeBounds.width + borderSize);
                        renderBounds.height = MAX(relativeBounds.height, widgets[i]->relativeBounds.y + widgets[i]->relativeBounds.height + borderSize);
                    }
                    
                }
            }
                break;
                
        }
        
        displayBounds = relativeBounds;
        
        if(parent != NULL) displayBounds.position += parent->displayBounds.position + parent->layoutBounds.position;
        
        borderBounds = ofRectangle(borderSize, borderSize, relativeBounds.width - borderSize, relativeBounds.height - borderSize);
        labelDisplayBounds = ofRectangle(displayBounds.x + labelBounds.x, displayBounds.y + labelBounds.y, labelBounds.width, labelBounds.height);
        layoutDisplayBounds = ofRectangle(displayBounds.x + layoutBounds.x, displayBounds.y + layoutBounds.y, layoutBounds.width, layoutBounds.height);
        
        bValueChanged = true;
        
        for(int i = 0; i < widgets.size(); i++){
            
            widgets[i]->recalculate();
            
            if(isChild()){
                if(parent->displayBounds.intersects(widgets[i]->displayBounds)){
                    widgets[i]->bOnScreen = true;
                }else{
                    widgets[i]->bOnScreen = false;
                }
            }
            
        }
        
        if(bUseFBO){
            
            if(renderBounds.width > relativeBounds.width) layoutDisplayBounds.width = layoutBounds.width - layoutBounds.x;
            if(renderBounds.height > relativeBounds.height) layoutDisplayBounds.height = layoutBounds.height - layoutBounds.y;
            
            if(!isChild()) kGuiDisplayBounds = displayBounds;
            
            layoutDisplayScissorBounds = layoutDisplayBounds.getIntersection(kGuiDisplayBounds);
            layoutDisplayScissorBounds.y = ofGetHeight() - layoutDisplayScissorBounds.height - layoutDisplayScissorBounds.y;
            displayScissorBounds = displayBounds.getIntersection(kGuiDisplayBounds);
            displayScissorBounds.y = ofGetHeight() - displayScissorBounds.height - displayScissorBounds.y;
            
            if(renderBounds.width != fbo.getWidth() || renderBounds.height != fbo.getHeight()){
                fbo.allocate(renderBounds.width, renderBounds.height);
                wfbo.allocate(renderBounds.width, renderBounds.height);
                wfbo.begin(); ofClear(0, 0, 0, 0); wfbo.end();
                bPixelsDirty = true;
            }
            
        }
        
    }
    
    virtual inline void checkMouse(BaseWidget* widget){
        
        if(kGuiActiveWidget == NULL){
            
            if(kGuiMouseDirty && ofGetMousePressed()){
                
                // mouse down on this child of the current widget
                if(widget->inside(kGuiMousePosition.x, kGuiMousePosition.y)){
                    
                    if(!widget->hasChildren() && ofGetMousePressed(0)){
                        
                        widget->select();
                        widget->mouseDown();
                        
                    }
                    else if(widget->hasChildren() && ofGetMousePressed(0)){
                        
                        if(widget->labelDisplayBounds.inside(kGuiMousePosition.x, kGuiMousePosition.y)){
                            widget->bMove = true;
                            widget->select();
                            //widget->mouseDown();
                        }
                        
                    }else if(widget->hasChildren() && widget->isChild() && ofGetMousePressed(2)){
                        
                        if(widget->layoutDisplayBounds.inside(kGuiMousePosition.x, kGuiMousePosition.y)){
                            
                            if(widget->renderBounds.width > widget->layoutBounds.width || widget->renderBounds.height > widget->layoutBounds.height){
                                widget->bScroll = true;
                                widget->select();
                            }
                            
                        }
                    }
                }
            }
            
        }else if(kGuiActiveWidget == widget){
            
            // ...mouse up/drag from this child of the current widget
            if(widget->inside(kGuiMousePosition.x, kGuiMousePosition.y)){
                
                if(kGuiMouseDirty){
                    
                    if(!widget->bScroll && !widget->bMove){
                        widget->mouseUp(true); // inside
                    }
                    
                    widget->deselect();
                    
                }else{
                    
                    if(widget->bScroll){
                        widget->mouseScroll();
                    }else{
                        widget->mouseDrag();
                    }
                    
                }
                
            }else{
                
                if(kGuiMouseDirty && (widget->bScroll || widget->bMove)){
                    widget->deselect();
                }else{
                    if(widget->bMove){
                        widget->mouseDrag();
                    }else if(widget->bScroll){
                        widget->mouseScroll();
                    }else{
                        widget->mouseUp(false); // outside
                        widget->deselect();
                    }
                }
                
            }
            
        }
        
    }
    
    virtual inline void select(){
        kGuiMouseDirty = false;
        kGuiMouseOrigin = kGuiMousePosition;
        kGuiActiveWidget = this;
    }
    
    virtual inline void deselect(){
        kGuiMouseDirty = true;
        kGuiMouseOrigin = ofPoint(-1,1);
        kGuiActiveWidget = NULL;
        bScroll = false;
        bMove = false;
    }
    
    virtual inline void mouseScroll(){}
    virtual inline void mouseDrag(){}
    virtual inline void mouseDown(){}
    virtual inline void mouseUp(bool bInside){}
    
    virtual inline bool hasChildren(){
        return (widgets.size() > 0);
    }
    
    virtual inline bool isChild(){
        return (parent != NULL);
    }
    
    virtual BaseWidget* getParent(){
        return parent;
    }
    
    virtual void setParent(BaseWidget* p){
        parent = p;
        recalculate();
    }
    
    virtual inline void drawBackground(){
        ofSetColor(cBackground);
        ofFill();
        ofRect(0, 0, relativeBounds.width, relativeBounds.height);
    }
    
    virtual inline void drawBorder(){
        ofSetColor(cOutline);
        ofNoFill();
        ofRect(borderBounds);
    }
    
    virtual inline void drawWidget(){};
    
    inline void drawLabel(){
        BaseWidget* cLabel = (BaseWidget*)label;
        if(cLabel == NULL) return;
        ofSetColor(cOutline);
        ofNoFill();
        ofRect(labelBounds);
        cLabel->draw();
    }
    
    virtual inline void scissorBegin(const float& x, const float& y, const float& w, const float& h){
        glPushAttrib(GL_SCISSOR_BIT);
        glScissor(x, y, w, h);
        glEnable(GL_SCISSOR_TEST);
        glPushMatrix();
    }
    
    virtual inline void scissorEnd(){
        glPopMatrix();
        glDisable(GL_SCISSOR_TEST);
        glPopAttrib();
    }
    
    ofFbo fbo, wfbo;
    
    Label* label;
    
    BaseWidget* parent;
    vector<BaseWidget*> widgets;
    
    bool bValueChanged;
    bool bWidgetDirty;
    
    bool bPixelsDirty;
    bool bScroll;
    bool bMove;
    bool bUseFBO;
    
    ResizeMode resizeMode;
    
    float borderSize;
    ofRectangle renderBounds;
    ofRectangle displayBounds;
    ofRectangle relativeBounds;
    ofRectangle layoutBounds;
    ofRectangle borderBounds;
    ofRectangle labelBounds;
    ofRectangle labelDisplayBounds;
    ofRectangle layoutDisplayBounds;
    ofRectangle layoutDisplayScissorBounds;
    ofRectangle displayScissorBounds;
    ofRectangle widgetsBounds;
    
    bool bDrawBorder;
    bool bDrawBackground;
    
    bool bVisible;
    bool bOnScreen;
    
    ofColor cOutline;
    ofColor cBackground;
    
};

class Label:public BaseWidget, public Parameter<string>{
    
public:
    
    Label(){
        name = "Label";
    }
    
    void setLabel(const string _label){
        setBounds(0, 0, (*value).length() * 8, 14);
        set(_label); // just set the parameter
    }
    
protected:
    
    inline void draw(){
        ofSetColor(cOutline);
        ofNoFill();
        kGuiFont.drawString(Parameter::getReference(), 0, 14);
    }
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void drawWidget(){
        
    }
    
};

class Text:public BaseWidget, public Parameter<string>{
    
public:
    
    Text(){
        name = "Text";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void drawWidget(){
        ofSetColor(cOutline);
        ofNoFill();
        kGuiFont.drawString(Parameter::getReference(), borderSize, borderSize);
    }
    
};

class CheckBox:public BaseWidget, public Parameter<bool>{
    
public:
    
    CheckBox(){
        name = "CheckBox";
        bMomentary = false;
        setBackgroundColor(0, 0, 127);
    }
    
    void setMomentary(bool b){
        bMomentary = b;
    }
    
protected:
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void drawWidget(){
        if(getReference()){
            ofSetColor(cBackground);
            ofFill();
            ofRect(borderBounds);
        }
    }
    
    inline void mouseDown(){
        if(bMomentary){
            set(true);
        }else{
            set(!getReference());
        }
    }
    
    inline void mouseUp(bool bInside){
        if(bMomentary) set(false);
    }
    
    bool bMomentary;
    
};

class ListButton:public CheckBox{
    
public:
    
    ListButton(){
        name = "ListButton";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void update(){
        //        Parameter::update();
    }
    
    inline void valueChanged(){
        bValueChanged = true;
        //        Parameter::valueChanged();
    }
    
    inline void mouseDown(){
        CheckBox::mouseDown();
        if(parent != NULL) parent->mouseDown();
    }
    
    inline void drawWidget(){
        CheckBox::drawWidget();
    }
    
};

class Button:public CheckBox{
    
public:
    
    Button(){
        name = "Button";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void drawWidget(){
        CheckBox::drawWidget();
    }
    
};

class Toggle:public CheckBox{
    
public:
    
    Toggle(){
        name = "Toggle";
        bCancellable = true;
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    void setCancellable(bool b){
        bCancellable = b;
    }
    
protected:
    
    inline void mouseDown(){
        if(bCancellable){
            // nothing?
        }else{
            if(bMomentary){
                set(true);
            }else{
                set(!getReference());
            }
        }
        
    }
    
    inline void mouseUp(bool bInside){
        if(bCancellable){
            
            if(bInside){
                if(bMomentary){
                    set(false);
                }else{
                    set(!getReference());
                }
            }
        }else{
            if(bMomentary) set(false);
        }
        
    }
    
    inline void layout(){
        
        activeBounds = ofRectangle(borderBounds.width - borderBounds.height + borderSize,
                                   borderBounds.y + borderSize,
                                   borderBounds.height - 2 * borderSize,
                                   borderBounds.height - 3 * borderSize);
        
    }
    
    inline void drawBorder(){
        BaseWidget::drawBorder();
        ofSetColor(cOutline);
        ofRect(activeBounds);
    }
    
    inline void drawWidget(){
        if(getReference()){
            ofSetColor(cBackground);
            ofFill();
            ofRect(activeBounds);
        }
    }
    
    bool bCancellable;
    bool bTempValue;
    ofRectangle activeBounds;
    
};

class IntSlider:public BaseWidget, virtual public Parameter<int>{
    
public:
    
    IntSlider(){
        name = "IntSlider";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        if(min != max) (*value) = CLAMP((*value), (*min), (*max));
        calculateHandle();
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void calculateHandle(){
        if(min != NULL && max != NULL){
            double pct = (double)(*value) / (double)((*max) - (*min));
            handleBounds.x = activeBounds.width * pct;
        }
    }
    
    inline void calculateValue(){
        if(min != NULL && max != NULL){
            double pct = (double)(kGuiMousePosition.x - displayBounds.x - activeBounds.x) / (double)(activeBounds.width);
            set((double)((*max) - (*min)) * pct);
        }
    }
    
    inline void drawWidget(){
        ofSetColor(cOutline);
        ofLine(handleBounds.position.x, handleBounds.position.y, handleBounds.position.x, handleBounds.position.y + handleBounds.height);
        ostringstream os; os.precision(3); os.setf( std::ios::fixed, std:: ios::floatfield );
        os << Parameter::getReference();
        kGuiFont.drawString(os.str(), borderBounds.x + borderBounds.width - 7 * 8, borderSize + 16);
    }
    
    inline void layout(){
        
        activeBounds = borderBounds;
        
        handleBounds.y = borderBounds.y + borderSize;
        handleBounds.width = borderSize;
        handleBounds.height = borderBounds.height - borderSize;
        
        calculateHandle();
        
    }
    
    inline void mouseDrag(){
        calculateValue();
    }
    
    inline void mouseDown(){
        calculateValue();
    }
    
    inline void mouseUp(bool bInside){
        calculateValue();
    }
    
    ofRectangle activeBounds;
    ofRectangle handleBounds;
    
};

class FloatSlider:public BaseWidget, virtual public Parameter<float>{
    
public:
    
    FloatSlider(){
        name = "FloatSlider";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        if(min != max) (*value) = CLAMP((*value), (*min), (*max));
        calculateHandle();
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void calculateHandle(){
        if(min != NULL && max != NULL){
            double pct = (double)(*value) / (double)((*max) - (*min));
            handleBounds.x = activeBounds.width * pct;
        }
    }
    
    inline void calculateValue(){
        if(min != NULL && max != NULL){
            double pct = (double)(kGuiMousePosition.x - displayBounds.x - activeBounds.x) / (double)(activeBounds.width);
            set((double)((*max) - (*min)) * pct);
        }
    }
    
    inline void drawWidget(){
        ofSetColor(cOutline);
        ofLine(handleBounds.position.x, handleBounds.position.y, handleBounds.position.x, handleBounds.position.y + handleBounds.height);
        ostringstream os; os.precision(3); os.setf( std::ios::fixed, std:: ios::floatfield );
        os << Parameter::getReference();
        kGuiFont.drawString(os.str(), borderBounds.x + borderBounds.width - 7 * 8, borderSize + 16);
    }
    
    inline void layout(){
        
        activeBounds = borderBounds;
        
        handleBounds.y = borderBounds.y + borderSize;
        handleBounds.width = borderSize;
        handleBounds.height = borderBounds.height - borderSize;
        
        calculateHandle();
        
    }
    
    inline void mouseDrag(){
        calculateValue();
    }
    
    inline void mouseDown(){
        calculateValue();
    }
    
    inline void mouseUp(bool bInside){
        calculateValue();
    }
    
    ofRectangle activeBounds;
    ofRectangle handleBounds;
    
};

class DoubleSlider:public BaseWidget, virtual public Parameter<double>{
    
public:
    
    DoubleSlider(){
        name = "DoubleSlider";
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
protected:
    
    inline void update(){
        Parameter::update();
    }
    
    inline void valueChanged(){
        if(min != max) (*value) = CLAMP((*value), (*min), (*max));
        calculateHandle();
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    inline void calculateHandle(){
        if(min != NULL && max != NULL){
            double pct = (double)(*value) / (double)((*max) - (*min));
            handleBounds.x = activeBounds.width * pct;
        }
    }
    
    inline void calculateValue(){
        if(min != NULL && max != NULL){
            double pct = (double)(kGuiMousePosition.x - displayBounds.x - activeBounds.x) / (double)(activeBounds.width);
            set((double)((*max) - (*min)) * pct);
        }
    }
    
    inline void drawWidget(){
        ofSetColor(cOutline);
        ofLine(handleBounds.position.x, handleBounds.position.y, handleBounds.position.x, handleBounds.position.y + handleBounds.height);
        ostringstream os; os.precision(3); os.setf( std::ios::fixed, std:: ios::floatfield );
        os << Parameter::getReference();
        kGuiFont.drawString(os.str(), borderBounds.x + borderBounds.width - 7 * 8, borderSize + 16);
    }
    
    inline void layout(){
        
        activeBounds = borderBounds;
        
        handleBounds.y = borderBounds.y + borderSize;
        handleBounds.width = borderSize;
        handleBounds.height = borderBounds.height - borderSize;
        
        calculateHandle();
        
    }
    
    inline void mouseDrag(){
        calculateValue();
    }
    
    inline void mouseDown(){
        calculateValue();
    }
    
    inline void mouseUp(bool bInside){
        calculateValue();
    }
    
    ofRectangle activeBounds;
    ofRectangle handleBounds;
    
};

class Canvas:public BaseWidget{
    
public:
    
    Canvas(){
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    void inline add(BaseWidget* widget){
        
        string widgetName = "";
        if(widget->label != NULL){
            widgetName = widget->label->getValue();
        }
        
        if(widgetName != "" && kGuiWidgetMap.find(widgetName) == kGuiWidgetMap.end()){
            kGuiWidgetMap.insert(pair<string, BaseWidget*>(widgetName, widget));
            cout << "adding: " << widgetName << endl;
            BaseWidget::add(widget);
        }else{
            BaseWidget::add(widget);
        }
        
    }
    
    inline BaseWidget* getWidget(const string& widgetName){
        map<string, BaseWidget*>::iterator it = kGuiWidgetMap.find(widgetName);
        if(it != kGuiWidgetMap.end()){
            return it->second;
        }else{
            return NULL;
        }
    }
    
protected:
    
    inline void mouseScroll(){
        //cout << "canvasScroll " << renderBounds << " " << layoutDisplayBounds << endl;
        
        if(renderBounds.width - borderSize > layoutDisplayBounds.width &&
           widgets[0]->relativeBounds.x - (kGuiMouseOrigin.x - kGuiMousePosition.x) <= 0 &&
           widgets[widgets.size() - 1]->relativeBounds.x - (kGuiMouseOrigin.x - kGuiMousePosition.x) +
           widgets[widgets.size() - 1]->relativeBounds.width +
           layoutBounds.x >= layoutDisplayBounds.width){
            
            for(int i = 0; i < widgets.size(); i++){
                widgets[i]->relativeBounds.x -= (kGuiMouseOrigin.x - kGuiMousePosition.x);
            }
            
        }
        
        if(renderBounds.height - borderSize > layoutDisplayBounds.height &&
           widgets[0]->relativeBounds.y - (kGuiMouseOrigin.y - kGuiMousePosition.y) <= 0 &&
           widgets[widgets.size() - 1]->relativeBounds.y - (kGuiMouseOrigin.y - kGuiMousePosition.y) +
           widgets[widgets.size() - 1]->relativeBounds.height >= layoutDisplayBounds.height){
            for(int i = 0; i < widgets.size(); i++){
                widgets[i]->relativeBounds.y -= (kGuiMouseOrigin.y - kGuiMousePosition.y);
            }
        }
        
        //cout << widgets[0]->relativeBounds.y << " -- " << widgets[widgets.size() - 1]->relativeBounds.y + widgets[widgets.size() - 1]->relativeBounds.height + layoutBounds.y << endl;
        
        bPixelsDirty = true;
        kGuiMouseOrigin = kGuiMousePosition;
        recalculate();
    }
    
    inline void mouseDrag(){
        //cout << "canvasDrag" << endl;
        
        relativeBounds.position -= (kGuiMouseOrigin - kGuiMousePosition);
        
        if(isChild()){
            relativeBounds.x = CLAMP(relativeBounds.x, 20 - relativeBounds.width, kGuiDisplayBounds.width - 20);
            relativeBounds.y = CLAMP(relativeBounds.y, 0, kGuiDisplayBounds.height - 44);
            
        }else{
            relativeBounds.x = CLAMP(relativeBounds.x, 20 - kGuiDisplayBounds.width, ofGetWidth() - 20);
            relativeBounds.y = CLAMP(relativeBounds.y, 0, ofGetHeight() - 44);
        }
        
        kGuiMouseOrigin = kGuiMousePosition;
        recalculate();
        
    }
    
    inline void mouseDown(){
        //cout << "canvasDown" << endl;
    }
    
    inline void mouseUp(bool bInside){
        //cout << "canvasUp" << endl;
        
        if(!bInside) mouseDrag();
    }
    
    inline void layout(){
        
        labelBounds = ofRectangle(0, 0, relativeBounds.width, 20);
        layoutBounds.position.y = labelBounds.height + borderSize;
        
    }
    
    inline void drawWidget(){
        
    }
    
};

class ImageTexture:public BaseWidget{
    
public:
    
    ImageTexture(){
        resizeMode = RESIZEMODE_GROW;
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    void setImage(ofTexture* texture){
        tex = texture;
        //        cout << "Image: " << tex->getWidth() << " " << tex->getHeight() << endl;
        //        BaseWidget::setSize(tex->getWidth(), tex->getHeight());
        if(parent != NULL) parent->recalculate();
    }
    
protected:
    
    inline void update(){
        bValueChanged = true;
    }
    
    inline void drawWidget(){
        
        if(tex->getWidth() != relativeBounds.width || tex->getHeight() != relativeBounds.height){
            cout << "realloc: " << tex->getWidth() << " " << tex->getHeight() << endl;
            BaseWidget::setSize(tex->getWidth(), tex->getHeight());
            if(parent != NULL) parent->recalculate();
        }
        
        ofSetColor(255,255,255, 255);
        tex->draw(0, 0);
        
    }
    
    ofTexture* tex;
    
};

class Matrix:public Canvas{
    
public:
    
    Matrix(){
        resizeMode = RESIZEMODE_GROW;
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    void setDimensions(int _cols, int _rows){
        // TODO: make it resizable!!
        xcols = _cols;
        yrows = _rows;
        
        for(int i = 0; i < yrows; i++){
            for(int j = 0; j < xcols; j++){
                CheckBox* checkBox = new CheckBox;
                checkBox->setup(20 * j, 20 * i, 20, 20);
                add(checkBox);
            }
        }
    }
    
    int cols(){
        return xcols;
    }
    
    int rows(){
        return yrows;
    }
    
    int size(){
        return xcols * yrows;
    }
    
    inline CheckBox* at(const int& index){
        if(widgets.size() > index){
            return (CheckBox*)widgets[index];
        }else{
            return NULL;
        }
    }
    
    inline CheckBox* at(const int& row, const int& col){
        return at(row * xcols + col);
    }
    
protected:
    
    inline void drawWidget(){
        
    }
    
    int xcols, yrows;
    
};

class List:public Canvas, public Parameter<string>{
    
public:
    
    List(){
        this->set("");
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    template <typename T>
    inline void setList(vector<T>& list){
        // TODO: make resizable
        for(int i = 0; i < list.size(); i++){
            class ListButton* button = new class ListButton;
            button->setLabel(ofToString(list[i]));
            button->setup(0, i * 20, relativeBounds.width, 20);
            button->set(false);
            add(button);
        }
        bPixelsDirty = true;
        recalculate();
    }
    
    int size(){
        return widgets.size();
    }
    
    inline ListButton* at(const int& index){
        if(widgets.size() > index){
            return (ListButton*)widgets[index];
        }else{
            return NULL;
        }
    }
    
protected:
    
    inline void update(){
        BaseWidget::update();
        //Parameter::update();
    }
    
    inline void valueChanged(){
        //cout << Parameter::getReference() << endl;
        bValueChanged = true;
        Parameter::valueChanged();
    }
    
    void mouseDown(){
        //cout<< "List down" << endl;
        bool bSet = false;
        for(int i = 0; i < widgets.size(); i++){
            ListButton* button = (ListButton*)widgets[i];
            if(button->getReference() == true){
                if(button->label->getValue() == this->getReference() || bSet){
                    button->set(false);
                    if(bSet) return;
                }else{
                    bSet = true;
                    this->set(button->label->getValue());
                }
                
            }
        }
        
    }
    
    inline void drawWidget(){
        
    }
    
};

class Gui:public Canvas{
    
public:
    
    Gui(){
        kGuiFont.loadFont(ofToDataPath("verdana.ttf"), 8);
        ofRegisterMouseEvents(this);
        ofAddListener(ofEvents().windowResized, this, &Gui::resized);
    }
    
    void setLabel(string _label){
        if(label == NULL){
            label = new Label;
            label->setParent(this);
        }
        label->setLabel(_label);
        recalculate();
    }
    
    inline void draw(){
        
        if(!bVisible) return;
        
        ofPushStyle();
        
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        
        BaseWidget::draw();
        
        ofDisableBlendMode();
        
        ofPopStyle();
    }
    
    void mouseMoved(ofMouseEventArgs & e){
        kGuiMousePosition = ofPoint(e.x, e.y);
    }
    
    void mouseDragged(ofMouseEventArgs & e){
        kGuiMousePosition = ofPoint(e.x, e.y);
        
    }
    
    void mousePressed(ofMouseEventArgs & e){
        if(inside(e.x, e.y)){
            kGuiMousePosition = ofPoint(e.x, e.y);
            kGuiMouseDirty = true;
        }
    }
    
    void mouseReleased(ofMouseEventArgs & e){
        kGuiMouseDirty = true;
    }
    
protected:
    
    void resized(ofResizeEventArgs& e){
        recalculate();
    }
    
};

#endif
