#include "testApp.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"





//--------------------------------------------------------------
void testApp::setup(){
    simple.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    simple.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    simple.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    simple.keyPressed(key);
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

    simple.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    simple.windowResized(w, h);
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
