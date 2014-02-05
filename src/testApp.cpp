#include "testApp.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"





//--------------------------------------------------------------
void testApp::setup(){
    
    mode = MODE_SIMPLE;
    
    
    //-------------------------------------
    // load fonts
    
    font.loadFont("fonts/Inconsolata.otf", 14, true,false,false,0.3,90);
    titleFont.loadFont("fonts/Inconsolata.otf", 28, true,false,false,0.3,90);
    secondFont.loadFont("fonts/Inconsolata.otf", 11, true,false,false,0.3,90);
    
    
    
    //-------------------------------------
    // load settings
    
    XML.loadFile("settings/projectGeneratorSettings.xml");
    string appToRoot = XML.getValue("settings:base:appToRoot", "../../../../");
    string defaultLoc = XML.getValue("settings:simplePage:defaultNewProjectLocation", "apps/myApps");
    bool bMultiPlaform = XML.getValue("settings:base:multiplatform", 0) == 0 ? false : true;
    basePGPage::pgSettings settings;
    settings.appRoot = appToRoot;
    settings.defaultNewProjectLocation = defaultLoc;
    settings.multiplatform = bMultiPlaform;
    
    cout << settings.appRoot << endl;
    
    simple.settings = settings;
    makeProjects.settings = settings;
    
    
    
    simple.font = &font;
    simple.titleFont = &titleFont;
    simple.secondFont = &secondFont;
    
    makeProjects.font = &font;
    makeProjects.titleFont = &titleFont;
    makeProjects.secondFont = &secondFont;
    
    //-------------------------------------
    
    simple.setup();
    makeProjects.setup();
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    if (mode == MODE_SIMPLE) simple.update();
    else if (mode == MODE_MAKE_ALL)  makeProjects.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    
    if (mode == MODE_SIMPLE) simple.draw();
    else if (mode == MODE_MAKE_ALL)  makeProjects.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    
    
    
    if (mode == MODE_SIMPLE) simple.keyPressed(key);
    else if (mode == MODE_MAKE_ALL)  makeProjects.keyPressed(key);
    
    
    if (key == OF_KEY_RIGHT){ mode++; mode %= 2; }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    if (mode == MODE_SIMPLE) simple.mousePressed(x,y,button);
    else if (mode == MODE_MAKE_ALL)  makeProjects.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
    if (mode == MODE_SIMPLE) simple.windowResized(w,h); 
    //makeProjectsPGPage.windowResized(w, h);
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
