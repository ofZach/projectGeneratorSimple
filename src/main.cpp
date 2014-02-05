#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){
    
    
   
    
    cout << "In main." << endl;
    cout << "ofGetTargetPlatform=" << ofGetTargetPlatform() << endl;
    
#ifdef TARGET_LINUX
    cout << "linux " << endl;
	if(argc==1){
		
        ofAppGlutWindow window;
        ofSetupOpenGL(&window, 1024, 610, OF_WINDOW);
        testApp * app = new testApp;
        
        ofRunApp( app );
        
	}else{
		ofAppNoWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		testApp * app = new testApp;
		app->makeProjects.buildAllExamples = false;
		app->makeProjects.projectPath = "";
		for(int i=1;i<argc;i++){
			string arg = argv[i];
			if(arg.find("--")==0){
				arg = arg.substr(2);
				if(arg=="linux"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUX );
				}else if(arg=="linux64"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUX64 );
				}else if(arg=="linuxarmv6l"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUXARMV6L );
				}else if(arg=="linuxarmv7l"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUXARMV7L );
				}else if(arg=="win_cb"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_WINGCC );
				}else if(arg=="vs"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_WINVS );
				}else if(arg=="osx"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_OSX );
				}else if(arg=="ios"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_IPHONE );
				}else if(arg=="android"){
					ofLogError() << "platform not supported yet" << endl;
					std::exit(1);
				}else if(arg=="allplatforms"){
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUX );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUX64 );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUXARMV6L );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_LINUXARMV7L );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_WINGCC );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_WINVS );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_OSX );
					app->makeProjects.targetsToMake.push_back( OF_TARGET_IPHONE );
				}else if(arg=="allexamples"){
					app->makeProjects.buildAllExamples = true;
				}else if(arg=="help"){
					cout << "OF Project Generator Usage:" << endl;
					cout << "projectGenerator [options] [pathToExample]" << endl;
					cout << "Options:" << endl;
					cout << "--osx: generate osx project files" << endl;
					cout << "--win_cb: generate windows codeblocks project files" << endl;
					cout << "--vs: generate windows visual studio project files" << endl;
					cout << "--linux: generate linux project files" << endl;
					cout << "--linux64: generate linux 64bits project files" << endl;
					cout << "--linuxarmv6l: generate linux armv6l project files" << endl;
					cout << "--linuxarmv7l: generate linux armv7l project files" << endl;
					cout << "--ios: generate iOS project files" << endl;
					cout << "--allplatforms: generate project files for all platforms" << endl;
					cout << "--allexamples: generate project files for all examples. [pathToExample] is not needed in this case." << endl;
					cout << endl;
					cout << "default: create project files for current platform for selected path" << endl;
					cout << "running over existing example updates project files for selected platforms" << endl;
					cout << "without parameters, shows gui" << endl;
					std::exit(0);
				}else{
					cout << "Error: Unknown argument --" << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
					std::exit(1);
				}
			}else if(arg.find("-", 0, 1)==0){
				cout << "Error: Unknown argument " << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
				std::exit(1);
			}else{
				app->makeProjects.projectPath = ofFilePath::removeTrailingSlash(ofFilePath::getPathForDirectory(ofFilePath::getAbsolutePath(arg,false)));
			}
		}
        
		if (!(app->makeProjects.buildAllExamples) && app->makeProjects.projectPath.empty()){
			cout << "Error: No target project given. Either run projectGenerator with the --allexamples option or supply a target project folder path. Run \'projectGenerator help\' for details." << endl;
			std::exit(1);
		}
        
		if(app->makeProjects.targetsToMake.empty())
			app->makeProjects.targetsToMake.push_back( ofGetTargetPlatform() );
		ofRunApp( app );
	}
#else
    printf("??");
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 610, OF_WINDOW);
    testApp * app = new testApp;
    //app->makeProjects.buildAllExamples = false;
    ofRunApp( app );
    
#endif
    
}
