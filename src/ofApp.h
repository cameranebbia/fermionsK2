#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxGui.h"

#include "ofxOpenCv.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxKFW2::Device kinect;
	ofShortPixels kinPix;
	ofPixels grayPix;

	int kinectWidth = 512;
	int kinectHeight = 424;


	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;


	////////// SOUNDS \\\\\\\\\
	    
	ofSoundPlayer  background;
	ofSoundPlayer  sound;


	float preVolumeSmooth = 0;


	ofxCvGrayscaleImage grayImage; // grayscale depth image

	ofxCvGrayscaleImage grayImageKinectOld; //
	ofxCvGrayscaleImage grayImageKinectDiff; //


	ofxPanel gui;

	ofParameter<int> farThreshold;
	ofParameter<bool> showgrab;
	ofParameter<bool> mirrorX;
	ofParameter<bool> mirrorY;
	ofParameter<int> dilate, erode;
	ofFbo fbo;
	ofPixels pix;

	bool showGui;

	bool showmouse;

	ofParameter<int> incolor, outcolor, incolorAm, outcolorAm, skip;
	ofParameter<float> extrusion, pointScale, lineAlpha, colR, colB, colG, camPosZ, camPosX, camPosY; //colorExtrusion,

	ofParameter<float> audioBackgroundVol, audioFermionsVol;

	ofParameter<int> cropLeft, cropRight;
	ofParameter<bool> showCrop;

	ofParameter<float> volFadeIn;
	ofParameter<float> volFadeOut;
	ofParameter<int> kinectDiffSkip;
	int kinectDiffCount;
	ofParameter<float> kinectMovementSpeed;


	ofParameter<int> thresholdAudioOn;
	ofParameter<int> currPresAmount;
	ofParameter<int> thresholdMoveAudioMin;
	ofParameter<int> thresholdMoveAudioMax;

	ofParameter<float> preVolumeMap;
	ofParameter<float> volumeFermions;


	int screenshotCount;

	ofImage screenshot;

};
