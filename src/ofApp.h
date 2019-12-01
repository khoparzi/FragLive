#pragma once

#include "ofMain.h"
#include "ofxTidalCycles.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "RandomShader.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void oscMessage();

    ofxTidalCycles *tidal;
    static const int NUM = 5;
    RandomShader randomShader[NUM];
    int brightness[NUM];
    
    bool guiToggle = 0;
    bool beatToggle = 0;
    ofxPanel gui;
    ofParameterGroup sliderGroup;
    
    ofParameter<float> rotXSlider, rotYSlider, rotZSlider;
    float rotX = 0.0, rotY = 0.0, rotZ = 0.0;
    
    ofParameter<float> rotXASlider, rotYASlider,  rotZASlider;
    float rotXA = 0.0, rotYA = 0.0, rotZA = 0.0;
    
    ofParameter<float> posXSlider, posYSlider, posZSlider;
    ofParameter<float> posZSliderMin, posZSliderMax;
    float posX = 0.0, posY = 0.0, posZ = 0.0;
    
    ofParameter<int32_t> zoomSlider;
    float zoom = 0;
    
    ofxOscReceiver receiver;
    
    ofEasyCam cam;
};
