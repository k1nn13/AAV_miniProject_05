#pragma once

#include "ofMain.h"
#include "ofxShadertoy.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "parameterSmoother.h"

#define NUMCHANNELS 7

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		//-------------------AUDIO-MAXIM---------------------------//
		void drawWaveform();
		void frequencyToAnalyse(int arrstart, int arrEnd, float scale, int fPosition);
		void frequencyToAnimate(int arrstart, int arrEnd, float scale, int fPosition);
		void audioOut(float * output, int bufferSize, int nChannels);
		void exit();
		//---------------------------------------------------------//

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
		
		//--------------------------------------------------------------------------------
		//--SETUP-SOUND--//
		ofSoundStream soundStream;
		unsigned bufferSize, sampleRate;
		double currentSample;
		double mix_0, mix_1, mix_2;							//channel mix
		double masterMix[2], outputs_0[2];	//stereo mix

		maxiSample audio;
		maxiMix channelMix[2];
		double audioTrack;

		//envelope
		maxiEnv env[NUMCHANNELS];
		int envTrigger[NUMCHANNELS];
		double envOut[NUMCHANNELS];

		//analysis
		ofxMaxiFFT fft;
		ofxMaxiFFTOctaveAnalyzer oct;
		float absValue;
		float average;
		float RMS;

		float sum;

		smoothValue positionA, positionB, positionC, position1, position2, position3;

		float valueA, valueB, valueC, value1, value2, value3;

		double visual[5];


		ofxShadertoy shadertoy;			// setup shadertoy

		//--------------------------------------------------------------------------------
		//--GUI--//
		ofxPanel gui;
		int displayGui = 1;

		//Sound Settings
		ofParameter<double> masterGainSlider;
		ofxToggle mode;

		ofParameter<float> Threshold_0;
		ofParameter<float> Threshold_1;
		ofParameter<float> Threshold_2;
		ofParameter<float> Threshold_3;

		ofParameter<int> RMSfreq1;

		ofParameter<float> RMSscale1;
		ofParameter<int> RMSstart1;
		ofParameter<int> RMSend1;

		ofParameter<float> RMSscale2;
		ofParameter<int> RMSstart2;
		ofParameter<int> RMSend2;

		ofParameter<float> RMSscale3;
		ofParameter<int> RMSstart3;
		ofParameter<int> RMSend3;

		// setup shader toy gui	
		ofParameter<ofVec2f> Position;
		ofParameter<ofVec2f> Rotation;
		ofParameter<float> Zoom;

		ofParameter<float> ScaleDetail;
		ofParameter<float> Thickness;
		ofParameter<float> Bias;
		ofParameter<float> sinBias;
		ofParameter<float> cosBias;

		ofParameter<float> bScale;
		ofParameter<float> bScaleDetail;
		ofParameter<float> bThickness;
		ofParameter<float> bBias;
		ofParameter<float> bsinBias;
		ofParameter<float> bcosBias;

		//sound
		//audio analysis

};
