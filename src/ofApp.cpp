#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetFullscreen(true);

    sliderGroup.setName("Rotations");
    sliderGroup.add(rotXSlider.set("rot x", 0.0, -1.0, 1.0));
    sliderGroup.add(rotYSlider.set("rot y", 0.0, -1.0, 1.0));
    sliderGroup.add(rotZSlider.set("rot z", 0.0, -1.0, 1.0));

    sliderGroup.add(rotXASlider.set("rot x a", 0.0, -180.0, 180.0));
    sliderGroup.add(rotYASlider.set("rot y a", 0.0, -180.0, 180.0));
    sliderGroup.add(rotZASlider.set("rot z a", 0.0, -180.0, 180.0));

    sliderGroup.add(posXSlider.set("pos x", 0, -1, 1));
    sliderGroup.add(posYSlider.set("pos y", 0, -1, 1));
    sliderGroup.add(posZSlider.set("pos z", 0, -1, 1));
    sliderGroup.add(posZSliderMax.set("pos z max", 2500, 2500, 500));
    sliderGroup.add(posZSliderMin.set("pos z min", 0, 1000, 0));

    sliderGroup.add(zoomSlider.set("zoom", 0.0, -10.0, 10.0));

    sliderGroup.add(sepSlider.set("separation", 0.0, 0.0, 1));
    gui.setup(sliderGroup);

    tidal = new ofxTidalCycles(3334, 4);
    for (int i = 0; i < NUM; i++) {
        brightness[i] = 0;
    }

    // For fonts
    // ofTrueTypeFont::setGlobalDpi(72);
    // verdana14.load("verdana.ttf", 14, true, true);
    // verdana14.setLineHeight(18.0f);
    // verdana14.setLetterSpacing(1.037);

    receiver.setup(3335);

    cam.setPosition( ofGetWidth() * 0.5, ofGetHeight() * 0.5, 500);
    
    light.setPosition(1000, 1000, 2000);
    
    effects.init();
    effects.createPass<FxaaPass>();
    effects.createPass<BloomPass>()->setEnabled(false);
    effects.createPass<PixelatePass>()->setEnabled(false);
    effects.createPass<KaleidoscopePass>()->setEnabled(false);
    effects.createPass<GodRaysPass>()->setEnabled(false);
    //monitorOrder[8] = { 7, 5, 3, 1, 0, 2, 4, 6 };
    // int monitorOrder[5] = { 3,0,1,2,4 };
    monitorOrder[3] = 0;
    monitorOrder[4] = 1;
    monitorOrder[2] = 2;
    monitorOrder[5] = 3;
    monitorOrder[1] = 4;
    monitorOrder[6] = 5;
    monitorOrder[0] = 6;
    monitorOrder[7] = 7;
    int mNUM = 8;
    cout << "Rem:" << (mNUM % 2) << endl;
    cout << "Div:" << (mNUM / 2) << endl;
    for (int i = 1; i <= (mNUM / 2); i++) {
        cout << (mNUM / 2 - i) << ": " << (i) << endl;
    }
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
    //cam.begin(); // For using normal cam with post fx
    effects.begin(cam);
    ofBackground(0);
    ofSetColor(255);

    int noteNum = 0;
    for (int i = 0; i < tidal->notes.size(); i++) {
        if (ofGetElapsedTimef() - tidal->notes[i].timeStamp < 32) {
            //float diff = ofGetElapsedTimef() - tidal->notes[i].timeStamp - tidal->notes[i].latency;
            float diff = ofGetElapsedTimef() - tidal->notes[i].timeStamp;
            if (diff > 0 && abs(diff) < 1.0 / ofGetFrameRate() && tidal->notes[i].s != "midi") {
                int instNum = tidal->notes[i].instNum % 5;
                
                brightness[monitorOrder[instNum]] += 255;
                if (brightness[monitorOrder[instNum]] > 255) {
                    brightness[monitorOrder[instNum]] = 255;
                }
                
                if (tidal->notes[i].s.find("dummy") == 0) {
                    // Make it blank if coming from dummy note
                    brightness[monitorOrder[instNum]] = 0;
                    // Rotate
                    rotX = tidal->notes[i].rotx;
                    rotXSlider.set(rotX);
                    rotY = tidal->notes[i].roty;
                    rotYSlider.set(rotY);
                    rotZ = tidal->notes[i].rotz;
                    rotZSlider.set(rotZ);
                    
                    // Animated rotate
                    rotXA = tidal->notes[i].rotxa;
                    rotXASlider.set(rotXA);
                    rotYA = tidal->notes[i].rotya;
                    rotYASlider.set(rotYA);
                    rotZA = tidal->notes[i].rotza;
                    rotZASlider.set(rotZA);
                    
                    if (rotZA == 0 && rotYA == 0 && rotXA == 0 && (!tidal->notes[i].saxis.empty())) {
                        saxis = tidal->notes[i].saxis;
                        if (tidal->notes[i].saxis == "x") {
                            rotXASlider.set(tidal->notes[i].cps);
                        } else if (tidal->notes[i].saxis == "y") {
                            rotYASlider.set(tidal->notes[i].cps);
                        } else if (tidal->notes[i].saxis == "z") {
                            rotZASlider.set(tidal->notes[i].cps);
                        }
                    }
                    
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

                if (tidal->notes[i].fs > -1 && tidal->notes[i].fs <= (randomShader[monitorOrder[instNum]].shaders.size() - 1)) {
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
        // The drawing code
        randomShader[i].fbo.draw(0, (ofGetHeight() / NUM * i) + (sep), ofGetWidth(), (ofGetHeight() / NUM) - (sep * 2));
        brightness[i] -= 32;
        if (brightness[i] < 0) {
            brightness[i] = 0;
        }
    }
    
    // Animated rotate
    if (rotXASlider.get() != 0.0) {
        cam.panDeg(rotXASlider.get());
    }
    if (rotZASlider.get() != 0.0) {
        cam.rollDeg(rotZASlider.get());
    }
    if (rotYASlider.get() != 0.0) {
        cam.tiltDeg(rotYASlider.get());
    }
    
    // Camera rotation controls
    if (rotXASlider.get() == 0.0 && rotYASlider.get() == 0.0 && rotZASlider.get() == 0.0) {
        cam.setOrientation(glm::angleAxis(ofDegToRad(90.f), glm::vec3{rotXSlider.get(), rotYSlider.get(), rotZSlider.get()}));
    }

    if (zoomSlider.get() != 0) {
        cam.dolly(zoom);
    } else {
        cam.dolly(0);
        cam.setPosition(
            ofMap(posXSlider.get(), -1, 1, 0, ofGetWidth()),
            ofMap(posYSlider.get(), -1, 1, 0, ofGetHeight()),
            ofMap(posZ, -1, 1, posZSliderMin, posZSliderMax));
    }

    //cam.end(); // For using normal cam with post fx
    effects.end();

    // ofSetColor(255);
    // verdana14.drawString(saxis, 30, 35);

    if (beatToggle) {
        float margin = ofGetWidth() / 16.0;
        tidal->drawNotes(margin, margin, ofGetWidth() - margin * 2, ofGetHeight() - margin * 2);
    }

    if (guiToggle) {
        gui.draw();
        
        // draw help
        ofSetColor(0, 255, 255);
        for (unsigned i = 0; i < effects.size(); ++i)
        {
            if (effects[i]->getEnabled()) ofSetColor(0, 255, 255);
            else ofSetColor(255, 0, 0);
            ostringstream oss;
            oss << i << ": " << effects[i]->getName() << (effects[i]->getEnabled()?" (on)":" (off)");
            ofDrawBitmapString(oss.str(), 10, (20 * (i + 2)) + 400);
        }
        
        ostringstream gPos;
        gPos << "Global Position:" << cam.getGlobalPosition() << endl;
        ofDrawBitmapString(gPos.str(), 10, 20 + 600);
        ostringstream gOr;
        gOr << "Global Orientation:" << cam.getGlobalOrientation() << endl;
        ofDrawBitmapString(gOr.str(), 10, 40 + 600);
        ostringstream gPoss;
        gPoss << "Camera Position:" << cam.getPosition() << endl;
        ofDrawBitmapString(gPoss.str(), 10, 60 + 600);
        ostringstream gOrs;
        gOrs << "Camera Orientation:" << cam.getOrientationEulerDeg() << endl;
        ofDrawBitmapString(gOrs.str(), 10, 80 + 600);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'o':
            cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
            break;
        case 'B':
        case 'b':
            beatToggle = beatToggle ? false : true;
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'U':
        case 'u':
            guiToggle = guiToggle ? false : true;
            break;
    }
    
    unsigned idx = key - '0';
    if (idx < effects.size()) effects[idx]->setEnabled(!effects[idx]->getEnabled());
}

void ofApp::oscMessage(){
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/fullscreen") {
            ofToggleFullscreen();
        }
        if (m.getAddress() == "/ui") {
            guiToggle = guiToggle ? false : true;
        }
        if (m.getAddress() == "/beat") {
            beatToggle = beatToggle ? false : true;
        }
        if (m.getAddress() == "/zmin") {
             posZSliderMin.set(m.getArgAsInt(0));
        }
        if (m.getAddress() == "/zmax") {
            posZSliderMax.set(m.getArgAsInt(0));
        }
        if (m.getAddress() == "/bloom") {
            effects[1]->setEnabled(!effects[1]->getEnabled());
        }
        if (m.getAddress() == "/pixelate") {
            effects[2]->setEnabled(!effects[2]->getEnabled());
        }
        if (m.getAddress() == "/kaleid") {
            effects[3]->setEnabled(!effects[3]->getEnabled());
        }
        if (m.getAddress() == "/godray") {
            effects[4]->setEnabled(!effects[4]->getEnabled());
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
