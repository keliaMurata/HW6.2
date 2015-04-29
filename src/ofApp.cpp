/*
Code created for the noise workshop by Andreas Muller 


broken down and commented by Kelia Murata
*/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//loads the specified font from file name, controls font size note: only loads it 
	fontSmall.loadFont("Fonts/DIN.otf", 8 ); 
	
	//sets the font for the gui interface, actually sets the font for use
	ofxGuiSetFont( "Fonts/DIN.otf", 8 ); 
	//sets width of gui interface box 
	ofxGuiSetDefaultWidth( 260 );
			
	//sets our xml settings path string to this string of characters 
	xmlSettingsPath = "Settings/NoiseGrid.xml";
	//creates a gui taking a collection name a the file path name
	gui.setup( "Noise Grid", xmlSettingsPath );

	//adds our frequency float bar to our gui interface, takes the name of the slider, 
	//starting point, min value, max value 
	gui.add( frequency.set("Frequency",  1.0f,  0.01f,  2.0f) );

	//adds our time frequency float bar to our gui interface, takes the name of the slider, 
	//starting point, min value, max value 
	gui.add( timeFrequency.set("Time Frequency",  1.0f,  0.001f,  3.0f) );

	//adds our magnitude float bar to our gui interface, takes the name of the slider, 
	//starting point, min value, max value 
	gui.add( magnitude.set("Magnitude",  0.3f,  0.0f,  5.0f) );

	//adds our grid size float bar to our gui interface, takes the name of the slider, 
	//starting point, min value, max value 
	gui.add( gridSize.set("Grid Size",  6.0f,  0.01f,  20.0f) );

	//adds our grid resolution int slider bar to the gui interface, it takes the name of 
	//the slider, starting default value, min and max values 
	gui.add( gridResolution.set("Grid Resolution",  20,  3,  100) );
			
	//tells our gui to load its information from the specified file, takes the file path name
	gui.loadFromFile( xmlSettingsPath );
	
	//ask about this in office hours 
	//tells the easy camera to set its auto distance to false 	
	camera.setAutoDistance( false );
	//sets the camera's position taking in a vector, can also take 3 floats 
	camera.setGlobalPosition( ofVec3f(0,5,-10) );
	//sets where the camera looks in the scene 
	camera.lookAt( ofVec3f(0,3,0) );
	//sets the near clip plane, this plane is a boundery of what is visible to the camera
	//nearest to the camera 
	camera.setNearClip( 0.0001f );
	//sets the far clip plane, this plane is a boundery of what is visible to the camera
	//farthest from the camera 
	camera.setFarClip( 512 );
	
	//sets our draw gui bool to true to make sure we aren't drawing our gui before its set up
	drawGui = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
	//note: both of these values are re-declared everytime update is called 

	//creating a float variable named time to hold the elapsed time at the moment its called 
	float time = ofGetElapsedTimef();
	//creates a float type variable named noise time that takes our saved elapsed time and 
	//multiplies it by our owned float value called time frequency that is controled by our 
	//time frequency slider bar on our gui interface 
	float noiseTime = time * timeFrequency;
	
	//creats two vectors that are taking in 3 floats
	//extent minimum is taking in the size of our gride size float variable that is being controlled 
	//by our gui interface and takes that value times 50% to get a half size smaller than our grid size
	ofVec3f extentMin( gridSize * -0.5,     0,    gridSize * -0.5 );
	//extent maximum is taking in the size of our grid from our gui interface and multiplying it by a positive 
	//50% to get 150% of our gride size 
	ofVec3f extentMax( gridSize *  0.5, gridSize, gridSize *  0.5 );
	
	//clears our mesh to make sure we're starting from a clean slate each time update is called
	mesh.clear();
	//sets our mesh to create our gird with lines (note: often seen using primitive triangles for full mesh coverage)
	mesh.setMode( OF_PRIMITIVE_LINES );
	
	//forloops that are going through all our points in our cube looking at each y, x, and z coordinate 
	//it is using grid resolution (an int value we control with our giu interface) that determines how many 
	//points make up a single line in our mesh (ie the cube is the same width but more or less points are 
	//spread across it, changing the density or "resolution" of the cube) 
	for( int y = 0; y < gridResolution; y++ )
	{
		for( int x = 0; x < gridResolution; x++ )
		{
			for( int z = 0; z < gridResolution; z++ )
			{
				//creates a vector named pos (short of position) that will take in 3 floats 
				ofVec3f pos;
				
				//sets each points (which we are accessing through these forloops) x, y, and z positions 
				//using ofMap to plot points evenly throughout the grid length by passing in the resolution of the grid
				//(how many points for each line) the smallest point of the plane we want to reach (extentmin) and the 
				//largest point we want to reach (extentmax) 
				pos.x = ofMap( x,  0, gridResolution-1, extentMin.x, extentMax.x );
				pos.y = ofMap( y,  0, gridResolution-1, extentMin.y, extentMax.y );
				pos.z = ofMap( z,  0, gridResolution-1, extentMin.z, extentMax.z );
				
				//creates a vector called noisePos (for noise position) which is the previous vector pos multiplyed by 
				//our float value frequency whose value is controlled by the gui interface 
				ofVec3f noisePos = pos * frequency;
				
				//creates a vector called vel (for velocity) 
				ofVec3f vel;
				//it sets the velocity of each point in the vector to the ofSignedNoise which is an of function that returns 
				//signed noise and takes in an x, y, z, and w(or time) input 
				vel.x = ofSignedNoise( noisePos.x, noisePos.y, noisePos.z, noiseTime );
				vel.y = ofSignedNoise( noiseTime,  noisePos.z, noisePos.y, noisePos.x );
				vel.z = ofSignedNoise( noisePos.z, noiseTime,  noisePos.y, noisePos.x );
				
				/*
				this works fine and just makes a bit more intuitive sense 
				the organization of the input is a bit cleaner and results in the same output 
				vel.x = ofSignedNoise( noisePos.x, noisePos.y, noisePos.z, noiseTime );
				vel.y = ofSignedNoise( noisePos.y, noisePos.z, noisePos.x, noiseTime );
				vel.z = ofSignedNoise( noisePos.z, noisePos.x, noisePos.y, noiseTime );
				*/

				
				//creates a vector named normVel (or normalized velocity) which takes vel and returns a normalized copy 
				ofVec3f normVel = vel.getNormalized();

				//creates a vector named velAsCol (or velocity as color) 
				//wait what is going on here?!?!??!
				ofVec3f velAsCol = (normVel + ofVec3f(1)) * 0.5;						
			    // determines the color the line by direction the line is pointing 
				ofFloatColor tmpCol( velAsCol.x, velAsCol.y, velAsCol.z );
				
				//as the new point pos as a new vertice to the mesh 
				mesh.addVertex( pos );
				//adds the color to the string 
				mesh.addColor( tmpCol );
					
				//adds a new vertice to the mesh determined by our vel value multilied by the magnitude(controlled by our giu interface)
				//and adding it to our pos
				mesh.addVertex( pos + (vel * magnitude));
				//adds our temp color that was determined by the way the line was facing and sets the alpha channel to 0.0 
				mesh.addColor( ofFloatColor(tmpCol, 0.0) );
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//creates a glowing gradient to the background at the middle of the screen 
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);
	
	//truns depth test to true to so rendering happens according to z-depth, not draw order 
	ofEnableDepthTest();
	
	//tells the easy camera to start 
	camera.begin();
			
	//sets the color of the lines we are about to draw for grid
	ofSetColor( ofColor(60) );
	ofPushMatrix();
	//rotates the grid so it appears on the "floor" below the mesh 
	ofRotate(90, 0, 0, -1);
	//draws a grid on the floor 
	ofDrawGridPlane( 10, 10, false );
	ofPopMatrix();
		
	//sets the color for the mesh 
	ofSetColor( ofColor::white );
	//ofDrawBox( 1.0f );

	//draws the mesh 
	mesh.draw();
		
	//turns the camera off 
	camera.end();

	//stops the depth test 
	ofDisableDepthTest();
	//if our draw gui bool is set to true(which it does once we get through setup) 
	if( drawGui ) 
	{ 
		//draw the gui interface 
		gui.draw(); 
	}
	
	//resets color to black 
	ofSetColor( ofColor::black );
	//
	fontSmall.drawString( ofToString(ofGetFrameRate(), 1), ofGetWidth() - 25, ofGetHeight() - 5 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//if the space bar is pressed do nothing 
	if( key == ' ' )			 
	{ 
	}
	//if tab key is pressed toggle the gui interface on or off (draw it or don't draw it)
	else if( key == OF_KEY_TAB ) 
	{ 
		drawGui = !drawGui; 
	}
	//if f key is pressed toggle if the scene is in full screen or not 
	else if( key == 'f' )		 
	{
		ofToggleFullscreen(); 
	}
}

