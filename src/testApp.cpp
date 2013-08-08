#include "testApp.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"





//--------------------------------------------------------------
void testApp::setup(){
    
    mode = MODE_SIMPLE;
    
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
