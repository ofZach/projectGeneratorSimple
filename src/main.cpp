#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){
    
    
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 610, OF_WINDOW);
    ofApp * app = new ofApp();
    //app->buildAllExamples = false;
    ofRunApp( app );    
}
