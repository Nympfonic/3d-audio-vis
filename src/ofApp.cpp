#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableSmoothing();

	// Set up ofSound
	int sampleRate = 44100;
	int bufferSize = 512;
	ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numInputChannels = 0;
	settings.numOutputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	// Set up audio stuff
	counter = 1; // clock tick counter
	tempo = 30;
	sample.load("../bin/data/bass-guitar-stab.wav"); // load the sample audio file
	clock.setTempo(tempo);
	clock.setTicksPerBeat(4);

	// Set up camera
	cam.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 300); // setting the position for the camera
	glm::vec3 target; // position of the centre of the visualizer
	target.x = ofGetWidth() / 2;
	target.y = ofGetHeight() / 2;
	target.z = 0;
	cam.setTarget(target); // make the camera face towards the visualizer

	// Set up lighting
	ofEnableDepthTest();
	ofEnableLighting();
	ofDisableAlphaBlending();
	ofEnableNormalizedTexCoords();

	colour = 0.3f;
	light.setAmbientColor(ofFloatColor(0.3f)); // using ambient light to change visualizer colour
	light.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0); // set light position to the centre of the visualizer so it is all covered by the light

	// Set up shape
	radius = 200;
	baseShape.set(radius, 20); // creating the sphere with the specified radius and resolution values
	baseShape.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0); // setting position of the visualizer

	// Deformation initialisation
	originalVertices = baseShape.getMesh().getVertices(); // save shape's original vertex positions
	deformAmount = 0.1;
	deformFrequency = 2;
}

//--------------------------------------------------------------
void ofApp::update(){
	audioGain = ofMap(mouseX, 0, ofGetWidth(), 0.5, 1.0, true); // map the gain of the audio based on mouseX to scale of 0.5 - 1.0

	deformAmount = ofMap(finalOut, 0, 1.0f, 0, 1.5f, true); // map the deform amount from the final audio output to scale of 0 - 1.5
	deformFrequency = ofMap(mouseY, ofGetHeight(), 0, 0, 10.0f, true); // map the deform frequency based on mouseY to scale of 0 - 10

	vector<glm::vec3> &vertices = baseShape.getMesh().getVertices();

	for (size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 v = originalVertices[i]; // look into the list of the vertices' original positions
		v = glm::normalize(v); // normalise the vertex's x, y and z values to make it easier to work with

		// using the sine function, deform the x, y and z positions using the deformFrequency variable
		float sx = sin(v.x * deformFrequency);
		float sy = sin(v.y * deformFrequency);
		float sz = sin(v.z * deformFrequency);

		// using ofSign(), ofSignedNoise() and ofRandom(), apply slightly varying deformation to the vertex x, y and z values
		v.x += sy * sz * ofSign(ofSignedNoise(ofRandom(v.x - 0.2f, v.x + 0.2f))) * deformAmount;
		v.y += sx * sz * ofSign(ofSignedNoise(ofRandom(v.y - 0.2f, v.y + 0.2f))) * deformAmount;
		v.z += sx * sy * ofSign(ofSignedNoise(ofRandom(v.z - 0.2f, v.z + 0.2f))) * deformAmount;

		v *= radius; // multiply the x, y and z values by the radius to return the visualizer to its original size

		vertices[i] = v; // overwrite the original vertex with the modified vertex in the list of vertices
	}

	colour = ofLerp(0.3f, 1.0f, finalOut); // smoother transition from original colour to new colour based on audio output
	light.setAmbientColor(ofFloatColor(colour, 0.3f, 0.3f)); // apply the colour to the ambient lighting

	clock.setTempo(tempo); // allow for tempo to be modified at runtime
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw information text
	ofSetColor(255);
	ofDrawBitmapString("-- 3D Audio Visualizer --", 10, 20);
	ofDrawBitmapString("Move mouse horizontally to adjust audio gain", 10, 40);
	ofDrawBitmapString("Move mouse vertically to adjust the frequency of the deformity", 10, 60);
	ofDrawBitmapString("Use 'W' and 'S' to change the tempo: " + ofToString(tempo), 10, 80);

	// Draw the visualizer
	cam.begin();
	light.enable();
	
	baseShape.drawWireframe(); // draw wireframe of the sphere, because it looks cooler than a solid sphere

	light.disable();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output) {
	std::size_t outChannels = output.getNumChannels();

	for (int i = 0; i < output.getNumFrames(); ++i) {
		clock.ticker();
		if (clock.tick) {
			sample.trigger();
		}
		counter = (counter + 1) % 32;

		finalOut = sample.playOnce() * audioGain;

		output[i * outChannels] = finalOut;
		output[i * outChannels + 1] = output[i * outChannels];
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		// 'W' key, account for lower and upper case
		case 'w':
			tempo = ofClamp(++tempo, 1, 40); // increase tempo by 1 each key press
			break;
		case 'W':
			tempo = ofClamp(++tempo, 1, 40);
			break;
		// 'S' key, account for lower and upper case
		case 's':
			tempo = ofClamp(--tempo, 1, 40); // decrease tempo by 1 each key press
			break;
		case 'S':
			tempo = ofClamp(--tempo, 1, 40);
			break;
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
