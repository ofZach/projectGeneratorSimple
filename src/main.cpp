#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){
    
    
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1124, 630, OF_WINDOW);
    testApp * app = new testApp;
    //app->buildAllExamples = false;
    ofRunApp( app );    
}
