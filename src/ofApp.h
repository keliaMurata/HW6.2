/*
Code created for the noise workshop by Andreas Muller 


broken down and commented by Kelia Murata
*/

//takes care of defines for class 
#pragma once

//includes main and gui library definitions 
#include "ofMain.h"
#include "ofxGui.h"

//includes the untility and easy camera definitions 
#include "Utils/ofEasyCamExt.h"

class ofApp : public ofBaseApp
{
	public:
		
		//variables for made program created here as global class variables 
		string				xmlSettingsPath;
		ofEasyCamExt		camera;
		ofMesh				mesh;
		ofxPanel			gui;
		ofParameter<float>	frequency;
		ofParameter<float>	timeFrequency;
		ofParameter<float>	magnitude;
		ofParameter<float>	gridSize;
		ofParameter<int>	gridResolution;
		bool				drawGui;
	
		ofTrueTypeFont		fontSmall;
		//functions for app defined here 
		void setup();
		void update();
		void draw();
		void keyPressed( int key );
		

		
};
