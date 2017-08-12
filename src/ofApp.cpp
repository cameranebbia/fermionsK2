#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	kinect.open();
	kinect.initDepthSource();

	grayPix.allocate(512, 424, OF_IMAGE_GRAYSCALE);

	ofSetLogLevel(OF_LOG_VERBOSE);
	
	screenshotCount = 0;

	showmouse = false;
	ofHideCursor();

	//GUI
	//gui
	ofxGuiSetFont("Questrial-Regular.ttf", 10, true, true);
	ofxGuiSetTextPadding(4);
	ofxGuiSetDefaultWidth(300);
	ofxGuiSetDefaultHeight(20);

	gui.setup("CONTROLS");
	gui.setPosition(1520, 20);
	farThreshold.set("farThreshold", 2000, 0, 7000);
	erode.set("erode", 0, 0, 30);
	dilate.set("dilate", 0, 0, 30);
	showgrab.set("show kinect", false);
	colR.set("red", 1, 0, 1);
	colG.set("green", 1, 0, 1);
	colB.set("blue", 1, 0, 1);
	incolor.set("fade in color", 255, 0, 255);
	outcolor.set("fade out color", 0, 0, 255);
	incolorAm.set("fade in amount", 10, 0, 255);
	outcolorAm.set("fade out amount", 10, 0, 255);
	mirrorX.set("mirror X", true);
	mirrorY.set("mirror Y", false);
	skip.set("skip points", 2, 1, 10);


	extrusion.set("extrusion", 2.4, 0, 10);
	// colorExtrusion.set("color extrusion",0.1,0,10);
	pointScale.set("point Size", 3, 0, 10);
	lineAlpha.set("alpha channel", 255, 0, 255);
	camPosZ.set("camera position Z", 400, 0, 5000);
	camPosX.set("camera position X", 0, -1000, 1000);
	camPosY.set("camera position Y", 0, -1000, 1000);

	audioBackgroundVol.set("audioBackgroundVol", 1, 0, 4);
	audioFermionsVol.set("audioFermionsVol", 1, 0, 4);


	cropLeft.set("cropLeft", 0, -20, 960);
	cropRight.set("cropRight", 0, -20, 960);
	showCrop.set("showCrop", false);

	volFadeIn.set("volFadeIn", 0.05, 0, 0.6);
	volFadeOut.set("volFadeOut", 0.05, 0, 0.6);
	kinectDiffSkip.set("kinectDiffSkip", 0, 0, 10);

	thresholdAudioOn.set("thresholdAudioOn", 0, 0, 15000);
	currPresAmount.set("currPresAmount", 0, 0, 15000);
	thresholdMoveAudioMin.set("thresholdMoveAudioMin", 0, 0, 100);
	thresholdMoveAudioMax.set("thresholdMoveAudioMax", 0, 0, 100);
	kinectMovementSpeed.set("kinectMovementSpeed", 0, 0, 100);

	preVolumeMap.set("preVolumeMap", 0, 0, 1);
	volumeFermions.set("volumeFermions", 0, 0, 1);

	gui.add(farThreshold);
	gui.add(showgrab);
	gui.add(mirrorX);
	gui.add(mirrorY);

	gui.add(cropLeft);
	gui.add(cropRight);
	gui.add(showCrop);
	
	gui.add(erode);
	gui.add(dilate);

	gui.add(audioBackgroundVol);
	gui.add(audioFermionsVol);

	gui.add(skip);
	gui.add(extrusion);
	//gui.add(colorExtrusion);
	gui.add(incolor);
	gui.add(incolorAm);
	gui.add(outcolor);
	gui.add(outcolorAm);
	gui.add(pointScale);
	gui.add(lineAlpha);
	gui.add(colR);
	gui.add(colG);
	gui.add(colB);
	gui.add(camPosX);
	gui.add(camPosY);
	gui.add(camPosZ);

	gui.add(volFadeIn);
	gui.add(volFadeOut);

	gui.add(thresholdAudioOn);
	gui.add(currPresAmount);

	gui.add(kinectDiffSkip);


	gui.add(thresholdMoveAudioMin);
	gui.add(thresholdMoveAudioMax);
	gui.add(kinectMovementSpeed);

	gui.add(preVolumeMap);
	gui.add(volumeFermions);


	fbo.allocate(kinectWidth, kinectHeight, GL_RGBA);//GL_RGBA32F

	ofSetCoordHandedness(OF_RIGHT_HANDED);



	ofSetCoordHandedness(OF_RIGHT_HANDED);


	ofSetVerticalSync(false);

	// load an image from disk
	img.loadImage("texture.jpg");

	fbo.begin();
	ofClearAlpha();
	fbo.end();


	mesh.setMode(OF_PRIMITIVE_POINTS);

	/////////// SOUNDS \\\\\\\\\\\
	    
	background.loadSound("audio/background.mp3");
	//sound.loadSound("audio/cracks.aiff");
	sound.loadSound("audio/chimes.mp3");

	background.setVolume(audioBackgroundVol);
	background.setLoop(true);
	sound.setVolume(0);
	sound.setLoop(true);

	background.play();
	sound.play();

	///////////////\\\\\\\\\\\\\\\
	    
	gui.loadFromFile("settings.xml");

	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points


	grayImage.allocate(kinectWidth, kinectHeight);
	grayImageKinectOld.allocate(kinectWidth, kinectHeight);
	grayImageKinectDiff.allocate(kinectWidth, kinectHeight);

	showCrop = false;

	kinectDiffCount = 0;
	kinectMovementSpeed = 0;

	currPresAmount = 0;

	preVolumeMap = 0;
	preVolumeSmooth = 0;
	volumeFermions = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

	background.setVolume(audioBackgroundVol);

	ofSoundUpdate();

	ofSetWindowTitle("FERMIONS ");

	cam.setPosition(camPosX, camPosY, camPosZ);
	glPointSize(pointScale); // make the points bigger

	kinect.update();
	if (kinect.isFrameNew())
	{
		kinPix = kinect.getDepthSource()->getPixels();
		//cout << " kinPix[200*500]" << int(kinPix[200 * 500]) << endl;
		for (int i = 0; i< grayPix.size(); i++) {
			grayPix[i] = 0;
			if (kinPix[i] < farThreshold &&  kinPix[i] !=0) {
				grayPix[i] = 255;
			}
			
		}
		grayImage.setFromPixels(grayPix);

		grayImage.mirror(mirrorY, mirrorX);

		for (int i = 0; i < erode; i++) {
			grayImage.erode();
		}
		for (int i = 0; i < dilate; i++) {
			grayImage.dilate();
		}

		// update the cv images
		grayImage.flagImageChanged();


		fbo.begin();

		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(incolor, incolor, incolor, incolorAm);//FADE IN
		grayImage.draw(0, 0);
		//grayImage.draw(grayImage.getWidth(),0,-grayImage.getWidth(),grayImage.getHeight());
		ofDisableBlendMode();

		ofEnableAlphaBlending();
		ofSetColor(outcolor, outcolor, outcolor, outcolorAm);//FADE OUT
		ofRect(0, 0, fbo.getWidth(), fbo.getHeight());
		ofDisableAlphaBlending();

		ofSetColor(255);
		fbo.end();

		fbo.readToPixels(pix);



		/////////////// SOUNDS \\\\\\\\\\\\\\\\\\
		
		
		currPresAmount = grayImage.countNonZeroInRegion(0, 0, grayImage.width, grayImage.height);

		if (currPresAmount < thresholdAudioOn) {
			// cout << "none" << endl;
			kinectMovementSpeed = thresholdMoveAudioMin;
			grayImageKinectOld = grayImage;
			grayImageKinectDiff.set(0);
		}
		else {
			if (kinectDiffCount >= kinectDiffSkip) {
				kinectDiffCount = 0;
				grayImageKinectDiff = grayImage;
				grayImageKinectDiff.absDiff(grayImageKinectOld);
				int loccurrPresAmount = currPresAmount;
				kinectMovementSpeed = grayImageKinectDiff.countNonZeroInRegion(0, 0, grayImageKinectDiff.width, grayImageKinectDiff.height) / sqrt(loccurrPresAmount);
				//kinectMovementSpeed = ofMap(kinectMovementSpeed, 0, 50000, 0, 1);
				grayImageKinectOld = grayImage;
			}
			kinectDiffCount++;
		}


		preVolumeMap = ofMap(kinectMovementSpeed, thresholdMoveAudioMax, thresholdMoveAudioMin, 1, 0, true);

		if (preVolumeMap > preVolumeSmooth) {
			preVolumeSmooth += (preVolumeMap - preVolumeSmooth) * volFadeIn;
		} else {
			preVolumeSmooth += (preVolumeMap - preVolumeSmooth) * volFadeOut;
		}
		


		volumeFermions = preVolumeSmooth * audioFermionsVol;

		sound.setVolume(volumeFermions);

	

	}

	if (pix.size() > 0) {
		mesh.clear();
		for (int y = 0; y < kinectHeight; y += skip) {
			for (int x = 0; x < kinectWidth; x += skip) {
				ofColor cur = img.getColor(x, y);

				//float depth = 0;
				float mask = 0;

				mask = ofMap(pix.getColor(x, y).getBrightness(), 0, 255, 1, 0);
				//depth = kinect.getPixels().getColor(x, y).getBrightness(); NON USATA!!!!!

				cur.a = lineAlpha;
				cur.r = cur.r * colR;
				cur.g = cur.g * colG;
				cur.b = cur.b * colB;

				mesh.addColor(cur);

				ofVec3f pos(x, y, img.getColor(x, y).getBrightness() * extrusion * mask);;// +depth * colorExtrusion * (1 - mask));
				mesh.addVertex(pos);

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {


	//ofEnableAlphaBlending();
	ofBackground(0);
	ofEnableDepthTest();



	cam.begin();
	ofScale(1, -1, 1); // flip the y axis 

	ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
	mesh.draw();
	cam.end();
	ofDisableDepthTest();

	ofSetColor(0, 255);
	if (showCrop)
		ofSetColor(255, 0, 0, 255);

	ofDrawRectangle(-2, 0, cropLeft, 1200);

	ofDrawRectangle(1922, 0, -cropRight, 1200);

	ofSetColor(255, 255);

	if (showGui) {

		ofDrawBitmapString("PRESS G -> TOGGLE GUI \nM -> TOGGLE MOUSE \nF -> TOGGLE FULLSCREEN \nS -> SAVE SETTINGS.XML \nL -> (RE)LOAD SETTINGS.XML", 20, 320);
		ofDrawBitmapString("FPS " + ofToString(ofGetFrameRate(), 2), 20, 400);

		gui.draw();
		if (showgrab) {
			ofDisableAlphaBlending();

			kinect.getDepthSource()->draw(40, 0, kinectWidth / 2, kinectHeight / 2);
			fbo.draw(60 + kinectWidth / 2, 0, fbo.getWidth() / 2, fbo.getHeight() / 2);
			grayImage.draw(80 + kinectWidth, 0, grayImage.getWidth() / 2, grayImage.getHeight() / 2);
			grayImageKinectDiff.draw(100 + kinectWidth * 3 / 2, 0, kinectWidth / 2, kinectHeight / 2);
			ofEnableAlphaBlending();


			ofDrawBitmapStringHighlight("INPUT KINECT", 40, kinectHeight / 2 + 10);
			ofDrawBitmapStringHighlight("FBO TRAIL", 380, kinectHeight / 2 + 10);
			ofDrawBitmapStringHighlight("KINECT CV", 720, grayImage.getHeight() / 2 + 10);
			ofDrawBitmapStringHighlight("KINECT DIFF AUDIO", 1060, grayImage.getHeight() / 2 + 10);
		}
	}



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'f') {
		ofToggleFullscreen();
		if (ofGetWindowMode() != OF_FULLSCREEN) {
			ofSetWindowPosition(50, 50);
		}
	}
	if (key == 'm') {
		showmouse = !showmouse;
		if (showmouse) { ofShowCursor(); }
		else { ofHideCursor(); }
	}
	if (key == 'g') {
		showGui = !showGui;
	}

	if (key == 'x') {
		screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		screenshot.save("screenshot" + ofToString(screenshotCount) + ".jpg");
		screenshotCount++;
	}

	if (key == 's') {
		gui.saveToFile("settings.xml");
	}

	if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
