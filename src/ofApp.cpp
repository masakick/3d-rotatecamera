#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    
    earth_radius = 100;
    distance = 250;
    tokyoCl = ofColor(255,255,0);
    sydneyCl = ofColor(0,255,255);
    
    tokyoPos = ofVec3f(sin(tokyo_lat)*cos(tokyo_lon),
                       sin(tokyo_lat)*sin(tokyo_lon),
                       cos(tokyo_lat)) * earth_radius*1.001;
    sydneyPos = ofVec3f(sin(sydney_lat)*cos(sydney_lon),
                       sin(sydney_lat)*sin(sydney_lon),
                       cos(sydney_lat)) * earth_radius*1.001;
    
    
    
    camPos = ofVec3f(0,0,distance);
    
    
    mCam.setPosition(camPos);
    mCam.lookAt(ofVec3f(0,0,0));
    
    defaultCameraVec = ofVec3f(0,0,0)-mCam.getPosition();
    defaultCameraVec.normalize();
    defaultCameraUpDir = mCam.getUpDir();
    
    mSphere.set(earth_radius,32);
    mSphereMesh = mSphere.getMesh();
    
    ofDisableArbTex();
    earthTexture.load("gebco_08_rev_bath_720x360_color.jpg");
    ofEnableArbTex();
    
    
    
    gui.setup();
    gui.add(cameraRotX.setup("camera rot x", 0, 0,2*PI));
    gui.add(useLookAt.setup("use lookAt func",false));
    gui.add(autoRotate.setup("Auto Rotation",true));
    gui.add(autoRotateSpeed.setup("Auto Rotation Speed",0.5,0.0,2.0));
    gui.add(lookAtCenter.setup("Look at center",false));
    gui.add(lookAtTokyo.setup("Look at Tokyo",true));
    gui.add(lookAtSydney.setup("Look at Sydnes",false));
    gui.add(useMakeRotateOrig.setup("use makeRotate_original func",false));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    if (autoRotate) {
        float time = ofGetElapsedTimef()*autoRotateSpeed;
        camPos.z = distance * cos(time);
        camPos.y = distance * sin(time);
    }
    else{
        camPos.z = distance * cos(cameraRotX);
        camPos.y = distance * sin(cameraRotX);
    }
    mCam.setPosition(camPos);
    
    if(useLookAt){
        if(lookAtCenter){
            mCam.lookAt(ofVec3f(0,0,0));
            lookAtTokyo = false;
            lookAtSydney = false;
        }
        else if(lookAtTokyo){
            mCam.lookAt(tokyoPos);
            lookAtCenter = false;
            lookAtSydney = false;
        }
        else{
            mCam.lookAt(sydneyPos);
            lookAtCenter = false;
            lookAtTokyo = false;
        }
    }
    else{
        
        ofVec3f camTarget;
        if(lookAtCenter){
            camTarget = ofVec3f(0,0,0);
            lookAtTokyo = false;
            lookAtSydney = false;
        }
        else if(lookAtTokyo){
            camTarget = tokyoPos;
            lookAtCenter = false;
            lookAtSydney = false;
        }
        else{
            camTarget = sydneyPos;
            lookAtCenter = false;
            lookAtTokyo = false;
        }
        
        
        ofVec3f camToTarget =  camTarget - camPos;
        camToTarget.normalize();
        ofQuaternion camQuat;
        float dot = camToTarget.normalize().dot(defaultCameraVec.normalize());
        cout << "dot: " << dot << endl;
        if(dot == -1.0){
            camQuat.makeRotate(180.0, defaultCameraUpDir, 180.0, camToTarget, 0, defaultCameraVec);
        }
        else{
            if(useMakeRotateOrig){
                camQuat.makeRotate_original(defaultCameraVec, camToTarget);
            }
            else{
                camQuat.makeRotate(defaultCameraVec, camToTarget);
            }
        }
        mCam.setOrientation(camQuat);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    
    
    
    mCam.begin();
    
    
    ofPushStyle();
    ofEnableDepthTest();
    earthTexture.bind();
    mSphereMesh.draw();
    earthTexture.unbind();

    
    ofSetColor(tokyoCl);
    ofDrawSphere(tokyoPos.x, tokyoPos.y, tokyoPos.z, 1.0);
    ofSetColor(sydneyCl);
    ofDrawSphere(sydneyPos.x, sydneyPos.y, sydneyPos.z, 1.0);
    
    ofDisableDepthTest();
    ofPopStyle();
    
    ofVec3f upVec = ofVec3f(0,1,1);
    
    ofVec3f camVec = ofVec3f(0,0,0)-mCam.getGlobalPosition();
    
    mCam.end();
    
    
    
    gui.draw();
    
    ofVec3f gPos = mCam.getGlobalPosition();
    ofVec3f upDir = mCam.getUpDir()*20.0;
    ofVec3f lookAtDir = mCam.getLookAtDir()*20.0;
    
    ofPushStyle();
    ofPushMatrix();
    
    
    //draw y-z plane
    
    {
        float scale = 0.4;
        float sRadius = earth_radius*scale;
        ofVec2f panelCenter;
        ofVec2f panelLeftTop;
        float panelWidh, panelHeight;
        panelCenter.x = ofGetWindowWidth()*0.85;
        panelCenter.y = ofGetWindowHeight()*0.85;
        panelLeftTop.x = 0-(sRadius*4.0);
        panelLeftTop.y = -(sRadius*4.0);
        panelWidh = panelHeight= sRadius*8.0;
        
        
        //draw base
        ofTranslate(panelCenter.x,panelCenter.y);
        ofSetColor(64, 64, 64,64);
        ofDrawRectangle(panelLeftTop.x,panelLeftTop.y,panelWidh,panelHeight);
        
        //draw earth
        ofSetColor(255, 255, 255);
        ofNoFill();
        ofDrawCircle(0, 0, sRadius);
        
        //draw camera
        ofFill();
        ofDrawCircle(camPos.z*scale, -1*camPos.y*scale, 3.0);
        
        //draw tokyo and sydney
        ofSetColor(tokyoCl);
        ofDrawCircle(tokyoPos.z*scale, -1*tokyoPos.y*scale, 3.0);
        ofSetColor(sydneyCl);
        ofDrawCircle(sydneyPos.z*scale, -1*sydneyPos.y*scale, 3.0);
        
        //draw upDir
        ofPushMatrix();
        ofTranslate(camPos.z*scale, -1*camPos.y*scale);
        ofSetColor(255,0, 0);
        ofDrawLine(0, 0, upDir.z,-1*upDir.y);
        //draw lookAtDir
        ofSetColor(255);
        ofDrawLine(0,0,lookAtDir.z, -1*lookAtDir.y);
        ofPopMatrix();
        
        //draw Euler
        ofVec3f camEuler = mCam.getOrientationEuler();
        float rectwScale = 0.5;
        float barHeight = 5.0;
        ofPushMatrix();
        ofTranslate(panelLeftTop.x,panelLeftTop.y);
        ofSetColor(255);
        ofDrawBitmapString("cam eulerX:" + ofToString(camEuler.x),2,10);
        ofDrawRectangle(panelWidh*0.5, 12, camEuler.x*rectwScale , barHeight);
        ofDrawBitmapString("cam eulerY:" + ofToString(camEuler.y),2,30);
        ofDrawRectangle(panelWidh*0.5, 32, camEuler.y*rectwScale , barHeight);
        ofDrawBitmapString("cam eulerZ:" + ofToString(camEuler.z),2,50);
        ofDrawRectangle(panelWidh*0.5, 52, camEuler.z*rectwScale , barHeight);
        ofPopMatrix();
        
    }
    ofPopMatrix();
    ofPopStyle();


    
    ofDrawBitmapStringHighlight("x,y,z: " + ofToString(gPos.x)+","+ofToString(gPos.y)+","+ofToString(gPos.z), 10, ofGetWindowHeight()-20);
    ofDrawBitmapStringHighlight("upDir x,y,z: " + ofToString(mCam.getUpDir().x)+","+ofToString(mCam.getUpDir().y)+","+ofToString(mCam.getUpDir().z), 10, ofGetWindowHeight()-40);
    ofDrawBitmapStringHighlight("mCam orientation x,y,z: " + ofToString(mCam.getOrientationEuler().x)+","+ofToString(mCam.getOrientationEuler().y)+","+ofToString(mCam.getOrientationEuler().z), 10, ofGetWindowHeight()-60);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
