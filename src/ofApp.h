#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

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

		void audioOut(ofSoundBuffer& output) override;

		ofSoundStream soundStream;

		maxiClock clock;
		maxiSample sample;

		unsigned int counter;
		bool distortSample;
		double audioGain, sampleOut, finalOut;

		int tempo;

		ofEasyCam cam;

		ofLight light;
		ofSpherePrimitive baseShape;

		vector<glm::vec3> originalVertices;
		unsigned int radius;
		float deformAmount, deformFrequency;

		float colour;
};
