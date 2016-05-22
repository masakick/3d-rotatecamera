#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
    
    
    float distance;
    float earth_radius;
    
    ofVec3f camPos;
    ofCamera mCam;
    ofSpherePrimitive mSphere;
    ofVboMesh mSphereMesh;
    ofImage earthTexture;
    
    float tokyo_lon = 139.45;
    float tokyo_lat = 35.40;
    float sydney_lon = 151.0;
    float sydney_lat = -34.0;
    
    ofVec3f tokyoPos;
    ofVec3f sydneyPos;
    ofColor tokyoCl;
    ofColor sydneyCl;
    
    ofxPanel gui;
    ofxFloatSlider cameraRotX;
    ofxFloatSlider autoRotateSpeed;
    ofxToggle useLookAt;
    ofxToggle autoRotate;
    ofxToggle lookAtTokyo;
    ofxToggle lookAtSydney;
    ofxToggle lookAtCenter;
    ofxToggle useMakeRotateOrig;
    
    ofVec3f defaultCameraVec;
    ofVec3f defaultCameraUpDir;
    
		
};
