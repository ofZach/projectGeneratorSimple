#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){
	
	ofGLFWWindowSettings settings;
	settings.width = 1024;
	settings.height = 610;
	settings.setPosition(ofVec2f(300,0));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	mainWindow->setVerticalSync(true);
	
	shared_ptr<testApp> mainApp(new testApp);
	
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
