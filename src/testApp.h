#pragma once

//#define COMMAND_LINE_ONLY

#include "ofMain.h"
#include "simplePGPage.h"
#include "makeProjectsPGPage.h"
#include "generateProjects.h"


class testApp : public ofBaseApp{

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
 
        simplePGPage simple;
        generateProjects makeProjects;
        ofxXmlSettings XML;
    
    
        ofTrueTypeFont font;
        ofTrueTypeFont titleFont;
        ofTrueTypeFont secondFont;
    
    
    enum {
        MODE_SIMPLE,
        MODE_MAKE_ALL
    };
        int mode;

        void generateProject();
    
        string addonsPath;
        string status;
    
        string appToRoot;
        string defaultLoc;
		
		float uiLeftX; 
    
    
        textButton  button;
        textButton  generateButton;
        textButton  addonButton;
        vector < textButton > buttons;

        bool isAddonCore(string addon);
        bool bInited;
        vector < string  > coreAddons;
    
        
        float statusSetTime;
        float statusEnergy;
        void setStatus(string newStatus);
        
        ofImage logo;
		void addAddon(string addon);
    
};
