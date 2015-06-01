#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main(  int argc, char *argv[]  ){
	
	ofSetupOpenGL(1024,610,OF_WINDOW);
	ofRunApp(new testApp());
}
