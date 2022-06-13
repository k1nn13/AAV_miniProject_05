#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// setup shader toy
    shadertoy.load("shaders/raymarch.frag");
    ofSetFrameRate(60);
    shadertoy.setAdvanceTime(true);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	// setup gui
	gui.setup();
	//add position
	gui.add(masterGainSlider.set("master gain", 1.45, 0.0, 1.5));
	gui.add(mode.setup("Visual / Analysis", true));

	gui.add(RMSfreq1.set("Select RMS Freq 1", 0.0, 0.0, 86));

	gui.add(RMSscale1.set("Select RMS Scale 1", 1100, 1.0, 10000));
	gui.add(RMSstart1.set("Select start freq 1", 0, 0, 77));
	gui.add(RMSend1.set("Select end freq 1", 10, 0, 77));

	gui.add(RMSscale2.set("Select RMS Scale 2", 650.0, 1.0, 10000));
	gui.add(RMSstart2.set("Select start freq 2", 10, 0, 77));
	gui.add(RMSend2.set("Select end freq 2", 20, 0, 77));

	gui.add(RMSscale3.set("Select RMS Scale 3", 3600.0, 1.0, 10000));
	gui.add(RMSstart3.set("Select start freq 3", 50, 0, 77));
	gui.add(RMSend3.set("Select end freq 3", 70, 0, 77));

	gui.add(Position.set("Position: ", ofVec2f(-38, 125), ofVec2f(-1000, -1000), ofVec2f(1000, 1000)));
	gui.add(Rotation.set("Rotation: ", ofVec2f(660, 1492), ofVec2f(0.0), ofVec2f(3080, 3080)));
	gui.add(Zoom.set("Zoom: ", 1.1, 0, 10));

	gui.add(ScaleDetail.set("Scale Detail: ", 0.008, 0.00, 10.00));
	gui.add(Thickness.set("Thickness: ", 0.99, 0.00, 0.99));
	gui.add(Bias.set("Bias: ", 1.4, 0.00, 5));
	gui.add(sinBias.set("Sin Bias: ", 1., 0.01, 4));
	gui.add(cosBias.set("Cos Bias: ", 1., 0.01, 4));

	//gui.add(bScale.set("Scale: ", 1.1, 0, 10));

	gui.add(bScaleDetail.set("B Scale Detail: ", 5., 0.00, 10.00));
	gui.add(bThickness.set("B Thickness: ", 0.0, 0.00, 0.99));
	gui.add(bBias.set("B Bias: ", 0, 0.00, 5));
	gui.add(bsinBias.set("B Sin Bias: ", 0.059, 0.01, 4));
	gui.add(bcosBias.set("B Cos Bias: ", 0.08, 0.0, 4));
	//---------------------------------------------------------


	//tempo 108

	envOut[0] = 0;
	env[0].setAttack(2000);
	env[0].setDecay(100);
	env[0].setSustain(10);
	env[0].setRelease(8000);

	envOut[1] = 0;
	env[1].setAttack(18000);
	env[1].setDecay(800);
	env[1].setSustain(500);
	env[1].setRelease(200000);

	envOut[2] = 0;
	env[2].setAttack(2000);
	env[2].setDecay(100);
	env[2].setSustain(10);
	env[2].setRelease(2000);

	envOut[3] = 0;
	env[3].setAttack(300000);
	env[3].setDecay(500);
	env[3].setSustain(1000);
	env[3].setRelease(200000);

	envOut[4] = 0;
	env[4].setAttack(20000);
	env[4].setDecay(500);
	env[4].setSustain(1000);
	env[4].setRelease(20000);


	envOut[5] = 0;
	env[5].setAttack(200000);
	env[5].setDecay(10000);
	env[5].setSustain(10000);
	env[5].setRelease(200000);

	envOut[6] = 0;
	env[6].setAttack(200000);
	env[6].setDecay(10000);
	env[6].setSustain(10000);
	env[6].setRelease(200000);

	value1 = 0;
	value2 = 0;
	value3 = 0;
	
	//---MAXIM-SETUP---//
	audio.load(ofToDataPath("./audio/luckyStrike.wav")); //load audio

	fft.setup(1024, 512, 256);
	oct.setup(44100, 1024, 10);
	sampleRate = 44100;
	bufferSize = 512;

	//smoothing setup
	positionA.targetValue = 0;
	positionA.smoother = new parameterSmoother(40, sampleRate);
	positionA.currentValue = positionA.smoother->process(positionA.targetValue);

	positionB.targetValue = 0;
	positionB.smoother = new parameterSmoother(20, sampleRate);
	positionB.currentValue = positionB.smoother->process(positionB.targetValue);

	positionC.targetValue = 0;
	positionC.smoother = new parameterSmoother(20, sampleRate);
	positionC.currentValue = positionC.smoother->process(positionC.targetValue);



	//smoothing setup
	position1.targetValue = 0;
	position1.smoother = new parameterSmoother(40, sampleRate);
	position1.currentValue = position1.smoother->process(position1.targetValue);

	position2.targetValue = 0;
	position2.smoother = new parameterSmoother(20, sampleRate);
	position2.currentValue = position2.smoother->process(position2.targetValue);

	position3.targetValue = 0;
	position3.smoother = new parameterSmoother(20, sampleRate);
	position3.currentValue = position3.smoother->process(position3.targetValue);


	//--SoundStream-Settings--//
	ofSoundStreamListDevices();
	ofSoundStreamSettings settings;
	settings.setApi(ofSoundDevice::MS_ASIO);  //select sound card


	//auto devices = soundStream.getMatchingDevices("default");

	//if (devices.empty()) {
	//	settings.setInDevice(devices[0]);
	//}
	/*
	settings.setInListener(this);
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	*/



	ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4);
	ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
	//=======================================================//
}



//--------------------------------------------------------------
void ofApp::update(){

	//std::cout << abs(value1) << std::endl;



}

//--------------------------------------------------------------
void ofApp::draw(){



	if (mode == true) { //switch between visual and analysis mode


		//-Bass------------------------------
		if (value1 > 0.07 && envOut[0] < .2) {
			envTrigger[0] = 1;
		}

		if (envOut[0] > .9) {  //only release enevelope when at max threshold
			envTrigger[0] = 0;
		}

		float yRot = ofMap(envOut[1], 0, 1, 0, 100); // add y rotation to camera

		//-Mid-------------------------------- //use threshold to manipulate certain parts of the track (break down)
		if (value1 > 0.03 && value1 < 0.05 &&
			value2 > 0.03 && value2 < 0.05 &&
			value3 > 0.002 && value3 < 0.004 &&
			envOut[1] < .1) {

			envTrigger[1] = 1;
			std::cout << "env 1" << std::endl;
		}

		if (envOut[1] > .9) {  //only release enevelope when at max threshold
			envTrigger[1] = 0;
		}

		double nBias = ofMap(envOut[1], 0, 1, 0, 0.2);

		//-HI-bBias--------------------------------
		if (value3 > 0.007 && envOut[2] < .01) {
			envTrigger[2] = 1;
		}

		if (envOut[2] > .9) {
			envTrigger[2] = 0;
		}


		//-Rotation---------------------------------
		if (value1 > 0.04 && envOut[3] < .01) {
			envTrigger[3] = 1;
		}

		if (envOut[3] > .9) {
			envTrigger[3] = 0;
		}

		float xRot = ofMap(envOut[3], 0, 1, 0, -800);


		//-scaleDetail------------------------------
		if (value1 > 0.16 && envOut[4] < .2) {
			envTrigger[4] = 1;
		}

		if (envOut[4] > .9) {  //only release enevelope when at max threshold
			envTrigger[4] = 0;
		}

		double gDetail = ofMap(envOut[4], 0, 1, 0.00, 0.008);
		
		//-Bias------------------------------
		if (value1 < 0.2 && 
			value2 < .2 && 
			value3 < .18 && envOut[5] < .2) {
			envTrigger[5] = 1;
		}

		if (envOut[5] > .9) {  //only release enevelope when at max threshold
			envTrigger[5] = 0;
		}

		double gBias = ofMap(envOut[5], 0, 1, 0, -.6);

		//-------------------------------------------------------------
		float dBias;
		if (value1 > .13 && 
			value2 > .10 && envOut[6] < .2) {
			envTrigger[6] = 1;
		}

		if (envOut[6] > .9) {  //only release enevelope when at max threshold
			envTrigger[6] = 0;
		}

		dBias = ofMap(envOut[6], 0, 1, 0, 0.2);

		// draw the shader on screen and apply envelope parameters
		shadertoy.setDimensions(ofGetWindowWidth(), ofGetWindowHeight()); // center the image
		shadertoy.begin();

		shadertoy.setUniform2f("Position", Position->x, Position->y);
		shadertoy.setUniform2f("M", Rotation->x + xRot, Rotation->y - yRot);
		shadertoy.setUniform1f("Zoom", Zoom);

		shadertoy.setUniform1f("ScaleDetail", ScaleDetail + gDetail );  //create lattice or ball
		shadertoy.setUniform1f("Thickness",  envOut[0]);
		shadertoy.setUniform1f("Bias", Bias + value3 + gBias + dBias + nBias);
		shadertoy.setUniform1f("sinBias", sinBias + dBias*.1);
		shadertoy.setUniform1f("cosBias", cosBias);

		shadertoy.setUniform1f("bScaleDetail", bScaleDetail );
		shadertoy.setUniform1f("bThickness", bThickness);
		shadertoy.setUniform1f("bBias", bBias + envOut[2]*2);
		shadertoy.setUniform1f("bsinBias", bsinBias);
		shadertoy.setUniform1f("bcosBias", bcosBias);
		shadertoy.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		shadertoy.end();



	} else {
	
		//analysis mode 
		drawWaveform();
		frequencyToAnalyse(RMSstart1, RMSend1, RMSscale1, 0);
		frequencyToAnalyse(RMSstart2, RMSend2, RMSscale2, 1);
		frequencyToAnalyse(RMSstart3, RMSend3, RMSscale3, 2);

	}

	if (displayGui == 1) {
		//gui display / hide
		gui.draw();
	}
	

}

//-----------------------AUDIO----------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {


	float sum = 0;

	for (int i = 0; i < bufferSize; i++) {



		audioTrack = audio.play(); // play the audio

		envOut[0] = env[0].adsr(1., envTrigger[0]);
		envOut[1] = env[1].adsr(1., envTrigger[1]);
		envOut[2] = env[2].adsr(1., envTrigger[2]);
		envOut[3] = env[3].adsr(1., envTrigger[3]);
		envOut[4] = env[4].adsr(1., envTrigger[4]);
		envOut[5] = env[5].adsr(1., envTrigger[5]);
		envOut[6] = env[6].adsr(1., envTrigger[6]);

		channelMix[0].stereo(audioTrack, outputs_0, 0.5);		// send to stereo channel	

		masterMix[0] = (outputs_0[0]);							// add stereo outputs to master mix
		masterMix[1] = (outputs_0[1]);

		//==========MASTER-OUT==============//
		output[i*nChannels] = masterMix[0] * masterGainSlider;
		output[i*nChannels + 1] = masterMix[1] * masterGainSlider;
		//==================================//

		//used for drawing fft
		//-----------PROCESS-FFT------------//
		double fftMix = (masterMix[0] + masterMix[1]) * .5;
		if (fft.process(fftMix)) {
			oct.calculate(fft.magnitudes);
		}
		//----------------------------------//

		sum += fabs(fftMix); //take absolute value of output

	
		//envTrigger[0] = 0;
	}

	//take average of samples
	average = sum / bufferSize;
	RMS = sqrt(average);

	frequencyToAnimate(RMSstart1, RMSend1, RMSscale1, 0);
	frequencyToAnimate(RMSstart2, RMSend2, RMSscale2, 1);
	frequencyToAnimate(RMSstart3, RMSend3, RMSscale3, 2);
}

//--------------------------------------------------------------
void ofApp::drawWaveform() {

	ofBackground(0, 0, 0);

	for (int i = 0; i < oct.nAverages; i++) {

		ofColor colour;
		colour.setHsb((int)ofMap(i, 0, oct.nAverages, 1, 255), 255, 255);
		ofSetColor(colour);

		int x = (int)((ofGetWidth() / 2) * i / oct.nAverages) + ofGetWidth() / 4;
		float y = ofGetHeight() * .4;
		float fftHeight = fft.magnitudes[i];
		ofDrawRectangle(x, y, 2, fftHeight * -8);	//draw rectangles with fft magnitude
		
	}

}


//--------------------------------------------------------------
void ofApp::frequencyToAnalyse(int arrStart, int arrEnd, float scale, int fPosition) {
	if (arrEnd >= arrStart) {

		for (int i = arrStart; i < arrEnd; i++) {

			int x = (int)((ofGetWidth() / 2) * i / oct.nAverages) + ofGetWidth() / 4;
			float y = ofGetHeight() * .6;
			double average = oct.averages[i] / bufferSize;
			double RMS = sqrt(average);

			float sScale = .6;

			if (fPosition == 0) {
				ofPushStyle();
				ofSetColor(255);
				ofDrawRectangle(x, y, 1, RMS * -scale);	   //draw rectangles with average magnitudes

				positionA.currentValue = positionA.smoother->process(positionA.targetValue);
				positionA.targetValue = RMS;
				ofSetColor(255, 0, 0);
				ofDrawRectangle(x, y, 1, positionA.currentValue * (5 * (scale*sScale))); // draw rectangle smoothed magnitudes
				valueA = positionA.currentValue;
				ofPopStyle();
			}
			else if (fPosition == 1) {
				ofPushStyle();
				ofSetColor(255);
				ofDrawRectangle(x, y, 1, RMS * -scale);	   //draw rectangles with average magnitudes

				positionB.currentValue = positionB.smoother->process(positionB.targetValue);
				positionB.targetValue = RMS;
				ofSetColor(0, 0, 255);
				ofDrawRectangle(x, y, 1, positionB.currentValue * (5 * (scale*sScale))); // draw rectangle smoothed magnitudes
				valueB = positionB.currentValue;
				ofPopStyle();
			}
			else if (fPosition == 2) {
				ofPushStyle();
				ofSetColor(255);
				ofDrawRectangle(x, y, 1, RMS * -scale);	   //draw rectangles with average magnitudes

				positionC.currentValue = positionC.smoother->process(positionC.targetValue);
				positionC.targetValue = RMS;
				ofSetColor(255, 255, 0);
				ofDrawRectangle(x, y, 1, positionC.currentValue * (5 * (scale*sScale))); // draw rectangle smoothed magnitudes
				valueC = positionC.currentValue;
				
				ofPopStyle();
			}
			//---------------------------------------------------------------------
	
		}

		float y = ofGetHeight() * .6;
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("Value A " + ofToString(valueA), ofGetWidth() * .8, y);
		ofSetColor(0, 0, 255);
		ofDrawBitmapString("Value B " + ofToString(valueB), ofGetWidth() * .8, y + 32);
		ofSetColor(255, 255, 0);
		ofDrawBitmapString("Value C " + ofToString(valueC), ofGetWidth() * .8, y + 64);

	} else {
		arrEnd = arrStart + 1;
	}




}

//--------------------------------------------------------------
void ofApp::frequencyToAnimate(int arrStart, int arrEnd, float scale, int fPosition) {
	if (arrEnd >= arrStart) {

		for (int i = arrStart; i < arrEnd; i++) {


			double average = oct.averages[i] / bufferSize;
			double RMS = sqrt(average);


			if (fPosition == 0) {

				position1.currentValue = position1.smoother->process(position1.targetValue);
				position1.targetValue = RMS;
				value1 = position1.currentValue;

			}
			else if (fPosition == 1) {

				position2.currentValue = position2.smoother->process(position2.targetValue);
				position2.targetValue = RMS;
				
				value2 = position2.currentValue;

			}
			else if (fPosition == 2) {


				position3.currentValue = position3.smoother->process(position3.targetValue);
				position3.targetValue = RMS;
				value3 = position3.currentValue;

			}
			//---------------------------------------------------------------------

		}



	}
	else {
		arrEnd = arrStart + 1;
	}




}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'g') {
		displayGui += 1;
	}

	if (displayGui > 1) {
		displayGui = 0;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key == 'f') {
        ofToggleFullscreen();
    }

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

//--------------------------------------------------------------
void ofApp::exit() {
	ofSoundStreamClose();
}
