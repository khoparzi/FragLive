#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    sliderGroup.setName("Rotations");
    sliderGroup.add(rotXSlider.set("rot x", 0.0, -180.0, 180.0));
    sliderGroup.add(rotYSlider.set("rot y", 0.0, -180.0, 180.0));
    sliderGroup.add(rotZSlider.set("rot z", 0.0, -180.0, 180.0));
    
    sliderGroup.add(rotXASlider.set("rot x a", 0.0, -180.0, 180.0));
    sliderGroup.add(rotYASlider.set("rot y a", 0.0, -180.0, 180.0));
    sliderGroup.add(rotZASlider.set("rot z a", 0.0, -180.0, 180.0));
    
    sliderGroup.add(posXSlider.set("pos x", 0, -1, 1));
    sliderGroup.add(posYSlider.set("pos y", 0, -1, 1));
    sliderGroup.add(posZSlider.set("pos z", 0, -10, 10));
    sliderGroup.add(posZSliderMin.set("pos z min", 1000, 1500, 500));
    sliderGroup.add(posZSliderMax.set("pos z max", 250, 500, 0));
    
    sliderGroup.add(zoomSlider.set("zoom", 0.0, -10.0, 10.0));
    
    sliderGroup.add(sepSlider.set("separation", 0.0, 0.0, 1));
    gui.setup(sliderGroup);
    
    tidal = new ofxTidalCycles(3334, 4);
    for (int i = 0; i < NUM; i++) {
        brightness[i] = 0;
    }
    
    receiver.setup(3335);
    
    cam.disableMouseInput();
    cam.setPosition( ofGetWidth() * 0.5, ofGetHeight() * 0.5, 500);
}

//--------------------------------------------------------------
void ofApp::update(){
    oscMessage();
    tidal->update();
    for (int i = 0; i < NUM; i++) {
        randomShader[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    ofBackground(0);
    ofSetColor(255);
    
    if (rotZSlider.get() != 0.0) {
        cam.rollDeg( rotZSlider.get() );
    }
    
    if (zoomSlider.get() != 0) {
        cam.dolly( zoom );
    } else {
        cam.setPosition(
            ofMap(posX, -1, 1, 0, ofGetWidth()),
            ofMap(posY, -1, 1, 0, ofGetHeight()),
            // ofMap(posZ, -1, 1, 1000, 250) // Default position
            ofMap(posZ, -1, 1, posZSliderMin, posZSliderMax)
        );
    }
    
    // ofDrawRectangle(0, ofGetHeight() * 0.5, ofGetWidth(), ofGetHeight());
    
    int noteNum = 0;
    int monitorOrder[5] = { 2,1,3,0,4 };
    for (int i = 0; i < tidal->notes.size(); i++) {
        if (ofGetElapsedTimef() - tidal->notes[i].timeStamp < 32) {
            //float diff = ofGetElapsedTimef() - tidal->notes[i].timeStamp - tidal->notes[i].latency;
            float diff = ofGetElapsedTimef() - tidal->notes[i].timeStamp;
            if (diff > 0 && abs(diff) < 1.0 / ofGetFrameRate() && tidal->notes[i].s != "midi") {
                int instNum = tidal->notes[i].instNum % 5;
                
                if (tidal->notes[i].orbit == 0) {
                    rotZ = tidal->notes[i].rotz;
                    rotZSlider.set(rotZ);
                    
                    if (tidal->notes[i].zoom != zoom) {
                        zoom = tidal->notes[i].zoom;
                        zoomSlider.set(zoom);
                    }
                    
                    if (tidal->notes[i].posx != posX) {
                        posX = tidal->notes[i].posx;
                        posXSlider.set(posX);
                    }
                    if (tidal->notes[i].posy != posY) {
                        posY = tidal->notes[i].posy;
                        posYSlider.set(posY);
                    }
                    if (tidal->notes[i].posz != posZ) {
                        posZ = tidal->notes[i].posz;
                        posZSlider.set(posZ);
                    }
                    if (tidal->notes[i].sep != sep) {
                        float rsep = tidal->notes[i].sep;
                        sepSlider.set(rsep);
                    }
                }
                
                brightness[monitorOrder[instNum]] += 255;
                if (brightness[monitorOrder[instNum]] > 255) {
                    brightness[monitorOrder[instNum]] = 255;
                    
                }
                
                if (tidal->notes[i].fs > -1) {
                    int shaderNum = tidal->notes[i].fs;
                    if (tidal->notes[i].fs >= (randomShader[monitorOrder[instNum]].shaders.size() - 1)) {
                        shaderNum = randomShader[monitorOrder[instNum]].shaders.size() - 1;
                    }
                    randomShader[monitorOrder[instNum]].num = tidal->notes[i].fs;
                } else {
                    randomShader[monitorOrder[instNum]].num = int(ofRandom(1, randomShader[monitorOrder[instNum]].shaders.size()) - 1);
                }
            
            }
        }
    }
    for (int i = 0; i < NUM; i++) {
        ofSetColor(brightness[i]);
        sep = ofMap(sepSlider, 0, 1, 0, (ofGetHeight() / NUM) / 2);
        randomShader[i].fbo.draw(0, (ofGetHeight() / NUM * i) + (sep), ofGetWidth(), (ofGetHeight() / NUM) - (sep * 2));
        brightness[i] -= 32;
        if (brightness[i] < 0) {
            brightness[i] = 0;
        }
    }
    
    cam.end();
    
    if (beatToggle) {
        float margin = ofGetWidth() / 16.0;
        tidal->drawNotes(margin, margin, ofGetWidth() - margin * 2, ofGetHeight() - margin * 2);
    }
    
    if (guiToggle) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case ' ':
            cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
            break;
        case 'B':
        case 'b':
            if (beatToggle) beatToggle = false;
            else beatToggle = true;
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'U':
        case 'u':
            if (guiToggle) guiToggle = false;
            else guiToggle = true;
            break;
    }
}

void ofApp::oscMessage(){
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/fullscreen") {
            ofToggleFullscreen();
        }
        if (m.getAddress() == "/ui") {
            if (guiToggle) guiToggle = false;
            else guiToggle = true;
        }
        if (m.getAddress() == "/beat") {
            if (beatToggle) beatToggle = false;
            else beatToggle = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
