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

    ofxTidalCycles *tidal;
    static const int NUM = 5;
    RandomShader randomShader[NUM];
    int brightness[NUM];
    
    bool guiToggle = 0;
    ofxPanel gui;
    ofParameterGroup sliderGroup;
    
    ofParameter<float> rotXSlider;
    ofParameter<float> rotYSlider;
    ofParameter<float> rotZSlider;
    float rotX = 0.0;
    float rotY = 0.0;
    float rotZ = 0.0;
    
    ofParameter<float> rotXASlider;
    ofParameter<float> rotYASlider;
    ofParameter<float> rotZASlider;
    float rotXA = 0.0;
    float rotYA = 0.0;
    float rotZA = 0.0;
    
    ofParameter<float> posXSlider;
    ofParameter<float> posYSlider;
    ofParameter<float> posZSlider;
    float posX = 0.0;
    float posY = 0.0;
    float posZ = 0.0;
    
    ofParameter<int32_t> zoomSlider;
    int zoom = 0;
    
    ofEasyCam cam;
};
