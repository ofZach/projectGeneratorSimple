#pragma once

//#define COMMAND_LINE_ONLY

#include "ofMain.h"
#include "basePGPage.h"

#include "CBLinuxProject.h"
#include "CBWinProject.h"
#include "visualStudioProject.h"
#include "xcodeProject.h"
#include <Poco/Path.h>

#ifndef COMMAND_LINE_ONLY
#include "ofxGui.h"
#endif

#include "textButton.h"

class makeProjectsPGPage : public basePGPage {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
 
		void setupForTarget(int targ);
        void generateExamplesCB();
		void generateExamples();
    
    
    
        // new:
        vector < textButton > buttons;
    
    
		
		baseProject * project;
    
        string projectPath;
        string target;
		vector <int> targetsToMake;
		bool buildAllExamples;

#ifndef COMMAND_LINE_ONLY
		string drawableOfPath;
		ofRectangle ofPathRect;
		ofPoint ofPathDrawPoint;

        ofxPanel panelOptions;
        ofxButton createProject, updateProject, createAndOpen, changeOFRoot;

		ofxPanel examplesPanel;
		ofxToggle osxToggle, iosToggle, wincbToggle, winvsToggle, linuxcbToggle, linux64cbToggle,linuxarmv6lcbToggle,linuxarmv7lcbToggle;
		ofxButton generateButton;
#endif
};
