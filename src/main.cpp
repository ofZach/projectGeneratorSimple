#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"

void checkOFRootConfig(){
	if(!checkConfigExists()){
		askOFRoot();
	}
	string ofRoot = getOFRootFromConfig();
	if(ofRoot==""){
		ofLogError() << "you need to set the OF root before running\n"
						"the project generator from the command line";
		std::exit(1);
	}else{
		cout << "using openFrameworks install from " << ofRoot << endl;
		setOFRoot(ofRoot);
	}
}


//========================================================================
int main(  int argc, char *argv[]  ){
    
#ifdef TARGET_LINUX
	if(argc==1){
		ofAppGlutWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		testApp * app = new testApp;
		app->commandLine = false;
		if(ofFilePath::getCurrentExeDir()=="/usr/bin/" || ofFilePath::getCurrentExeDir()=="/usr/local/bin/"){
			checkOFRootConfig();
			ofSetDataPathRoot(ofFilePath::join(getOFRoot(),"/usr/share/ofprojectgenerator/data/"));
			cout << "running with data path at " << ofToDataPath("") << endl;
		}
		ofRunApp( app );
	}else{
		ofAppNoWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		testApp * app = new testApp;
		app->commandLine = true;
		app->projectPath = "";

		for(int i=1;i<argc;i++){
			string arg = argv[i];
			if(arg.find("--")==0){
				arg = arg.substr(2);
				if(arg=="help"){
					cout << "OF Project Generator Usage:" << endl;
					cout << "projectGenerator [pathToExample]" << endl;
					cout << endl;
					cout << "Default: create project files for current platform for selected path" << endl;
					cout << "To add addons to an existing project" << endl;
					cout << "add a line with the name of each addon in addons.make" << endl;
					cout << "and rerun passing the path of the project" << endl;
					cout << endl;
					cout << "Without parameters, shows gui" << endl;
					std::exit(0);
				}else{
					cout << "Error: Unknown argument --" << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
					std::exit(1);
				}
			}else if(arg.find("-", 0, 1)==0){
				cout << "Error: Unknown argument " << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
				std::exit(1);
			}else{
				app->projectPath = ofFilePath::removeTrailingSlash(ofFilePath::getPathForDirectory(ofFilePath::getAbsolutePath(arg,false)));
			}
		}
		checkOFRootConfig();
		ofRunApp( app );
	}
#else
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,400, OF_WINDOW);
	testApp * app = new testApp;
	app->commandLine = false;
	ofRunApp( app );
#endif
}
